#pragma once
#include "Library.h"

class Player
{
public:
	double x;
	double y;
	int hp;

private:
	double r;

	double speed;
	double amountEnergy;
	float sensitivity;
	bool run;
	bool go;

	int initialHp;
	double initialAmountEnergy;
	double initialSpeed;

public:
	// get
	double getPositionX();
	double getPositionY();
	double getPositionR();

	int getHp();
	double getSpeed();
	double getAmountEnergy();
	float getSensitivity();
	bool getRun();
	bool getGo();

	int getInitialHp();
	double getInitialAmountEnergy();
	double getInitialSpeed();

	// set
	void setPositionX(double newPositionX);
	void setPositionY(double newPositionY);
	void setPositionR(double newPositionR);

	void setHp(int newHp);
	void setSpeed(double newSpeed);
	void setAmountEnergy(double newAmountEnergy);
	void setSensitivity(float newSensitivity);
	void setRun(bool newRun);
	void setGo(bool newGo);

	void setInitialHp(int newInitialHp);
	void setInitialAmountEnergy(double newInitialAmountEnergy);
	void setInitialSpeed(double newInitialSpeed);

	//	Конструктор
	Player();

	//	Ходьба
	void motion(wstring& map, const int& mapSizeHorizontal, double& ratioNumberFrames);

};