#include "Chat.h"
#include "ReversiSFML.h"
#include <cassert>
#include <sstream>
#include <algorithm>

Chat::Chat(ReversiSFML* app)
	:
	mpApp(app)
{
	assert(mpApp);

	// empty messages
	for (auto& msg : mMessages)
	{
		for (auto& c : msg)
		{
			c = '\0';
		}
	}
}

void Chat::Init()
{
	mText.at(TEXT_LOG).setFont(mpApp->resources.GetFontAt(Resources::FONT_MAIN));
	mText.at(TEXT_LOG).setCharacterSize(gc::CHAR_SIZE_REG);
	mText.at(TEXT_LOG).setFillColor(sf::Color::Black);
	mText.at(TEXT_LOG).setString("<No Chat>");
	mText.at(TEXT_LOG).setOrigin({ 0.0f,mText.at(TEXT_LOG).getGlobalBounds().height});
	mText.at(TEXT_LOG).setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_BL] + sf::Vector2f{ 64.0f,-128.0f });

	mText.at(TEXT_INPUT).setFont(mpApp->resources.GetFontAt(Resources::FONT_MAIN));
	mText.at(TEXT_INPUT).setCharacterSize(gc::CHAR_SIZE_REG);
	mText.at(TEXT_INPUT).setFillColor(sf::Color::Blue);
	mText.at(TEXT_INPUT).setString("Type Here.");
	mText.at(TEXT_INPUT).setPosition({ 300.0f,400.0f });
	mText.at(TEXT_INPUT).setOrigin({ 0.0f,0.0f });
	mText.at(TEXT_INPUT).setPosition(mText.at(TEXT_LOG).getPosition());

}

void Chat::UpdateLog()
{
	if (display)
	{
		std::ostringstream oss;
		for (auto& msg : mMessages)
		{

			int i = 0;
			// stops at EOT
			while (i < (int)msg.size() && msg.at(i) != '\0')
			{
				oss << msg.at(i);
				++i;
			}

			oss << '\n';
		}

		mText.at(TEXT_LOG).setString(oss.str());
		// scroll up each message
		mText.at(TEXT_LOG).setOrigin({ 0.0f,mText.at(TEXT_LOG).getGlobalBounds().height });

	}
}
void Chat::Render()
{
	if (display)
	{
		for (auto& t : mText)
		{
			mpApp->window.draw(t);
		}
	}
}

void Chat::TextEntered(unsigned int key)
{

	const bool canType = mInputString.length() < MSG_BODY_SIZE - MSG_PADDING;

	ClientSendData clientSend;
	std::string temp = "";

	switch (key)
	{
	case '\b':// backspace
		if (!mInputString.empty())
		{
			mInputString.erase(mInputString.size() - 1, 1);
		}
		break;
		// 
	case '\r':
		if (mInputString.length() > 0)
		{
			temp = mpApp->localPlayer.userData.name + std::string(": ") + mInputString;

			assert(temp.length() <= MSG_SIZE);

			if (mpApp->gameType != ReversiSFML::GameType::JOIN)
			{
				// host or local
				//todo remove AddMessage(mInputString.c_str());
				AddMessage(temp.c_str());
			}
			else
			{
				// client
				//todo remove std::copy(mInputString.begin(), mInputString.end(), clientSend.msg);
				std::copy(temp.begin(), temp.end(), clientSend.msg);
				mpApp->threadPool.PushInputQueue(clientSend);
			}
		}

		std::fill(mInputString.begin(), mInputString.end(), '\0');
		mInputString.clear();
		// todo send message (client) or add message (host) then clear
		break;
	default:
		if (canType)
		{
			mInputString += key;
		}
		break;
	}

	mText.at(TEXT_INPUT).setString(mInputString);
}

void Chat::AddMessage(const char msg[])
{
	const int sizeA = sizeof(*msg) / sizeof(msg[0]);
	const int sizeB = sizeof(mMessages[0]) / sizeof(mMessages[0][0]);
	const int size = (sizeA > sizeB)? sizeA : sizeB;
	assert(size <= ClientSendData::MSG_SIZE); // message length

	// reset message
	std::fill(mMessages.front().begin(), mMessages.front().end(), '\0');

	// new message oldest message
	std::copy_n( msg, size, mMessages.front().begin());

	// new message to last element to appear most recent
	std::rotate(mMessages.begin(), mMessages.begin() + 1, mMessages.end());

	UpdateLog();
}

void Chat::SetChatLog(const std::array<std::array<char, Chat::MSG_SIZE>, MSG_COUNT>& msgs)
{
	mMessages = msgs;
}

const std::array<char, Chat::MSG_SIZE>& Chat::GetRecentChatEntry() const
{
	return mMessages.back();
}

const std::array<std::array<char, Chat::MSG_SIZE>, Chat::MSG_COUNT>& Chat::GetChatMessages()
{
	return mMessages;
	// TODO: insert return statement here
}


