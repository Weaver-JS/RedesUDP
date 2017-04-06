#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
class PlayerInfo
{
	sf::IpAddress player_ip;
	unsigned short player_port;
	uint16_t x, y;
	short playerID;
	

public:
	PlayerInfo(sf::IpAddress & ip, unsigned short & port);
	~PlayerInfo();

	const sf::IpAddress & getPlayerIP();
	const unsigned short & getPlayerPort();
	void setPlayerIP(sf::IpAddress & ip);
	void setPlayerPort(unsigned short & port);
	uint16_t & getX();
	uint16_t & getY();
	short & getPlayerID();
};