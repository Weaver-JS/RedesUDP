#pragma once
#include "SFML\Network.hpp"
#include <iostream>
#include <PlayerInfo.h>
#include <OutputMemoryBitStream.h>
#include <InputMemoryBitStream.h>
#define PORT 5000
#define BINARYPACKETYPELENGTH 3
#define POSITION_BYNARY_LENGTH 10
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
enum PacketType
{
	PT_EMPTY,
	PT_HELLO,
	PT_WELCOME,
	PT_FULL,
	PT_PING,
	PT_PLAYER_POSITION,
	PT_DISCONNECT,
	PT_RESETPLAYER
};
class UDPmanager
{
private:
	std::vector<PlayerInfo*> playerList; //En udp al poseer un socket, lo que guardamos son ip:puerto
	sf::UdpSocket socket;
	bool abort;

public:
	UDPmanager();
	~UDPmanager();
	void initConnection();

	int  getPlayerID(short port);
	void readMessage(char*  _message,const size_t & _sizeMessage,sf::IpAddress & ip, unsigned short & port);
	void sendMessage(char* message, sf::IpAddress & ip, unsigned short & port, uint32_t sizeBuffer);

	/* 
		
	*/

	
};

