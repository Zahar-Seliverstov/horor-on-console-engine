#include "Library.h"
#include "IMapInfo.h"
#include "IPlayer.h"
#include "IEngine.h"
#include "IScreensavers.h"

MapInfo mapInfo;
Player player;

enum keys
{
	escape = 27,
	enter = 13,
	arrowLeft = 75,
	arrowRight = 77,
	arrowUp = 72,
	arrowDown = 80,
	keyA = 97,
	keyD = 100,
	keyW = 119,
	keyS = 115
};
Engine::Engine()
{
	levelDrawing = 10;
	fov = 3.14159f / 2;
	gameOver = false;
	printMinimap = false;
}
void Engine::checkPlayerInTeleport()
{
	if (int(player.x) == mapInfo.finishCoordinat.first
		&& int(player.y) == mapInfo.finishCoordinat.second)
	{
		/*mapInfo.createmap();
		player.x = mapInfo.startCoordinat.first;
		player.y = mapInfo.startCoordinat.second;*/
		system("cls");
		DownloadScreensaver(screenWidth, screenHeight);
		system("cls");
		Run();
	}
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
	bufferSize = { short(screenWidth), short(screenHeight) };
	windowSize = { 0, 0, short(screenWidth - 1), short(screenHeight - 1) };
	SetConsoleScreenBufferSize(console, bufferSize);
	SetConsoleWindowInfo(console, TRUE, &windowSize);
}
void Engine::cursoreVisibleFalse()
{
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(console, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(console, &cursorInfo);
}
void Engine::outputInfo()
{
	std::wstringstream stream;
	stream << L" [ FPS: " << int(1 / timeInSeconds)
		<< L" ] [ X: " << std::fixed << std::setprecision(2) << player.x
		<< L" ] [ Y: " << std::fixed << std::setprecision(2) << player.y
		<< L" ]  ";
	wstring fpsString = stream.str();
	wstring minimap = L" ~ map ~ ";
	wstring hpTexture = L"";
	wstring hp = L" ~ HP ~ ";
	for (int i = 0; i <= player.initialHp; i += player.initialHp / 10)
	{
		if (i <= player.hp) hpTexture += L" [\u2665]";
		else hpTexture += L" [X]";
	}
	for (int i = 0; i < fpsString.size(); ++i)
	{
		calculationScreen[i].Char.UnicodeChar = fpsString[i];
		calculationScreen[i].Attributes = 11;
	}
	for (int i = 0; i < hp.length(); i++)
	{
		calculationScreen[i + screenWidth * (screenHeight - 2)].Char.UnicodeChar = hp[i];
		calculationScreen[i + screenWidth * (screenHeight - 2)].Attributes = 7;
	}
	for (int i = 0; i < hpTexture.size(); i++)
	{
		calculationScreen[i + screenWidth * (screenHeight - 1)].Char.UnicodeChar = hpTexture[i];
		if (i % 2 == 0) calculationScreen[i + screenWidth * (screenHeight - 1)].Attributes = 4;
		else calculationScreen[i + screenWidth * (screenHeight - 1)].Attributes = 15;
	}
	if (printMinimap)
	{
		for (int i = 0; i < minimap.size(); ++i)
		{
			calculationScreen[i + screenWidth * 2].Char.UnicodeChar = minimap[i];
			calculationScreen[i + screenWidth * 2].Attributes = 7;
		}
		for (int x = 0; x < mapInfo.mapSizeHorizontal; x++)
			for (int y = 0; y < mapInfo.mapSizeVertical; y++)
			{
				int index = (y * mapInfo.mapSizeHorizontal) + x;
				if (x < screenWidth - 3 && y < screenHeight - 3)
				{
					calculationScreen[(y + 3) * screenWidth + x].Char.UnicodeChar = mapInfo.map[index];
					calculationScreen[(y + 3) * screenWidth + x].Attributes = 15;
				}
			}
		calculationScreen[((int)player.y + 3) * screenWidth + (int)player.x].Char.UnicodeChar = L'P';
		calculationScreen[((int)player.y + 3) * screenWidth + (int)player.x].Attributes = 10;
		calculationScreen[(mapInfo.startCoordinat.second + 3) * screenWidth + (mapInfo.startCoordinat.first)].Char.UnicodeChar = ' ';
		calculationScreen[(mapInfo.startCoordinat.second + 3) * screenWidth + (mapInfo.startCoordinat.first)].Attributes = 5;
		calculationScreen[(mapInfo.finishCoordinat.second + 3) * screenWidth + (mapInfo.finishCoordinat.first)].Char.UnicodeChar = 'F';
		calculationScreen[(mapInfo.finishCoordinat.second + 3) * screenWidth + (mapInfo.finishCoordinat.first)].Attributes = 9;

	}
}
void Engine::settings()
{
	int startX = screenWidth / 2;
	int startY = screenHeight / 2 - 5;
	int command = 5;
	bool treker = true;
	vector<string> items{
		"   [^] Дальность прорисовки    ",
		"   [S] Скорость ходьбы         ",
		"   [-] Чувствительность        ",
		"   [R] Поле зрения             ",
		"   [S] Размер экрана           ",
		"   [<] Продолжить              ",
		"   [X] Выйти из игры           " };

	system("cls");
	while (treker) {
		printf("\x1b[%d;%dH", startY - 5, startX - 15);
		printf("\x1b[0m________ \x1b[93mМеню настроек\x1b[0m ________\n\n\x1b[90m\x1b[48; 5; 0m");
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
		printf("\x1b[%d;%dH", startY + 9 - 5, startX - 15);
		printf("\x1b[0m_______________________________");

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
			printf("\x1b[0m__________ Настройки уровня прорисовки __________");
			printf("\x1b[%d;%dH", startY - 1, startX - 25);
			for (int i = 1; i <= levelDrawing - 1; i++) {
				if (i < 5) printf("\x1b[91m");
				else if (i < 8) printf("\x1b[38;5;208m");
				else if (i < 11) printf("\x1b[92m");
				else printf("\x1b[92m");
				cout << "[" << i << "]";
			}
			cout << "                                                   ";
			printf("\x1b[%d;%dH", startY + 1, startX - 25);
			printf(" \x1b[91m~\x1b[0m Управление \x1b[93m <-+->");
			printf("\x1b[%d;%dH", startY + 2, startX - 25);
			cout << "\x1b[0m_________________________________________________";
			printf("\x1b[%d;%dH", startY + 4, startX - 25);
			cout << "\x1b[90m <== { \x1b[97mesc\x1b[90m }";

			switch (_getwch()) {
			case arrowLeft: levelDrawing = levelDrawing - 1 >= 2 ? levelDrawing -= 1 : 15;
				break;
			case arrowRight: levelDrawing = levelDrawing + 1 <= 15 ? levelDrawing += 1 : 2;
				break;
			case escape: treker = false;
				break;
			}
		}
		break;
	case 1://	CКОРОСТЬ
		while (treker)
		{
			printf("\x1b[%d;%dH", startY - 3, startX - 15);
			printf("\x1b[0m______Настройки скорости______\n\n\x1b[0m");
			printf("\x1b[%d;%dH", startY - 1, startX - 15);
			for (int i = 1; i <= player.speed; i++) {
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
			case arrowLeft: player.speed = player.speed - 1 >= 1 ? player.speed -= 1 : 8;
				break;
			case arrowRight: player.speed = player.speed + 1 <= 8 ? player.speed += 1 : 1;
				break;
			case escape: treker = false;
				break;
			}
		}
		break;
	case 2://	ЧУВСТВИТЕЛЬНОСТЬ
		while (treker) {
			printf("\x1b[%d;%dH", startY - 3, startX - 16);
			printf("\x1b[0m___Настройки чувствительности___\x1b[0m\n");
			printf("\x1b[%d;%dH", startY - 1, startX - 16);
			for (double i = 1; i <= player.sensitivity; i += 1) {
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
			case arrowLeft: player.sensitivity = player.sensitivity - 1 >= 1 ? player.sensitivity -= 1 : 10;
				break;
			case arrowRight: player.sensitivity = player.sensitivity + 1 <= 10 ? player.sensitivity += 1 : 1;
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
			startX = screenWidth / 2;
			startY = screenHeight / 2 - 5;

			system("cls");
			getConsoleSize();
			printf("\x1b[%d;%dH", startY - 3, startX - 20);
			printf("\x1b[0m____________ \x1b[0mНастройка экрана\x1b[0m ____________");
			printf("\x1b[%d;%dH", startY + 2 - 3, startX - 20);
			printf("\x1b[90m # \x1b[94mРастените окно до нужного размера\x1b[90m. . .");
			printf("\x1b[%d;%dH", startY + 4 - 3, startX - 20);
			printf("\x1b[91m ~\x1b[0m Размеры \x1b[90m[ \x1b[93m");
			cout << (to_string(screenWidth) + " \x1b[90mx\x1b[93m " + to_string(screenHeight) + " \x1b[90m]");
			printf("\x1b[%d;%dH", startY + 5 - 3, startX - 20);
			printf("\x1b[0m__________________________________________");
			printf("\x1b[%d;%dH", startY + 7 - 3, startX - 20);
			printf("\x1b[90m <== { \x1b[0mesc\x1b[90m }");

			if (_kbhit()) {
				switch (_getch()) {
				case escape: treker = false;
				}
			}
			this_thread::sleep_for(chrono::milliseconds(400));
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
	settings();
}
void Engine::renderingConsoleGraphics()
{
	calculationScreen = new CHAR_INFO[screenWidth * screenHeight];
	while (!gameOver) {
		mapInfo.clearmap();
		timeFinish = chrono::high_resolution_clock::now();
		timeInSeconds = chrono::duration<double>(timeFinish - timeStart).count();
		timeStart = chrono::high_resolution_clock::now();
		player.motion(mapInfo.map, mapInfo.mapSizeHorizontal, timeInSeconds);
	/*	if (_kbhit()) {
			switch (_getwch()) {
			case 109: printMinimap = printMinimap ? false : true;
				break;
			case 27: settings();
				break;
			}
			cursoreVisibleFalse();
		}*/
		for (int x = 0; x < screenWidth; x++) {
			double rayAngle = player.r + fov / 2.0f - x * fov / screenWidth;
			double rayX = sinf(rayAngle);
			double rayY = cosf(rayAngle);
			double distanceWall = 0;
			bool itWall = false;
			bool itTeleport = false;
			bool itMonster = false;
			bool itBound = false;

			while (!itWall && distanceWall < levelDrawing) {
				distanceWall += 0.01f;
				int testX = (int)(player.x + rayX * distanceWall);
				int testY = (int)(player.y + rayY * distanceWall);

				if (mapInfo.map[testY * mapInfo.mapSizeHorizontal + testX] == L'&') { itTeleport = true; }
				if (testX < 0 || testX >= mapInfo.mapSizeHorizontal || testY < 0 || testY >= mapInfo.mapSizeVertical) {
					itWall = true;
					distanceWall = levelDrawing;
				}
				else if (mapInfo.map[testY * mapInfo.mapSizeHorizontal + testX] == L'#' || itTeleport) {
					itWall = true;
					vector<pair<double, double>> boundsVector;
					for (int tx = 0; tx < 2; tx++) {
						for (int ty = 0; ty < 2; ty++) {
							double vectorX = testX + tx - player.x;
							double vectorY = testY + ty - player.y;
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
				else { mapInfo.map[testY * mapInfo.mapSizeHorizontal + testX] = '.'; }
			}

			int celling = (float)(screenHeight / 2.0) - screenHeight / ((float)distanceWall);
			int floor = screenHeight - celling;

			for (int y = 0; y < screenHeight; y++)
			{
				double centerScreen = 1 - double(y - screenHeight / 2) / (screenHeight / 2);

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

					calculationScreen[y * screenWidth + x].Char.UnicodeChar = cellingTexture;
					calculationScreen[y * screenWidth + x].Attributes = 8;

				}
				else if (y > celling && y <= floor)
				{
					wchar_t wallTexture;

					if (itBound) { wallTexture = L' '; }
					else if (distanceWall <= levelDrawing / 5)
						wallTexture = L'█';
					else if (distanceWall <= levelDrawing / 4)
						wallTexture = L'▓';
					else if (distanceWall <= levelDrawing / 3)
						wallTexture = L'▒';
					else if (distanceWall <= levelDrawing / 1)
						wallTexture = L'░';
					else
						wallTexture = ' ';

					if (itBound) calculationScreen[y * screenWidth + x].Attributes = 0;
					else calculationScreen[y * screenWidth + x].Attributes = 8;

					if (itTeleport)
					{
						wchar_t teleportTexture;
						if (itBound)
						{
							wallTexture = ' ';
							calculationScreen[y * screenWidth + x].Attributes = 56;
						}
						else
						{
							wallTexture = L'↑';
							calculationScreen[y * screenWidth + x].Attributes = 3;
						}
					}
					else
					{
						if (itBound)
						{
							wallTexture = ' ';
							calculationScreen[y * screenWidth + x].Attributes = 8;
						}
					}
					calculationScreen[y * screenWidth + x].Char.UnicodeChar = wallTexture;
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

					calculationScreen[y * screenWidth + x].Char.UnicodeChar = floorTexture;
					calculationScreen[y * screenWidth + x].Attributes = 8;
				}
			}
		}
		outputInfo();
		screen = calculationScreen;
		checkPlayerInTeleport();

	}
}
void Engine::Run()
{
	initialScreensaver();
	mapInfo.createmap();
	// Появление в далеке от телепорта
	//player.x = mapInfo.startCoordinat.first;
	//player.y = mapInfo.startCoordinat.second;

	// Появление рядом с телепортом
	player.x = mapInfo.finishCoordinat.first + 1;
	player.y = mapInfo.finishCoordinat.second + 1;

	cursoreVisibleFalse();
	getConsoleSize();
	setScreenSize();
	PrintGameTitle(screenWidth, screenHeight);

	screen = new CHAR_INFO[screenWidth * screenHeight];
	thread dravingFrame([this]() {
		while (true)
		{
			//this_thread::sleep_for(chrono::microseconds(100));
			WriteConsoleOutput(console, screen, bufferSize, { 0,0 }, &windowSize);
		}
		});

	thread infoKey([this]() {
		while (true)
		{
			if (_kbhit()) {
				switch (_getwch()) {
				case 109: printMinimap = printMinimap ? false : true;
					break;
				case 27: settings();
					break;
				}
				cursoreVisibleFalse();
			}
		}
		});
	renderingConsoleGraphics();
	dravingFrame.detach();
	infoKey.detach();
}