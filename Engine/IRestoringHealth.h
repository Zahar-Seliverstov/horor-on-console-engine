#include "Library.h"
#include "IPlayer.h"
#pragma once

class RestoringHealth
{
private:
	int x;
	int y;

	int attributeEdge;
	int attributeFrontColor;
	wchar_t edgeSkin;
	wchar_t frontSkin;
	wchar_t mapSkin;
	int levelCure;

public:
	bool used;
	RestoringHealth();
	//	get
	int getX();
	int getY();
	wchar_t getEdgeSkin();
	wchar_t getFrontSkin();
	wchar_t getMapSkin();
	int getAttributeEdge();
	int getAttributeFrontColor();
	void cure(Player& obj);

	//	set
	void setX(int newX);
	void setY(int newY);
	void setLevelCure(int newLevelCure);
	void setAttributeEdge(int newAttributeEdge);
	void setAttributeFrontColor(int newAttributeFrontColor);
	void setEdgeSkin(wchar_t newEdgeSkin);
	void setFrontSkin(wchar_t newFrontSkin);
	void setMapSkin(wchar_t newMapSkin);
};