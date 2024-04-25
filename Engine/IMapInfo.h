#pragma once
#include "Library.h"
#include "IMonster.h"
#include "IPlayer.h"
#include "ITeleport.h"
#include "IRestoringHealth.h"
#include "IRestoringEnergy.h"

class MapInfo
{
private:
	vector<vector<int>> maze;       //	��������� ������ ��� �������� ����� ���������� � ���� ��������
	wstring mazeString;             //	��������� ������ ��� �������� ����� ���������� � ���� ������
	int ROOM_MIN_HORIZONTAL;        //	����������� �������������� ����� �������
	int ROOM_MIN_VERTICAL;          //	����������� ������������ ������ �������
	int ROOM_MAX_HORIZONTAL;        //	������������ �������������� ����� �������
	int ROOM_MAX_VERTICAL;          //	������������ ������������ ������ �������
	int EXTRA_DOOR_CHANCE;          //	���� �������� �������������� ������ (%)
	int MIN_LENGTH_FOR_EXTRA_DOOR;  //	����������� ����� ���� ��� �������� �������������� �����
	//int NumberFloor;
	//	����� ��� �������� ����������
	void createDungeon();
	//	����������� ����� ��� ���������� ������������
	void division(int startX, int endX, int startY, int endY);
	//	����� ��� �������� �������
	void createRoom(bool isHorizontal, int startX, int endX, int startY, int endY);
	//	����� ��� �������� ������ ��� �����
	void createBorders();

public:
	//	����� ��� ������� � �������� ����� �������� �����
	MapInfo* mapInfo;
	Player* playerInfo;
	Monster* monsterInfo;
	Teleport* teleportInfo;
	RestoringHealth* restoringHealthInfo;
	RestoringEnergy* restoryngEnergyInfo;

	wstring map;			//	�����
	bool* mapPlayerSaw;		//	������ ����� ��� ������������ ���� ��� ����� �����
	wstring initialMap;		//	������������ ����� ����� ( ������������ ��� ������� ����� )
	int mapSizeHorizontal;	//	�������������� ������ �����
	int mapSizeVertical;	//	������������ ������ �����

	//	����������� ������
	MapInfo(Player& _playerInfo,
		Monster& _monsterInfo,
		Teleport& _teleportInfo,
		RestoringHealth& _restoringHealthInfo,
		RestoringEnergy& _restoryngEnergyInfo);
	//	����� ��� ��������� �����
	void clearmap();
	//	����� ��� ������ ����� ���������� � �������
	void printDungeon();
	//	����� ����� ��
	vector<vector<int>> getIntMaze();
	// ����� ��� �������� �� ������ � wstring
	wstring getWstringMaze();
	//	������������� ��������� ������� ������ �� ����� 
	void setPlayerStartingCoordinates();
	//	������������� ���������� ��������� �� �����
	void setTeleportCoordinates();
	//	�������� �����
	void createmap();
	// ��������� ������� ������ �� �����
	template <typename T>
	void setPositionObject(T minDistance = 1) {
		srand(time(nullptr));
		size_t count = 5;
		vector<COORD> objPosition;

		for (size_t c = 0; c < count; ++c) {
			srand(rand());
			bool validLocation = false;

			while (!validLocation) {
				int objectX = rand() % (mapSizeHorizontal - 2) + 1;
				int objectY = rand() % (mapSizeVertical - 2) + 1;

				if (maze[objectY][objectX] == 0 || maze[objectY][objectX] == 2) {
					bool tooClose = false;

					for (const auto& pos : objPosition) {
						// ��������� ���������� ����� ����� �������� � ��� �������������
						if (std::sqrt(std::pow(objectX - pos.X, 2) + std::pow(objectY - pos.Y, 2)) < minDistance) {
							tooClose = true;
							break;
						}
					}

					if (!tooClose) {
						validLocation = true;
						COORD cord;
						cord.Y = objectY;
						cord.X = objectX;
						objPosition.push_back(cord);
					}
				}
			}
		}

		playerInfo->setPositionX(objPosition[0].X);
		playerInfo->setPositionY(objPosition[0].Y);

		monsterInfo->setX(objPosition[1].X);
		monsterInfo->setY(objPosition[1].Y);

		restoringHealthInfo->setX(objPosition[2].X);
		restoringHealthInfo->setY(objPosition[2].Y);
		map[restoringHealthInfo->getY() * mapSizeHorizontal + restoringHealthInfo->getX()] = restoringHealthInfo->getMapSkin();


		restoryngEnergyInfo->setX(objPosition[3].X);
		restoryngEnergyInfo->setY(objPosition[3].Y);
		map[restoryngEnergyInfo->getY() * mapSizeHorizontal + restoryngEnergyInfo->getX()] = restoryngEnergyInfo->getMapSkin();


		teleportInfo->setX(objPosition[4].X);
		teleportInfo->setY(objPosition[4].Y);
	}


};