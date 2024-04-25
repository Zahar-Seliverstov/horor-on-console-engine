#include "Library.h"
#include "IMapInfo.h"
#include "IPlayer.h"
#include "IEngine.h"
#include "IScreensavers.h"
#include "IRestoringHealth.h"
#include "IMonster.h"



int distans;
//	Конструктор
Engine::Engine(
	Player& _playerInfo,
	Monster& _monsterInfo,
	Teleport& _teleportInfo,
	RestoringHealth& _restoringHealthInfo,
	RestoringEnergy& _restoringEnergyInfo,
	MapInfo& _mapInfo)
{
	//	Инициализируем обьекты карты
	mapInfo = &_mapInfo;
	playerInfo = &_playerInfo;
	monsterInfo = &_monsterInfo;
	restoringHealthInfo = &_restoringHealthInfo;
	restoringEnergyInfo = &_restoringEnergyInfo;
	teleportInfo = &_teleportInfo;

	startFrameGeneration = chrono::high_resolution_clock::now();
	endFrameGeneration = chrono::high_resolution_clock::now();
	console = GetStdHandle(STD_OUTPUT_HANDLE);
	drawingRange = 10;
	fov = 3.14159f / 1.5f;
	texturingLevel = 0.1f;
	playerMovedToNextFloor = false;
	gameIsOver = false;
	displayMap = false;
	settingsIsOpen = false;
	frameIsBuild = true;
}

//	Чекаем игрок использовал обьект на карте или нет
void Engine::checkPlayerUseObject()
{
	//	Переход на следующий этаж
	if (int(playerInfo->getPositionX()) == teleportInfo->getX()
		&& int(playerInfo->getPositionY()) == teleportInfo->getY())
		playerMovedToNextFloor = true;
	else playerMovedToNextFloor = false;

	//	Востановление ЭНЕРГИИ
	if (!restoringEnergyInfo->used
		&& int(playerInfo->getPositionX()) == restoringEnergyInfo->getX()
		&& int(playerInfo->getPositionY()) == restoringEnergyInfo->getY())
	{
		playerInfo->setAmountEnergy(playerInfo->getInitialAmountEnergy());
		mapInfo->initialMap[restoringEnergyInfo->getY() * mapInfo->mapSizeHorizontal + restoringEnergyInfo->getX()] = L' ';
		restoringEnergyInfo->used = true;
		mciSendString(L"play sounds/energyUp.wav", NULL, 0, NULL);
	}

	//	Востановление ЗДОРОВЬЯ
	if (!restoringHealthInfo->used
		&& int(playerInfo->getPositionX()) == restoringHealthInfo->getX()
		&& int(playerInfo->getPositionY()) == restoringHealthInfo->getY())
	{
		playerInfo->setHp(playerInfo->getInitialHp());
		mapInfo->initialMap[restoringHealthInfo->getY() * mapInfo->mapSizeHorizontal + restoringHealthInfo->getX()] = L' ';
		restoringHealthInfo->used = true;
		mciSendString(L"play sounds/healthUp.wav", NULL, 0, NULL);
	}
}

//	Получаем размер консольного окна
void Engine::getConsoleSize()
{
	HANDLE hWndConsole;
	if (hWndConsole = GetStdHandle(-12))
	{
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		if (GetConsoleScreenBufferInfo(hWndConsole, &consoleInfo))
		{
			pair<int, int> screenSize;
			screenWidth = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;
			screenHeight = consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top + 1;
		}
	}
}

//	Устанавливаем размер кадра под размер консоли
void Engine::setScreenSize()
{
	consoleBufferSize = { short(screenWidth), short(screenHeight) };
	windowSize = { 0, 0, short(screenWidth - 1), short(screenHeight - 1) };
	SetConsoleScreenBufferSize(console, consoleBufferSize);
	SetConsoleWindowInfo(console, TRUE, &windowSize);
}

