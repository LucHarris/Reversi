#pragma once

class GameTimer;

class State
{
public:
	virtual void Initialize() = 0;
	virtual void Update(const GameTimer&) = 0;
	virtual void Render(const GameTimer&) = 0;
	virtual void Input() = 0;
};

