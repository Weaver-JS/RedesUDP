#pragma once
#include "SFML\Network.hpp"
#include <iostream>
#include <PlayerInfo.h>
#include <OutputMemoryBitStream.h>
#include <InputMemoryBitStream.h>
#include "AccumMoveServer.h"
#define PORT 50000
#define BINARYPACKETYPELENGTH 4
#define POSITION_BYNARY_LENGTH 10
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define MAX_PLAYER 2
#define TOTALBALLS 10
#define OFFSET SCREEN_WIDTH/TOTALBALLS

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
	gameStates gamestate;
	std::vector<PlayerInfo> playerList; //En udp al poseer un socket, lo que guardamos son ip:puerto
	sf::UdpSocket socket;
	int player1count;
	int player2count;
	bool p1wins, p2wins;
	int actualMovementBalls = 0;
	bool abort;
	bool isConnected;
	bool shutdown;
	sf::Clock server_clock;
	sf::Clock shutdown_clock;
	sf::Clock ping_clock;
	bool failedPosition_p1, failedPosition_p2;
	std::vector<Ball> ballList;

	
	AccumMoveServer accumResponse;

public:
	UDPmanager();
	~UDPmanager();
	void initConnection();

	int  getPlayerID(short  port);
	void readMessage(char*  _message,const size_t & _sizeMessage,sf::IpAddress & ip, unsigned short & port);
	bool sendMessage(char* message, sf::IpAddress & ip, unsigned short & port, uint32_t sizeBuffer);
	void serverDisconnection();
	void throw_command();
	void ping();
	void initBalls();
	void moveBalls();
	void sendBalls();
	void checkIfwin();
	/* 

		
	*/

	
};

