#pragma once
#include "Library.h"
#include "IMapInfo.h"
#include "IPlayer.h"
enum keys;
class Engine
{
public:
	Engine(Player& player, MapInfo& mapInfo);
	void start();
	bool checkPlayerInTeleport();
	void getConsoleSize();
	void setScreenSize();
	void setCursoreVisible();
	void generateInfoFrame();
	void openSettings();
	void generateFrame();

	MapInfo* mapInfo;
	Player* playerInfo;
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
	
};