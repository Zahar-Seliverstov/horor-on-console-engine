#include "ITeleport.h"

Teleport::Teleport() {
	x = 0;
	y = 0;

	attributeEdge = 6;
	attributeFrontColor = 7;
	edgeSkin = L'X';
	frontSkin = L'\x2666';
	mapSkin = L'T';
}

// get
int Teleport::getX() { return  x; }
int Teleport::getY() { return y; }
int Teleport::getAttributeEdge() { return  attributeEdge; }
int Teleport::getAttributeFrontColor() { return  attributeFrontColor; }
wchar_t Teleport::getEdgeSkin() { return  edgeSkin; }
wchar_t Teleport::getFrontSkin() { return  frontSkin; }
wchar_t Teleport::getMapSkin() { return  mapSkin; }

// set
void Teleport::setX(int newX) { x = newX; }
void Teleport::setY(int newY) { y = newY; }
void Teleport::setAttributeEdge(int newAttributeEdge) { attributeEdge = newAttributeEdge; }
void Teleport::setAttributeFrontColor(int newAttributeFrontColor) { attributeFrontColor = newAttributeFrontColor; }
void Teleport::setEdgeSkin(wchar_t newEdgeSkin) { edgeSkin = newEdgeSkin; }
void Teleport::setFrontSkin(wchar_t newFrontSkin) { frontSkin = newFrontSkin; }
void Teleport::setMapSkin(wchar_t newMapSkin) { mapSkin = newMapSkin; }


