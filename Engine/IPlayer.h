#pragma once
#include "Library.h"

class Player
{
public:
	int initialHp;
	int hp;
	double x;
	double y;
	double r;
	double speed;
	double amountEnergy;
	double initialAmountEnergy;
	float sensitivity;
	double initialSpeed;
	bool run;
	Player();
	void motion(wstring& map,const int& mapSizeHorizontal, double& ratioNumberFrames);
};