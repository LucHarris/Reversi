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
	virtual void Update(float dt) = 0;
	virtual void Render(float dt) = 0;
	virtual void MouseInput(const sf::Vector2i& pos) = 0;
	virtual void KeyInput(sf::Keyboard::Key key) = 0;
};

