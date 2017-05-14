
#pragma once
#include <SFML\Graphics.hpp>
#include "SFML\Graphics.hpp"
#include <string>
#include <iostream>
#include <vector>
#include "UDPmanager.h"
#include "character.h"
#include "AccumMovClient.h"
#define PLAYER1 0
#define PLAYER2 1
#define TOTALBALLS 10
#define OFFSET SCREEN_WIDTH/TOTALBALLS
class windowManagement
{
private:
	AccumMovClient accumlist;
	sf::Vector2i screenDimensions;
	sf::RenderWindow windowRenderer;
	sf::Font font;
	character* main_character;
	UDPmanager udpManager;
	sf::RectangleShape separator;
	std::vector<character> playerList;
	sf::Clock timer_client;
	gameStates state;
	sf::Clock win_clock;
//	std::vector<Ball> ballList;
	std::vector<sf::CircleShape> ballsRenderList;

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
	void interpolateMovement();
	void initBalls();
	void moveBalls();
	void tryToPushBall(std::vector<Ball> & v);
	void chechIfWin();
	sf::RenderWindow & getWindowRenderer();
};

