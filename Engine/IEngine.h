#pragma once
#include "Library.h"
#include "IMapInfo.h"
#include "IPlayer.h"
#include "IMonster.h"
#include "IRestoringHealth.h"

enum keys;
class Engine
{
public:
	Engine(
		Player& _playerInfo,
		Monster& _monsterInfo,
		Teleport& _teleportInfo,
		RestoringHealth& _restoringHealthInfo,
		RestoringEnergy& _restoringEnergyInfo,
		MapInfo& _mapInfo);	//	�����������

	bool mapViewBoundaryCheck(int x, int y); //	����� ��� ������������ ������ ��������� �����
	void checkPlayerUseObject();	//	������ ��� ����� � ��������� ��� ���
	void getConsoleSize();			//	�������� ������ ����������� ����
	void setScreenSize();			//	������������� ������ ����� ��� ������ �������
	void setCursoreVisible();		//	������������� ��������� �������
	void generateInfoFrame();		//	���������� ����� �����
	void openSettings();			//	�������� ��������
	void generateFrame();			//	���������� ����
	void start();					//	��������� ������

	//	����� ��� ������� � �������� ����� �������� ������
	MapInfo* mapInfo;
	Player* playerInfo;
	Monster* monsterInfo;
	Teleport* teleportInfo;
	RestoringHealth* restoringHealthInfo;
	RestoringEnergy* restoringEnergyInfo;

	//	���� ������
	HANDLE console;
	COORD consoleBufferSize;
	SMALL_RECT windowSize;
	CHAR_INFO* screen;
	int screenWidth;
	int screenHeight;
	chrono::high_resolution_clock::time_point startFrameGeneration;
	chrono::high_resolution_clock::time_point endFrameGeneration;
	double frameGenerationTimeInSeconds;
	double drawingRange;
	double fov;
	bool gameIsOver;
	bool displayMap;
	bool displayFps;
	bool settingsIsOpen;
	bool playerMovedToNextFloor;
	bool frameIsBuild;
	bool frameInfoIsBuild;
	double texturingLevel;
	
};