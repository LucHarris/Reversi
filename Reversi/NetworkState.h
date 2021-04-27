#pragma once
#include "State.h"
#include "SFML/Graphics.hpp"
#include <string>
#include <array>

class ReversiSFML;

// user inputs ip address ("localhost")
class NetworkState :
    public State
{
	enum
	{
		TEXT_INPUT_IP = 0,TEXT_INPUT_PORT, TEXT_PROMPT, TEXT_TITLE, TEXT_COUNT,
		BTN_PLAY = 0,BTN_COUNT
	
	};
	std::array<sf::Text, TEXT_COUNT> mText;
	std::array<std::string,2> mInputString;
	std::array<sf::Sprite, 1> mButtons;

	size_t mActiveText = 0;

public:
	NetworkState(ReversiSFML* app);
	virtual void Init();
	virtual void Update(float dt);
	virtual void Render(float dt);
	virtual void MouseInput(const sf::Vector2f& pos);
	virtual void KeyInput(sf::Keyboard::Key key);
	virtual void TextEntered(unsigned int key);
	virtual void Reset();
private:
};

