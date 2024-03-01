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
	vector<vector<int>> maze;       // ��������� ������ ��� �������� ����� ���������� � ���� ��������
	wstring mazeString;             // ��������� ������ ��� �������� ����� ���������� � ���� ������
	int ROOM_MIN_HORIZONTAL;        // ����������� �������������� ����� �������
	int ROOM_MIN_VERTICAL;          // ����������� ������������ ������ �������
	int ROOM_MAX_HORIZONTAL;        // ������������ �������������� ����� �������
	int ROOM_MAX_VERTICAL;          // ������������ ������������ ������ �������
	int EXTRA_DOOR_CHANCE;          // ���� �������� �������������� ������ (%)
	int MIN_LENGTH_FOR_EXTRA_DOOR;  // ����������� ����� ���� ��� �������� �������������� �����
	wstring map;
	wstring initialMap;
	int mapSizeHorizontal;			// �������������� ������ �����
	int mapSizeVertical;			// ������������ ������ �����
	pair<int, int> startCoordinat;
	pair<int, int> finishCoordinat;
	char teleportSkin;
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