#include "Library.h"
#include "IMapInfo.h"

//	����� ��� �������� ����������
void MapInfo::createDungeon()
{
	bool dungeonCreated = false;
	while (!dungeonCreated)
	{
		maze.clear();
		maze.resize(mapSizeVertical, vector<int>(mapSizeHorizontal, 0));

		division(1, mapSizeHorizontal - 2, 1, mapSizeVertical - 2); // �������� ����������

		dungeonCreated = false; // ������������, ��� ���������� �������

		// ��������, ���� �� ������� ���������� (���� � ����� ����������� �����)
		for (int y = 0; y < mapSizeVertical; ++y)
		{
			for (int x = 0; x < mapSizeHorizontal; ++x)
			{
				if (maze[y][x] == 1)
				{
					dungeonCreated = true; // ���� ������� �����, �� ���������� �������
					break;
				}
			}
			if (dungeonCreated)
				break;
		}
	}
}
//	����������� ����� ��� ���������� ������������
void MapInfo::division(int startX, int endX, int startY, int endY)
{
	srand(rand());
	// ������� ������ �� ��������: ���������� ����������, ���� ������ ������������ ������ ��� ������� ����������� �������
	if (endX - startX + 1 < ROOM_MIN_HORIZONTAL * 2 && endY - startY + 1 < ROOM_MIN_VERTICAL * 2)
		return;

	// ����������� ����������� ���������� �� ������ �������� ������������
	if (endX - startX > endY - startY)
	{
		if (endX - startX + 1 < ROOM_MIN_HORIZONTAL * 2)
			return;
		createRoom(true, startX, endX, startY, endY); // �������� �������������� �������
	}
	else if (endX - startX < endY - startY)
	{
		if (endY - startY + 1 < ROOM_MIN_VERTICAL * 2)
			return;
		createRoom(false, startX, endX, startY, endY); // �������� ������������ �������
	}
	else
	{
		if (rand() % 2 == 0)
		{
			if (endX - startX + 1 < ROOM_MIN_HORIZONTAL * 2)
				return;
			createRoom(true, startX, endX, startY, endY); // �������� �������������� �������
		}
		else
		{
			if (endY - startY + 1 < ROOM_MIN_VERTICAL * 2)
				return;
			createRoom(false, startX, endX, startY, endY); // �������� ������������ �������
		}
	}
}
//	����� ��� �������� �������
void MapInfo::createRoom(bool isHorizontal, int startX, int endX, int startY, int endY)
{
	srand(rand());
	int roomWidth = rand() % (ROOM_MAX_HORIZONTAL - ROOM_MIN_HORIZONTAL + 1) + ROOM_MIN_HORIZONTAL;
	int roomHeight = rand() % (ROOM_MAX_VERTICAL - ROOM_MIN_VERTICAL + 1) + ROOM_MIN_VERTICAL;
	if (isHorizontal)
	{
		// ��������� ��������� ������� ��� ������������ �����
		int verticalWall = startX + roomWidth + rand() % (endX - startX - roomWidth + 1);
		if (verticalWall % 2 == 0)
		{ // ������ �������� �����
			verticalWall++;
			if (verticalWall >= endX + 1 - roomWidth)
				verticalWall -= 2;
		}
		// ���������� ������������ ������������ ������
		for (int i = startY; i <= endY; ++i)
		{
			maze[i][verticalWall] = 1;
		}
		// ���������� �������������� ������
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
			maze[randomDoor][verticalWall] = 2; // ������� �����
		}
		// ����������� ���������� ������������ ����� � ������ �� �����
		division(startX, verticalWall - 1, startY, endY);
		division(verticalWall + 1, endX, startY, endY);
	}
	else
	{
		// ��������� ��������� ������� ��� �������������� �����
		int horizontalWall = startY + roomHeight + rand() % (endY - startY - roomHeight + 1);
		if (horizontalWall % 2 == 0)
		{ // ������ �������� �����
			horizontalWall++;
			if (horizontalWall >= endY + 1 - roomHeight)
				horizontalWall -= 2;
		}
		// ���������� ������������ �������������� ������
		for (int i = startX; i <= endX; ++i)
		{
			maze[horizontalWall][i] = 1;
		}
		// ���������� �������������� ������
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
			maze[horizontalWall][randomDoor] = 2; // ������� �����
		}
		// ����������� ���������� ������������ ���� � ���� �����
		division(startX, endX, startY, horizontalWall - 1);
		division(startX, endX, horizontalWall + 1, endY);
	}
}
//	����� ��� �������� ������ ��� �����
void MapInfo::createBorders()
{
	for (int i = 0; i < mapSizeHorizontal; ++i)
	{
		maze[0][i] = 1;                         // ������� �������
		maze[mapSizeVertical - 1][i] = 1;     // ������ �������
	}

	for (int i = 0; i < mapSizeVertical; ++i)
	{
		maze[i][0] = 1;                         // ����� �������
		maze[i][mapSizeHorizontal - 1] = 1;  // ������ �������
	}
}
//	����� ��� ��������� �����
void MapInfo::clearmap() { map = initialMap; }
//	����� ��� ������ ����� ���������� � �������
void MapInfo::printDungeon()
{
	for (int y = 0; y < mapSizeVertical; ++y)
	{
		for (int x = 0; x < mapSizeHorizontal; ++x)
		{
			if (maze[y][x] == 0)
				cout << '.'; // ������ ������������
			else if (maze[y][x] == 1)
				cout << '#'; // �����
			else if (maze[y][x] == 2)
				cout << 'D'; // �����
			else
				cout << '?'; // �� ������������ ������
		}
		cout << endl;
	}
}
//	����� ��� ��������� maze
vector<vector<int>> MapInfo::getIntMaze() { return maze; }
//	����� ��� �������� �� ������ � wstring
wstring MapInfo::getWstringMaze()
{
	for (int y = 0; y < mapSizeVertical; ++y)
	{
		for (int x = 0; x < mapSizeHorizontal; ++x)
		{
			if (maze[y][x] == 0)
				mazeString += L' '; // ������ ������������
			else if (maze[y][x] == 1)
				mazeString += L'#'; // �����
			else if (maze[y][x] == 2)
				mazeString += L' '; // �����
			else
				mazeString += L'?'; // �� ������������
		}
		//mazeString += L'\n';
	}
	return mazeString;
}
//	��������� ���������� ������
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
//  ���������� ���������
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
	// ��������� ��������, ����� �� ������� ����� ���������� ���������� ����� ������������� ����� �������
	// ����� �����, ��������, ������������� �������� ���������� ��� �������� �������

	srand(time(nullptr)); // �������������� ��������� ��������� �����
	teleportInfo->setX(rand() % (mapSizeHorizontal - 2) + 1);
	teleportInfo->setY(rand() % (mapSizeVertical - 2) + 1);
}
//// �������� ���������� ���������
//void MapInfo::setteleport�oordinates()
//{
//	while (true)
//	{
//		teleport�oordinates.first = rand() % (mapSizeHorizontal - 2) + 1;
//		teleport�oordinates.second = rand() % (mapSizeVertical - 2) + 1;
//		if (map[teleport�oordinates.second * mapSizeHorizontal + teleport�oordinates.first] != '#'
//			&& abs(teleport�oordinates.first - playerStartingCoordinates.first) > mapSizeHorizontal * 0.6
//			&& abs(teleport�oordinates.second - playerStartingCoordinates.second) > mapSizeVertical * 0.6)
//			return;
//	}
//}
// 

