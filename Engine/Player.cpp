#include "Library.h"
#include "IPlayer.h"

Player::Player() 
{ 
	hp = 100;
	initialHp = hp;
	r = 0;
	speed = 4;
	sensitivity = 5;
}
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