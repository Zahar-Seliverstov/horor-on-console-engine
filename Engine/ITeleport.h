#pragma once
class Teleport {

private:
	int x;
	int y;
	int attributeEdge;
	int attributeFrontColor;
	wchar_t edgeSkin;
	wchar_t frontSkin;
	wchar_t mapSkin;
public:
    //  Конструктор
    Teleport();
    //Teleport(int x, int y, int attributeEdge, int attributeFrontColor, wchar_t edgeSkin, wchar_t frontSkin, wchar_t mapSkin);

    // get
    int getX();
    int getY();
    int getAttributeEdge();
    int getAttributeFrontColor();
    wchar_t getEdgeSkin();
    wchar_t getFrontSkin();
    wchar_t getMapSkin();

    // set
    void setX(int newX);
    void setY(int newY);
    void setAttributeEdge(int newAttributeEdge);
    void setAttributeFrontColor(int newAttributeFrontColor);
    void setEdgeSkin(wchar_t newEdgeSkin);
    void setFrontSkin(wchar_t newFrontSkin);
    void setMapSkin(wchar_t newMapSkin);

};