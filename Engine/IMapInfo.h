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
	void setPositionObject(int minDistance);

};