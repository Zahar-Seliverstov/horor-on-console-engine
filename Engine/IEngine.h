#pragma once
#include "Library.h"

enum Keys;

void initialScreensaver();
void getConsoleSize();
void setScreenSize();
void cursoreVisibleFalse();
void outputInfo();
void settings();
bool compareByModule(const pair<double, double>& point1, const pair<double, double>& point2);
void RenderingConsoleGraphics();
void Run();