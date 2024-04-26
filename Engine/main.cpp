#include "Library.h"
#include "IScreensavers.h"
#include "IMapInfo.h"
#include "IPlayer.h"
#include "IEngine.h"
#include "IMonster.h"
#include "ITeleport.h"
#include "IRestoringHealth.h"
#include "IRestoringEnergy.h"


//	������� ������ ��� ������ � ���� �����
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

//	���������� ��������� ����
void generateInitialFloor() {
	mapInfo.createmap();					//	������� �����
	engineInfo.getConsoleSize();			//	�������� ������ �������
	PrintGameTitle(engineInfo.screenWidth, engineInfo.screenHeight);	//	����� ����
	initialScreensaver();					//	����� ������
	engineInfo.getConsoleSize();			//	�������� ������ �������	
	engineInfo.setScreenSize();				//	������������� ������ ������� ������� ��������� �������
	engineInfo.setCursoreVisible();			//	������ ������ �� �������
	engineInfo.start();						//	��������� ������
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
	// ��� ��� �� ���������� �� ������� � ��� �� ���������� ����
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(fontInfo);
	GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo); // �������� ������� �����

	// ������ �������� ������
	fontInfo.dwFontSize.X = 12; // ������ ������
	fontInfo.dwFontSize.Y = 18; // ������ ������
	wcscpy_s(fontInfo.FaceName, L"Cascadia Code"); // �������� ������

	SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);

	setlocale(LC_ALL, "rus");
	system("chcp 65001");
	//system("chcp 1251");
	srand(time(NULL));

	//	��������� ���� ��������� � ����������� �� ������
	for (int numberFloor = 1; numberFloor <= maxCountFloor; numberFloor++)
		numberFloor == 1 ? generateInitialFloor() : generateNextFloor();

	return 0;
}