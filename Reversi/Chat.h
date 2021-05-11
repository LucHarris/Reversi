#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include <array>

class ReversiSFML;

class Chat
{
	enum{
		TEXT_LOG = 0,TEXT_INPUT,TEXT_COUNT,
		MSG_LENGTH = 15, MSG_COUNT = 9,
	};
	// 9 messages each with 15 chars
	std::array<std::array<char, MSG_LENGTH>, MSG_COUNT> mMessages;

	std::array<sf::Text, TEXT_COUNT> mText;
	std::string mInputString;
	ReversiSFML* mpApp;

public:
	// toggle for text display
	bool display = true;

	Chat(ReversiSFML* app);

	void Init();
	void UpdateLog();
	void Render();
	void TextEntered(unsigned int key);

	void AddMessage(const char msg[]);
};