// �������� �����
void MapInfo::createmap()
{
	createDungeon();
	createBorders();
	map = getWstringMaze();	//	������� ����� 
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

	mapPlayerSaw = new bool[mapSizeHorizontal * mapSizeVertical];	//	������ ���� ��� ����� �����

	//	��������� ����� ���������� ��� �� ����� ���� �������
	for (int x = 0; x < mapSizeHorizontal; x++)
		for (int y = 0; y < mapSizeVertical; y++)
			mapPlayerSaw[y * x] = true;

	setPositionObject(20);	//	����������� ������� �� �����
	////	������������� ��������� ������� ������ �� ����� 
	//setPlayerStartingCoordinates();

	////	������������� �������� ��������� �� �����
	//setTeleportCoordinates();

	////	��������
	//map[teleportInfo->getY() * mapSizeHorizontal + teleportInfo->getX()] = teleportInfo->getMapSkin();

	////	���� ����� ������ � ���������
	//playerInfo->setPositionX(teleportInfo->getX() + 2);
	//playerInfo->setPositionY(teleportInfo->getY() + 2);

	////	���� ����� ����� � ���������
	//restoringHealthInfo->setX(teleportInfo->getX() + 1);
	//restoringHealthInfo->setY(teleportInfo->getY() + 1);
	//map[restoringHealthInfo->getY() * mapSizeHorizontal + restoringHealthInfo->getX()] = restoringHealthInfo->getMapSkin();

	////	���� ����� ������� � ���������
	//restoryngEnergyInfo->setX(teleportInfo->getX() - 2);
	//restoryngEnergyInfo->setY(teleportInfo->getY() - 2);
	//map[restoryngEnergyInfo->getY() * mapSizeHorizontal + restoryngEnergyInfo->getX()] = restoryngEnergyInfo->getMapSkin();

	////	���� ����� ������� � ���������
	//monsterInfo->setX(teleportInfo->getX() - 1);
	//monsterInfo->setY(teleportInfo->getY() - 1);

	//	������ �����
	initialMap = map;
}

//	����������� ������
MapInfo::MapInfo(
	Player& _playerInfo,
	Monster& _monsterInfo,
	Teleport& _teleportInfo,
	RestoringHealth& _restoringHealthInfo,
	RestoringEnergy& _restoryngEnergyInfo)
{
	//	�������������� ������� �����
	playerInfo = &_playerInfo;
	monsterInfo = &_monsterInfo;
	restoringHealthInfo = &_restoringHealthInfo;
	restoryngEnergyInfo = &_restoryngEnergyInfo;
	teleportInfo = &_teleportInfo;

	//	�� ���� ��� ���
	maze.resize(mapSizeVertical, vector<int>(mapSizeHorizontal, 0));
	ROOM_MIN_HORIZONTAL = 6;        // ����������� �������������� ����� �������
	ROOM_MIN_VERTICAL = 6;          // ����������� ������������ ������ �������
	ROOM_MAX_HORIZONTAL = 10;       // ������������ �������������� ����� �������
	ROOM_MAX_VERTICAL = 10;         // ������������ ������������ ������ �������
	EXTRA_DOOR_CHANCE = 15;         // ���� �������� �������������� ������ (%)
	MIN_LENGTH_FOR_EXTRA_DOOR = 40;	// ����������� ����� ���� ��� �������� �������������� �����
	mapSizeHorizontal = 50;         // �������������� ������ �����
	mapSizeVertical = 50;           // ������������ ������ �����

}