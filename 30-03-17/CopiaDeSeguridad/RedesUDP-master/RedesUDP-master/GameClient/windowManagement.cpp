#include "windowManagement.h"





void windowManagement::init(float x, float y, std::string chatName)
{
	

	screenDimensions.x = x;
	screenDimensions.y = y;
	windowRenderer.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), chatName);

	if (!font.loadFromFile("courbd.ttf"))
	{
		std::cout << "Can't load the font file" << std::endl;
	}
	





	separator.setSize(sf::Vector2f(800, 5));
	separator.setFillColor(sf::Color(200, 200, 200, 255));
	separator.setPosition(0, 550);

	

}

void windowManagement::loop()
{
	

	while (windowRenderer.isOpen())
	{
		sf::Event evento;
		while (windowRenderer.pollEvent(evento))
		{
			windowStuff(evento);
		}
		windowRenderer.draw(separator);
	
	
		//NETWORKING
		
		


		//NETWORKING
		windowRenderer.display();
		windowRenderer.clear();
	}
}

void windowManagement::windowStuff(sf::Event & evento)
{
	switch (evento.type)
	{
	
	}
}

sf::RenderWindow & windowManagement::getWindowRenderer()
{
	
	return get().windowRenderer;
}


windowManagement::~windowManagement()
{
}
