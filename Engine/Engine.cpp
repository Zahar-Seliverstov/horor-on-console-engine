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
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cmath>
using namespace std;

wstring map;
wstring globalMap;
int mapWidth = 0;
int mapHeight = 0;

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
int ratioSizeScreen = 0;
double fov = 3.14159f / 3;
bool gameOver = false;
bool printMiniMap = false;

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
public:
	int initialHp;
	int Hp = 100;
	double X = 3;
	double Y = 2;
	double R = 0;
	double Speed = 4;
	float Sensitivity = 5;
	Player() { initialHp = Hp; }

	void motion()
	{
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000) R -= (Sensitivity * 0.75f) * timeInSeconds;
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000) R += (Sensitivity * 0.75f) * timeInSeconds;
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
		{
			X += sinf(R) * Speed * timeInSeconds;
			Y += cosf(R) * Speed * timeInSeconds;
			if (map[(int)Y * mapWidth + (int)X] == '#')
			{
				X -= sinf(R) * Speed * timeInSeconds;
				Y -= cosf(R) * Speed * timeInSeconds;
			}
		}
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			X -= sinf(R) * Speed * timeInSeconds;
			Y -= cosf(R) * Speed * timeInSeconds;
			if (map[(int)Y * mapWidth + (int)X] == '#')
			{
				X += sinf(R) * Speed * timeInSeconds;
				Y += cosf(R) * Speed * timeInSeconds;
			}
		}
	}
};
class RecursiveDungeon
{
public:
	vector<vector<int>> getIntMaze() { return maze; }
	wstring getWstringMaze()
	{
		for (int y = 0; y < MAP_SIZE_VERTICAL; ++y)
		{
			for (int x = 0; x < MAP_SIZE_HORIZONTAL; ++x)
			{
				if (maze[y][x] == 0)
					mazeString += L' '; // Пустое пространство
				else if (maze[y][x] == 1)
					mazeString += L'#'; // Стена
				else if (maze[y][x] == 2)
					mazeString += L' '; // Дверь
				else
					mazeString += L'?'; // Не обработанное
			}
			//mazeString += L'\n';
		}
		return mazeString;
	}

	// Конструктор класса
	RecursiveDungeon()
	{
		maze.resize(MAP_SIZE_VERTICAL, vector<int>(MAP_SIZE_HORIZONTAL, 0));
	}

	// Метод для создания подземелья
	void createDungeon()
	{
		bool dungeonCreated = false;
		while (!dungeonCreated)
		{
			maze.clear();
			maze.resize(MAP_SIZE_VERTICAL, vector<int>(MAP_SIZE_HORIZONTAL, 0));

			division(1, MAP_SIZE_HORIZONTAL - 2, 1, MAP_SIZE_VERTICAL - 2); // Создание подземелья

			dungeonCreated = false; // Предполагаем, что подземелье создано

			// Проверка, было ли создано подземелье (если в карте отсутствуют стены)
			for (int y = 0; y < MAP_SIZE_VERTICAL; ++y)
			{
				for (int x = 0; x < MAP_SIZE_HORIZONTAL; ++x)
				{
					if (maze[y][x] == 1)
					{
						dungeonCreated = true; // Если найдены стены, то подземелье создано
						break;
					}
				}
				if (dungeonCreated)
					break;
			}
		}
	}

