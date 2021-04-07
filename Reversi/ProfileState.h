#pragma once
#include "State.h"
class ProfileState :
    public State
{
	sf::Text mUserText;
public:
	ProfileState(ReversiSFML* app);
	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Render(float dt) override;
	virtual void MouseInput(const sf::Vector2f& pos) override;
	virtual void KeyInput(sf::Keyboard::Key key) override;
	virtual void TextEntered(unsigned int key) override;
	virtual void Reset() override;
};

