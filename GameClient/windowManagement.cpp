#include "windowManagement.h"





void windowManagement::init(float x, float y, std::string chatName)
{
	
	timer_client.restart();
	screenDimensions.x = x;
	screenDimensions.y = y;
	
	
	initBalls();
	
	
	main_character = new character(x/2,y/2);
	if (!font.loadFromFile("courbd.ttf"))
	{
		std::cout << "Can't load the font file" << std::endl;
	}
	
	
	udpManager.initConnection();
	
	windowRenderer.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), chatName);
	
//	playerList.resize(MAXPLAYERS);
	for (int i = 0; i < MAXPLAYERS; i++)
	{
		character p(0, 0);
		playerList.push_back(p);
		

	}
	state = LOBBY;

	

}

void windowManagement::loop()
{

	sf::Thread recv_thread(&UDPmanager::recv, &udpManager);



	if (!udpManager.isDisconnected())
	{




		recv_thread.launch();
		
			while (windowRenderer.isOpen())
			{
				switch(state)
					
				{
				case PLAY:
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
					chechIfWin();
					moveBalls();
					interpolateMovement();
					for (int i = 0; i < playerList.size(); i++)
					{
						int16_t x = udpManager.getPlayer(i).getX(), y = udpManager.getPlayer(i).getY();

						playerList[i].setPosition(sf::Vector2<int16_t>(x, y));

					}
					//PING
					if (timer_client.getElapsedTime() > sf::milliseconds(200))
					{

						//NETWORKING

					//	playerList[udpManager.getPlayer().getPlayerID()].getPosition().x += accumlist.getFinalPosition();

						udpManager.ping(accumlist.getFinalPosition(),
							playerList[udpManager.getPlayer().getPlayerID()].getPosition().y, accumlist.getMovementCheckList().size());


						timer_client.restart();
					}
					//PING


					for (int i = 0; i < playerList.size(); i++)
					{


						windowRenderer.draw(playerList[i].getCircleshape());
					}
					for (int i = 0; i < TOTALBALLS; ++i)
					{

						ballsRenderList[i].setPosition(udpManager.getBallList()[i].getX(), udpManager.getBallList()[i].getY());
						windowRenderer.draw(ballsRenderList[i]);

					}



					windowRenderer.display();
					windowRenderer.clear();

				}
				break;
			case LOBBY:

				if (udpManager.getPlayer(PLAYER2).getX() == SCREEN_WIDTH)
				{
					state = PLAY;
				}
				break;
			case GAMEOVER:
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
				if (udpManager.getPlayer(PLAYER1).getPlayerWIn())
				{
					for (int i = 0; i < TOTALBALLS; i++)
					{
						udpManager.getBallList()[i].getX() = i  * OFFSET;
						udpManager.getBallList()[i].getY() = 40;
						ballsRenderList[i].setPosition(udpManager.getBallList()[i].getX(), udpManager.getBallList()[i].getY());
						windowRenderer.draw(ballsRenderList[i]);
					}
				}
				else
				{
					for (int i = 0; i < TOTALBALLS; i++)
					{
						udpManager.getBallList()[i].getX() = i  * OFFSET;
						udpManager.getBallList()[i].getY() = SCREEN_HEIGHT - 40;
						ballsRenderList[i].setPosition(udpManager.getBallList()[i].getX(), udpManager.getBallList()[i].getY());
						windowRenderer.draw(ballsRenderList[i]);
					}
				}
				for (int i = 0; i < playerList.size(); i++)
				{


					windowRenderer.draw(playerList[i].getCircleshape());
				}
				if (win_clock.getElapsedTime() > sf::seconds(2.0f))
				{
					udpManager.disconnect();
				}
				windowRenderer.display();
				windowRenderer.clear();
			}
				break;

				}


		}
	}
		recv_thread.terminate();
	}


void windowManagement::windowStuff(sf::Event & evento)
{
	switch (evento.type)
	{
	case sf::Event::KeyPressed:
		if (evento.key.code == sf::Keyboard::Escape)
		{
			udpManager.disconnect();

		}

		if (evento.key.code == sf::Keyboard::Space || evento.key.code == sf::Keyboard::Insert)
		{
			tryToPushBall(udpManager.getBallList());
		}

		if (evento.key.code == sf::Keyboard::A)
		{
			if (udpManager.getPlayer(udpManager.getPlayer().getPlayerID()).getX() - VEL > 0 )
			{
				int16_t x, y;
				x = udpManager.getPlayer().getX();
				y = udpManager.getPlayer().getY();

				udpManager.getPlayer(udpManager.getPlayer().getPlayerID()).getX() -= VEL;// .setPosition(sf::Vector2<int16_t>(x + 2, y));
				accumlist.addMovement(movKey::LEFT);
			}
			
		}
		if (evento.key.code == sf::Keyboard::D)
		{
			if(udpManager.getPlayer(udpManager.getPlayer().getPlayerID()).getX() + VEL < SCREEN_WIDTH)
			{
			int16_t x, y;
			x = udpManager.getPlayer().getX();
			y = udpManager.getPlayer().getY();

			udpManager.getPlayer(udpManager.getPlayer().getPlayerID()).getX() += VEL;// .setPosition(sf::Vector2<int16_t>(x + 2, y));
			accumlist.addMovement(movKey::RIGHT);
			}
			
		}

	case sf::Event::MouseButtonPressed:

	/*	playerList[udpManager.getPlayer().getPlayerID()].setPosition((sf::Vector2<int16_t>((sf::Mouse::getPosition(windowRenderer).x - main_character->getCircleshape()->getRadius()),
			 (sf::Mouse::getPosition(windowRenderer).y - main_character->getCircleshape()->getRadius()))));*/
			
		break;

		
	}
}

