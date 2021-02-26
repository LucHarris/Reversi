#include "ReversiSFML.h"
#include "Constants.h"

ReversiSFML::ReversiSFML()
	:
	mStateManager(this)
{
}

void ReversiSFML::Init()
{
	window.create(sf::VideoMode(gc::ViewportWidth, gc::ViewportHeight), gc::AppTitle);
	resources.Load();
	mStateManager.Init();
}

void ReversiSFML::Run()
{

	sf::Clock clock;
	sf::Time deltaTime;
	float dt = 0.0f;

	while (window.isOpen())
	{
		sf::Event sfEvent;
		deltaTime = clock.restart();

		dt = deltaTime.asSeconds();

		mStateManager.Update(deltaTime.asSeconds());

		while (window.pollEvent(sfEvent))
		{
			switch (sfEvent.type)
			{
			case sf::Event::Closed: 
				window.close(); 
				break;
			case sf::Event::MouseButtonReleased:
				mStateManager.MouseInput(sf::Mouse::getPosition());
			case sf::Event::KeyReleased:
				mStateManager.KeyInput(sfEvent.key.code);
			default:
				break;
			}
		}

		//render
		Render(dt);
	}
}

void ReversiSFML::Render(float dt)
{
	window.clear(sf::Color::Black);

	mStateManager.Render(dt);

	window.display();
}
