#pragma once
#include "State.h"
#include "Discs.h"
class GameSampleState :    public State
{
	sf::Sprite mBoardSprite;
	Discs mDiscSprites;
public:
	GameSampleState(ReversiSFML* app);
	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Render(float dt) override;
	virtual void MouseInput(const sf::Vector2f& pos) override;
	virtual void KeyInput(sf::Keyboard::Key key) override;
};