void windowManagement::interpolateMovement()
{
	
		if (udpManager.doWehavetointerpolate())
		{
			if (udpManager.getPlayer().getPlayerID() == PLAYER1)
			{
				
				if (udpManager.getPlayer(PLAYER2).getX() < udpManager.getFinalPosition())
				{
					udpManager.getPlayer(PLAYER2).getX() += VEL/ VEL;
				}
				else if (udpManager.getPlayer(PLAYER2).getX() > udpManager.getFinalPosition())
				{
					udpManager.getPlayer(PLAYER2).getX() -= VEL / VEL;
				}
				else if (udpManager.getPlayer(PLAYER2).getX() == udpManager.getFinalPosition())
				{
					//udpManager.doWehavetointerpolate() = false;
				}
			}
			else
			{
				if (udpManager.getPlayer(PLAYER1).getX() < udpManager.getFinalPosition())
				{
					udpManager.getPlayer(PLAYER1).getX() += VEL / VEL;
				}
				else if (udpManager.getPlayer(PLAYER1).getX() > udpManager.getFinalPosition())
				{
					udpManager.getPlayer(PLAYER1).getX() -= VEL / VEL;
				}
				else if (udpManager.getPlayer(PLAYER1).getX() == udpManager.getFinalPosition())
				{
					//udpManager.doWehavetointerpolate() = false;
				}
			}
		}
	
}

void windowManagement::moveBalls()
{


	int count = 0;
		for (int i = 0; i < TOTALBALLS; ++i)
		{
			
			if (udpManager.getBallList()[i].getActivated())
			{
				count++;
		
					if (udpManager.getBallList()[i].getY() <= 0 && udpManager.getBallList()[i].getDirection())
					{
						udpManager.getBallList()[i].getY() = 40;
					udpManager.getBallList()[i].getActivated() = false;
						//ENVIAR PT_BALL AQUÍ 
						
					}
					
					else if (udpManager.getBallList()[i].getDirection())
					{
						udpManager.getBallList()[i].getY() -= 1.0;
					}
					
					
				else if(!udpManager.getBallList()[i].getDirection())
				{
					if (udpManager.getBallList()[i].getY() >= SCREEN_HEIGHT)
					{
						udpManager.getBallList()[i].getY() = SCREEN_HEIGHT - 60;
					udpManager.getBallList()[i].getActivated() = false;
						//ENVIAR PT_BALL AQUÍ
					}
					else
					udpManager.getBallList()[i].getY() += 1.0;
					
				}
			}


		}
		//std::cout <<  << " que mierda?" << std::endl;
		udpManager.SetmovementBalls(count);
	//	std::cout << udpManager.getmovementBalls() << "sda" << std::endl;
	}


void windowManagement::initBalls()
{
	for (int i = 0; i < TOTALBALLS; i++)
	{
		Ball b;
		sf::CircleShape c;
		udpManager.getBallList().push_back(b);
		ballsRenderList.push_back(c);
		if (i < 5)
		{
			udpManager.getBallList()[i].getX() = i  * OFFSET ;
			udpManager.getBallList()[i].getY() = 60 - 20;
		}
		else
		{
			udpManager.getBallList()[i].getX() = i*OFFSET;
			udpManager.getBallList()[i].getY() = SCREEN_HEIGHT - 60;// ballList[i].getRadius();
		}
		udpManager.getBallList()[i].getRadius() = 20;
		ballsRenderList[i].setPosition(udpManager.getBallList()[i].getX(), udpManager.getBallList()[i].getY());
		ballsRenderList[i].setRadius(udpManager.getBallList()[i].getRadius());
		ballsRenderList[i].setFillColor(sf::Color(0, 0, 255, 255));
		ballsRenderList[i].setOrigin(0, 0);
	}
}



void windowManagement::tryToPushBall(std::vector<Ball> & v)
{
	
	for (int i = 0; i < TOTALBALLS; i++)
	{
		sf::Vector2<int16_t> vec((udpManager.getPlayer(udpManager.getPlayer().getPlayerID()).getX()  ) - (v[i].getX() ),
			(udpManager.getPlayer(udpManager.getPlayer().getPlayerID()).getY() ) - (v[i].getY()));
	

		if (sqrt(vec.x * vec.x + vec.y * vec.y)<(RADIUS + v[i].getRadius()) && !udpManager.getBallList()[i].getActivated())
		{
			/*int p = udpManager.getmovementBalls() + 1;
			udpManager.SetmovementBalls(p);*/
			std::cout << "YEAH " << i << std::endl;
			v[i].getActivated() = true;
			udpManager.isThereAballtoSend() = true;
			if(udpManager.getPlayer().getPlayerID() == 0)
			{ 
				v[i].getDirection() = true;
				
			}
			else
			{
				v[i].getDirection() = false;
			}
		}
	}
}

void windowManagement::chechIfWin()
{
	if (udpManager.getPlayer(PLAYER1).getPlayerWIn())
	{
		state = gameStates::GAMEOVER;
		win_clock.restart();
	}
	else if(udpManager.getPlayer(PLAYER2).getPlayerWIn())
	{
		state = gameStates::GAMEOVER;
		win_clock.restart();
	}
}

sf::RenderWindow & windowManagement::getWindowRenderer()
{
	
	return get().windowRenderer;
}


windowManagement::~windowManagement()
{
}