	// Рекурсивный метод для разделения пространства и создания комнат
	void division(int startX, int endX, int startY, int endY)
	{
		srand(rand());
		// Условие выхода из рекурсии: прекращаем разделение, если размер пространства меньше чем размеры минимальной комнаты
		if (endX - startX + 1 < ROOM_MIN_HORIZONTAL * 2 && endY - startY + 1 < ROOM_MIN_VERTICAL * 2)
			return;

		// Определение направления разделения на основе размеров пространства
		if (endX - startX > endY - startY)
		{
			if (endX - startX + 1 < ROOM_MIN_HORIZONTAL * 2)
				return;
			createRoom(true, startX, endX, startY, endY); // Создание горизонтальной комнаты
		}
		else if (endX - startX < endY - startY)
		{
			if (endY - startY + 1 < ROOM_MIN_VERTICAL * 2)
				return;
			createRoom(false, startX, endX, startY, endY); // Создание вертикальной комнаты
		}
		else
		{
			if (rand() % 2 == 0)
			{
				if (endX - startX + 1 < ROOM_MIN_HORIZONTAL * 2)
					return;
				createRoom(true, startX, endX, startY, endY); // Создание горизонтальной комнаты
			}
			else
			{
				if (endY - startY + 1 < ROOM_MIN_VERTICAL * 2)
					return;
				createRoom(false, startX, endX, startY, endY); // Создание вертикальной комнаты
			}
		}
	}
	// Метод для создания комнаты
	void createRoom(bool isHorizontal, int startX, int endX, int startY, int endY)
	{
		srand(rand());
		if (isHorizontal)
		{
			// Генерация случайной позиции для вертикальной стены
			int verticalWall = startX + ROOM_MIN_HORIZONTAL + rand() % (endX - startX - ROOM_MIN_HORIZONTAL + 1);
			if (verticalWall % 2 == 0)
			{ // Только нечетные числа
				verticalWall++;
				if (verticalWall >= endX + 1 - ROOM_MIN_HORIZONTAL)
					verticalWall -= 2;
			}
			// Заполнение пространства вертикальной стеной
			for (int i = startY; i <= endY; ++i)
			{
				maze[i][verticalWall] = 1;
			}
			// Добавление дополнительных дверей
			int doorNumbers = 1;
			if (EXTRA_DOOR_CHANCE > rand() % 100 && endY - startY >= MIN_LENGTH_FOR_EXTRA_DOOR)
				doorNumbers++;
			for (int i = 0; i < doorNumbers; ++i)
			{
				int randomDoor = startY + rand() % (endY - startY + 1);
				if (randomDoor % 2 != 0)
				{
					randomDoor++;
					if (randomDoor > endY)
						randomDoor -= 2;
				}
				maze[randomDoor][verticalWall] = 2; // Пометка двери
			}
			// Рекурсивное разделение пространства слева и справа от стены
			division(startX, verticalWall - 1, startY, endY);
			division(verticalWall + 1, endX, startY, endY);
		}
		else
		{
			// Генерация случайной позиции для горизонтальной стены
			int horizontalWall = startY + ROOM_MIN_VERTICAL + rand() % (endY - startY - ROOM_MIN_VERTICAL + 1);
			if (horizontalWall % 2 == 0)
			{ // Только нечетные числа
				horizontalWall++;
				if (horizontalWall >= endY + 1 - ROOM_MIN_VERTICAL)
					horizontalWall -= 2;
			}
			// Заполнение пространства горизонтальной стеной
			for (int i = startX; i <= endX; ++i)
			{
				maze[horizontalWall][i] = 1;
			}
			// Добавление дополнительных дверей
			int doorNumbers = 1;
			if (EXTRA_DOOR_CHANCE > rand() % 100 && endX - startX >= MIN_LENGTH_FOR_EXTRA_DOOR)
				doorNumbers++;
			for (int i = 0; i < doorNumbers; ++i)
			{
				int randomDoor = startX + rand() % (endX - startX + 1);
				if (randomDoor % 2 != 0)
				{
					randomDoor++;
					if (randomDoor > endX)
						randomDoor -= 2;
				}
				maze[horizontalWall][randomDoor] = 2; // Пометка двери
			}
			// Рекурсивное разделение пространства выше и ниже стены
			division(startX, endX, startY, horizontalWall - 1);
			division(startX, endX, horizontalWall + 1, endY);
		}
	}

	// Метод для создания границ для карты
	void createBorders()
	{
		for (int i = 0; i < MAP_SIZE_HORIZONTAL; ++i)
		{
			maze[0][i] = 1;                         // Верхняя граница
			maze[MAP_SIZE_VERTICAL - 1][i] = 1;     // Нижняя граница
		}

		for (int i = 0; i < MAP_SIZE_VERTICAL; ++i)
		{
			maze[i][0] = 1;                         // Левая граница
			maze[i][MAP_SIZE_HORIZONTAL - 1] = 1;  // Правая граница
		}
	}

	// Метод для вывода карты подземелья в консоль
	void printDungeon()
	{
		for (int y = 0; y < MAP_SIZE_VERTICAL; ++y)
		{
			for (int x = 0; x < MAP_SIZE_HORIZONTAL; ++x)
			{
				if (maze[y][x] == 0)
					cout << '.'; // Пустое пространство
				else if (maze[y][x] == 1)
					cout << '#'; // Стена
				else if (maze[y][x] == 2)
					cout << 'D'; // Дверь
				else
					cout << '?'; // Не обработанный символ
			}
			cout << endl;
		}
	}

