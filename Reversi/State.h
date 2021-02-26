#pragma once

class GameTimer;

class ReversiSFML;

class State
{
private:
	ReversiSFML* mpApp;
public:
	virtual void Init() = 0;
	virtual void Update(float) = 0;
	virtual void Render(float) = 0;
	virtual void Input() = 0;
};

