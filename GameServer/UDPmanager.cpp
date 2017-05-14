#include "UDPmanager.h"



UDPmanager::UDPmanager()
{
	abort = false;
	isConnected = true;
	shutdown = false;
	socket.setBlocking(true);
	server_clock.restart();
	gamestate = gameStates::LOBBY;
	failedPosition_p1 = false;
	failedPosition_p2 = false;
	p1wins = false;
	p2wins = false;
	actualMovementBalls = 0;
	 player1count = TOTALBALLS / 2;
	 player2count = player1count;
	initBalls();
}


UDPmanager::~UDPmanager()
{
	socket.unbind();
	
}


void UDPmanager::initConnection()
{
	
	
	sf::Socket::Status status = socket.bind(PORT);
	if (status != sf::Socket::Done)
	{
		std::cout << "No se puede vincular al puerto: " << PORT << std::endl;
	}
	else if (status == sf::Socket::Done)
	{
		std::cout << "Puerto vinculado: " << PORT << std::endl;
		socket.setBlocking(false);
		

	}
	ping_clock.restart();
	while (isConnected)
	{
		
		
		sf::IpAddress senderIP;
		unsigned short senderPort;
		sf::Packet packet;
		
			if (socket.receive(packet, senderIP, senderPort) == sf::Socket::Done)
			{
				readMessage((char*)packet.getData(), packet.getDataSize(), senderIP, senderPort);
			}
			if (gamestate == gameStates::PLAY)
			{
				//Meter aquí el ping
				moveBalls();
				checkIfwin();
				if (ping_clock.getElapsedTime() >= sf::milliseconds(250))
				{
					if (!shutdown)
						ping();

					ping_clock.restart();

				}
			}
			else if (gamestate == gameStates::GAMEOVER)
			{
				if (ping_clock.getElapsedTime() >= sf::milliseconds(250))
				{
					ping();
					ping_clock.restart();
					
				}
			}
			//ping
		
		
			
	}
}

