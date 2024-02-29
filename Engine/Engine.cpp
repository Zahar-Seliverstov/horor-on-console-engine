#include "Library.h"
#include "IMapInfo.h"
#include "IPlayer.h"
#include "IEngine.h"

MapInfo mapInfo;
Player player;
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD bufferSize;
SMALL_RECT windowSize;
CHAR_INFO* screen;
int screenWidth;
int screenHeight;

auto timeStart = chrono::high_resolution_clock::now();
auto timeFinish = chrono::high_resolution_clock::now();
double timeInSeconds;

double levelDrawing = 10;
double fov = 3.14159f / 3;
bool gameOver = false;
bool printMinimap = false;

enum Keys
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

void initialScreensaver()
{
	string nameGame[18] =
	{
		"            #   #   ###   #     ###   ###   #\\ /#   ###      ###   ###               ",
		"            # * #   #=    #     #     [ ]   # * #   #=        #    [ ]              ",
		"            #/ \\#   ###   ###   ###   ###   #   #   ###       #    ###              ",
		"                                                                                   ",
		"                                                                                   ",
		"    ____________    ____________    __        __    ____________    __________     ",
		"   /           /|  /           /|  / /|      / /|  /           /|  /         /|    ",
		"   ############ |  ############ |  ## |      ## |  ############ |  ########## /|   ",
		"   ############/   ############ |  ## |      ## |  ############/   ########### |   ",
		"        ## |       ## |      ## |  ## |      ## |  ## |_________   ##       ## |   ",
		"        ## |       ## |      ## |  ## |      ## |  ##/         /|  ##       ##/    ",
		"        ## |       ## |      ## |  ## |      ## |  ############ |  ########## |    ",
		"        ## |       ## |      ## |  ## | ____ ## |  ############/   ##########/     ",
		"        ## |       ## |______## |  ## |/   /\\## |  ## |________    ## |  ## \\      ",
		"        ## |       ##/       ## |  ##/ ####  ## |  ##/         /|  ## |   ## \\     ",
		"        ## |       ############ |  ####    #### |  ############ |  ## |    ## \\    ",
		"        ##/        ############/   ##        ##/   ############/   ##/      ##/    ",
		"                                                                                   "
	};
	printf("\n\n\n\n\n\n\x1b[31;48m");
	string line = "";
	for (int i = 0; i < 17; i++)
	{
		cout << "\t" << nameGame[i] << endl;
	}
	printf("\x1b[0m\x1b[35;0H ~ Перед началом игры, РАСТЕНИТЕ окно до нужного размера . . .\n ~ ");
	system("pause");
}
void getConsoleSize()
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
void setScreenSize()
{
	bufferSize = { short(screenWidth), short(screenHeight) };
	windowSize = { 0, 0, short(screenWidth - 1), short(screenHeight - 1) };
	SetConsoleScreenBufferSize(console, bufferSize);
	SetConsoleWindowInfo(console, TRUE, &windowSize);
}
void cursoreVisibleFalse()
{
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(console, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(console, &cursorInfo);
}
void outputInfo()
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
		screen[i].Char.UnicodeChar = fpsString[i];
		screen[i].Attributes = 11;
	}
	for (int i = 0; i < hp.length(); i++)
	{
		screen[i + screenWidth * (screenHeight - 2)].Char.UnicodeChar = hp[i];
		screen[i + screenWidth * (screenHeight - 2)].Attributes = 7;
	}
	for (int i = 0; i < hpTexture.size(); i++)
	{
		screen[i + screenWidth * (screenHeight - 1)].Char.UnicodeChar = hpTexture[i];
		if (i % 2 == 0) screen[i + screenWidth * (screenHeight - 1)].Attributes = 4;
		else screen[i + screenWidth * (screenHeight - 1)].Attributes = 15;
	}
	if (printMinimap)
	{
		for (int i = 0; i < minimap.size(); ++i)
		{
			screen[i + screenWidth * 2].Char.UnicodeChar = minimap[i];
			screen[i + screenWidth * 2].Attributes = 7;
		}
		for (int x = 0; x < mapInfo.mapSizeHorizontal; x++)
			for (int y = 0; y < mapInfo.mapSizeVertical; y++)
			{
				int index = (y * mapInfo.mapSizeHorizontal) + x;
				if (x < screenWidth - 3 && y < screenHeight - 3)
				{
					screen[(y + 3) * screenWidth + x].Char.UnicodeChar = mapInfo.map[index];
					screen[(y + 3) * screenWidth + x].Attributes = 15;
				}
			}
		screen[((int)player.y + 3) * screenWidth + (int)player.x].Char.UnicodeChar = L'☺';
		screen[((int)player.y + 3) * screenWidth + (int)player.x].Attributes = 9;
		screen[(mapInfo.startCoordinat.second + 3) * screenWidth + (mapInfo.startCoordinat.first)].Char.UnicodeChar = '&';
		screen[(mapInfo.startCoordinat.second + 3) * screenWidth + (mapInfo.startCoordinat.first)].Attributes = 5;
		screen[(mapInfo.finishCoordinat.second + 3) * screenWidth + (mapInfo.finishCoordinat.first)].Char.UnicodeChar = '&';
		screen[(mapInfo.finishCoordinat.second + 3) * screenWidth + (mapInfo.finishCoordinat.first)].Attributes = 5;

	}
}
void settings()
{
	int command = 5;
	bool treker = true;
	vector<wstring> items{
		L"   [^] Дальность прорисовки    ",
		L"   [S] Скорость ходьбы         ",
		L"   [-] Чувствительность        ",
		L"   [R] Поле зрения             ",
		L"   [S] Размер экрана           ",
		L"   [<] Продолжить              ",
		L"   [X] Выйти из игры           " };

	system("cls");
	while (treker)
	{
		printf("\x1b[H\x1b[0m\t~ Меню настроек ~\n_______________________________\n\n\x1b[90m\x1b[48; 5; 0m");
		for (int i = 0; i < items.size(); i++)
		{
			if (i == command)
			{
				if (i == items.size() - 1) printf("\x1b[38;5;0m\x1b[48;5;9m");
				else printf("\x1b[38;5;0m\x1b[48;5;15m");
				wcout << items.at(i) << endl;
				printf("\x1b[90m\x1b[48;5;0m");
			}
			else wcout << items.at(i) << endl;
		}

		switch (_getwch())
		{
		case arrowUp: command = command - 1 >= 0 ? command -= 1 : items.size() - 1;
			break;
		case arrowDown: command = command < items.size() - 1 ? command += 1 : 0;
			break;
		case enter: treker = false;
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

			switch (_getwch())
			{
			case arrowLeft: levelDrawing = levelDrawing - 1 >= 2 ? levelDrawing -= 1 : 20;
				break;
			case arrowRight: levelDrawing = levelDrawing + 1 <= 20 ? levelDrawing += 1 : 2;
				break;
			case escape: treker = false;
				break;
			}
		}
		break;
	case 1:
		while (treker)
		{
			system("cls");
			printf("\x1b[0m\t~ Настройки скорости ~\n_________________________________________________\n\n\x1b[0m");
			for (int i = 1; i <= player.speed; i++)
			{
				if (i < 3) printf("\x1b[91m");
				else if (i < 5) printf("\x1b[38;5;208m");
				else printf("\x1b[92m");
				cout << "[" << i << "]";
			}
			printf("\x1b[90m\n\n {A} <-+-> {D}\n\n\n <= {esc}");

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
	case 2:
		while (treker)
		{
			system("cls");
			printf("\x1b[0m\t~ Настройки чувствительности ~\n_________________________________________________\n\n\x1b[0m");
			for (double i = 1; i <= player.sensitivity; i += 1)
			{
				if (i <= 3) printf("\x1b[91m");
				else if (i <= 7) printf("\x1b[38;5;208m");
				else printf("\x1b[92m");
				cout << "[" << i << "]";
			}
			printf("\x1b[90m\n\n {A} <-+-> {D}\n\n\n <= {esc}");

			switch (_getwch())
			{
			case arrowLeft: player.sensitivity = player.sensitivity - 1 >= 1 ? player.sensitivity -= 1 : 10;
				break;
			case arrowRight: player.sensitivity = player.sensitivity + 1 <= 10 ? player.sensitivity += 1 : 1;
				break;
			case escape: treker = false;
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

			switch (_getwch())
			{
			case arrowLeft: fov = fov - 0.5 >= 0.5 ? fov -= 0.5 : 4;
				break;
			case arrowRight: fov = fov + 0.5 <= 4 ? fov += 0.5 : 0.5;
				break;
			case escape: treker = false;
				break;
			}
		}
		break;
	case 4:
		while (treker)
		{
			system("cls");
			getConsoleSize();
			printf("\x1b[0m\t~ Настройки экрана ~\n_________________________________________\n\n\x1b[91m  < Растените окно до нужного размера >\x1b[90m\n\n  Размер окна: \x1b[0m");

			cout << screenWidth << " x " << screenHeight;
			printf("\x1b[90m\n\n\n  <= {esc}");
			if (_kbhit())
			{
				switch (_getch())
				{
				case escape: treker = false;
				}
			}
			this_thread::sleep_for(chrono::milliseconds(200));
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
}
bool compareByModule(const pair<double, double>& point1, const pair<double, double>& point2)
{
	double module1 = sqrt(point1.first * point1.first + point1.second * point1.second);
	double module2 = sqrt(point2.first * point2.first + point2.second * point2.second);
	return module1 < module2;
}
void RenderingConsoleGraphics()
{
	while (!gameOver)
	{
		mapInfo.clearmap();
		timeFinish = chrono::high_resolution_clock::now();
		timeInSeconds = chrono::duration<double>(timeFinish - timeStart).count();
		timeStart = chrono::high_resolution_clock::now();
		player.motion(mapInfo.map, mapInfo.mapSizeHorizontal , timeInSeconds);
		if (_kbhit())
		{
			switch (_getwch())
			{
			case 109:
				printMinimap = printMinimap ? false : true;
				break;
			case 27: settings();
				break;
			}
			cursoreVisibleFalse();
		}
		for (int x = 0; x < screenWidth; x++)
		{
			double rayAngle = player.r + fov / 2.0f - x * fov / screenWidth;
			double rayX = sinf(rayAngle);
			double rayY = cosf(rayAngle);
			double distanceWall = 0;
			bool itWall = false;
			bool itTeleport = false;
			bool itMonster = false;
			bool itBound = false;

			while (!itWall && distanceWall < levelDrawing)
			{
				distanceWall += 0.01f;
				int testX = (int)(player.x + rayX * distanceWall);
				int testY = (int)(player.y + rayY * distanceWall);

				if (mapInfo.map[testY * mapInfo.mapSizeHorizontal + testX] == L'&') { itTeleport = true; }
				if (testX < 0 || testX >= mapInfo.mapSizeHorizontal || testY < 0 || testY >= mapInfo.mapSizeVertical)
				{
					itWall = true;
					distanceWall = levelDrawing;
				}
				else if (mapInfo.map[testY * mapInfo.mapSizeHorizontal + testX] == L'#' || itTeleport)
				{
					itWall = true;
					vector<pair<double, double>> boundsVector;
					for (int tx = 0; tx < 2; tx++)
					{
						for (int ty = 0; ty < 2; ty++)
						{
							double vectorX = testX + tx - player.x;
							double vectorY = testY + ty - player.y;
							double vectorModule = sqrt(vectorX * vectorX + vectorY * vectorY);
							double cosAngle = rayX * vectorX / vectorModule + rayY * vectorY / vectorModule;
							boundsVector.push_back(make_pair(vectorModule, cosAngle));
						}
					}
					sort(boundsVector.begin(), boundsVector.end(), compareByModule);
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

					screen[y * screenWidth + x].Char.UnicodeChar = cellingTexture;
					screen[y * screenWidth + x].Attributes = 8;

				}
				else if (y > celling && y <= floor)
				{
					wchar_t wallTexture;

					if (itBound)
					{
						wallTexture = L' ';
					}
					else if (distanceWall <= levelDrawing / 4)
						wallTexture = L'█';
					else if (distanceWall <= levelDrawing / 3)
						wallTexture = L'▓';
					else if (distanceWall <= levelDrawing / 2)
						wallTexture = L'▒';
					else if (distanceWall <= levelDrawing / 1)
						wallTexture = L'░';
					else
						wallTexture = ' ';

					if (itBound) screen[y * screenWidth + x].Attributes = 8;
					else screen[y * screenWidth + x].Attributes = 8;

					if (itTeleport)
					{
						wchar_t teleportTexture;
						if (itBound)
						{
							wallTexture = ' ';
							screen[y * screenWidth + x].Attributes = 56;
						}
						else
						{
							wallTexture = L'↑';
							screen[y * screenWidth + x].Attributes = 3;

						}
					}
					screen[y * screenWidth + x].Char.UnicodeChar = wallTexture;
				}
				else
				{
					char floorTexture;

					if (centerScreen < 0.15f)
						floorTexture = '&';
					else if (centerScreen < 0.4f)
						floorTexture = '#';
					else if (centerScreen < 0.7f)
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
		outputInfo();
		WriteConsoleOutput(console, screen, bufferSize, { 0,0 }, &windowSize);
	}
}
void Run()
{
	mapInfo.createmap();
	player.x = mapInfo.startCoordinat.first;
	player.y = mapInfo.startCoordinat.second;

	initialScreensaver();
	cursoreVisibleFalse();
	getConsoleSize();
	setScreenSize();

	screen = new CHAR_INFO[screenWidth * screenHeight];

	RenderingConsoleGraphics();
}
