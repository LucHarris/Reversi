#pragma once
#include "State.h"
#include "Button.h"
#include <array>

class MainMenuState : public State
{
	sf::Sprite mLogoStretch;
	sf::Sprite mLogo;
	std::array<MenuButton,2> mButtons;
public:
	MainMenuState(ReversiSFML* app) ;
	virtual void Init() override;
	virtual void Update(float)	override;
	virtual void Render(float) override;
	virtual void MouseInput(const sf::Vector2f&) override;
	virtual void KeyInput(sf::Keyboard::Key) override;
	virtual void TextEntered(unsigned int key) override;
	virtual void Reset() override {};

};