constexpr unsigned int str2int(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

void UDPmanager::sendBalls()
{
	
	OutputMemoryBitStream ombs;
	ombs.Write(PT_BALL, BINARYPACKETYPELENGTH);
	ombs.Write(actualMovementBalls, BINARYPACKETYPELENGTH);
	int cont = 0;
	for (int i = 0; i < TOTALBALLS; i++)
	{
		if (ballList[i].getActivated())
		{
			cont++;
			ombs.Write(i, BINARYPACKETYPELENGTH);
			ombs.Write(ballList[i].getDirection(), 1);

		}
		
	}
	if (cont > 0)
	{
		
		for (int i = 0; i < MAX_PLAYER; i++)
		{
			sf::IpAddress ipA = playerList[i].getPlayerIP();
			unsigned short pr = playerList[i].getPlayerPort();
			sendMessage(ombs.GetBufferPtr(), ipA,pr , ombs.GetByteLength());
		}
	}

}
void UDPmanager::checkIfwin()
{
	if (player1count == TOTALBALLS )
	{
		std::cout << "PLAYER 2 WINS" << std::endl;
		gamestate = gameStates::GAMEOVER;
		p1wins = true;
	}
	if (player2count == TOTALBALLS)
	{
		std::cout << "PLAYER 1 WINS" << std::endl;
		gamestate = gameStates::GAMEOVER;
		p2wins = true;
	}
}
void UDPmanager::ping()
{
	if (gamestate == gameStates::PLAY)
	{
		
		for (int i = 0; i < MAX_PLAYER; i++)
		{
			int16_t xPos = 0;
			xPos = accumResponse.sendAcumMovement(i);

			OutputMemoryBitStream ombs;
			sf::IpAddress ip = playerList[i].getPlayerIP();
			unsigned short port = playerList[i].getPlayerPort();

			if (i == 0)
			{
				ombs.Write(PT_OKMOVE, BINARYPACKETYPELENGTH);
				ombs.Write(i+1, BINARYPACKETYPELENGTH);//ID
				xPos = playerList[i+1].getX();
				ombs.Write(playerList[i+1].getX(), POSITION_BYNARY_LENGTH);
				ombs.Write(playerList[i+1].getY(), POSITION_BYNARY_LENGTH);
				ombs.Write(failedPosition_p1, 1);

				if (failedPosition_p1)
				{
					
					ombs.Write(playerList[i].getX(), POSITION_BYNARY_LENGTH);
				
				}
				
		
				
				sendMessage(ombs.GetBufferPtr(), ip, port, ombs.GetByteLength());
				//	sendBalls(ip, port);

				
			}
			else
			{
				ombs.Write(PT_OKMOVE, BINARYPACKETYPELENGTH);
				ombs.Write(i - 1, BINARYPACKETYPELENGTH);//ID
				xPos = playerList[i - 1].getX();
				ombs.Write(playerList[i - 1].getX(), POSITION_BYNARY_LENGTH);
				ombs.Write(playerList[i - 1].getY(), POSITION_BYNARY_LENGTH);
				ombs.Write(failedPosition_p2, 1);

				if (failedPosition_p2)
				{
					ombs.Write(playerList[i].getX(), POSITION_BYNARY_LENGTH);
					
				}
				
			}
				sendMessage(ombs.GetBufferPtr(), ip, port, ombs.GetByteLength());
				//sendBalls(ip, port);
			
		}
		
	
		
	}
	else if (gamestate == gameStates::GAMEOVER && !shutdown )
	{

		OutputMemoryBitStream ombs;
		if (p1wins)
		{
		
			ombs.Write(PT_WIN, BINARYPACKETYPELENGTH);
			ombs.Write(0, 1);
			
		}
		else if( p2wins)
		{
			
			ombs.Write(PT_WIN, BINARYPACKETYPELENGTH);
			ombs.Write(1, 1);
		}
		for (int i = 0; i < MAX_PLAYER; i++)
		{
			sf::IpAddress ipA = playerList[i].getPlayerIP();
			unsigned short pr = playerList[i].getPlayerPort();
			sendMessage(ombs.GetBufferPtr(), ipA, pr, ombs.GetByteLength());
		}
	}
}

void UDPmanager::initBalls()
{
	for (int i = 0; i < TOTALBALLS; i++)
	{
		Ball b;

		ballList.push_back(b);

		if (i < 5)
		{
			ballList[i].getX() = i  * OFFSET;
			ballList[i].getY() = 40;

		}
		else
		{
			ballList[i].getX() = i*OFFSET;
			ballList[i].getY() = SCREEN_HEIGHT - 60;// ballList[i].getRadius
		}
		ballList[i].getActivated() = false;
	}
}

void UDPmanager::moveBalls()
{
	int count = 0;
	int p1count = 0;
	int p2count = 0;
		for (int i = 0; i < TOTALBALLS; ++i)
		{
			if (ballList[i].getY() <= 80)
			{
				p1count++;
			}
			if (ballList[i].getY() >= SCREEN_HEIGHT - 80)
			{
				p2count++;
			}

			if (ballList[i].getActivated())
			{
				count++;
				//	actualMovementBalls++;
				if (ballList[i].getDirection())
				{
					if (ballList[i].getY() <= 0)
					{
						ballList[i].getY() = 40;

						
					
						//ballList[i].getActivated() = false;
						
						
						//actualMovementBalls--;
					}
					else
					{
						ballList[i].getY() -= 1.0;
					}
				}
				else
				{
					if (ballList[i].getY() >= SCREEN_HEIGHT)
					{
						ballList[i].getY() = SCREEN_HEIGHT - 60;
						//ballList[i].getActivated() = false;
					
						//	actualMovementBalls--;
					}
					else
					{
						ballList[i].getY() += 1.0;
					}
				}
			}


		}
		

		
		player1count = p1count;
		player2count = p2count;
		
	actualMovementBalls = count;

}

void UDPmanager::throw_command()
{
	while (isConnected)
	{
		std::string command = "";
		std::cin >> command;
		if (command != "")
		{
			switch (str2int(command.c_str()))
			{
			case str2int("exit"):
				serverDisconnection();
				break;
			case str2int("EXIT"):
				serverDisconnection();
				break;

			default:
				std::cout << "Wrong command: Availables command are 'exit' or 'EXIT'" << std::endl;
				break;
			}
		}
	}
	
}

int  UDPmanager::getPlayerID(short  port)
{
	for (int i = 0; i < playerList.size(); i++)
	{
		if (playerList[i].getPlayerPort() == port)
			return i;
	}
	return -1;
}

void UDPmanager::readMessage(char*  _message, const size_t & _sizeMessage, sf::IpAddress & ip, unsigned short & port)
{
	//ESTA DEMASIADO ENFOCADO A SERVIDOR


	InputMemoryBitStream imbs((char*)_message, _sizeMessage * 8);
	PacketType pt = PacketType::PT_EMPTY;
	std::string receivedString;
	imbs.Read(&pt, BINARYPACKETYPELENGTH);





	switch (pt)
	{
	case PT_EMPTY:
		std::cout << "Paquete vacío" << std::endl;
		break;
	case PT_HELLO:
	{
		if (playerList.size() >= MAX_PLAYER)
		{
			std::cout << "Server is full" << std::endl;
			OutputMemoryBitStream ombs;
			ombs.Write(PacketType::PT_FULL, BINARYPACKETYPELENGTH);
			sendMessage(ombs.GetBufferPtr(), ip, port, ombs.GetByteLength());
			break;
		}
		else
		{
			for (int i = 0; i < playerList.size(); i++)
			{
				if (playerList[i].getPlayerPort() == port)
				{
					std::cout << "Jugador ya en la lista" << std::endl;
					abort = true;
				}
			}
			if (abort)
			{
				abort = false;
				break;
			}

			std::cout << "JUGADOR ACEPTADO" << ip << " " << port << std::endl;
			PlayerInfo newPlayer(ip, port);
			playerList.push_back(newPlayer);
			OutputMemoryBitStream ombs;
			ombs.Write(PacketType::PT_WELCOME, BINARYPACKETYPELENGTH);

			int16_t id, x, y;
			switch (playerList.size() - 1)
			{
			case 0:
				x = 0;
				y = SCREEN_HEIGHT - 40;
				break;
			case 1:
				x = SCREEN_WIDTH;
				y = 40;
				break;
			}
			
			id = playerList.size() - 1;
			uint16_t idpararetrasados = id;
			playerList[playerList.size() - 1].getX() = x;
			playerList[playerList.size() - 1].getY() = y;
			playerList[playerList.size() - 1].setPlayerID(idpararetrasados);
			

			ombs.Write(id, BINARYPACKETYPELENGTH);
			ombs.Write(x, POSITION_BYNARY_LENGTH);
			ombs.Write(y, POSITION_BYNARY_LENGTH);
			sendMessage(ombs.GetBufferPtr(), ip, port, ombs.GetByteLength());
			if (playerList.size() == MAX_PLAYER)
			{
				gamestate = gameStates::PLAY;
				for (int i = 0; i < MAX_PLAYER; i++)
				{
					std::cout << "ENVIANDO DATOS IMPORTANTES" << std::endl;
					
					OutputMemoryBitStream ombs_;
					ombs_.Write(PT_PLAYER_POSITION,BINARYPACKETYPELENGTH);
					ombs_.Write(i, BINARYPACKETYPELENGTH);
					ombs_.Write(playerList[i].getX(), POSITION_BYNARY_LENGTH);
					ombs_.Write(playerList[i].getY(), POSITION_BYNARY_LENGTH);
					for (int j = 0; j < MAX_PLAYER; j++)
					{
						sf::IpAddress ipD = playerList[j].getPlayerIP();
						unsigned short portD = playerList[j].getPlayerPort();
						std::cout << playerList[j].getPlayerIP() << " PORT " << playerList[j].getPlayerPort() << std::endl;
						sendMessage(ombs_.GetBufferPtr(), ipD, portD, ombs_.GetByteLength());
						std::cout << i << " " << playerList[i].getX() << " " << playerList[i].getY() << std::endl;
						std::cout << "ENVIANDO DATOS IMPORTANTES" << std::endl;
					}
				}
			}
		}

	}
	break;
	case PT_PLAYER_POSITION:
	{
		uint16_t id = 100;
		int16_t x, y;
		imbs.Read(&id, BINARYPACKETYPELENGTH);
		imbs.Read(&x, POSITION_BYNARY_LENGTH);
		imbs.Read(&y, POSITION_BYNARY_LENGTH);
		if (x != 0 && y != 0)
		{
			playerList[id].getX() = x;
			playerList[id].getY() = y;
		}
	}
	break;
	case PT_MOV:
	{
		
		bool negative = false;
		bool isThereAball = false;
		uint16_t id = 100;
		uint16_t packetID;
		int16_t diff = 0, x = 0;


		int8_t ballID = 0;
		int8_t numb= 0;
		imbs.Read(&id, BINARYPACKETYPELENGTH);
		int p = sizeof(packetID);
		imbs.Read(&packetID, POSITION_BYNARY_LENGTH);
		imbs.Read(&diff, POSITION_BYNARY_LENGTH);
		imbs.Read(&x, POSITION_BYNARY_LENGTH);
		imbs.Read(&playerList[id].getY(), POSITION_BYNARY_LENGTH);
		imbs.Read(&negative, 1);
		imbs.Read(&numb, BINARYPACKETYPELENGTH);
	
		for (int i = 0; i < numb; ++i)
		{
			std::cout << "BALL IN " << std::endl;
			imbs.Read(&ballID, BINARYPACKETYPELENGTH);
			imbs.Read(&ballList[ballID].getDirection(), 1);
			if(!ballList[ballID].getActivated())
			ballList[ballID].getActivated() = true;
		
		}
			
		
		if (negative)
		{
			diff *= -1;
		}
		

		int16_t xCalc = playerList[id].getX();
		if (xCalc + diff != x)
		{
			std::cout << "ERROR EN MOVIMIENTO, MOVIMIENTO ERRONEO" << std::endl;
			switch (id)
			{
			case 0:
				failedPosition_p1 = true;
			break;

			case 1:
				failedPosition_p2 = true;
				break;
			}
		}
		if (xCalc + diff >= 0 && xCalc + diff <= 800)
		{
			playerList[id].getX() += diff;
			accumResponse.addMovement(diff, id);
		}
		sendBalls();

		
	
	
	}
		break;

	case PT_DISCONNECT:
		std::cout << "Solcitud de desconexion de Cliente: " << ip << "Con puerto: " << port << std::endl;
		serverDisconnection();

		break;
	
	default:
		std::cout << "Tipo de paquete no identificado, por favor revisa lo que se envía." << std::endl;

	


	}
}





bool UDPmanager::sendMessage(char * message, sf::IpAddress & ip, unsigned short & port,uint32_t sizeBuffer)
{
	
		sf::Packet packet;
		packet.append(message, sizeBuffer);
		sf::Socket::Status status = socket.send(packet, ip, port);
	
		if (status == sf::Socket::Done)
			return true;
		else
			return false;
	
	
	

}

void UDPmanager::serverDisconnection()
{
	shutdown = true;
	shutdown_clock.restart();
	std::cout << "Server Is About to ShutDown" << std::endl;
	while (playerList.size() != 0)
	{
		if (shutdown_clock.getElapsedTime() > sf::seconds(3.0))
		{
			for (int i = 0; i < playerList.size(); i++)
			{
				playerList.erase(playerList.begin() + i);

			}
			break;
		}
			for (int i = 0; i < playerList.size(); i++)
			{
			
					OutputMemoryBitStream ombs;
					ombs.Write(PacketType::PT_SHUTDOWN, BINARYPACKETYPELENGTH);
					sf::IpAddress ip = playerList[i].getPlayerIP();
					unsigned short port = playerList[i].getPlayerPort();
					sendMessage(ombs.GetBufferPtr(), ip, port, ombs.GetByteLength());
				
			}

			
		
	}
	isConnected = false;
}


