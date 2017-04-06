#include "windowManagement.h"





void windowManagement::init(float x, float y, std::string chatName)
{
	

	screenDimensions.x = x;
	screenDimensions.y = y;
	windowRenderer.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), chatName);

	main_character = new character(x/2,y/2);
	if (!font.loadFromFile("courbd.ttf"))
	{
		std::cout << "Can't load the font file" << std::endl;
	}
	





	

	

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
		windowRenderer.draw(*main_character->getCircleshape());
		windowRenderer.display();
		windowRenderer.clear();
	}
}

void windowManagement::windowStuff(sf::Event & evento)
{
	switch (evento.type)
	{
	case sf::Event::MouseButtonPressed:
		
	
		
		main_character->setPosition((sf::Vector2<int16_t>( (sf::Mouse::getPosition(windowRenderer).x- main_character->getCircleshape()->getRadius()),
			 (sf::Mouse::getPosition(windowRenderer).y - main_character->getCircleshape()->getRadius()  ))));
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
