#include "windowManagement.h"





void windowManagement::init(float x, float y, std::string chatName)
{
	
	timer_client.restart();
	screenDimensions.x = x;
	screenDimensions.y = y;
	

	main_character = new character(x/2,y/2);
	if (!font.loadFromFile("courbd.ttf"))
	{
		std::cout << "Can't load the font file" << std::endl;
	}
	
	
	udpManager.initConnection();
	
	windowRenderer.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), chatName);
	

	

}

void windowManagement::loop()
{
	int16_t xU =0;
	int16_t yU = 0;
	main_character->setPosition(sf::Vector2<int16_t>(xU, yU));
	playerList.push_back(main_character);
	sf::Thread recv_thread(&UDPmanager::recv, &udpManager);

	recv_thread.launch();

	if (!udpManager.isDisconnected())
	{
		
		
		while (windowRenderer.isOpen())
		{
			sf::Event evento;
			while (windowRenderer.pollEvent(evento))
			{
				windowStuff(evento);
			}
			if (udpManager.isDisconnected())
			{
				windowRenderer.close();
			}
			windowRenderer.draw(separator);


			if(timer_client.getElapsedTime() > sf::seconds(1))
			{
				udpManager.ping( 0, 0);
				

				//NETWORKING
				if (playerList.size() < udpManager.getPlayerSize())
				{
					for (int i = playerList.size(); i < udpManager.getPlayerSize(); i++)
					{
						character* p = new character(0, 0);
						playerList.push_back(p);
						//playerList.resize(udpManager.getPlayerSize());
					}
				}
				for (int i = 0; i < playerList.size(); i++)
				{
					int16_t x = udpManager.getPlayer(i).getX(), y = udpManager.getPlayer(i).getY();
					playerList[i]->setPosition(sf::Vector2<int16_t>(x, y));
					
				}
				timer_client.restart();
			}


				//NETWORKING
			for (int i = 0; i < playerList.size(); i++)
			{
				windowRenderer.draw(*playerList[i]->getCircleshape());
			}
			//windowRenderer.draw(*main_character->getCircleshape());
			windowRenderer.display();
			windowRenderer.clear();
		}
	
	
	}
	recv_thread.terminate();
}

void windowManagement::windowStuff(sf::Event & evento)
{
	switch (evento.type)
	{
	case sf::Event::KeyPressed:
		if (sf::Keyboard::Escape)
		{
				
				udpManager.disconnect();
			
		}
		
	case sf::Event::MouseButtonPressed:
		
	
		
		main_character->setPosition((sf::Vector2<int16_t>( (sf::Mouse::getPosition(windowRenderer).x- main_character->getCircleshape()->getRadius()),
			 (sf::Mouse::getPosition(windowRenderer).y - main_character->getCircleshape()->getRadius()))));
		break;
	}
}

sf::RenderWindow & windowManagement::getWindowRenderer()
{
	
	return get().windowRenderer;
}


windowManagement::~windowManagement()
{
}
