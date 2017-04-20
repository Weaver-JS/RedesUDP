
#pragma once
#include <SFML\Graphics.hpp>
#include "SFML\Graphics.hpp"
#include <string>
#include <iostream>
#include <vector>

#include "character.h"

class windowManagement
{
private:
	
	sf::Vector2i screenDimensions;
	sf::RenderWindow windowRenderer;
	sf::Font font;
	
	
	sf::RectangleShape separator;
	

	windowManagement() {}
	~windowManagement();

public:
	static windowManagement& get() {
		static windowManagement instance;
		return instance;
	}
	windowManagement(windowManagement const&) = delete;
	void operator=(windowManagement const&) = delete;

	void init(float x, float y, std::string chatName);
	void loop();
	void windowStuff(sf::Event & evento);
	sf::RenderWindow & getWindowRenderer();
};

