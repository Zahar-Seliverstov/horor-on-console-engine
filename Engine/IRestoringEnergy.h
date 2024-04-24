#pragma once
class RestoringEnergy 
{
private:
	int x;
	int y;
	int attributeEdge;
	int attributeFrontColor;
	int levelCure;
	wchar_t edgeSkin;
	wchar_t frontSkin;
	wchar_t mapSkin;

public:
	bool used;
	//	Конструктор
	RestoringEnergy();

	//	get
	int getX();

	int getY();

	int getAttributeEdge();

	int getAttributeFrontColor();

	int getLevelCure();

	wchar_t getEdgeSkin();

	wchar_t getFrontSkin();

	wchar_t getMapSkin();

	//	set
	void setX(int newX);

	void setY(int newY);

	void setAttributeEdge(int newAttributeEdge);

	void setAttributeFrontColor(int newAttributeFrontColor);

	void setLevelCure(int newLevelCure);

	void setEdgeSkin(wchar_t newEdgeSkin);

	void setFrontSkin(wchar_t newFrontSkin);

	void setMapSkin(wchar_t newMapSkin);

};