	vector<vector<int>> maze;                            // Двумерный вектор для хранения карты подземелья в виде значений
	wstring mazeString;                                  // Двумерный вектор для хранения карты подземелья в виде строки
	static constexpr int MAP_SIZE_HORIZONTAL = 70;       // Горизонтальный размер карты
	static constexpr int MAP_SIZE_VERTICAL = 60;         // Вертикальный размер карты
	static constexpr int ROOM_MIN_HORIZONTAL = 11;       // Минимальная горизонтальная длина комнаты
	static constexpr int ROOM_MIN_VERTICAL = 9;          // Минимальная вертикальная высота комнаты
	static constexpr int EXTRA_DOOR_CHANCE = 15;         // Шанс создания дополнительных дверей (%)
	static constexpr int MIN_LENGTH_FOR_EXTRA_DOOR = 40; // Минимальная длина пути для создания дополнительной двери
};

RecursiveDungeon maze;

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

void getConsoleSize() {
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
void setConsoleSize()
{
	bufferSize = { short(screenWidth + ratioSizeScreen), short(screenHeight + ratioSizeScreen * 0.5) };
	windowSize = { 0, 0, short(screenWidth + ratioSizeScreen - 1), short(screenHeight + ratioSizeScreen * 0.5 - 1) };
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

void createMap()
{
	maze.createDungeon();
	maze.createBorders();
	map = maze.getWstringMaze();
	globalMap = map;
}
void getMapSize()
{
	/*int line = 0;
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
	mapHeight = map.length() / mapWidth;*/
	mapWidth = maze.MAP_SIZE_HORIZONTAL;
	mapHeight = maze.MAP_SIZE_VERTICAL;
}

void outputInfo(double& playerX, double& playerY, int& playerHp, int& initialHp)
{
	std::wstringstream stream;
	stream << L" [ FPS: " << int(1 / timeInSeconds)
		<< L" ] [ X: " << std::fixed << std::setprecision(2) << playerX
		<< L" ] [ Y: " << std::fixed << std::setprecision(2) << playerY << L" ] ";
	wstring FPSString = stream.str();
	wstring miniMap = L" ~ MAP ~ ";
	wstring hpTexture = L"";
	wstring hp = L" ~ HP ~ ";
	for (int i = 0; i <= initialHp; i += initialHp / 10)
	{
		if (i <= playerHp) hpTexture += L" [\u2665]";
		else hpTexture += L" [X]";
	}
	for (int i = 0; i < FPSString.size(); ++i)
	{
		screen[i].Char.UnicodeChar = FPSString[i];
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
	if (printMiniMap)
	{
		for (int i = 0; i < miniMap.size(); ++i)
		{
			screen[i + screenWidth * 2].Char.UnicodeChar = miniMap[i];
			screen[i + screenWidth * 2].Attributes = 15;
		}
		for (int x = 0; x < mapWidth; x++)
			for (int y = 0; y < mapHeight; y++)
			{
				int index = (y * mapWidth) + x;
				screen[(y + 3) * screenWidth + x].Char.UnicodeChar = map[index];
				screen[(y + 3) * screenWidth + x].Attributes = 8;
			}
		screen[((int)playerY + 3) * screenWidth + (int)playerX].Char.UnicodeChar = 'P';
		screen[((int)playerY + 3) * screenWidth + (int)playerX].Attributes = 10;
	}
}
void settings(float& playerSensitivity, double& playerSpeed)
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
		printf("\x1b[H\x1b[0m\t~ Меню настроек ~\n_______________________________\n\n\x1b[90m\x1b[48;5;0m");
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
			for (int i = 1; i <= playerSpeed; i++)
			{
				if (i < 3) printf("\x1b[91m");
				else if (i < 5) printf("\x1b[38;5;208m");
				else printf("\x1b[92m");
				cout << "[" << i << "]";
			}
			printf("\x1b[90m\n\n {A} <-+-> {D}\n\n\n <= {esc}");

			switch (_getwch())
			{
			case arrowLeft: playerSpeed = playerSpeed - 1 >= 1 ? playerSpeed -= 1 : 8;
				break;
			case arrowRight: playerSpeed = playerSpeed + 1 <= 8 ? playerSpeed += 1 : 1;
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
			for (double i = 1; i <= playerSensitivity; i += 1)
			{
				if (i <= 3) printf("\x1b[91m");
				else if (i <= 7) printf("\x1b[38;5;208m");
				else printf("\x1b[92m");
				cout << "[" << i << "]";
			}
			printf("\x1b[90m\n\n {A} <-+-> {D}\n\n\n <= {esc}");

			switch (_getwch())
			{
			case arrowLeft: playerSensitivity = playerSensitivity - 1 >= 1 ? playerSensitivity -= 1 : 10;
				break;
			case arrowRight: playerSensitivity = playerSensitivity + 1 <= 10 ? playerSensitivity += 1 : 1;
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
			printf("\x1b[0m\t~ Настройки экрана ~\n_________________________________________________\n\n\x1b[0m");
			for (int i = 0; i <= ratioSizeScreen; i += 10)
			{
				printf("\x1b[92m");

				cout << "[" << i << "]";

			}
			printf("\x1b[90m\n\n {A} <-+-> {D}\n\n\n <= {esc}");

			switch (_getwch())
			{
			case arrowLeft: ratioSizeScreen = ratioSizeScreen - 10 >= 0 ? ratioSizeScreen -= 10 : 30;
				break;
			case arrowRight: ratioSizeScreen = ratioSizeScreen + 10 <= 30 ? ratioSizeScreen += 10 : 0;
				break;
			case escape: treker = false;
				break;
			}
			setConsoleSize();
			getConsoleSize();
			screen = new CHAR_INFO[screenWidth * screenHeight];
		}
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

int main()
{
	setlocale(LC_ALL, "rus");
	srand(time(NULL));

	initialScreensaver();
	createMap();
	getMapSize();
	getConsoleSize();
	cursoreVisibleFalse();
	setConsoleSize();

	Player player;
	screen = new CHAR_INFO[screenWidth * screenHeight];

	while (!gameOver)
	{
		map = globalMap;
		timeFinish = chrono::high_resolution_clock::now();
		timeInSeconds = chrono::duration<double>(timeFinish - timeStart).count();
		timeStart = chrono::high_resolution_clock::now();
		player.motion();
		if (_kbhit())
		{
			switch (_getwch())
			{
			case 109: printMiniMap = printMiniMap ? false : true;
				break;
			case 27: settings(player.Sensitivity, player.Speed);
				break;
			}
		}
		for (int x = 0; x < screenWidth; x++)
		{
			double rayAngle = player.R + fov / 2.0f - x * fov / screenWidth;
			double rayX = sinf(rayAngle);
			double rayY = cosf(rayAngle);
			double distanceWall = 0;
			bool itWall = false;
			bool itBound = false;

			while (!itWall && distanceWall < levelDrawing)
			{
				distanceWall += 0.01f;
				int testX = (int)(player.X + rayX * distanceWall);
				int testY = (int)(player.Y + rayY * distanceWall);

				if (testX < 0 || testX >= mapWidth || testY < 0 || testY >= mapHeight)
				{
					itWall = true;
					distanceWall = levelDrawing;
				}
				else if (map[testY * mapWidth + testX] == L'#')
				{
					itWall = true;
					vector<pair<double, double>> boundsVector;
					for (int tx = 0; tx < 2; tx++)
					{
						for (int ty = 0; ty < 2; ty++)
						{
							double vectorX = testX + tx - player.X;
							double vectorY = testY + ty - player.Y;
							double vectorModule = sqrt(vectorX * vectorX + vectorY * vectorY);
							double cosAngle = rayX * vectorX / vectorModule + rayY * vectorY / vectorModule;
							boundsVector.push_back(make_pair(vectorModule, cosAngle));
						}
					}
					sort(boundsVector.begin(), boundsVector.end(), compareByModule);
					double boundAngle = 0.05 / distanceWall;
					if (acos(boundsVector[0].second) < boundAngle || acos(boundsVector[1].second) < boundAngle) { itBound = true; }
				}
				else { map[testY * mapWidth + testX] = '.'; }
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
					screen[y * screenWidth + x].Attributes = 7;
				}
			}
		}
		outputInfo(player.X, player.Y, player.Hp, player.initialHp);
		WriteConsoleOutput(console, screen, bufferSize, { 0,0 }, &windowSize);
	}
	//playerIsAlive.detach();
	return 0;
}