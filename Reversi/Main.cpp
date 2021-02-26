
//#include <SFML/Graphics.hpp>
//#include <SFML/Graphics/Color.hpp>

#include "ReversiSFML.h"

int main()
{
	ReversiSFML app;

	app.Init();

	app.Run();
	

	//std::string title = "Reversi";
	//sf::RenderWindow window(sf::VideoMode(800, 600),title);

	//sf::Texture text;
	//sf::Sprite sprite;
	//
	//if (text.loadFromFile("../Data/patternBackground.png"))
	//{
	//	text.setRepeated(true);
	//	text.setSmooth(true);
	//	sprite.setTexture(text);
	//	sprite.setTextureRect({ 0	,0,800,600 });
	//}

	//sf::Clock clock;
	//sf::Time deltaTime;
	//int x = 0;
	//float angle = 0;

	//sf::Font font;
	//sf::Text note;
	//note.setString("woof");

	//while (window.isOpen())
	//{
	//	sf::Event event;
	//	deltaTime = clock.restart();
	//	++x;
	//	angle += deltaTime.asSeconds();
	//	sprite.setRotation(angle);


	//	

	//	while (window.pollEvent(event))
	//	{
	//		switch (event.type)
	//		{
	//		case sf::Event::Closed:
	//			window.close();
	//			break;
	//		}
	//	}
	//	window.clear(sf::Color::Black);

	//	//window.draw(text);

	//	window.draw(sprite);
	//	window.display();

	//}
}