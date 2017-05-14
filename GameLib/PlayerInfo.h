#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include "Ball.h"
enum gameStates {
	LOBBY,
	PLAY,
	GAMEOVER

};
class PlayerInfo
{
	sf::IpAddress player_ip;
	unsigned short player_port;
	uint16_t x, y;
	uint16_t playerID;
	bool win;
	

public:
	PlayerInfo(sf::IpAddress & ip, unsigned short & port);
	PlayerInfo();
	~PlayerInfo();

	const sf::IpAddress & getPlayerIP();
	const unsigned short & getPlayerPort();
	void setPlayerIP(sf::IpAddress & ip);
	void setPlayerPort(unsigned short & port);
	void setPlayerID(uint16_t & id);
	uint16_t & getX();
	uint16_t & getY();
	uint16_t & getPlayerID();
	bool & getPlayerWIn();
};