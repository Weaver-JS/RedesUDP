#pragma once
#include "SFML\Network.hpp"
#include <iostream>
#include <OutputMemoryBitStream.h>
#include <InputMemoryBitStream.h>
#include <PlayerInfo.h>
#include "AccumMovClient.h"


#define PORT 50000
#define BINARYPACKETYPELENGTH 4
#define POSITION_BYNARY_LENGTH 10
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define MAXPLAYERS 2
#define IP "127.0.0.1"
#define VELOCITY 5

enum PacketType
{
	PT_EMPTY,
	PT_WIN,
	PT_MOV,
	PT_OKMOVE,
	PT_HELLO,
	PT_WELCOME,
	PT_FULL,
	PT_FAILUREMOVE,
	PT_PLAYER_POSITION,
	PT_DISCONNECT,
	PT_SHUTDOWN,
	PT_BALL

};
class UDPmanager
{
private:
	PlayerInfo* player; //En udp al poseer un socket, lo que guardamos son ip:puerto
	int16_t finalPosition;
	std::vector<Ball> ballList;
	std::vector<PlayerInfo> playerList;
	int size;
	sf::UdpSocket socket;
	bool notConnected;
	bool disconnected;
	sf::Clock client_clock;
	sf::Clock shutdown_Clock;
	
	bool interpolateMovement;
	bool thereIsBallToSend;
	int movementBalls ;
	

public:
	UDPmanager();
	~UDPmanager();
	void initConnection();
	void ping(int16_t x, int16_t y,int packetID);
	void readMessage(char*  _message,const size_t & _sizeMessage,sf::IpAddress & ip, unsigned short & port);
	void recv();
	void disconnect();
	void sendAccumList(std::vector<movement> & movlist);
	bool sendMessage(char* message, sf::IpAddress  ip, unsigned short  port, uint32_t sizeBuffer);
	bool & doWehavetointerpolate();

	int16_t & getFinalPosition();
	PlayerInfo & getPlayer();
	PlayerInfo & getPlayer(int i);
	int  getPlayerSize();
	bool isDisconnected();
	bool getNotConnected();
	bool & isThereAballtoSend();
	std::vector<Ball> & getBallList();
	void  SetmovementBalls(int & i);
	int getmovementBalls();
};

