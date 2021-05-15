#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include <array>

class ReversiSFML;

class Chat
{
	enum{
		TEXT_LOG = 0,TEXT_INPUT,TEXT_COUNT,
		MSG_NAME_SIZE = 16, MSG_BODY_SIZE = 48,MSG_PADDING = 6, MSG_SIZE = MSG_BODY_SIZE + MSG_NAME_SIZE,  // msg length shorter than msg body size to allow for extra characters 
		MSG_COUNT = 9,
	};
	// 9 messages each with 15 chars
	std::array<std::array<char, MSG_SIZE>, MSG_COUNT> mMessages;

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
	void SetChatLog(const std::array<std::array<char, MSG_SIZE>, MSG_COUNT>& msgs);
	const std::array<char, MSG_SIZE>& GetRecentChatEntry() const;
	const std::array<std::array<char, MSG_SIZE>, MSG_COUNT>& GetChatMessages();
};

