#pragma once
#include "State.h"
#include <array>
class ProfileState :
    public State
{

	enum{TEXT_USER_OUT,TEXT_INPUT};
	std::array<sf::Text,2> mText;
	std::string mInputString;
	
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

