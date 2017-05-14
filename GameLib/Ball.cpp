#include "Ball.h"

Ball::Ball()
{
	activated = false;
	direction = false;
}

Ball::~Ball()
{

}

int16_t & Ball::getX()
{
	return x;
}

int16_t & Ball::getY()
{
	return y;
}

bool & Ball::getDirection()
{
	return direction;
}

bool & Ball::getActivated()
{
	return activated;
}

int8_t & Ball::getRadius()
{
	return radius;
}