//	Устанавливаем видимость курсора
void Engine::setCursoreVisible()
{
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(console, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(console, &cursorInfo);
}

//	Открытие настроик
void Engine::openSettings()
{
	int startX = screenWidth / 2;
	int startY = screenHeight / 2 - 5;
	int command = 6;
	bool treker = true;
	vector<string> items{
		"         DRAWING RANGE         ",
		"         WALKING SPEED         ",
		"          SENSITIVITY          ",
		"              FOV              ",
		"        TEXTURING LEVEL        ",
		"          SCREEN SIZE          ",
		"           CONTINUE            ",
		"         EXIT THE GAME         " };
	this_thread::sleep_for(chrono::milliseconds(100));
	system("cls");
	while (treker) {
		printf("\x1b[%d;%dH", startY - 5, startX - 15);
		printf("\x1b[90m~~~~~~~~ \x1b[93mSETTINGS MENU\x1b[90m ~~~~~~~~\n\n\x1b[90m\x1b[48; 5; 0m");
		for (int i = 0; i < items.size(); i++) {
			printf("\x1b[%d;%dH", (startY + i + 2) - 5, startX - 15);
			if (i == command)
			{
				if (i == items.size() - 1) printf("\x1b[38;5;0m\x1b[48;5;9m");
				else printf("\x1b[38;5;0m\x1b[48;5;15m");
				cout << items.at(i) << endl;
				printf("\x1b[90m\x1b[48;5;0m");
			}
			else cout << items.at(i) << endl;
		}
		printf("\x1b[%d;%dH", startY + 9 - 4, startX - 15);
		printf("\x1b[90m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

		switch (_getwch()) {
		case arrowUp: command = command - 1 >= 0 ? command -= 1 : items.size() - 1;
			PlaySound(L"sounds/choice.wav", NULL, SND_ASYNC);
			break;
		case arrowDown: command = command < items.size() - 1 ? command += 1 : 0;
			PlaySound(L"sounds/choice.wav", NULL, SND_ASYNC);
			break;
		case enter: treker = false;
			PlaySound(L"sounds/choice.wav", NULL, SND_ASYNC);
			break;
		}
		//this_thread::sleep_for(chrono::milliseconds(100));
	}
	treker = true;
	system("cls");
	switch (command) {
	case 0:// УРОВЕНЬ ПРОРИСОВКИ
		while (treker) {
			printf("\x1b[%d;%dH", startY - 3, startX - 25);
			printf("\x1b[90m~~~~~~~~~~~~~~~~~ DRAWING LEVEL ~~~~~~~~~~~~~~~~~");
			printf("\x1b[%d;%dH", startY - 1, startX - 24);
			for (int i = 1; i <= drawingRange - 1; i++) {
				if (i < 5) printf("\x1b[91m");
				else if (i < 8) printf("\x1b[38;5;208m");
				else if (i < 11) printf("\x1b[92m");
				else printf("\x1b[92m");
				cout << "[" << i << "]";
			}
			cout << "                                                   ";
			printf("\x1b[%d;%dH", startY + 1, startX - 25);
			printf(" \x1b[91m~\x1b[0m Management \x1b[93m <-+->");
			printf("\x1b[%d;%dH", startY + 3, startX - 25);
			cout << "\x1b[90m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\x1b[0m";
			printf("\x1b[%d;%dH", startY + 5, startX - 25);
			cout << "\x1b[90m <~~ { \x1b[97mesc\x1b[90m }";

			switch (_getwch()) {
			case arrowLeft: drawingRange = drawingRange - 1 >= 2 ? drawingRange -= 1 : 15;
				break;
			case arrowRight: drawingRange = drawingRange + 1 <= 15 ? drawingRange += 1 : 2;
				break;
			case escape: treker = false;
				break;
			}
			PlaySound(L"sounds/choice.wav", NULL, SND_ASYNC);
		}
		break;
	case 1://	CКОРОСТЬ
		playerInfo->setSpeed(playerInfo->getInitialSpeed());
		while (treker)
		{
			printf("\x1b[%d;%dH", startY - 3, startX - 15);
			printf("\x1b[90m~~~~~~~~~~ \x1b[0mSPEED \x1b[90m~~~~~~~~~\n\n\x1b[0m");
			printf("\x1b[%d;%dH", startY - 1, startX - 14);
			for (int i = 1; i <= playerInfo->getSpeed(); i++) {
				if (i < 3) printf("\x1b[91m");
				else if (i < 4)  printf("\x1b[38;5;208m");
				else if (i < 6) printf("\x1b[92m");
				else if (i < 7)  printf("\x1b[38;5;208m");
				else printf("\x1b[91m");
				cout << "[" << i << "]";
			}
			cout << "                                       ";
			printf("\x1b[%d;%dH", startY + 1, startX - 15);
			printf(" \x1b[91m~\x1b[0m Management \x1b[93m <-+->");
			printf("\x1b[%d;%dH", startY + 2, startX - 15);
			cout << "\x1b[90m~~~~~~~~~~~~~~~~~~~~~~~~~~";
			printf("\x1b[%d;%dH", startY + 4, startX - 15);
			printf("\x1b[90m <~~ { \x1b[0mesc\x1b[90m }");

			switch (_getwch())
			{
			case arrowLeft: playerInfo->setSpeed(playerInfo->getSpeed() - 1 >= 1 ? playerInfo->getSpeed() - 1 : 8);
				break;
			case arrowRight: playerInfo->setSpeed(playerInfo->getSpeed() + 1 <= 8 ? playerInfo->getSpeed() + 1 : 1);
				break;
			case escape: treker = false;
				break;
			}
			PlaySound(L"sounds/choice.wav", NULL, SND_ASYNC);
		}
		playerInfo->setInitialSpeed(playerInfo->getSpeed());
		break;
	case 2://	ЧУВСТВИТЕЛЬНОСТЬ
		while (treker) {
			printf("\x1b[%d;%dH", startY - 3, startX - 16);
			printf("\x1b[90m~~~~~~~~~~ \x1b[0mSENSITIVITY \x1b[90m~~~~~~~~~~\x1b[0m\n");
			printf("\x1b[%d;%dH", startY - 1, startX - 15);
			for (double i = 1; i <= playerInfo->getSensitivity(); i += 1) {
				if (i < 3) printf("\x1b[91m");
				else if (i < 4) printf("\x1b[38;5;208m");
				else if (i < 7) printf("\x1b[92m");
				else if (i < 9) printf("\x1b[38;5;208m");
				else printf("\x1b[91m");
				cout << "[" << i << "]";
			}
			cout << "                                       ";
			printf("\x1b[%d;%dH", startY + 1, startX - 16);
			printf("\x1b[91m ~ \x1b[0mManagement  \x1b[93m<-+->");
			printf("\x1b[%d;%dH", startY + 2, startX - 16);
			printf("\x1b[90m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\x1b[0m\n");
			printf("\x1b[%d;%dH", startY + 4, startX - 16);
			printf("\x1b[90m <~~ { \x1b[0mesc\x1b[90m }");

			switch (_getwch()) {
			case arrowLeft: playerInfo->setSensitivity(playerInfo->getSensitivity() - 1 >= 1 ? playerInfo->getSensitivity() - 1 : 10);
				break;
			case arrowRight: playerInfo->setSensitivity(playerInfo->getSensitivity() + 1 <= 10 ? playerInfo->getSensitivity() + 1 : 1);
				break;
			case escape: treker = false;
				break;
			}
			PlaySound(L"sounds/choice.wav", NULL, SND_ASYNC);
		}
		break;
	case 3://	ПОЛЕ ЗРЕНИЯ

		while (treker) {
			printf("\x1b[%d;%dH", startY - 3, startX - 14);
			printf("\x1b[90m~~~~~~~~~ \x1b[0mFOV \x1b[90m~~~~~~~~~\n\n\x1b[0m ");
			printf("\x1b[%d;%dH", startY - 1, startX - 13);
			for (double i = 1; i <= fov * 2; i++) {
				if (i < 2) printf("\x1b[91m");
				else if (i < 3) printf("\x1b[38;5;208m");
				else if (i < 5) printf("\x1b[92m");
				else if (i < 6)  printf("\x1b[38;5;208m");
				else printf("\x1b[91m");
				cout << "[" << i << "]";
			}
			cout << "                                       ";
			printf("\x1b[%d;%dH", startY + 1, startX - 14);
			printf("\x1b[91m ~ \x1b[0mManagement \x1b[93m <-+->");
			printf("\x1b[%d;%dH", startY + 2, startX - 14);
			printf("\x1b[90m~~~~~~~~~~~~~~~~~~~~~~~");
			printf("\x1b[%d;%dH", startY + 4, startX - 14);
			printf("\x1b[90m <~~ { \x1b[0mesc\x1b[90m }");

			switch (_getwch())
			{
			case arrowLeft: fov = fov - 0.5 > 0 ? fov -= 0.5 : 3.5;
				break;
			case arrowRight: fov = fov + 0.5 < 4 ? fov += 0.5 : 0.5;
				break;
			case escape: treker = false;
				break;
			}
			PlaySound(L"sounds/choice.wav", NULL, SND_ASYNC);
		}
		break;
	case 4://	УРОВЕНЬ ТЕКСТУРИРОВАНИЕ
		while (treker) {
			printf("\x1b[%d;%dH", startY - 3, startX - 14);
			printf("\x1b[90m~~~~~~~~ \x1b[0mTEXTURING LEVEL\x1b[90m ~~~~~~~~\n\n\x1b[0m ");
			printf("\x1b[%d;%dH", startY - 1, startX - 13);
			for (double i = 0.1; i <= texturingLevel; i += 0.1) {
				if (i > 0.5) printf("\x1b[91m");
				else if (i > 0.2) printf("\x1b[38;5;208m");
				else if (i >= 0.1) printf("\x1b[92m");
				cout << "[" << i * 10 << "]";
			}
			cout << "                                       ";
			printf("\x1b[%d;%dH", startY + 1, startX - 14);
			printf("\x1b[91m ~ \x1b[0mManagement \x1b[93m <-+->");
			printf("\x1b[%d;%dH", startY + 2, startX - 14);
			printf("\x1b[90m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
			printf("\x1b[%d;%dH", startY + 4, startX - 14);
			printf("\x1b[90m <~~ { \x1b[0mesc\x1b[90m }");

			switch (_getwch())
			{
			case arrowLeft: texturingLevel = texturingLevel - 0.1 > 0.1 ? texturingLevel -= 0.1 : 1;
				break;
			case arrowRight: texturingLevel = texturingLevel + 0.1 <= 1 ? texturingLevel += 0.1 : 0.1;
				break;
			case escape: treker = false;
				break;
			}
			PlaySound(L"sounds/choice.wav", NULL, SND_ASYNC);
		}
		break;
	case 5://	ЭКРАН
		while (treker)
		{
			getConsoleSize();
			if (startX != screenWidth / 2 || startY != screenHeight / 2 - 5)
			{
				startX = screenWidth / 2;
				startY = screenHeight / 2 - 5;
				system("cls");
			}

			printf("\x1b[%d;%dH", startY - 3, startX - 20);
			printf("\x1b[90m~~~~~~~~~~ SCREEN ~~~~~~~~~~\x1b[0m");
			printf("\x1b[%d;%dH", startY + 2 - 3, startX - 20);
			printf("\x1b[90m # \x1b[94mStretch the window\x1b[90m. . .");
			printf("\x1b[%d;%dH", startY + 4 - 3, startX - 20);
			printf("\x1b[91m ~\x1b[0m Screen size \x1b[90m[ \x1b[93m");
			cout << (to_string(screenWidth) + " \x1b[90mx\x1b[93m " + to_string(screenHeight) + " \x1b[90m]");
			printf("\x1b[%d;%dH", startY + 6 - 3, startX - 20);
			printf("\x1b[90m~~~~~~~~~~~~~~~~~~~~~~~~~~~~\x1b[0m");
			printf("\x1b[%d;%dH", startY + 8 - 3, startX - 20);
			printf("\x1b[90m <~~ { \x1b[0mesc\x1b[90m }");

			if (_kbhit()) {
				switch (_getch()) {
				case escape: treker = false;
				}
			}
		}
		setScreenSize();
		delete[] screen, screen = nullptr;
		screen = new CHAR_INFO[screenWidth * screenHeight];
		break;
	case 6:
		return;
	case 7:
		exit(0);
		break;
	}
	openSettings();
}

//	Генерирует маску кадра
void Engine::generateInfoFrame()
{
	frameInfoIsBuild = true; //	Началась генерация маски кадра
	//	Высчитываем FPS
	int frames = int(1 / frameGenerationTimeInSeconds);

	//	Нужные переменные для генерации текстуры интерфейса
	wstringstream fpsTexture;
	wstring settingTexture = L" ESCAPE ☰ ";
	wstring healthTexture = L"";
	wstring energyTexture = L"";
	wstring minimap = L"  FLOOR MAP  ";

	//	Заполняем textureFps
	if (displayFps) {
		fpsTexture << L"[ FPS: " << frames
			<< (frames < 10 ? L"   ]" :
				(frames >= 10 && frames < 100) ? L"  ]" :
				(frames >= 100 && frames < 1000) ? L" ]" : L"]")
			<< L" [ X: " << std::fixed << std::setprecision(1) << playerInfo->getPositionX()
			<< (playerInfo->getPositionX() < 10 ? L"   ]" :
				(playerInfo->getPositionX() < 100 && playerInfo->getPositionX() >= 10 ? L"  ]" :
					(playerInfo->getPositionX() < 1000 && playerInfo->getPositionX() >= 100 ? L" ]" : L"]")))
			<< L" [ Y: " << std::fixed << std::setprecision(1) << playerInfo->getPositionY()
			<< (playerInfo->getPositionY() < 10 ? L"   ]" :
				(playerInfo->getPositionY() < 100 && playerInfo->getPositionY() >= 10 ? L"  ]" :
					(playerInfo->getPositionY() < 1000 && playerInfo->getPositionY() >= 100 ? L" ]" : L"]")))
			<< L"  SPEED: " << "[ " << playerInfo->getSpeed() << " ]"
			<< L" RUN: " << (playerInfo->getRun() ? L"[ true  ] " : L"[ false ] ")
			<< L"GO: " << (playerInfo->getGo() ? L"[ true  ]" : L"[ false ]")
			<< L" HEALTH: [" << playerInfo->getHp() << (playerInfo->getHp() >= 100 ? L"]" : playerInfo->getHp() == 0 ? L"  ]" : L" ]")
			<< L" D: " << distans;
	}
	//	Отрисовываем интерфейс ( prompt the setting )
	for (int i = 0; i < settingTexture.length(); i++) {
		int value = screenWidth - settingTexture.length();
		screen[value + i].Char.UnicodeChar = settingTexture[i];
		screen[value + i].Attributes = 11;
	}
	//	Отрисовываем интерфейс ( player energy )
	for (int x = 0; x < playerInfo->getInitialAmountEnergy(); x += playerInfo->getInitialAmountEnergy() / 10)
		energyTexture += (x <= playerInfo->getAmountEnergy() ? L"▒♦▒ " : L"░♢░ ");
	for (int i = 0; i < energyTexture.length(); i++) {
		int value = screenWidth * (screenHeight - 2);
		screen[value + i].Char.UnicodeChar = energyTexture[i];
		screen[value + i].Attributes = 6;
	}
	//	Отрисовываем интерфейс ( player health )
	for (int i = 10; i <= playerInfo->getInitialHp(); i += playerInfo->getInitialHp() / 10)
		healthTexture += (i <= playerInfo->getHp() ? L"▒\u2665▒ " : L"▒░▒ ");
	for (int i = 0; i < healthTexture.size(); i++) {
		int value = screenWidth * (screenHeight - 1);
		screen[i + value].Char.UnicodeChar = healthTexture[i];
		screen[i + value].Attributes = 4;
	}
	//	Отрисовываем интерфейс ( fps )
	if (displayFps) {
		for (int i = 0; i < (fpsTexture.str()).size(); i++) {
			screen[i].Char.UnicodeChar = (fpsTexture.str())[i];
			screen[i].Attributes = 11;
		}
	}
	//	Отрисовываем интерфейс ( map )
	if (displayMap) {
		//	Карта
		for (int i = 0; i < minimap.size(); i++) {
			//	Отрисовываем надпись ( FLOOR MAP )
			screen[i + (screenWidth * 2)].Char.UnicodeChar = minimap[i];
			screen[i + (screenWidth * 2)].Attributes = 7;
		}
		/*for (int y = 2; y < mapInfo->mapSizeVertical; y++) {
			int index = (y * mapInfo->mapSizeHorizontal) + x;
			if (x < screenWidth - 1 && y + 3 < screenHeight) {
				if (mapInfo->mapPlayerSaw[index]) {
					screen[(y + 3) * screenWidth + x].Char.UnicodeChar = mapInfo->map[index];
					screen[(y + 3) * screenWidth + x].Attributes = 15;
				}
			}
		}*/
		//	Отрисовываем карту
		for (int x = 0; x < mapInfo->mapSizeHorizontal; x++)
		{
			for (int y = 0; y < mapInfo->mapSizeVertical; y++)
			{
				if (x < screenWidth && y < screenHeight - 6)
				{
					if (mapInfo->mapPlayerSaw[(y * mapInfo->mapSizeHorizontal) + x])
					{
						screen[((y + 3) * screenWidth) + x].Char.UnicodeChar = mapInfo->map[(y * mapInfo->mapSizeHorizontal) + x];
						screen[((y + 3) * screenWidth) + x].Attributes = 15;
					}
				}
			}
		}
		//	ОТРИСОВКА МАСКИ КАРТЫ ЭТАЖА ДЛЯ ОБЬЕКТОВ

		//	Игрок
		if ((playerInfo->getPositionX() < screenWidth)
			&& (playerInfo->getPositionY() + 3 < screenHeight - 3))
		{
			screen[((int)playerInfo->getPositionY() + 3) * screenWidth + (int)playerInfo->getPositionX()].Char.UnicodeChar = L'●';
			screen[((int)playerInfo->getPositionY() + 3) * screenWidth + (int)playerInfo->getPositionX()].Attributes = 9;
		}
		//	Монстер
		if (monsterInfo->getX() < screenWidth
			&& monsterInfo->getY() + 3 < screenHeight)
		{
			screen[((int)monsterInfo->getY() + 3) * screenWidth + (int)monsterInfo->getX()].Char.UnicodeChar = L'■';
			screen[((int)monsterInfo->getY() + 3) * screenWidth + (int)monsterInfo->getX()].Attributes = 2;

		}
		//	Телепорт
		if (teleportInfo->getX() < screenWidth
			&& teleportInfo->getY() + 3 < screenHeight)
		{
			screen[(teleportInfo->getY() + 3) * screenWidth + (teleportInfo->getX())].Char.UnicodeChar = L'⚑';
			screen[(teleportInfo->getY() + 3) * screenWidth + (teleportInfo->getX())].Attributes = 9;
		}

		//	Хилка
		if (restoringHealthInfo->getX() < screenWidth
			&& restoringHealthInfo->getY() + 3 < screenHeight
			&& !restoringHealthInfo->used)
		{
			screen[((int)restoringHealthInfo->getY() + 3) * screenWidth + (int)restoringHealthInfo->getX()].Char.UnicodeChar = L'❤';
			screen[((int)restoringHealthInfo->getY() + 3) * screenWidth + (int)restoringHealthInfo->getX()].Attributes = 5;
		}

		//	Стамина
		if (restoringEnergyInfo->getX() < screenWidth
			&& restoringEnergyInfo->getY() + 3 < screenHeight
			&& !restoringEnergyInfo->used)
		{
			screen[((int)restoringEnergyInfo->getY() + 3) * screenWidth + (int)restoringEnergyInfo->getX()].Char.UnicodeChar = L'✦';
			screen[((int)restoringEnergyInfo->getY() + 3) * screenWidth + (int)restoringEnergyInfo->getX()].Attributes = 6;
		}
	}
	frameInfoIsBuild = false;	//	Генерация маски кадра завершена
}

//	Вспомогательный метод для проверки на то что игрок уже видел
bool Engine::mapViewBoundaryCheck(int x, int y)
{
	if (displayMap && ((x < mapInfo->mapSizeHorizontal) && y > 2 && (y < mapInfo->mapSizeVertical + 3) && y < screenHeight - 3))
	{
		if (mapInfo->mapPlayerSaw[((y - 3) * (mapInfo->mapSizeHorizontal)) + x] /*== true*/)	// ПРиколЛ
			return false;
		else
			return true;
	}
	else
		return true;
}

//	Генерирует кадр
void Engine::generateFrame()
{
	if (!settingsIsOpen && !playerMovedToNextFloor)
	{
		//	Отчищаем карту от угла обзора игрока
		mapInfo->clearmap();
		//mapInfo->map[monsterInfo->getY() * mapInfo->mapSizeHorizontal + monsterInfo->getX()] = monsterInfo->getMapSkin();
		frameIsBuild = true;	//	Генерация кадра началась
		for (int x = 0; x < screenWidth; x++) {
			int testX;
			int testY;
			double rayAngle = playerInfo->getPositionR() + fov / 2.0f - x * fov / screenWidth;
			double rayX = sinf(rayAngle);
			double rayY = cosf(rayAngle);
			double distanceWall = 0;
			int celling;
			int floor;
			bool itWall = false;
			bool itTeleport = false;
			bool itRestoringHealth = false;
			bool itRestoringEnergy = false;
			bool itMonster = false;
			bool itBound = false;

			while (!itWall
				&& !itTeleport
				&& !itRestoringEnergy
				&& !itRestoringHealth
				&& !itMonster
				&& distanceWall <= drawingRange)
			{
				distanceWall += texturingLevel;
				testX = (int)(playerInfo->getPositionX() + rayX * distanceWall);
				testY = (int)(playerInfo->getPositionY() + rayY * distanceWall);
				//	Проверяем что луч стлкнулся с МОНСТРОМ
				//if (mapInfo->map[testY * mapInfo->mapSizeHorizontal + testX] == L'M') { itMonster = true; }
				if (testY == monsterInfo->getY() && testX == monsterInfo->getX()) { itMonster = true; }
				//	Проверяем что луч столкнулся с ХИЛКОЙ 
				else if (mapInfo->map[testY * mapInfo->mapSizeHorizontal + testX] == L'H') { itRestoringHealth = true; }
				//	Проверяем что луч столкнулся с ТЕЛЕПОРТОМ
				else if (mapInfo->map[testY * mapInfo->mapSizeHorizontal + testX] == L'T') { itTeleport = true; }
				//	Проверяем что луч столкнулся со СТАМИНОЙ
				else if (mapInfo->map[testY * mapInfo->mapSizeHorizontal + testX] == L'E') { itRestoringEnergy = true; }
				//	Проверяем что луч столкнулся со СТЕНКОЙ
				else if (mapInfo->map[testY * mapInfo->mapSizeHorizontal + testX] == L'#') { itWall = true; }
				//	Отрисовываем на карте угол обзора
				if (!itWall
					&& !itTeleport
					&& !itRestoringHealth
					&& !itRestoringEnergy
					&& !itMonster
					&& (distanceWall <= drawingRange)) {
					//	Отрисовываем угол обзора на карте
					mapInfo->map[testY * mapInfo->mapSizeHorizontal + testX] = '.';
				}
				//	Отмечаем увиденные облости карты
				mapInfo->mapPlayerSaw[testY * mapInfo->mapSizeHorizontal + testX] = true;
				if (itWall) { mapInfo->mapPlayerSaw[testY * mapInfo->mapSizeHorizontal + testX] = true; }
			}
			//	Высчитываем грани обьектов
			if ((itWall || itMonster) && distanceWall < drawingRange) {
				vector<pair<double, double>> boundsVector;
				double playerX = playerInfo->getPositionX();
				double playerY = playerInfo->getPositionY();
				for (int tx = 0; tx < 2; ++tx) {
					for (int ty = 0; ty < 2; ++ty) {
						double vectorX = testX + tx - playerX;
						double vectorY = testY + ty - playerY;
						double vectorModuleSquare = vectorX * vectorX + vectorY * vectorY;
						double cosAngle = (rayX * vectorX + rayY * vectorY) / sqrt(vectorModuleSquare);
						boundsVector.emplace_back(vectorModuleSquare, cosAngle);
					}
				}
				sort(boundsVector.begin(), boundsVector.end(), [](const auto& point1, const auto& point2) {
					return point1.first < point2.first;
					});
				double boundAngle = 0.04 / distanceWall;
				if ((acos(boundsVector[0].second) < boundAngle && distanceWall > 0.5)
					|| (acos(boundsVector[1].second) < boundAngle && distanceWall > 0.5)) {
					itBound = true;
				}
			}

			celling = (float)(screenHeight / 2.0) - screenHeight / ((float)distanceWall);	//	Высчитываем потолок
			floor = screenHeight - celling;													//	Высчитываем пол

			for (int y = 0; y < screenHeight; y++) {
				int gg = 1;
				double centerScreen = 1 - double(y - screenHeight / 2) / (screenHeight / 2);	//	Высчитываем цент экрана
				//	Не отрисовываем где будет отрисовываться маска для кадра
				if ((displayFps ? (x > 106 || y != 0) : true)		//	FPS
					&& (y != screenHeight - 1 || x > 39)			//	HP
					&& (y != screenHeight - 2 || x > 39)			//	ENERGY
					&& (mapViewBoundaryCheck(x, y)))				//	MAP
				{
					//	Текстурируем потолок
					if (y <= celling) {
						char cellingTexture;

						if (centerScreen > 1.9f)
							cellingTexture = '$';
						else if (centerScreen > 1.7f)
							cellingTexture = '#';
						else if (centerScreen > 1.4f)
							cellingTexture = '*';
						else if (centerScreen > 1.15f)
							cellingTexture = ':';
						else
							cellingTexture = ' ';

						screen[y * screenWidth + x].Char.UnicodeChar = cellingTexture;
						screen[y * screenWidth + x].Attributes = 8;

					}
					//	Текстурируем пространство между потолком и полом
					else if (y > celling && y <= floor) {
						//	Текстурируем ТЕЛЕПОРТ
						if (itTeleport) {
							wchar_t teleportTexture = L' ';
							teleportTexture = (rand() % 10) % 3 == 0 ? teleportTexture : teleportInfo->getFrontSkin();
							screen[y * screenWidth + x].Attributes = (rand() % 10) % 4 == 0 ? 15 : 5;
							screen[y * screenWidth + x].Char.UnicodeChar = teleportTexture;
						}
						//	Текстурируем СТЕНЫ
						else if (itWall) {
							wchar_t wallTexture = L' ';
							if (itBound) { screen[y * screenWidth + x].Attributes = 8; }
							else {
								if (distanceWall <= drawingRange / 5) { wallTexture = L'█'; }
								else if (distanceWall <= drawingRange / 4) { wallTexture = L'▓'; }
								else if (distanceWall <= drawingRange / 2.3) { wallTexture = L'▒'; }
								else if (distanceWall <= drawingRange) { wallTexture = L'░'; }
								//else { wallTexture = L' '; }
								screen[y * screenWidth + x].Attributes = 8;
							}
							screen[y * screenWidth + x].Char.UnicodeChar = wallTexture;
						}
						//	Текстурируем ХИЛКУ
						else if (itRestoringHealth) {
							wchar_t restoringHealthTexture = restoringHealthInfo->getFrontSkin();
							if (itBound) { screen[y * screenWidth + x].Attributes = restoringHealthInfo->getAttributeEdge(); }
							else {
								restoringHealthTexture = restoringHealthInfo->getEdgeSkin();
								screen[y * screenWidth + x].Attributes = restoringHealthInfo->getAttributeFrontColor();
							}
							screen[y * screenWidth + x].Char.UnicodeChar = restoringHealthTexture;
						}
						//	Текстурирует МОНСТРА
						else if (itMonster) {
							wchar_t monsterTexture = monsterInfo->getEdgeSkin();
							if (itBound) { screen[y * screenWidth + x].Attributes = monsterInfo->getAttributeEdge(); }
							else {
								screen[y * screenWidth + x].Attributes = monsterInfo->getAttributeFrontColor();
								monsterTexture = monsterInfo->getFrontSkin();
							}
							screen[y * screenWidth + x].Char.UnicodeChar = monsterTexture;
						}
						//	Текстурируем СТАМИНУ
						else if (itRestoringEnergy) {
							wchar_t restoringEnergyTexture = restoringEnergyInfo->getEdgeSkin();
							if (itBound) { screen[y * screenWidth + x].Attributes = restoringEnergyInfo->getAttributeEdge(); }
							else {
								screen[y * screenWidth + x].Attributes = restoringEnergyInfo->getAttributeFrontColor();
								restoringEnergyTexture = restoringEnergyInfo->getFrontSkin();
							}
							screen[y * screenWidth + x].Char.UnicodeChar = restoringEnergyTexture;
						}
						else {
							screen[y * screenWidth + x].Char.UnicodeChar = L' ';
						}
					}
					//	Текстурируем пол
					else {
						short floorTexture;

						if (centerScreen < 0.2f)
							floorTexture = '&';
						else if (centerScreen < 0.3f)
							floorTexture = '#';
						else if (centerScreen < 0.5f)
							floorTexture = '*';
						else if (centerScreen < 0.9f)
							floorTexture = ':';
						else
							floorTexture = ' ';

						screen[y * screenWidth + x].Char.UnicodeChar = floorTexture;
						screen[y * screenWidth + x].Attributes = 8;
					}
				}
			}
		}
		frameIsBuild = false; //	Генерация кадра завершилась
	}
}

//	Запускаем движок
void Engine::start()
{

	screen = new CHAR_INFO[screenWidth * screenHeight];	//	Создание массива для экрана

	thread monsterMovement([&]() {
		while (!gameIsOver && !settingsIsOpen) {
			//	Движение монстра
			monsterInfo->movement(
				mapInfo->mapSizeVertical,
				mapInfo->mapSizeHorizontal,
				mapInfo->map,
				playerInfo->x,
				playerInfo->y,
				distans);

			//	Если монстр может кдарить бьет
			if (monsterInfo->canHit) {
				monsterInfo->setAttributeFrontColor(4);
				playerInfo->setHp(monsterInfo->hitPlayer(playerInfo->getHp()));
				mciSendString(L"play sounds/hitPlayer2.wav", NULL, SND_ASYNC, NULL);
			}
			else monsterInfo->setAttributeFrontColor(8);

			//	Если игрок умер
			if (!frameInfoIsBuild && playerInfo->getHp() == 0) {
				this_thread::sleep_for(chrono::milliseconds(100));
				gameIsOver = true;
				mciSendString(L"play sounds/die.wav wait", NULL, 0, NULL);
			}

		}});
	thread playPlayerMovementSound([&]()
		{
			while (!gameIsOver && !playerMovedToNextFloor)
			{
				if (playerInfo->getGo() && !playerInfo->getRun())
				{
					int waitingTime = 1420;
					PlaySound(L"sounds/go x1.wav", NULL, SND_ASYNC);
					this_thread::sleep_for(chrono::milliseconds(waitingTime));
				}
				if (playerInfo->getGo() && playerInfo->getRun())
				{
					int waitingTime = 640;
					PlaySound(L"sounds/run x1.wav", NULL, SND_ASYNC);
					this_thread::sleep_for(chrono::milliseconds(waitingTime));
				}
			}
		});	//	Звуки ходьбы
	thread playGameSounds([&]()
		{

			while (!gameIsOver && !playerMovedToNextFloor)
			{
				vector<wstring> listAllSounds
				{
					L"sounds/gameSound1.wav",
					L"sounds/gameSound2.wav",
					L"sounds/gameSound3.wav",
					L"sounds/gameSound4.wav"
				};
				wstring randomSound;
				randomSound = listAllSounds[rand() % listAllSounds.size()];
				//mciSendString((L"play " + randomSound).c_str(), NULL, 0, 0);
				mciSendString((L"play " + randomSound).c_str(), NULL, 0, 0);
			}
		});			//	Звуки задеого фона игры
	thread checkEnergyInfo([&]() {
		while (!gameIsOver && !playerMovedToNextFloor)
		{
			if (playerInfo->getRun())
			{
				while (playerInfo->getAmountEnergy() >= 0 && playerInfo->getRun())
				{
					bool motion = playerInfo->getGo();
					if (motion)
					{
						playerInfo->setAmountEnergy((playerInfo->getAmountEnergy() -
							(playerInfo->getInitialAmountEnergy() / 10)));
						this_thread::sleep_for(chrono::milliseconds(400));
					}
					if (!motion)
					{
						break;
					}
				}
				playerInfo->setSpeed(playerInfo->getInitialSpeed());
				playerInfo->setRun(false);
			}
			if (!playerInfo->getRun())
			{
				while (playerInfo->getAmountEnergy() < playerInfo->getInitialAmountEnergy() && !playerInfo->getRun())
				{
					if (playerInfo->getAmountEnergy() + (playerInfo->getInitialAmountEnergy() / 10) != playerInfo->getInitialAmountEnergy())
					{
						this_thread::sleep_for(chrono::milliseconds(1500));
					}
					playerInfo->setAmountEnergy(playerInfo->getAmountEnergy() +
						playerInfo->getInitialAmountEnergy() / 10);
				}
			}
		}
		});			//	Статус энергии игрока
	thread checkStatusKeyInfo([&]()
		{
			while (!gameIsOver && !playerMovedToNextFloor)
			{
				if (_kbhit() && !settingsIsOpen)
				{
					//	Чекаем игрок в телепорте или нет 
					checkPlayerUseObject();
					//	Проигрываем звуки при нажатии на спец клавиши ( ESCAPE, F, M )
					switch (_getch())
					{
					case 109:
						mciSendString(L"play sounds/buttonsInfo.wav", NULL, SND_ASYNC, 0);
						displayMap = displayMap ? false : true;
						break;
					case 102:
						mciSendString(L"play sounds/buttonsInfo.wav", NULL, SND_ASYNC, 0);
						displayFps = displayFps ? false : true;
						break;
					case 27:
						mciSendString(L"play sounds/buttonsInfo.wav", NULL, SND_ASYNC, 0);
						settingsIsOpen = true;
						openSettings();
						setCursoreVisible();
						settingsIsOpen = false;
						break;
					}

				}
			}
		});		//	Статус клавиш специального назначения
	thread generateAndDisplayFrame([&]() {
		while (!gameIsOver && !playerMovedToNextFloor)
		{
			startFrameGeneration = chrono::high_resolution_clock::now();
			if (!settingsIsOpen)
			{

				playerInfo->motion(mapInfo->map, mapInfo->mapSizeHorizontal, frameGenerationTimeInSeconds);
				generateInfoFrame();
				generateFrame();
				WriteConsoleOutput(console, screen, consoleBufferSize, { 0,0 }, &windowSize);
			}
			endFrameGeneration = chrono::high_resolution_clock::now();
			frameGenerationTimeInSeconds = chrono::duration<double>(endFrameGeneration - startFrameGeneration).count();
		}
		});	//	Генерация кадра

	while (true) {}
	//std:cout << "gfgfgfg";
	//PlaySound(nullptr, NULL, 0);
	//	НЕРАБОТАЕТ ПРЕРЫВАНИЕ ЗВУКА
	//mciSendString((L"close " + randomSound).c_str(), nullptr, 0, nullptr);
	system("cls");

	//	Завершение открытых потоков
	checkStatusKeyInfo.join();
	checkEnergyInfo.join();
	playPlayerMovementSound.join();
	playGameSounds.join();
	generateAndDisplayFrame.join();
	return;

}