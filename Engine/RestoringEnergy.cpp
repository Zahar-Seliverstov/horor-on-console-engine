#include "IRestoringEnergy.h"

//	Конструктор
RestoringEnergy::RestoringEnergy()
{
	x = 0;
	y = 0;

	attributeEdge = 3;
	attributeFrontColor = 6;
	levelCure = 10;
	edgeSkin = L'\x2726';	// ✦
	frontSkin = L'+';	//	𐓏
	mapSkin = L'E';

	used = false;

}

//	get
int RestoringEnergy::getX() { return x; }

int RestoringEnergy::getY() { return y; }

int RestoringEnergy::getAttributeEdge() { return attributeEdge; }

int RestoringEnergy::getAttributeFrontColor() { return attributeFrontColor; }

int RestoringEnergy::getLevelCure() { return levelCure; }

wchar_t RestoringEnergy::getEdgeSkin() { return edgeSkin; }

wchar_t RestoringEnergy::getFrontSkin() { return frontSkin; }

wchar_t RestoringEnergy::getMapSkin() { return mapSkin; }

//	set
void RestoringEnergy::setX(int newX) { x = newX; }

void RestoringEnergy::setY(int newY) { y = newY; }

void RestoringEnergy::setAttributeEdge(int newAttributeEdge) { attributeEdge = newAttributeEdge; }

void RestoringEnergy::setAttributeFrontColor(int newAttributeFrontColor) { attributeFrontColor = newAttributeFrontColor; }

void RestoringEnergy::setLevelCure(int newLevelCure) { levelCure = newLevelCure; }

void RestoringEnergy::setEdgeSkin(wchar_t newEdgeSkin) { edgeSkin = newEdgeSkin; }

void RestoringEnergy::setFrontSkin(wchar_t newFrontSkin) { frontSkin = newFrontSkin; }

void RestoringEnergy::setMapSkin(wchar_t newMapSkin) { mapSkin = newMapSkin; }