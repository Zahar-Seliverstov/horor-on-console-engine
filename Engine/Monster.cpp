#include "IMonster.h"
#include "IPlayer.h"
#include "Library.h"

//	Конструктор
Monster::Monster(double& _playerPositionX, double& _playerPositionY) {
	x = 0;
	y = 0;
	/*playerPositionX = &_playerPositionX;
	playerPositionY = &_playerPositionY;*/

	health = 100;
	damage = 10;
	speed = 3;

	initialSpeed = speed;
	initialHealth = health;

	attributeFrontColor = 7;
	attributeEdge = 0;
	edgeSkin = L' ';
	frontSkin = L'▒';
	mapSkin = L'M';
}

// get
int Monster::getX() { return x; }
int Monster::getY() { return y; }
double* Monster::getPlayerPositionX() { return playerPositionX; }
double* Monster::getPlayerPositionY() { return playerPositionY; }
int Monster::getHealth() { return health; }
int Monster::getDamage() { return damage; }
int Monster::getSpeed() { return speed; }
int Monster::getInitialSpeed() { return initialSpeed; }
int Monster::getInitialHealth() { return initialHealth; }
int Monster::getAttributeFrontColor() { return attributeFrontColor; }
int Monster::getAttributeEdge() { return attributeEdge; }
wchar_t Monster::getEdgeSkin() { return edgeSkin; }
wchar_t Monster::getFrontSkin() { return frontSkin; }
wchar_t Monster::getMapSkin() { return mapSkin; }

// set
void Monster::setX(int newX) { x = newX; }
void Monster::setY(int newY) { y = newY; }
void Monster::setPlayerPositionX(double* newX) { playerPositionX = newX; }
void Monster::setPlayerPositionY(double* newY) { playerPositionY = newY; }
void Monster::setHealth(int newHealth) { health = newHealth; }
void Monster::setDamage(int newDamage) { damage = newDamage; }
void Monster::setSpeed(int newSpeed) { speed = newSpeed; }
void Monster::setInitialSpeed(int newInitialSpeed) { initialSpeed = newInitialSpeed; }
void Monster::setInitialHealth(int newInitialHealth) { initialHealth = newInitialHealth; }
void Monster::setAttributeFrontColor(int newAttributeFrontColor) { attributeFrontColor = newAttributeFrontColor; }
void Monster::setAttributeEdge(int newAttributeEdge) { attributeEdge = newAttributeEdge; }
void Monster::setEdgeSkin(wchar_t newEdgeSkin) { edgeSkin = newEdgeSkin; }
void Monster::setFrontSkin(wchar_t newFrontSkin) { frontSkin = newFrontSkin; }
void Monster::setMapSkin(wchar_t newMapSkin) { mapSkin = newMapSkin; }

//	Удар
int Monster::hitPlayer(int playerHp) {
	//PlaySound(L"sounds/hitPlayer.wav", NULL, SND_ASYNC);

	return (playerHp - damage < 0 ? 0 : (playerHp - damage));
}

//	Движение
void Monster::movement(int& mapSizeVertical,
	int& mapSizeHorizontal,
	wstring& map,
	double playerPositionX, double playerPositionY)
{
	int gg = 0;
	int px = int(playerPositionX);
	int py = int(playerPositionY);
	gg = abs((x + y) - (px + py));
	if (gg <= 2) { canHit = true; }
	else { canHit = false; }

	//	Ходьба монстра ЗА ИГРОКОМ
	if (gg <= 10) {
		mciSendString(L"play sounds/moveWall.wav", NULL, SND_ASYNC, NULL);
		if (px > x) { x += 1; }
		if (px < x) { x -= 1; }
		if (py > y) { y += 1; }
		if (py < y) { y -= 1; }
		this_thread::sleep_for(chrono::milliseconds(1500));
	}
	else
	{
		//	ИДЕТ К СТЕНКЕ
		if (map[(y * mapSizeHorizontal) + x] != L'#')
		{
			mciSendString(L"play sounds/moveWall.wav", NULL, SND_ASYNC, NULL);

			//y += 1;
			y -= 1;
			//x += 1;
			//x -= 1;
			this_thread::sleep_for(chrono::milliseconds(1500));
		}
		//	ХОДИТ ПО СТЕНКЕ
		else
		{
			//mciSendString(L"play sounds/moveWall.wav", NULL, SND_ASYNC, NULL);
			switch (rand() % 4 + 1)
			{
			case 1:
				if (map[(y + 1) * mapSizeHorizontal + x] == L'#' && (y + 1) < mapSizeVertical)
				{
					y += 1;
				}break;
			case 2:
				if (map[(y - 1) * mapSizeHorizontal + x] == L'#' && (y - 1) > mapSizeVertical)
				{
					y -= 1;
				}break;
			case 3:
				if (map[y * mapSizeHorizontal + (x + 1)] == L'#' && (x + 1) < mapSizeHorizontal)
				{
					x += 1;
				}break;
			case 4:
				if (map[y * mapSizeHorizontal + (x - 1)] == L'#' && (x - 1) > mapSizeHorizontal)
				{
					x -= 1;
				}break;
				this_thread::sleep_for(chrono::milliseconds(200));
			}
		}
	}
}
