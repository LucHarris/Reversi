#include "ReversiSFML.h"
#include "Constants.h"

ReversiSFML::ReversiSFML()
	:
	stateManager(this)
{
}

void ReversiSFML::Init()
{
	window.create(sf::VideoMode(gc::VIEWPORT_WIDTH_U, gc::VIEWPORT_HEIGHT_U), gc::AppTitle, sf::Style::Close);
	resources.Load();
	music.Init();
	music.SetMasterVolume(masterVolume * gc::HALF);
	music.PlayNext();
	reversiGame.Initialize();
	stateManager.Init();

	InitText(debugLog);
	debugLog.setString("Debug");

}

void ReversiSFML::Run()
{
	sf::ContextSettings sc;
	sf::Clock clock;
	sf::Time deltaTime;
	float dt = 0.0f;

	while (window.isOpen())
	{
		sf::Event sfEvent;
		deltaTime = clock.restart();

		dt = deltaTime.asSeconds();

		stateManager.Update(dt);
		music.Update(dt);

		while (window.pollEvent(sfEvent))
		{
			switch (sfEvent.type)
			{
			case sf::Event::Closed: 
				window.close(); 
				break;
			case sf::Event::MouseButtonReleased:
				stateManager.MouseInput((sf::Vector2f)sf::Mouse::getPosition(window));
				
				break;
			case sf::Event::KeyReleased:
				stateManager.KeyInput(sfEvent.key.code);
				break;
			case sf::Event::TextEntered:
				if (sfEvent.text.unicode < 128 && sfEvent.text.unicode >= 32)
				{
					stateManager.TextEntered(sfEvent.text.unicode);
					//music.PlayNext(5.0f);
				}
				break;
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

	stateManager.Render(dt);

	window.draw(debugLog);

	window.display();
}

void ReversiSFML::InitText(sf::Text& t)
{
	t.setFont(resources.GetFontAt(Resources::FONT_MAIN));
	t.setCharacterSize(gc::CHAR_SIZE_REG);
	t.setFillColor(sf::Color::Magenta);
}
