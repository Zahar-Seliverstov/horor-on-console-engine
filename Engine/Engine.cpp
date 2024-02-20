#include <iostream>
#include <string>
#include <cmath>
#include <conio.h>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <cwchar>
#include <vector>
#include <algorithm>
#include <ctime>
#include <future>
using namespace std;

class Monster
{
public:
	int x = 4, y = 4;
	int hp = 10;

	void movement(std::wstring& map, int& mapWidth)
	{
		int motion = rand() % 4 + 1;
		int index;

		switch (motion)
		{
		case 1:
			map[y * mapWidth + x + 1] != L'#' ? x += 1 : x;
			break;
		case 2:
			map[y * mapWidth + x - 1] != L'#' ? x -= 1 : x;
			break;
		case 3:
			map[(y - 1) * mapWidth + x] != L'#' ? y -= 1 : y;
			break;
		case 4:
			map[(y + 1) * mapWidth + x] != L'#' ? y += 1 : y;
			break;
		}
	}
};
class Player
{

};

void getConsoleSize(int& screenWidth, int& screenHeight) {
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

void cursoreVisibleFalse(HANDLE& console)
{
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(console, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(console, &cursorInfo);
}

void setConsoleSize(int& screenWidth, int& screenHeight, HANDLE& console, COORD& bufferSize, SMALL_RECT& windowSize)
{
	bufferSize = { short(screenWidth), short(screenHeight) };
	windowSize = { 0, 0, short(screenWidth - 1), short(screenHeight - 1) };
	SetConsoleScreenBufferSize(console, bufferSize);
	SetConsoleWindowInfo(console, TRUE, &windowSize);
}

void getMapSize(int& mapWidth, int& mapHeight, wstring& map)
{
	int line = 0;
	for (int i = 0; i < map.length(); i++)
	{
		if (map[i] == '#')
			line += 1;
		else
		{
			if (mapWidth <= line)
				mapWidth = (line - 1);
			break;
		}
	}
	mapHeight = map.length() / mapWidth;
}

void outputInfo(wstring& map, CHAR_INFO*& screen, int& screenWidth, int& screenHeight, int& mapWidth, int& mapHeight, double& playerX, double& playerY, int& playerHp, double& timeInSeconds, bool& printMiniMap)
{
	wstring FPSString = L" [ FPS:" + to_wstring(int(1 / timeInSeconds)) + L" ]" + L" [ X:" + to_wstring((float)playerX) + L" ] [ Y:" + to_wstring((float)playerY) + L" ] ";
	wstring items = L" [ SITTINGS ] < {esc} ";
	wstring miniMap = L"   ~ mini MAP ~   ";
	wstring levelHp = L"";
	wstring hp = L" ~ HEALTH ~                              ";
	for (int i = 0; i < 10; i++)
	{
		if (i > playerHp) levelHp += L" [x]";
		else levelHp += L" [\u2665]";
		if (i == 9) levelHp += L" ";
	}
	for (size_t i = 0; i < FPSString.size(); ++i)
	{
		screen[i].Char.UnicodeChar = FPSString[i];
		screen[i].Attributes = 11;
	}
	for (size_t i = 0; i < items.size(); ++i)
	{
		screen[i + screenWidth - items.length()].Char.UnicodeChar = items[i];
		screen[i + screenWidth - items.length()].Attributes = 11;
	}
	for (size_t i = 0; i < miniMap.size(); ++i)
	{
		screen[i + screenWidth * 2 + mapWidth - miniMap.length()].Char.UnicodeChar = miniMap[i];
		screen[i + screenWidth * 2 + mapWidth - miniMap.length()].Attributes = 15;
	}
	for (int i = 0; i < hp.length(); i++)
	{
		screen[i + screenWidth * (screenHeight - 2)].Char.UnicodeChar = hp[i];
		screen[i + screenWidth * (screenHeight - 2)].Attributes = 7;
	}
	for (int i = 0; i < levelHp.size(); i++)
	{
		screen[i + screenWidth * (screenHeight - 1)].Char.UnicodeChar = levelHp[i];
		if (i % 2 == 0) screen[i + screenWidth * (screenHeight - 1)].Attributes = 4;
		else screen[i + screenWidth * (screenHeight - 1)].Attributes = 15;
	}
	if (printMiniMap)
	{
		for (int x = 0; x < mapWidth; x++)
			for (int y = 0; y < mapHeight; y++)
			{
				int index = (y * mapWidth) + x;
				screen[(y + 3) * screenWidth + x].Char.UnicodeChar = map[index];
				screen[(y + 3) * screenWidth + x].Attributes = 15;
			}
	}
	screen[((int)playerY + 3) * screenWidth + (int)playerX].Char.UnicodeChar = 'P';
	screen[((int)playerY + 3) * screenWidth + (int)playerX].Attributes = 10;
	/*screen[((int)monster.y + 3) * screenWidth + (int)monster.x].Char.UnicodeChar = 'M';
	screen[((int)monster.y + 3) * screenWidth + (int)monster.x].Attributes = 4;*/
}

void settings(double& levelDrawing, double& fov, float& playerSensitivity, double& playerSpeed)
{
	int command = 4;
	bool treker = true;
	vector<string> items
	{
		"   [?] Дальность прорисовки    ",
		"   [s] Скорость ходьбы         ",
		"   [?] Чувствительность        ",
		"   [R] Поле зрения             ",
		"   [<] Продолжить              ",
		"   [X] Выйти из игры           "
	};
	system("cls");
	while (treker)
	{
		printf("\x1b[H\x1b[0m\t~ Меню настроек ~\n_______________________________\n\n\x1b[90m\x1b[48;5;0m");
		for (int i = 0; i < items.size(); i++)
		{
			if (i == command)
			{
				if (i == items.size() - 1) printf("\x1b[38;5;0m\x1b[48;5;9m");
				else printf("\x1b[38;5;0m\x1b[48;5;15m");
				cout << items.at(i) << endl;
				printf("\x1b[90m\x1b[48;5;0m");
			}
			else cout << items.at(i) << endl;
		}

		switch (_getch())
		{
		case 119:
			command = command - 1 >= 0 ? command -= 1 : items.size() - 1;
			break;
		case 115:
			command = command < items.size() - 1 ? command += 1 : 0;
			break;
		case 13:
			treker = false;
			break;
		}
	}
	treker = true;
	switch (command)
	{
	case 0:
		while (treker)
		{
			system("cls");
			printf("\x1b[0m\t~ Настройки уровня прорисовки ~\n_________________________________________________\n\n\x1b[0m");
			for (int i = 2; i <= levelDrawing; i++)
			{
				if (i < 7) printf("\x1b[91m");
				else if (i < 15) printf("\x1b[38;5;208m");
				else printf("\x1b[92m");
				cout << "[" << i << "]";
			}
			printf("\x1b[90m\n\n {A} <-+-> {D}\n\n\n <= {esc}");

			switch (_getch())
			{
			case 97:
				levelDrawing = levelDrawing - 1 >= 2 ? levelDrawing -= 1 : 20;
				break;
			case 100:
				levelDrawing = levelDrawing + 1 <= 20 ? levelDrawing += 1 : 2;
				break;
			case 27:
				treker = false;
				break;
			}
		}
		break;
	case 1:
		while (treker)
		{
			system("cls");
			printf("\x1b[0m\t~ Настройки скорости ~\n_________________________________________________\n\n\x1b[0m");
			for (int i = 1; i <= playerSpeed; i++)
			{
				if (i < 3) printf("\x1b[91m");
				else if (i < 5) printf("\x1b[38;5;208m");
				else printf("\x1b[92m");
				cout << "[" << i << "]";
			}
			printf("\x1b[90m\n\n {A} <-+-> {D}\n\n\n <= {esc}");

			switch (_getch())
			{
			case 97:
				playerSpeed = playerSpeed - 1 > 0 ? playerSpeed -= 1 : 8;
				break;
			case 100:
				playerSpeed = playerSpeed + 1 <= 8 ? playerSpeed += 1 : 1;
				break;
			case 27:
				treker = false;
				break;
			}
		}
		break;
	case 2:
		while (treker)
		{
			system("cls");
			printf("\x1b[0m\t~ Настройки чувствительности ~\n_________________________________________________\n\n\x1b[0m");
			for (double i = 1; i <= playerSensitivity; i += 1)
			{
				if (i <= 3) printf("\x1b[91m");
				else if (i <= 7) printf("\x1b[38;5;208m");
				else printf("\x1b[92m");
				cout << "[" << i << "]";
			}
			printf("\x1b[90m\n\n {A} <-+-> {D}\n\n\n <= {esc}");

			switch (_getch())
			{
			case 97:
				playerSensitivity = playerSensitivity - 1 >= 1 ? playerSensitivity -= 1 : 10;
				break;
			case 100:
				playerSensitivity = playerSensitivity + 1 <= 10 ? playerSensitivity += 1 : 1;
				break;
			case 27:
				treker = false;
				break;
			}
		}
		break;
	case 3:
		while (treker)
		{
			system("cls");
			printf("\x1b[0m\t~ Настройки поля зрения ~\n_________________________________________________\n\n\x1b[0m");
			for (double i = 0.5; i <= fov; i += 0.5)
			{
				if (i <= 1.5) printf("\x1b[91m");
				else if (i <= 2.5) printf("\x1b[38;5;208m");
				else printf("\x1b[92m");
				cout << "[" << i << "]";
			}
			printf("\x1b[90m\n\n {A} <-+-> {D}\n\n\n <= {esc}");

			switch (_getch())
			{
			case 97:
				fov = fov - 0.5 >= 0 ? fov -= 0.5 : 4;
				break;
			case 100:
				fov = fov + 0.5 <= 4 ? fov += 0.5 : 0.5;
				break;
			case 27:
				treker = false;
				break;
			}
		}
		break;
	case 4:
		return;
	case 5:
		exit(0);
		break;
	}
}

bool compareByModule(const std::pair<double, double>& point1, const std::pair<double, double>& point2) 
{
	double module1 = std::sqrt(point1.first * point1.first + point1.second * point1.second);
	double module2 = std::sqrt(point2.first * point2.first + point2.second * point2.second);
	return module1 < module2;
}

int main()
{
	setlocale(LC_ALL, "rus");
	srand(time(NULL));

	int playerHp = 10;
	double playerX = 3;
	double playerY = 2;
	double playerR = 0;
	double playerSpeed = 4;
	float playerSensitivity = 5;

	wstring map = L"";
	map += L"##############";
	map += L"#            #";
	map += L"#            #";
	map += L"#            #";
	map += L"#            #";
	map += L"#            #";
	map += L"#            #";
	map += L"#            #";
	map += L"##############";

	int mapWidth = 0;
	int mapHeight = 0;
	getMapSize(mapWidth, mapHeight, map);
	bool printMiniMap = false;
	double levelDrawing = 10;
	double fov = 3.14159f / 3;
	bool gameOver = false;
	system("pause");

	/*string nameGame[10] = {
		" ############   ############   ##        ##   ############   ##########    ",
		" ############   ############   ##        ##   ############   ###########   ",
		"      ##        ##        ##   ##        ##   ##             ##       ##   ",
		"      ##        ##        ##   ##        ##   ##             ##       ##   ",
		"      ##        ##        ##   ##        ##   ############   ##########    ",
		"      ##        ##        ##   ##        ##   ############   ##########    ",
		"      ##        ##        ##   ##        ##   ##             ##    ##      ",
		"      ##        ##        ##   ##  ####  ##   ##             ##     ##     ",
		"      ##        ############   ####    ####   ############   ##      ##    ",
		"      ##        ############   ##        ##   ############   ##       ##   ",
	};

	printf("\x1b[91m\n\n\n");
	for (string line : nameGame)
		cout << "\t\t" << line << endl;
	printf("\x1b[0m\x1b[91m\x1b[27;0H ~ Перед началом игры растените окно до нужного размера . . .\n ~ ");
	system("pause");*/

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD bufferSize;
	SMALL_RECT windowSize;
	int screenWidth;
	int screenHeight;
	getConsoleSize(screenWidth, screenHeight);
	cursoreVisibleFalse(console);
	setConsoleSize(screenWidth, screenHeight, console, bufferSize, windowSize);
	CHAR_INFO* screen = new CHAR_INFO[screenWidth * screenHeight];

	auto timeStart = std::chrono::high_resolution_clock::now();
	auto timeFinish = std::chrono::high_resolution_clock::now();
	double timeInSeconds;

	thread move([&]()
		{
			while (true)
			{
				if (GetAsyncKeyState((unsigned short)'D') & 0x8000) playerR -= (playerSensitivity * 0.75f) * timeInSeconds;
				if (GetAsyncKeyState((unsigned short)'A') & 0x8000) playerR += (playerSensitivity * 0.75f) * timeInSeconds;
				if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
				{
					playerX += sinf(playerR) * playerSpeed * timeInSeconds;;
					playerY += cosf(playerR) * playerSpeed * timeInSeconds;;
					if (map[(int)playerY * mapWidth + (int)playerX] == '#')
					{
						playerX -= sinf(playerR) * playerSpeed * timeInSeconds;;
						playerY -= cosf(playerR) * playerSpeed * timeInSeconds;;
					}
				}
				if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
				{
					playerX -= sinf(playerR) * playerSpeed * timeInSeconds;;
					playerY -= cosf(playerR) * playerSpeed * timeInSeconds;;
					if (map[(int)playerY * mapWidth + (int)playerX] == '#')
					{
						playerX += sinf(playerR) * playerSpeed * timeInSeconds;;
						playerY += cosf(playerR) * playerSpeed * timeInSeconds;;
					}
				}
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) settings(levelDrawing, fov, playerSensitivity, playerSpeed);
				if (GetAsyncKeyState((unsigned short)'M') & 0x8000) printMiniMap = printMiniMap ? false : true;
				outputInfo(map, screen, screenWidth, screenHeight, mapWidth, mapHeight, playerX, playerY, playerHp, timeInSeconds, printMiniMap);
				WriteConsoleOutput(console, screen, bufferSize, { 0,0 }, &windowSize);
			}
		});

	while (!gameOver)
	{
		timeFinish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time = timeFinish - timeStart;
		timeInSeconds = time.count();
		timeStart = std::chrono::high_resolution_clock::now();

		for (int x = 0; x < screenWidth; x++)
		{
			double rayAngle = playerR + fov / 2.0f - x * fov / screenWidth;
			double rayX = sinf(rayAngle);
			double rayY = cosf(rayAngle);
			double distanceWall = 0;
			bool itWall = false;
			bool itBound = false;

			while (!itWall && distanceWall < levelDrawing)
			{
				distanceWall += 0.01f;
				int testX = (int)(playerX + rayX * distanceWall);
				int testY = (int)(playerY + rayY * distanceWall);

				if (testX < 0 || testX >= mapWidth || testY < 0 || testY >= mapHeight)
				{
					itWall = true;
					distanceWall = levelDrawing;
				}
				else if (map[testY * mapWidth + testX] == L'#')
				{
					itWall = true;
					std::vector<std::pair<double, double>> boundsVector;
					for (int tx = 0; tx < 2; tx++)
					{
						for (int ty = 0; ty < 2; ty++)
						{
							double vx = testX + tx - playerX;
							double vy = testY + ty - playerY;
							double vectorModule = std::sqrt(vx * vx + vy * vy);
							double cosAngle = rayX * vx / vectorModule + rayY * vy / vectorModule;
							boundsVector.push_back(std::make_pair(vectorModule, cosAngle));
						}
					}

					std::sort(boundsVector.begin(), boundsVector.end(), compareByModule);
					double boundAngle = 0.04 / distanceWall;
					if (std::acos(boundsVector[0].second) < boundAngle || std::acos(boundsVector[1].second) < boundAngle)
					{
						itBound = true;
					}
				}
				else
				{
					//map[testY * mapWidth + testX] = '*';
				}
			}

			int celling = (float)(screenHeight / 2.0) - screenHeight / ((float)distanceWall);
			int floor = screenHeight - celling;

			for (int y = 0; y < screenHeight; y++)
			{
				if (y <= celling)
				{
					char cellingTexture;

					double centerScreen = 1 - static_cast<double>(y - screenHeight / 2) / (screenHeight / 2);
					if (centerScreen > 1.9f)
						cellingTexture = '@';
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
					short wallTexture;
					if (itBound)
						wallTexture = '|';
					else if (distanceWall <= levelDrawing / 4)
						wallTexture = 0x2588;
					else if (distanceWall <= levelDrawing / 3)
						wallTexture = 0x2593;
					else if (distanceWall <= levelDrawing / 2)
						wallTexture = 0x2592;
					else if (distanceWall <= levelDrawing / 1)
						wallTexture = 0x2591;
					else
						wallTexture = ' ';

					screen[y * screenWidth + x].Char.UnicodeChar = wallTexture;
					screen[y * screenWidth + x].Attributes = 8;
				}
				else
				{
					char floatTexture;

					double centerScreen = 1 - static_cast<double>(y - screenHeight / 2) / (screenHeight / 2);
					if (centerScreen < 0.1)
						floatTexture = '&';
					else if (centerScreen < 0.3)
						floatTexture = 'X';
					else if (centerScreen < 0.6)
						floatTexture = '*';
					else if (centerScreen < 0.9)
						floatTexture = ':';
					else
						floatTexture = ' ';

					screen[y * screenWidth + x].Char.UnicodeChar = floatTexture;
					screen[y * screenWidth + x].Attributes = 7;
				}
			}
		}
		if (playerHp <= 0) gameOver = true;
	}
	//t1.detach();
	return 0;
}