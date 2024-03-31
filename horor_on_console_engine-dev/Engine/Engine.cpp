#include "Library.h"
#include "IMapInfo.h"
#include "IPlayer.h"
#include "IEngine.h"
#include "IScreensavers.h"

Engine::Engine(Player& _playerInfo, MapInfo& _mapInfo)
{
	playerInfo = &_playerInfo;
	mapInfo = &_mapInfo;
	startFrameGeneration = chrono::high_resolution_clock::now();
	endFrameGeneration = chrono::high_resolution_clock::now();
	console = GetStdHandle(STD_OUTPUT_HANDLE);
	drawingRange = 10;
	fov = 3.14159f / 2;
	playerMovedToNextFloor = false;
	gameIsOver = false;
	displayMap = false;
	settingsIsOpen = false;
	frameIsBuild = true;
}
bool Engine::checkPlayerInTeleport()
{
	if (int(playerInfo->x) == mapInfo->finishCoordinat.first
		&& int(playerInfo->y) == mapInfo->finishCoordinat.second)
		return true;
	return false;
}
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
void Engine::setScreenSize()
{
	consoleBufferSize = { short(screenWidth), short(screenHeight) };
	windowSize = { 0, 0, short(screenWidth - 1), short(screenHeight - 1) };
	SetConsoleScreenBufferSize(console, consoleBufferSize);
	SetConsoleWindowInfo(console, TRUE, &windowSize);
}
void Engine::setCursoreVisible()
{
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(console, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(console, &cursorInfo);
}
void Engine::generateInfoFrame()
{
	std::wstringstream stream;
	int frames = int(1 / frameGenerationTimeInSeconds);
	stream << L"[ FPS: " << frames
		<< (frames < 10 ? L"   ]" :
			(frames >= 10 && frames < 100) ? L"  ]" :
			(frames >= 100 && frames < 1000) ? L" ]" : L"]")
		<< L" [ X: " << std::fixed << std::setprecision(1) << playerInfo->x
		<< (playerInfo->x < 10 ? L"   ]" :
			(playerInfo->x < 100 && playerInfo->x >= 10 ? L"  ]" :
				(playerInfo->x < 1000 && playerInfo->x >= 100 ? L" ]" : L"]")))
		<< L" [ Y: " << std::fixed << std::setprecision(1) << playerInfo->y
		<< (playerInfo->y < 10 ? L"   ]" :
			(playerInfo->y < 100 && playerInfo->y >= 10 ? L"  ]" :
				(playerInfo->y < 1000 && playerInfo->y >= 100 ? L" ]" : L"]")))
		<< L"  SPEED: " << "[ " << playerInfo->speed << " ]"
		<< L" RUN: " << (playerInfo->run ? L"[ true  ] " : L"[ false ] ");
	wstring fpsString = stream.str();
	wstring minimap = L" ~ MAP ~ ";
	wstring energyInfo = L" ENERGY";
	wstring energyTexture = L"";
	wstring hpTexture = L"";
	wstring hp = L" HEALTH";

	for (int x = 0; x < playerInfo->initialAmountEnergy; x += playerInfo->initialAmountEnergy / 10)
	{
		if (x <= playerInfo->amountEnergy) energyTexture += L" ▒✦▒";
		else energyTexture += L" ░✧░";
	}
	for (int x = 0; x < energyTexture.length(); x++)
	{
		int value = screenWidth * (screenHeight - 2) + energyInfo.length();
		screen[value + x].Char.UnicodeChar = energyTexture[x];
		screen[x + value].Attributes = 6;
	}

	for (int i = 0; i < playerInfo->initialHp; i += playerInfo->initialHp / 10)
	{
		if (i <= playerInfo->hp) hpTexture += L" ▒❤︎▒";
		//\u2665
		else hpTexture += L" ▒░░▒";
	}
	for (int i = 0; i < energyInfo.length(); i++)
	{
		int value = screenWidth * (screenHeight - 2);
		screen[i + value].Char.UnicodeChar = energyInfo[i];
		screen[i + value].Attributes = 15;
	}
	if (displayFps)
	{
		for (int i = 0; i < fpsString.size(); ++i)
		{
			screen[i].Char.UnicodeChar = fpsString[i];
			screen[i].Attributes = 11;
		}
	}
	for (int i = 0; i < hp.length(); i++)
	{
		screen[i + screenWidth * (screenHeight - 1)].Char.UnicodeChar = hp[i];
		screen[i + screenWidth * (screenHeight - 1)].Attributes = 15;
	}
	for (int i = 0; i < hpTexture.size(); i++)
	{
		int value = screenWidth * (screenHeight - 1) + hp.length();
		screen[i + value].Char.UnicodeChar = hpTexture[i];
		if (i % 2 == 0) screen[i + value].Attributes = 4;
		else screen[i + value].Attributes = 4;
	}
	if (displayMap)
	{
		frameInfoIsBuild = true;
		for (int i = 0; i < minimap.size(); ++i)
		{
			screen[i + screenWidth * 2].Char.UnicodeChar = minimap[i];
			screen[i + screenWidth * 2].Attributes = 7;
		}
		for (int x = 0; x < mapInfo->mapSizeHorizontal; x++)
			for (int y = 0; y < mapInfo->mapSizeVertical; y++)
			{
				int index = (y * mapInfo->mapSizeHorizontal) + x;
				if (x < screenWidth - 3 && y < screenHeight - 3)
				{
					screen[(y + 3) * screenWidth + x].Char.UnicodeChar = mapInfo->map[index];
					screen[(y + 3) * screenWidth + x].Attributes = 15;
				}
			}
		if (playerInfo->y < screenHeight - 3
			&& playerInfo->x < screenWidth - 3
			&& mapInfo->finishCoordinat.first < screenWidth - 3
			&& mapInfo->finishCoordinat.second < screenHeight - 3)
		{
			screen[((int)playerInfo->y + 3) * screenWidth + (int)playerInfo->x].Char.UnicodeChar = L'☻';
			screen[((int)playerInfo->y + 3) * screenWidth + (int)playerInfo->x].Attributes = 10;
			screen[(mapInfo->startCoordinat.second + 3) * screenWidth + (mapInfo->startCoordinat.first)].Char.UnicodeChar = ' ';
			screen[(mapInfo->startCoordinat.second + 3) * screenWidth + (mapInfo->startCoordinat.first)].Attributes = 5;
			screen[(mapInfo->finishCoordinat.second + 3) * screenWidth + (mapInfo->finishCoordinat.first)].Char.UnicodeChar = L'✡︎';
			screen[(mapInfo->finishCoordinat.second + 3) * screenWidth + (mapInfo->finishCoordinat.first)].Attributes = 9;
		}
		frameInfoIsBuild = false;
	}
}
void Engine::openSettings()
{
	int startX = screenWidth / 2;
	int startY = screenHeight / 2 - 5;
	int command = 5;
	bool treker = true;
	vector<string> items{
		"         DRAWING RANGE         ",
		"         WALKING SPEED         ",
		"          SENSITIVITY          ",
		"              FOV              ",
		"          SCREEN SIZE          ",
		"           CONTINUE            ",
		"         EXIT THE GAME         " };

	system("cls");
	while (treker) {
		printf("\x1b[%d;%dH", startY - 5, startX - 15);
		printf("\x1b[0m:::::::: \x1b[93mSETTINGS MENU\x1b[0m ::::::::\n\n\x1b[90m\x1b[48; 5; 0m");
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
		printf("\x1b[0m:::::::::::::::::::::::::::::::");

		switch (_getwch()) {
		case arrowUp: command = command - 1 >= 0 ? command -= 1 : items.size() - 1;
			break;
		case arrowDown: command = command < items.size() - 1 ? command += 1 : 0;
			break;
		case enter: treker = false;
			break;
		}
	}
	treker = true;
	system("cls");
	switch (command) {
	case 0:// УРОВЕНЬ ПРОРИСОВКИ
		while (treker) {
			printf("\x1b[%d;%dH", startY - 3, startX - 25);
			printf("\x1b[90m~~~~~~~~~~~~~~~~ DRAWING LEVEL ~~~~~~~~~~~~~~~~");
			printf("\x1b[%d;%dH", startY - 1, startX - 25);
			for (int i = 1; i <= drawingRange - 1; i++) {
				if (i < 5) printf("\x1b[91m");
				else if (i < 8) printf("\x1b[38;5;208m");
				else if (i < 11) printf("\x1b[92m");
				else printf("\x1b[92m");
				cout << "[" << i << "]";
			}
			cout << "                                                   ";
			printf("\x1b[%d;%dH", startY + 1, startX - 25);
			printf(" \x1b[91m~\x1b[0m Управление \x1b[93m <-+->");
			printf("\x1b[%d;%dH", startY + 3, startX - 25);
			cout << "\x1b[90m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\x1b[0m";
			printf("\x1b[%d;%dH", startY + 5, startX - 25);
			cout << "\x1b[90m <== { \x1b[97mesc\x1b[90m }";

			switch (_getwch()) {
			case arrowLeft: drawingRange = drawingRange - 1 >= 2 ? drawingRange -= 1 : 15;
				break;
			case arrowRight: drawingRange = drawingRange + 1 <= 15 ? drawingRange += 1 : 2;
				break;
			case escape: treker = false;
				break;
			}
		}
		break;
	case 1://	CКОРОСТЬ
		playerInfo->speed = playerInfo->initialSpeed;
		while (treker)
		{
			printf("\x1b[%d;%dH", startY - 3, startX - 15);
			printf("\x1b[0m______Настройки скорости______\n\n\x1b[0m");
			printf("\x1b[%d;%dH", startY - 1, startX - 15);
			for (int i = 1; i <= playerInfo->speed; i++) {
				if (i < 3) printf("\x1b[91m");
				else if (i < 4)  printf("\x1b[38;5;208m");
				else if (i < 6) printf("\x1b[92m");
				else if (i < 7)  printf("\x1b[38;5;208m");
				else printf("\x1b[91m");
				cout << "[" << i << "]";
			}
			cout << "                                       ";
			printf("\x1b[%d;%dH", startY + 1, startX - 15);
			printf(" \x1b[91m~\x1b[0m Управление \x1b[93m <-+->");
			printf("\x1b[%d;%dH", startY + 2, startX - 15);
			cout << "\x1b[0m______________________________";
			printf("\x1b[%d;%dH", startY + 4, startX - 15);
			printf("\x1b[90m <== { \x1b[0mesc\x1b[90m }");

			switch (_getwch())
			{
			case arrowLeft: playerInfo->speed = playerInfo->speed - 1 >= 1 ? playerInfo->speed -= 1 : 8;
				break;
			case arrowRight: playerInfo->speed = playerInfo->speed + 1 <= 8 ? playerInfo->speed += 1 : 1;
				break;
			case escape: treker = false;
				break;
			}
		}
		playerInfo->initialSpeed = playerInfo->speed;
		break;
	case 2://	ЧУВСТВИТЕЛЬНОСТЬ
		while (treker) {
			printf("\x1b[%d;%dH", startY - 3, startX - 16);
			printf("\x1b[0m___Настройки чувствительности___\x1b[0m\n");
			printf("\x1b[%d;%dH", startY - 1, startX - 16);
			for (double i = 1; i <= playerInfo->sensitivity; i += 1) {
				if (i < 3) printf("\x1b[91m");
				else if (i < 4) printf("\x1b[38;5;208m");
				else if (i < 7) printf("\x1b[92m");
				else if (i < 9) printf("\x1b[38;5;208m");
				else printf("\x1b[91m");
				cout << "[" << i << "]";
			}
			cout << "                                       ";
			printf("\x1b[%d;%dH", startY + 1, startX - 16);
			printf("\x1b[91m~ \x1b[0mУправление  \x1b[93m<-+->");
			printf("\x1b[%d;%dH", startY + 2, startX - 16);
			printf("\x1b[0m________________________________\x1b[0m\n");
			printf("\x1b[%d;%dH", startY + 4, startX - 16);
			printf("\x1b[90m <== { \x1b[0mesc\x1b[90m }");

			switch (_getwch()) {
			case arrowLeft: playerInfo->sensitivity = playerInfo->sensitivity - 1 >= 1 ? playerInfo->sensitivity -= 1 : 10;
				break;
			case arrowRight: playerInfo->sensitivity = playerInfo->sensitivity + 1 <= 10 ? playerInfo->sensitivity += 1 : 1;
				break;
			case escape: treker = false;
				break;
			}
		}
		break;
	case 3://	ПОЛЕ ЗРЕНИЯ

		while (treker) {
			printf("\x1b[%d;%dH", startY - 3, startX - 14);
			printf("\x1b[0m___ Настройки поля зрения ___\n\n\x1b[0m");
			printf("\x1b[%d;%dH", startY - 1, startX - 14);
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
			printf("\x1b[91m~\x1b[0m Управление \x1b[93m <-+->");
			printf("\x1b[%d;%dH", startY + 2, startX - 14);
			printf("\x1b[0m_____________________________");
			printf("\x1b[%d;%dH", startY + 4, startX - 14);
			printf("\x1b[90m <== { \x1b[0mesc\x1b[90m }");

			switch (_getwch())
			{
			case arrowLeft: fov = fov - 0.5 > 0 ? fov -= 0.5 : 3.5;
				break;
			case arrowRight: fov = fov + 0.5 < 4 ? fov += 0.5 : 0.5;
				break;
			case escape: treker = false;
				break;
			}
		}
		break;
	case 4://	ЭКРАН
		while (treker)
		{
			getConsoleSize();
			if (startX != screenWidth / 2 && startY != screenHeight / 2 - 5)
			{
				startX = screenWidth / 2;
				startY = screenHeight / 2 - 5;
				system("cls");
			}

			printf("\x1b[%d;%dH", startY - 3, startX - 20);
			printf("\x1b[90m~~~~~~~~~~~~ Настройка экрана ~~~~~~~~~~~~\x1b[0m");
			printf("\x1b[%d;%dH", startY + 2 - 3, startX - 20);
			printf("\x1b[90m # \x1b[94mРастените окно до нужного размера\x1b[90m. . .");
			printf("\x1b[%d;%dH", startY + 4 - 3, startX - 20);
			printf("\x1b[91m ~\x1b[0m Screen size \x1b[90m[ \x1b[93m");
			cout << (to_string(screenWidth) + " \x1b[90mx\x1b[93m " + to_string(screenHeight) + " \x1b[90m]");
			printf("\x1b[%d;%dH", startY + 6 - 3, startX - 20);
			printf("\x1b[90m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\x1b[0m");
			printf("\x1b[%d;%dH", startY + 8 - 3, startX - 20);
			printf("\x1b[90m <== { \x1b[0mesc\x1b[90m }");

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
	case 5:
		return;
	case 6:
		exit(0);
		break;
	}
	openSettings();
}
void Engine::generateFrame()
{
	if (!settingsIsOpen && !playerMovedToNextFloor)
	{
		frameIsBuild = true;
		mapInfo->clearmap();
		startFrameGeneration = chrono::high_resolution_clock::now();

		for (int x = 0; x < screenWidth; x++) {
			double rayAngle = playerInfo->r + fov / 2.0f - x * fov / screenWidth;
			double rayX = sinf(rayAngle);
			double rayY = cosf(rayAngle);
			double distanceWall = 0;
			bool itWall = false;
			bool itTeleport = false;
			bool itRestoringHealth = false;
			bool itMonster = false;
			bool itBound = false;

			while (!itWall && distanceWall < drawingRange) {
				distanceWall += 0.01f;
				int testX = (int)(playerInfo->x + rayX * distanceWall);
				int testY = (int)(playerInfo->y + rayY * distanceWall);

				if (mapInfo->map[testY * mapInfo->mapSizeHorizontal + testX] == L'*') { itRestoringHealth = true; }
				if (mapInfo->map[testY * mapInfo->mapSizeHorizontal + testX] == L'&') { itTeleport = true; }
				if (testX < 0 || testX >= mapInfo->mapSizeHorizontal || testY < 0 || testY >= mapInfo->mapSizeVertical) {
					itWall = true;
					distanceWall = drawingRange;
				}
				else if (mapInfo->map[testY * mapInfo->mapSizeHorizontal + testX] == L'#' || itTeleport) {
					itWall = true;
					vector<pair<double, double>> boundsVector;
					for (int tx = 0; tx < 2; tx++) {
						for (int ty = 0; ty < 2; ty++) {
							double vectorX = testX + tx - playerInfo->x;
							double vectorY = testY + ty - playerInfo->y;
							double vectorModule = sqrt(vectorX * vectorX + vectorY * vectorY);
							double cosAngle = rayX * vectorX / vectorModule + rayY * vectorY / vectorModule;
							boundsVector.push_back(make_pair(vectorModule, cosAngle));
						}
					}
					sort(boundsVector.begin(), boundsVector.end(), [&](const pair<double, double>& point1, const pair<double, double>& point2) {
						double module1 = sqrt(point1.first * point1.first + point1.second * point1.second);
						double module2 = sqrt(point2.first * point2.first + point2.second * point2.second);
						return module1 < module2;
						});
					double boundAngle = 0.03 / distanceWall;
					if ((acos(boundsVector[0].second) < boundAngle && distanceWall > 0.5)
						|| (acos(boundsVector[1].second) < boundAngle && distanceWall > 0.5)) {
						itBound = true;
					}
				}
				else { mapInfo->map[testY * mapInfo->mapSizeHorizontal + testX] = '.'; }
			}

			int celling = (float)(screenHeight / 2.0) - screenHeight / ((float)distanceWall);
			int floor = screenHeight - celling;

			for (int y = 0; y < screenHeight; y++)
			{
				double centerScreen = 1 - double(y - screenHeight / 2) / (screenHeight / 2);
				if ((displayFps ? (x > 69 || y != 0) : true)	//	FPS
					&& (y != screenHeight - 1 || x > 56)		//	HP
					&& (y != screenHeight - 2 || x > 46)		//	ENERGY
					&& (displayMap ?							//	MAP
						(x > (mapInfo->mapSizeHorizontal - 1)
							|| (y < 3 || y >(mapInfo->mapSizeVertical + 2))
							&& (y != 2 || x > 8)) : true))
				{
					if (y <= celling)
					{
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
					else if (y > celling && y <= floor)
					{
						wchar_t wallTexture;
						if (distanceWall <= drawingRange / 5)
							wallTexture = L'█';
						else if (distanceWall <= drawingRange / 4)
							wallTexture = L'▓';
						else if (distanceWall <= drawingRange / 3)
							wallTexture = L'▒';
						else if (distanceWall <= drawingRange / 1)
							wallTexture = L'░';
						else
							wallTexture = ' ';

						if (itBound) screen[y * screenWidth + x].Attributes = 0;
						else screen[y * screenWidth + x].Attributes = 8;

						if (itTeleport)
						{
							wchar_t teleportTexture;
							if (itBound)
							{
								wallTexture = L' ';
								screen[y * screenWidth + x].Attributes = 15;
							}
							else
							{
								wallTexture = (rand() % 10) % 3 == 0 ? L' ' : L'✦';
								screen[y * screenWidth + x].Attributes = (rand() % 10) % 5 == 0 ? 15 : 5;
							}
						}
						else
						{
							if (itBound)
							{
								wallTexture = L' ';
								screen[y * screenWidth + x].Attributes = 8;
							}
						}
						screen[y * screenWidth + x].Char.UnicodeChar = wallTexture;
					}
					else
					{
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
		frameIsBuild = false;
		endFrameGeneration = chrono::high_resolution_clock::now();
		frameGenerationTimeInSeconds = chrono::duration<double>(endFrameGeneration - startFrameGeneration).count();
	}
}
void Engine::start()
{
	screen = new CHAR_INFO[screenWidth * screenHeight];
	thread statusKeyInfo([&]()
		{
			while (!gameIsOver)
			{
				if (_kbhit() && !settingsIsOpen)
				{
					switch (_getwch())
					{
					case 109:
						displayMap = displayMap ? false : true;
						break;
					case 102:
						displayFps = displayFps ? false : true;
						break;
					case 27:
						settingsIsOpen = true;
						openSettings();
						setCursoreVisible();
						settingsIsOpen = false;
						break;
					case 32:
						playerInfo->run = !playerInfo->run ? true : false;
						!playerInfo->run || playerInfo->amountEnergy == 0 ? playerInfo->speed = playerInfo->initialSpeed : playerInfo->speed += 2;
						break;
					}

				}
			}

		});
	thread checkEnergyInfo([&]() {
		while (!gameIsOver)
		{
			if (playerInfo->run)
			{
				while (playerInfo->amountEnergy >= 0 && playerInfo->run)
				{
					playerInfo->amountEnergy -= playerInfo->initialAmountEnergy / 10;
					this_thread::sleep_for(chrono::milliseconds(200));
				}
				playerInfo->speed = playerInfo->initialSpeed;
				playerInfo->run = false;
			}
			if (!playerInfo->run)
			{
				while (playerInfo->amountEnergy < playerInfo->initialAmountEnergy && !playerInfo->run)
				{
					playerInfo->amountEnergy += playerInfo->initialAmountEnergy / 10;
					this_thread::sleep_for(chrono::milliseconds(700));
				}
			}
		}
		});
	thread displayFrameAndGenerateHelpersForFrame([&]()
		{
			while (!gameIsOver)
			{
				if (!settingsIsOpen)
				{
					generateInfoFrame();
					WriteConsoleOutput(console, screen, consoleBufferSize, { 0,0 }, &windowSize);
				}
			}
		});
	thread generateFrame([&]() {
		while (!gameIsOver)
		{
			if (!settingsIsOpen)
			{
				generateFrame();
				playerInfo->motion(mapInfo->map, mapInfo->mapSizeHorizontal, frameGenerationTimeInSeconds);
			}
		}
		});
	thread playSoundGame([&]() {
		vector<wstring> listSounds{
			L"sounds/gameSound1.wav",
			L"sounds/gameSound2.wav",
			L"sounds/gameSound3.wav",
			L"sounds/gameSound4.wav"
		};
		while (!gameIsOver)
		{
			PlaySound(listSounds[rand() % listSounds.size()].c_str(), NULL, SND_ASYNC);
			this_thread::sleep_for(chrono::seconds(149));
		}
		});
	while (!gameIsOver)
	{
		if (checkPlayerInTeleport())
		{
			statusKeyInfo.detach();
			displayFrameAndGenerateHelpersForFrame.detach();
			generateFrame.detach();
			checkEnergyInfo.detach();
			return;
		}
	}
}