#pragma once
#include "Library.h"

class MapInfo
{
private:
	// Метод для создания подземелья
	void createDungeon();
	// Рекурсивный метод для разделения пространства
	void division(int startX, int endX, int startY, int endY);
	// Метод для создания комнаты
	void createRoom(bool isHorizontal, int startX, int endX, int startY, int endY);
	// Метод для создания границ для карты
	void createBorders();
public:
	vector<vector<int>> maze;       // Двумерный вектор для хранения карты подземелья в виде значений
	wstring mazeString;             // Двумерный вектор для хранения карты подземелья в виде строки
	int ROOM_MIN_HORIZONTAL;        // Минимальная горизонтальная длина комнаты
	int ROOM_MIN_VERTICAL;          // Минимальная вертикальная высота комнаты
	int ROOM_MAX_HORIZONTAL;        // Максимальная горизонтальная длина комнаты
	int ROOM_MAX_VERTICAL;          // Максимальная вертикальная высота комнаты
	int EXTRA_DOOR_CHANCE;          // Шанс создания дополнительных дверей (%)
	int MIN_LENGTH_FOR_EXTRA_DOOR;  // Минимальная длина пути для создания дополнительной двери
	wstring map;
	wstring initialMap;
	int mapSizeHorizontal;			// Горизонтальный размер карты
	int mapSizeVertical;			// Вертикальный размер карты
	pair<int, int> startCoordinat;
	pair<int, int> finishCoordinat;
	char teleportSkin;
	// Конструктор класса
	MapInfo();
	// Метод для отчищения карты
	void clearmap();
	// Метод для вывода карты подземелья в консоль
	void printDungeon();
	// Метод зачем то
	vector<vector<int>> getIntMaze();
	// Метод для перевода из масива в wstring
	wstring getWstringMaze();
	void setStartCoordinat();
	void setFinishCoordinat();
	// Создание карты
	void createmap();
};