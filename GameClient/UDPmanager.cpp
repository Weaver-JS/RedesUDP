#include "UDPmanager.h"



UDPmanager::UDPmanager()
{
	notConnected = true;
	socket.setBlocking(true);
	disconnected = false;
}


UDPmanager::~UDPmanager()
{
}

void UDPmanager::initConnection()
{
	/*sf::Socket::Status status = socket.bind(PORT);

	if (status != sf::Socket::Done)
	{
		std::cout << "No se puede vincular al puerto: " << PORT << std::endl;
	}
	else if (status == sf::Socket::Done)
	{


	}*/
	

	while (notConnected)
	{
		OutputMemoryBitStream ombs;
		ombs.Write(PacketType::PT_HELLO, BINARYPACKETYPELENGTH);
		//sf::sleep(sf::seconds(0.5));
		if (sendMessage(ombs.GetBufferPtr(), IP, PORT, ombs.GetByteLength()))
		{
			sf::IpAddress senderIP;
			unsigned short senderPort;
			sf::Packet packet;
			if (socket.receive(packet, senderIP, senderPort) == sf::Socket::Done)
			{
				readMessage((char*)packet.getData(), packet.getDataSize(), senderIP, senderPort);
				
			}
		}

	}


	
}

void UDPmanager::ping()
{
	OutputMemoryBitStream ombs;
	ombs.Write(PacketType::PT_PING, BINARYPACKETYPELENGTH);

	

	sendMessage(ombs.GetBufferPtr(), IP, PORT, ombs.GetBitLength());
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
	case PT_WELCOME:
	{
		uint16_t x = 0, y = 0;
		player = new PlayerInfo(ip,port);
		imbs.ReadBits(&player->getPlayerID(), BINARYPACKETYPELENGTH);
		imbs.ReadBits(&player->getX() , POSITION_BYNARY_LENGTH);
		imbs.ReadBits(&player->getY() , POSITION_BYNARY_LENGTH);
		notConnected = false;
	}
		break;
	case PT_DISCONNECT:
		disconnected = true;
		break;
	case PT_FULL:
		disconnected = true;
		break;
	case PT_PING:
	{
		imbs.ReadBits(&size, BINARYPACKETYPELENGTH);
		if (playerList.size() != size)
		{
			playerList.resize(size);
		}
		int id = 0;
		imbs.ReadBits(&id, BINARYPACKETYPELENGTH);
		imbs.ReadBits(&playerList[id]->getX(), POSITION_BYNARY_LENGTH);
		imbs.ReadBits(&playerList[id]->getY(), POSITION_BYNARY_LENGTH);
	}
		break;
	default:
		std::cout << _message << std::endl;

	}

	
}

void UDPmanager::recv()
{
	sf::IpAddress senderIP;
	unsigned short senderPort;
	sf::Packet packet;
	if (socket.receive(packet, senderIP, senderPort) == sf::Socket::Done)
	{
		readMessage((char*)packet.getData(), packet.getDataSize(), senderIP, senderPort);

	}
}
bool UDPmanager::sendMessage(char * message, sf::IpAddress ip, unsigned short port,uint32_t sizeBuffer)
{
	sf::Packet packet;
	packet.append(message, sizeBuffer);
	return (socket.send(packet, ip, port) == sf::Socket::Done) ? true : false;
	

}

PlayerInfo & UDPmanager::getPlayer()
{
	return *player;
}

PlayerInfo & UDPmanager::getPlayer(int i)
{

		return *playerList[i];
	
}

int  UDPmanager::getPlayerSize()
{
	return playerList.size();
}

void UDPmanager::disconnect()
{
	OutputMemoryBitStream ombs;
	ombs.Write(PacketType::PT_DISCONNECT, BINARYPACKETYPELENGTH);

	

	sendMessage(ombs.GetBufferPtr(), IP, PORT, ombs.GetBitLength());
	
}

bool UDPmanager::isDisconnected()
{
	return disconnected;
}
