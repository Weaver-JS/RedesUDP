#include "AccumMoveServer.h"

AccumMoveServer::AccumMoveServer()
{
}

void AccumMoveServer::addMovement(int16_t x, int id)
{
	movementCheck mov;
	mov.idPacket = id;
	mov.xCoord = x;
	movementList[id].push_back(mov);
}

int16_t AccumMoveServer::sendAcumMovement(int id)
{
	int16_t accumX = 0;
	for (int i = 0; i < movementList[id].size(); i++)
	{
		accumX += movementList[id][i].xCoord;
	}
	movementList[id].clear();
	return accumX;
}

int AccumMoveServer::getSize(int & id)
{
	return movementList[id].size();
}

AccumMoveServer::~AccumMoveServer()
{
}
