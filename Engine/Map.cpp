#include "Library.h"
#include "IMapInfo.h"

//	Метод для создания подземелья
void MapInfo::createDungeon()
{
	bool dungeonCreated = false;
	while (!dungeonCreated)
	{
		maze.clear();
		maze.resize(mapSizeVertical, vector<int>(mapSizeHorizontal, 0));

		division(1, mapSizeHorizontal - 2, 1, mapSizeVertical - 2); // Создание подземелья

		dungeonCreated = false; // Предполагаем, что подземелье создано

		// Проверка, было ли создано подземелье (если в карте отсутствуют стены)
		for (int y = 0; y < mapSizeVertical; ++y)
		{
			for (int x = 0; x < mapSizeHorizontal; ++x)
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
//	Рекурсивный метод для разделения пространства
void MapInfo::division(int startX, int endX, int startY, int endY)
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
//	Метод для создания комнаты
void MapInfo::createRoom(bool isHorizontal, int startX, int endX, int startY, int endY)
{
	srand(rand());
	int roomWidth = rand() % (ROOM_MAX_HORIZONTAL - ROOM_MIN_HORIZONTAL + 1) + ROOM_MIN_HORIZONTAL;
	int roomHeight = rand() % (ROOM_MAX_VERTICAL - ROOM_MIN_VERTICAL + 1) + ROOM_MIN_VERTICAL;
	if (isHorizontal)
	{
		// Генерация случайной позиции для вертикальной стены
		int verticalWall = startX + roomWidth + rand() % (endX - startX - roomWidth + 1);
		if (verticalWall % 2 == 0)
		{ // Только нечетные числа
			verticalWall++;
			if (verticalWall >= endX + 1 - roomWidth)
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
		int horizontalWall = startY + roomHeight + rand() % (endY - startY - roomHeight + 1);
		if (horizontalWall % 2 == 0)
		{ // Только нечетные числа
			horizontalWall++;
			if (horizontalWall >= endY + 1 - roomHeight)
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
//	Метод для создания границ для карты
void MapInfo::createBorders()
{
	for (int i = 0; i < mapSizeHorizontal; ++i)
	{
		maze[0][i] = 1;                         // Верхняя граница
		maze[mapSizeVertical - 1][i] = 1;     // Нижняя граница
	}

	for (int i = 0; i < mapSizeVertical; ++i)
	{
		maze[i][0] = 1;                         // Левая граница
		maze[i][mapSizeHorizontal - 1] = 1;  // Правая граница
	}
}
//	Метод для отчищения карты
void MapInfo::clearmap() { map = initialMap; }
//	Метод для вывода карты подземелья в консоль
void MapInfo::printDungeon()
{
	for (int y = 0; y < mapSizeVertical; ++y)
	{
		for (int x = 0; x < mapSizeHorizontal; ++x)
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
//	Метод для получения maze
vector<vector<int>> MapInfo::getIntMaze() { return maze; }
//	Метод для перевода из масива в wstring
wstring MapInfo::getWstringMaze()
{
	for (int y = 0; y < mapSizeVertical; ++y)
	{
		for (int x = 0; x < mapSizeHorizontal; ++x)
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
//	Стартовые координаты игрока
void MapInfo::setPlayerStartingCoordinates()
{
	while (true)
	{
		playerInfo->setPositionX(rand() % (mapSizeHorizontal - 2) + 1);
		playerInfo->setPositionY(rand() % (mapSizeVertical - 2) + 1);
		if (
			map[playerInfo->getPositionY() * mapSizeHorizontal + playerInfo->getPositionX()] != '#'
			&& (playerInfo->getPositionY() < (mapSizeVertical * 0.10) || playerInfo->getPositionY() > (mapSizeVertical * 0.90))
			|| (playerInfo->getPositionX() < (mapSizeHorizontal * 0.10) || playerInfo->getPositionX() > (mapSizeHorizontal * 0.90))
			)
			return;
	}
}
//  Координаты телепорта
void MapInfo::setTeleportCoordinates()
{
	int const maxAttemts = 100;
	int attempts = 0;
	while (attempts < maxAttemts)
	{
		teleportInfo->setX(rand() % (mapSizeHorizontal - 2) + 1);
		teleportInfo->setY(rand() % (mapSizeVertical - 2) + 1);
		if (map[teleportInfo->getY() * mapSizeHorizontal + teleportInfo->getX()] != '#')
		{
			if (abs(teleportInfo->getX() - playerInfo->getPositionX()) > mapSizeHorizontal * 0.6 &&
				abs(teleportInfo->getY() - playerInfo->getPositionY()) > mapSizeVertical * 0.6)
			{
				return;
			}
		}
		attempts++;
	}
	// Обработка ситуации, когда не удалось найти подходящие координаты после максимального числа попыток
	// Здесь можно, например, сгенерировать финишные координаты без проверки условий

	srand(time(nullptr)); // Инициализируем генератор случайных чисел
	teleportInfo->setX(rand() % (mapSizeHorizontal - 2) + 1);
	teleportInfo->setY(rand() % (mapSizeVertical - 2) + 1);
}
//// Финишные координаты телепорта
//void MapInfo::setteleportСoordinates()
//{
//	while (true)
//	{
//		teleportСoordinates.first = rand() % (mapSizeHorizontal - 2) + 1;
//		teleportСoordinates.second = rand() % (mapSizeVertical - 2) + 1;
//		if (map[teleportСoordinates.second * mapSizeHorizontal + teleportСoordinates.first] != '#'
//			&& abs(teleportСoordinates.first - playerStartingCoordinates.first) > mapSizeHorizontal * 0.6
//			&& abs(teleportСoordinates.second - playerStartingCoordinates.second) > mapSizeVertical * 0.6)
//			return;
//	}
//}
// 

// Создание карты
void MapInfo::createmap()
{
	createDungeon();
	createBorders();
	map = getWstringMaze();	//	Полуаем карту 
	/*map += L"##############";
	map += L"#            #";
	map += L"#            #";
	map += L"#            #";
	map += L"#            #";
	map += L"#            #";
	map += L"#            #";
	map += L"#            #";
	map += L"#            #";
	map += L"#            #";
	map += L"#            #";
	map += L"#            #";
	map += L"#            #";
	map += L"##############";

	mapSizeHorizontal = 14;
	mapSizeVertical = 14;*/

	mapPlayerSaw = new bool[mapSizeHorizontal * mapSizeVertical];	//	Массив того что видел игрок

	//	Заполняем масив изночально что бы карта была закрыта
	for (int x = 0; x < mapSizeHorizontal; x++)
		for (int y = 0; y < mapSizeVertical; y++)
			mapPlayerSaw[y * x] = true;

	setPositionObject(20);	//	Распологаем обьекты на карте
	////	Устанавливаем стартовую позицию игрока на карте 
	//setPlayerStartingCoordinates();

	////	Устанавливаем кпозицию телепорта на карте
	//setTeleportCoordinates();

	////	Телепорт
	//map[teleportInfo->getY() * mapSizeHorizontal + teleportInfo->getX()] = teleportInfo->getMapSkin();

	////	ТЕСТ спаун ИГРОКА у телепорта
	//playerInfo->setPositionX(teleportInfo->getX() + 2);
	//playerInfo->setPositionY(teleportInfo->getY() + 2);

	////	ТЕСТ спаун ХИЛКИ у телепорта
	//restoringHealthInfo->setX(teleportInfo->getX() + 1);
	//restoringHealthInfo->setY(teleportInfo->getY() + 1);
	//map[restoringHealthInfo->getY() * mapSizeHorizontal + restoringHealthInfo->getX()] = restoringHealthInfo->getMapSkin();

	////	ТЕСТ спаун СТАМИНЫ у телепорта
	//restoryngEnergyInfo->setX(teleportInfo->getX() - 2);
	//restoryngEnergyInfo->setY(teleportInfo->getY() - 2);
	//map[restoryngEnergyInfo->getY() * mapSizeHorizontal + restoryngEnergyInfo->getX()] = restoryngEnergyInfo->getMapSkin();

	////	ТЕСТ спаун МОНСТРА у телепорта
	//monsterInfo->setX(teleportInfo->getX() - 1);
	//monsterInfo->setY(teleportInfo->getY() - 1);

	//	СТРОГО ЗДЕСЬ
	initialMap = map;
}

//	Конструктор класса
MapInfo::MapInfo(
	Player& _playerInfo,
	Monster& _monsterInfo,
	Teleport& _teleportInfo,
	RestoringHealth& _restoringHealthInfo,
	RestoringEnergy& _restoryngEnergyInfo)
{
	//	Инициализируем обьекты карты
	playerInfo = &_playerInfo;
	monsterInfo = &_monsterInfo;
	restoringHealthInfo = &_restoringHealthInfo;
	restoryngEnergyInfo = &_restoryngEnergyInfo;
	teleportInfo = &_teleportInfo;

	//	Не знаю что это
	maze.resize(mapSizeVertical, vector<int>(mapSizeHorizontal, 0));
	ROOM_MIN_HORIZONTAL = 6;        // Минимальная горизонтальная длина комнаты
	ROOM_MIN_VERTICAL = 6;          // Минимальная вертикальная высота комнаты
	ROOM_MAX_HORIZONTAL = 10;       // Максимальная горизонтальная длина комнаты
	ROOM_MAX_VERTICAL = 10;         // Максимальная вертикальная высота комнаты
	EXTRA_DOOR_CHANCE = 15;         // Шанс создания дополнительных дверей (%)
	MIN_LENGTH_FOR_EXTRA_DOOR = 40;	// Минимальная длина пути для создания дополнительной двери
	mapSizeHorizontal = 50;         // Горизонтальный размер карты
	mapSizeVertical = 50;           // Вертикальный размер карты

}