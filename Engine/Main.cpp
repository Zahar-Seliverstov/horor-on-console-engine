#include "Library.h"
#include "IScreensavers.h"
#include "IMapInfo.h"
#include "IPlayer.h"
#include "IEngine.h"
#include "IMonster.h"
#include "ITeleport.h"
#include "IRestoringHealth.h"
#include "IRestoringEnergy.h"

Player playerInfo;
RestoringEnergy restoringEnergyInfo;
RestoringHealth restoringHealthInfo;
Teleport teleportInfo;
Monster monsterInfo(playerInfo.x, playerInfo.y);
MapInfo mapInfo(
	playerInfo,
	monsterInfo,
	teleportInfo,
	restoringHealthInfo,
	restoringEnergyInfo);
Engine engineInfo(
	playerInfo,
	monsterInfo,
	teleportInfo,
	restoringHealthInfo,
	restoringEnergyInfo,
	mapInfo);

void generateInitialFloor()
{
	mapInfo.createmap();
	initialScreensaver();

	//playerInfo.setPositionX(teleportInfo.getX() + 1);
	//playerInfo.setPositionX(teleportInfo.getY() + 1);

	engineInfo.getConsoleSize();
	engineInfo.setScreenSize();

	engineInfo.setCursoreVisible();
	//PrintGameTitle(engine.screenWidth, engine.screenHeight);
	engineInfo.start();
}
void generateNextFloor()
{
	mapInfo.createmap();
	//initialScreensaver();

	playerInfo.setPositionX(teleportInfo.getX() + 1);
	playerInfo.setPositionX(teleportInfo.getY() + 1);

	engineInfo.getConsoleSize();
	engineInfo.setScreenSize();

	engineInfo.setCursoreVisible();
	DownloadScreensaver(engineInfo.screenWidth, engineInfo.screenHeight);
	engineInfo.start();
}

int main()
{
	//int numberFloor;
	int maxCountFloor = 10;
	// Это так мы баловались со шрифтом и что то получилось даже
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(fontInfo);
	GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo); // Получаем текущий шрифт

	// Задаем параметр шрифта
	fontInfo.dwFontSize.X = 12; // Ширина шрифта
	fontInfo.dwFontSize.Y = 18; // Высота шрифта
	wcscpy_s(fontInfo.FaceName, L"Cascadia Code"); // Название шрифта

	SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);

	setlocale(LC_ALL, "rus");
	//system("chcp 65001");
	system("chcp 1251");

	srand(time(NULL));
	/*UINT codePage = GetConsoleOutputCP();
	std::cout << codePage;*/

	//	Запускаем цикл генерации и перемещения по этажам
	for (int numberFloor = 1; numberFloor <= maxCountFloor; numberFloor++)
		numberFloor == 1 ? generateInitialFloor() : generateNextFloor();
	/*generateInitialFloor();
	generateNextFloor();*/

	return 0;
}