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
	vector<vector<int>> maze;       //	Двумерный вектор для хранения карты подземелья в виде значений
	wstring mazeString;             //	Двумерный вектор для хранения карты подземелья в виде строки
	int ROOM_MIN_HORIZONTAL;        //	Минимальная горизонтальная длина комнаты
	int ROOM_MIN_VERTICAL;          //	Минимальная вертикальная высота комнаты
	int ROOM_MAX_HORIZONTAL;        //	Максимальная горизонтальная длина комнаты
	int ROOM_MAX_VERTICAL;          //	Максимальная вертикальная высота комнаты
	int EXTRA_DOOR_CHANCE;          //	Шанс создания дополнительных дверей (%)
	int MIN_LENGTH_FOR_EXTRA_DOOR;  //	Минимальная длина пути для создания дополнительной двери
	//int NumberFloor;
	//	Метод для создания подземелья
	void createDungeon();
	//	Рекурсивный метод для разделения пространства
	void division(int startX, int endX, int startY, int endY);
	//	Метод для создания комнаты
	void createRoom(bool isHorizontal, int startX, int endX, int startY, int endY);
	//	Метод для создания границ для карты
	void createBorders();

public:
	//	Места под обьекты с которыми может работать карта
	MapInfo* mapInfo;
	Player* playerInfo;
	Monster* monsterInfo;
	Teleport* teleportInfo;
	RestoringHealth* restoringHealthInfo;
	RestoringEnergy* restoryngEnergyInfo;

	wstring map;			//	Карта
	bool* mapPlayerSaw;		//	Вторая карта для отслеживания того что видел игрок
	wstring initialMap;		//	Неизменяймая копия карты ( используется для отчиски карты )
	int mapSizeHorizontal;	//	Горизонтальный размер карты
	int mapSizeVertical;	//	Вертикальный размер карты

	//	Конструктор класса
	MapInfo(Player& _playerInfo,
		Monster& _monsterInfo,
		Teleport& _teleportInfo,
		RestoringHealth& _restoringHealthInfo,
		RestoringEnergy& _restoryngEnergyInfo);
	
	//	Метод для отчищения карты
	void clearmap();
	
	//	Метод для вывода карты подземелья в консоль
	void printDungeon();
	
	//	Метод зачем то
	vector<vector<int>> getIntMaze();
	
	// Метод для перевода из масива в wstring
	wstring getWstringMaze();
	
	//	Устанавливаем стартовую позицию игрока на карте 
	void setPlayerStartingCoordinates();
	
	//	Устанавливаем координаты телепорта на карте
	void setTeleportCoordinates();
	
	//	Создание карты
	void createmap();

	// Генерация позиции объкта на карте
	void setPositionObject(int minDistance);

};