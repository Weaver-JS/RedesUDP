#include "UDPmanager.h"



UDPmanager::UDPmanager()
{
	abort = false;
}


UDPmanager::~UDPmanager()
{
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

	while (true)
	{
		sf::IpAddress senderIP;
		unsigned short senderPort;
		sf::Packet packet;
		if (socket.receive(packet, senderIP, senderPort) == sf::Socket::Done)
			readMessage((char*)packet.getData(), packet.getDataSize(), senderIP, senderPort);
			
	}
}



int  UDPmanager::getPlayerID(short port)
{
	for (int i = 0; i < playerList.size(); i++)
	{
		if (playerList[i]->getPlayerPort() == port)
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
		
		for (int i = 0; i < playerList.size(); i++)
		{
			if (playerList[i]->getPlayerPort() == port)
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
		PlayerInfo* newPlayer = new PlayerInfo(ip, port);
		playerList.push_back(newPlayer);
		OutputMemoryBitStream ombs;
		ombs.Write(PacketType::PT_WELCOME, BINARYPACKETYPELENGTH);
	
		int16_t x, y;
		x = rand() % SCREEN_WIDTH;
		y = rand() % SCREEN_HEIGHT;
		ombs.Write(playerList.size() - 1, BINARYPACKETYPELENGTH);
		playerList[playerList.size() - 1]->getX() = x;
		playerList[playerList.size() - 1]->getY() = y;

		ombs.Write(x, POSITION_BYNARY_LENGTH);
		ombs.Write(y, POSITION_BYNARY_LENGTH);
		sendMessage(ombs.GetBufferPtr(), ip, port, ombs.GetByteLength()); 

	}
		break;
	case PT_PING:
		
		for (int i = 0; i < playerList.size(); i++)
		{

			OutputMemoryBitStream ombs;
			ombs.Write(PacketType::PT_PLAYER_POSITION, BINARYPACKETYPELENGTH);
			ombs.Write(playerList.size(), BINARYPACKETYPELENGTH);
			ombs.Write(i, BINARYPACKETYPELENGTH);
			ombs.Write(playerList[i]->getX(),POSITION_BYNARY_LENGTH);
			ombs.Write(playerList[i]->getY(), POSITION_BYNARY_LENGTH);
			sendMessage(ombs.GetBufferPtr(), ip, port, ombs.GetByteLength());

		 
		}
		break;
	case PT_DISCONNECT:
	{
		std::cout << "Solcitud de desconexion de Cliente: " << ip << "Con puerto: " << port << std::endl;
		OutputMemoryBitStream ombs;
		ombs.Write(PacketType::PT_DISCONNECT, BINARYPACKETYPELENGTH);
		sendMessage(ombs.GetBufferPtr(), ip, port, ombs.GetByteLength());

		if(getPlayerID(port) != -1)
		playerList.erase(playerList.begin() + getPlayerID(port));

	}
		break;
	default:
		std::cout << "Tipo de paquete no identificado, por favor revisa lo que se envía." << std::endl;

	}

	
}





void UDPmanager::sendMessage(char * message, sf::IpAddress & ip, unsigned short & port,uint32_t sizeBuffer)
{
	sf::Packet packet;
	packet.append(message, sizeBuffer);
	socket.send(packet, ip, port);

}


