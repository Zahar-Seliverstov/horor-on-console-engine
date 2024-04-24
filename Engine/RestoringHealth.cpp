#include "IRestoringHealth.h"

RestoringHealth::RestoringHealth() {
	x = 0;
	y = 0;

	levelCure = 10;

	attributeEdge = 8;
	attributeFrontColor = 2;
	edgeSkin = L'\x2665';
	frontSkin = L'\x2666';
	mapSkin = L'H';

	used = false;
}
//	get
int RestoringHealth::getX() { return x; }
int RestoringHealth::getY() { return y; }
wchar_t RestoringHealth::getEdgeSkin() { return edgeSkin; }
wchar_t RestoringHealth::getFrontSkin() { return frontSkin; }
wchar_t RestoringHealth::getMapSkin() { return mapSkin; }
int RestoringHealth::getAttributeEdge() { return attributeEdge; }
int RestoringHealth::getAttributeFrontColor() { return attributeFrontColor; }
void RestoringHealth::cure(Player& obj) {
	if (obj.getHp() != obj.getInitialHp())
		obj.setHp(obj.getHp() + levelCure);
}

//	set
void RestoringHealth::setX(int newX) { x = newX; }
void RestoringHealth::setY(int newY) { y = newY; }
void RestoringHealth::setLevelCure(int newLevelCure) { levelCure = newLevelCure; }
void RestoringHealth::setAttributeEdge(int newAttributeEdge) { attributeEdge = newAttributeEdge; }
void RestoringHealth::setAttributeFrontColor(int newAttributeFrontColor) { attributeFrontColor = newAttributeFrontColor; }
void RestoringHealth::setEdgeSkin(wchar_t newEdgeSkin) { edgeSkin = newEdgeSkin; }
void RestoringHealth::setFrontSkin(wchar_t newFrontSkin) { frontSkin = newFrontSkin; }
void RestoringHealth::setMapSkin(wchar_t newMapSkin) { mapSkin = newMapSkin; }

