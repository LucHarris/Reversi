#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


class GameTimer;

class ReversiSFML;

class State
{
protected:
	ReversiSFML* mpApp;
public:
	State(ReversiSFML* app);
	virtual void Init() = 0;
	virtual void Update(float) = 0;
	virtual void Render(float) = 0;
	virtual void MouseInput(const sf::Vector2i&) = 0;
	virtual void KeyInput(sf::Keyboard::Key) = 0;
};

