#pragma once
#include "Library.h"

class Monster
{
private:
	int x;
	int y;
	double* playerPositionY;
	double* playerPositionX;

	int health;
	int damage;
	int speed;

	int initialSpeed;
	int initialHealth;

	int attributeFrontColor;
	int attributeEdge;
	wchar_t edgeSkin;
	wchar_t frontSkin;
	wchar_t mapSkin;


public:
    bool canHit;
	//	Удар по игроку
    int hitPlayer(int playerHp);

    // get
    int getX();
    int getY();
    double* getPlayerPositionX();
    double* getPlayerPositionY();
    int getHealth();
    int getDamage();
    int getSpeed();
    int getInitialSpeed();
    int getInitialHealth();
    int getAttributeFrontColor();
    int getAttributeEdge();
    wchar_t getEdgeSkin();
    wchar_t getFrontSkin();
    wchar_t getMapSkin();

    // set
    void setX(int newX);
    void setY(int newY);
    void setPlayerPositionX(double* newX);
    void setPlayerPositionY(double* newY);
    void setHealth(int newHealth);
    void setDamage(int newDamage);
    void setSpeed(int newSpeed);
    void setInitialSpeed(int newInitialSpeed);
    void setInitialHealth(int newInitialHealth);
    void setAttributeFrontColor(int newAttributeFrontColor);
    void setAttributeEdge(int newAttributeEdge);
    void setEdgeSkin(wchar_t newEdgeSkin);
    void setFrontSkin(wchar_t newFrontSkin);
    void setMapSkin(wchar_t newMapSkin);


	//	Ходьба
	void movement(int& mapSizeVertical,
        int& mapSizeHorizontal,
        wstring& map,
        double playerPositionX, double playerPositionY);

	//	Конструктор
	Monster(double& y, double& x);
};
