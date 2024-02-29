#pragma once
#include "Library.h"

class Player
{
public:
	int initialHp;
	int hp = 100;
	double x;
	double y;
	double r = 0;
	double speed = 4;
	float sensitivity = 5;
	Player();

	void motion(wstring& map,const int& mapSizeHorizontal, double& ratioNumberFrames);
};