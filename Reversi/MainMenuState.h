#pragma once
#include "State.h"
#include "Button.h"
#include <array>

class MainMenuState : public State
{
	enum{BTN_SINGLE_GAME = 0, BTN_HOST_GAME,BTN_JOIN_GAME,BTN_COUNT};
	sf::Sprite mLogoStretch;
	sf::Sprite mLogo;
	std::array<MenuButton, BTN_COUNT> mButtons;
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

