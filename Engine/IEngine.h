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
		MapInfo& _mapInfo);	//	Конструктор

	bool mapViewBoundaryCheck(int x, int y); //	Метод для отслеживания границ увиденной карты
	void checkPlayerUseObject();	//	Чекаем что игрок в телепорте или нет
	void getConsoleSize();			//	Получаем размер консольного окна
	void setScreenSize();			//	Устанавливаем размер кадра под размер консоли
	void setCursoreVisible();		//	Устанавливаем видимость курсора
	void generateInfoFrame();		//	Генерирует маску кадра
	void openSettings();			//	Открытие настроик
	void generateFrame();			//	Генерирует кадр
	void start();					//	Запускаем движок

	//	Места под обьекты с которыми может работать движок
	MapInfo* mapInfo;
	Player* playerInfo;
	Monster* monsterInfo;
	Teleport* teleportInfo;
	RestoringHealth* restoringHealthInfo;
	RestoringEnergy* restoringEnergyInfo;

	//	Поля движка
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