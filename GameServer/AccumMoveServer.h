
#include <vector>
#include <iostream>

struct movementCheck
{
	int16_t xCoord;
	int idPacket;

};
class AccumMoveServer 
{
	
private:
	std::vector<movementCheck> movementList[2];
public:

	AccumMoveServer();
	void addMovement(int16_t x, int id);
	int16_t sendAcumMovement(int id);
	int getSize(int & id);
	~AccumMoveServer();
};