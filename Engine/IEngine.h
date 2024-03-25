#pragma once
#include "Library.h"

enum keys;
class Engine
{
private:
	void checkPlayerInTeleport();
	void getConsoleSize();
	void setScreenSize();
	void cursoreVisibleFalse();
	void outputInfo();
	void settings();
	void renderingConsoleGraphics();
public:
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD bufferSize;
	SMALL_RECT windowSize;
	CHAR_INFO* screen;
	int screenWidth;
	int screenHeight;

	chrono::high_resolution_clock::time_point timeStart = chrono::high_resolution_clock::now();
	chrono::high_resolution_clock::time_point timeFinish = chrono::high_resolution_clock::now();
	double timeInSeconds;

	double levelDrawing;
	double fov;
	bool gameOver;
	bool printMinimap;
	bool settingsIsOpen;
	bool playerGoNextLevel;
	bool frameBuilding;
	
	Engine();
	void Run();
};