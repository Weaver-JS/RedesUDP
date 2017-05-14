#pragma once
#include <iostream>
#define BALLVELOCITY 5

class Ball
{
private:
	int16_t x, y;
	bool direction;
	int8_t radius;
	bool activated;
public:
	Ball();
	~Ball();

	int16_t & getX();
	int16_t & getY();
	bool & getDirection();
	bool & getActivated();
	int8_t & getRadius();
};

