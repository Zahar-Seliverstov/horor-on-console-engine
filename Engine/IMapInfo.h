#pragma once
#include "Library.h"

class MapInfo
{
private:

	// ����� ��� �������� ����������
	void createDungeon();
	// ����������� ����� ��� ���������� ������������
	void division(int startX, int endX, int startY, int endY);
	// ����� ��� �������� �������
	void createRoom(bool isHorizontal, int startX, int endX, int startY, int endY);
	// ����� ��� �������� ������ ��� �����
	void createBorders();
public:

	wstring map;
	wstring initialMap;
	static constexpr int mapSizeHorizontal = 60;       // �������������� ������ �����
	static constexpr int mapSizeVertical = 60;         // ������������ ������ �����
	pair<int, int> startCoordinat;
	pair<int, int> finishCoordinat;
	char teleportSkin = '&';
	// ����������� ������
	MapInfo();
	// ����� ��� ��������� �����
	void clearmap();
	// ����� ��� ������ ����� ���������� � �������
	void printDungeon();
	// ����� ����� ��
	vector<vector<int>> getIntMaze();
	// ����� ��� �������� �� ������ � wstring
	wstring getWstringMaze();
	void setStartCoordinat();
	void setFinishCoordinat();
	// �������� �����
	void createmap();
};