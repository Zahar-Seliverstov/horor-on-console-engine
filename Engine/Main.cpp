#include "Library.h"
#include "IScreensavers.h"
#include "IMapInfo.h"
#include "IPlayer.h"
#include "IEngine.h"

MapInfo mapInfo;
Player player;
Engine engine(player, mapInfo);
int numberFloor = 0;

void generateInitialFloor()
{
	cout << "Map is generating. . .";
	mapInfo.createmap();
	initialScreensaver();

	player.x = mapInfo.finishCoordinat.first + 1;
	player.y = mapInfo.finishCoordinat.second + 1;

	engine.getConsoleSize();
	engine.setScreenSize();

	engine.setCursoreVisible();
	PrintGameTitle(engine.screenWidth, engine.screenHeight);
	engine.start();
}

int main()
{
	setlocale(LC_ALL, "rus");
	system("chcp 1251");
	srand(time(NULL));

	generateInitialFloor();
	return 0;
}