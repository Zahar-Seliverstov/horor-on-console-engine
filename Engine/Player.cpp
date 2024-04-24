#include "Library.h"
#include "IPlayer.h"

//	Конструктор
Player::Player()
{
	x = 0;
	y = 0;
	r = 0;

	hp = 100;
	speed = 2;
	sensitivity = 5;
	amountEnergy = 100;
	run = false;
	go = false;

	initialHp = hp;
	initialSpeed = speed;
	initialAmountEnergy = amountEnergy;
}

//	Ходьба
void Player::motion(wstring& map, const int& mapSizeHorizontal, double& ratioNumberFrames)
{
	bool goingForward = true;
	bool goingBack = true;

	if (GetAsyncKeyState('D') & 0x8000) { r -= (sensitivity * 0.75f) * ratioNumberFrames; }
	if (GetAsyncKeyState('A') & 0x8000) { r += (sensitivity * 0.75f) * ratioNumberFrames; }
	if (GetAsyncKeyState('W') & 0x8000) {
		x += sinf(r) * speed * ratioNumberFrames;
		y += cosf(r) * speed * ratioNumberFrames;
		if (map[(int)y * mapSizeHorizontal + (int)x] == '#') {
			x -= sinf(r) * speed * ratioNumberFrames;
			y -= cosf(r) * speed * ratioNumberFrames;
		}
		else { if (!go)go = true; }
	}
	else { goingForward = false; }
	if (GetAsyncKeyState('S') & 0x8000) {
		x -= sinf(r) * speed * ratioNumberFrames;
		y -= cosf(r) * speed * ratioNumberFrames;
		if (map[(int)y * mapSizeHorizontal + (int)x] == '#') {
			x += sinf(r) * speed * ratioNumberFrames; 
			y += cosf(r) * speed * ratioNumberFrames;
		}
		else { if (!go) { go = true; } }
	}
	else { goingBack = false; }
	speed = (!run || (amountEnergy == 0) ? initialSpeed : initialSpeed + 1);
	if ((GetAsyncKeyState(VK_SHIFT) & 0x8000))
	{
		if (!run
			&& amountEnergy >= initialAmountEnergy / 10
			&& (goingForward || goingBack)
			)
		{
			run = true;
			PlaySound(NULL, NULL, 0);
		}
	}
	else { if (run) { run = false; } }
	if (!goingForward && !goingBack)
	{
		if (!run
			|| !go
			|| (!run && go)
			|| (run && !go)
			) {
			PlaySound(NULL, NULL, 0);
		}
		if (go) { go = false; }
	}
	if (r == 6) { r = 0; };
}

//	get
double Player::getPositionY() { return Player::y; }
double Player::getPositionX() { return Player::x; }
double Player::getPositionR() { return Player::r; }

int Player::getHp()  { return Player::hp;}
double Player::getSpeed() { return Player::speed; }
double Player::getAmountEnergy() { return Player::amountEnergy; }
float Player::getSensitivity() { return Player::sensitivity; }
bool Player::getRun() { return Player::run; }
bool Player::getGo() { return Player::go; }

int Player::getInitialHp() { return Player::initialHp; }
double Player::getInitialAmountEnergy() { return Player::initialAmountEnergy; }
double Player::getInitialSpeed() { return Player::initialSpeed; }

// set
void Player::setPositionX(double newPositionX) { Player::x = newPositionX; }
void Player::setPositionY(double newPositionY) { Player::y = newPositionY; }
void Player::setPositionR(double newPositionR) { Player::r = newPositionR; }
  
void Player::setHp(int newHp) { Player::hp = newHp; }
void Player::setSpeed(double newSpeed) { Player::speed = newSpeed; }
void Player::setAmountEnergy(double newAmountEnergy) { Player::amountEnergy = newAmountEnergy; }
void Player::setSensitivity(float newSensitivity) { Player::sensitivity = newSensitivity; }
void Player::setRun(bool newRun) { Player::run = newRun; }
void Player::setGo(bool newGo) { Player::go = newGo; }

void Player::setInitialHp(int newInitialHp) { Player::initialHp = newInitialHp; }
void Player::setInitialAmountEnergy(double newInitialAmountEnergy) { Player::initialAmountEnergy = newInitialAmountEnergy; }
void Player::setInitialSpeed(double newInitialSpeed) { Player::initialSpeed = newInitialSpeed; }