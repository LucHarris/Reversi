#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

class ReversiSFML;

class MenuButton
{
	sf::Sprite mSprite;
	sf::Text mText;
	sf::Color mColor;
	ReversiSFML* mpApp;

public:
	MenuButton(ReversiSFML* app);
	void Init (const std::string& prompt, float pos);
	bool Contains(const sf::Vector2f& pos);

	void Render(float dt);

};

