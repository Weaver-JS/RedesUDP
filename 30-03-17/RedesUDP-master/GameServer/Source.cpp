#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include "UDPmanager.h"

int main()
{
	UDPmanager udp;
	udp.initConnection();
	return 0;
}