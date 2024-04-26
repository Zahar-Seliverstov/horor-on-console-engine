#include "IMonster.h"
#include "IPlayer.h"
#include "Library.h"

//	Конструктор
Monster::Monster(double& _playerPositionX, double& _playerPositionY) {
	x = 0;
	y = 0;

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
	return (playerHp - damage < 0 ? 0 : (playerHp - damage));
}

//	Движение
void Monster::movement(
	int& mapSizeVertical,
	int& mapSizeHorizontal,
	wstring& map,
	double playerPositionX,
	double playerPositionY,
	int& distans)
{
	//	Получаем и округляем в БОЛЬШУЮ часть координаты игрока
	int playerX = round(playerPositionX);
	int playerY = round(playerPositionY);

	//	Вычисляем разницу между координатами игрока и монстра
	int coordinateDifferenceX = abs(x - playerX);
	int coordinateDifferenceY = abs(y - playerY);

	// Суммируем разницу по осям
	int totalCoordinateDifference = coordinateDifferenceX + coordinateDifferenceY;
	distans = totalCoordinateDifference;

	//	Если разница в координатах позволяет ударить то бьем
	if (totalCoordinateDifference <= 3) canHit = true;
	else { canHit = false; }

	//	Ходьба монстра за ИГРОКОМ
		//	Устанавливаем радиус поиска игрока
	if (int(totalCoordinateDifference) <= 10.0f) {
		//	Звук движения
		if ((x + 1) <= mapSizeHorizontal
			&& playerPositionX > x) {
			mciSendString(L"play sounds/moveWall.wav", NULL, SND_ASYNC, NULL);
			x += 1;
		}
		if ((x - 1) >= 0
			&& playerPositionX < x) {
			mciSendString(L"play sounds/moveWall.wav", NULL, SND_ASYNC, NULL);
			x -= 1;
		}
		if ((y + 1) <= mapSizeVertical
			&& playerPositionY > y) {
			mciSendString(L"play sounds/moveWall.wav", NULL, SND_ASYNC, NULL);
			y += 1;
		}
		if ((y - 1) >= 0
			&& playerPositionY < y) {
			mciSendString(L"play sounds/moveWall.wav", NULL, SND_ASYNC, NULL);
			y -= 1;
		}
		this_thread::sleep_for(chrono::milliseconds(1500));
		return;
	}
	//	Если некого не нашел в радиусе ИДЕТ К СТЕНЕ
	else
	{
		//	ХОДИТ ПО ПОЛЮ
		if (map[(y * mapSizeHorizontal) + x] != L'#')
		{
			//	Поля для того что бы найти ближайшую стенку
			int up = 0;
			int down = 0;
			int left = 0;
			int right = 0;
			int command = 0;

			//	RIGTH
			for (int i = x; i < mapSizeHorizontal; i++)
				if (map[y * mapSizeHorizontal + i] != L'#') right += 1;
				else break;
			//	LEFT
			for (int i = x; i > 0; i--)
				if (map[y * mapSizeHorizontal + i] != L'#') left += 1;
				else break;
			//	DOWN
			for (int i = y; i > 0; i--)
				if (map[i * mapSizeHorizontal + x] != L'#') down += 1;
				else break;
			//	UP
			for (int i = y; i < mapSizeVertical; i++)
				if (map[i * mapSizeHorizontal + x] != L'#') up += 1;
				else break;

			if (right < left
				&& right < up
				&& right < down) {
				command = 1;
			}

			else if (left < right
				&& left < up
				&& left < down) {
				command = 2;
			}

			else if (down < right
				&& down < up
				&& down < left) {
				command = 3;
			}

			else if (up < right
				&& up < down
				&& up < left) {
				command = 4;
			}
			else { command = 4; }

			//	Идет к ближайшей стенке
			switch (command)
			{

			case 1://	RIGTH
				if (map[y * mapSizeHorizontal + x] != L'#') { x += 1; }
				break;

			case 2://	LEFT
				if (map[y * mapSizeHorizontal + x] != L'#') { x -= 1; }
				break;

			case 3://	DOWN
				if (map[y * mapSizeHorizontal + x] != L'#') { y -= 1; }
				break;

			case 4://	UP
				if (map[y * mapSizeHorizontal + x] != L'#') { y += 1; }

				break;
			}

			this_thread::sleep_for(chrono::milliseconds(1500));
			return;
		}
		//	ХОДИТ ПО СТЕНКЕ
		else
		{
			switch (rand() % 4 + 1) {
			case 1:
				if ((y + 1) < mapSizeVertical && map[(y + 1) * mapSizeHorizontal + x] == L'#') {
					y += 1;
				}
				else if ((y + 2) < mapSizeVertical && map[(y + 2) * mapSizeHorizontal + x] == L'#') {
					y += 2;
				}
				break;
			case 2:
				if ((y - 1) >= 0 && map[(y - 1) * mapSizeHorizontal + x] == L'#') {
					y -= 1;
				}
				else if ((y - 2) >= 0 && map[(y - 2) * mapSizeHorizontal + x] == L'#') {
					y -= 2;
				}
				break;
			case 3:
				if ((x + 1) < mapSizeHorizontal && map[y * mapSizeHorizontal + (x + 1)] == L'#') {
					x += 1;
				}
				else if ((x + 2) < mapSizeHorizontal && map[y * mapSizeHorizontal + (x + 2)] == L'#') {
					x += 2;
				}
				break;
			case 4:
				if ((x - 1) >= 0 && map[y * mapSizeHorizontal + (x - 1)] == L'#') {
					x -= 1;
				}
				else if ((x - 2) >= 0 && map[y * mapSizeHorizontal + (x - 2)] == L'#') {
					x -= 2;
				}
				break;
			}

			this_thread::sleep_for(chrono::milliseconds(50));
			return;
		}
	}
}
