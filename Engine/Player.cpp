#include "Library.h"
#include "IPlayer.h"

int initialHp;
int hp = 100;
double x;
double y;
double r = 0;
double speed = 4;
float sensitivity = 5;
Player::Player() { initialHp = hp; }

void Player::motion(wstring& map, const int& mapSizeHorizontal, double& ratioNumberFrames)
{
	if (GetAsyncKeyState((unsigned short)'D') & 0x8000) r -= (sensitivity * 0.75f) * ratioNumberFrames;
	if (GetAsyncKeyState((unsigned short)'A') & 0x8000) r += (sensitivity * 0.75f) * ratioNumberFrames;
	if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
	{
		x += sinf(r) * speed * ratioNumberFrames;
		y += cosf(r) * speed * ratioNumberFrames;
		if (map[(int)y * mapSizeHorizontal + (int)x] == '#')
		{
			x -= sinf(r) * speed * ratioNumberFrames;
			y -= cosf(r) * speed * ratioNumberFrames;
		}
	}
	if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
	{
		x -= sinf(r) * speed * ratioNumberFrames;
		y -= cosf(r) * speed * ratioNumberFrames;
		if (map[(int)y * mapSizeHorizontal + (int)x] == '#')
		{
			x += sinf(r) * speed * ratioNumberFrames;
			y += cosf(r) * speed * ratioNumberFrames;
		}
	}
}
