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

	wstring map;
	wstring initialMap;
	static constexpr int mapSizeHorizontal = 60;       // Горизонтальный размер карты
	static constexpr int mapSizeVertical = 60;         // Вертикальный размер карты
	pair<int, int> startCoordinat;
	pair<int, int> finishCoordinat;
	char teleportSkin = '&';
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