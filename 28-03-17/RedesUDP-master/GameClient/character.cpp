#include "character.h"

/*character::character()
{
	position = sf::Vector2<int8_t>(0, 0);
	circle_shape = new sf::CircleShape(RADIUS, size_t(30));
	circle_shape->setPosition(sf::Vector2f(position.x - circle_shape->getRadius() / 2, position.y - circle_shape->getRadius() / 2));
}*/

character::character(int16_t x, int16_t y)
{
	position.x =x;
	position.y = y;
	circle_shape = new sf::CircleShape(RADIUS,size_t(30));
	circle_shape->setPosition(sf::Vector2f(x - circle_shape->getRadius() /2 , y - circle_shape->getRadius() /2 ));
	circle_shape->setFillColor(sf::Color(255, 0, 0, 255));
	circle_shape->setOutlineColor(sf::Color(255, 0, 0, 255));
	circle_shape->setOrigin(0, 0);
	
	//circle_shape->set

}

character::character(sf::Vector2<int16_t> pos)
{
	position = pos;
	circle_shape = new sf::CircleShape(RADIUS, size_t(30));
	circle_shape->setPosition(sf::Vector2f(position.x - circle_shape->getRadius() / 2, position.y - circle_shape->getRadius() / 2));
}

character::~character()
{
	delete circle_shape;
}

sf::Vector2<int16_t> & character::getPosition()
{
	return position;
}

void character::setPosition(sf::Vector2<int16_t>& v)
{
	position = v;
	circle_shape->setPosition(position.x - circle_shape->getRadius()/2,position.y - circle_shape->getRadius() / 2);
}

sf::CircleShape * character::getCircleshape()
{
	return circle_shape;
}
