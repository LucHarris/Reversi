#include "NetworkState.h"
#include "Board.h"
#include "ReversiSFML.h"
#include <algorithm>

NetworkState::NetworkState(ReversiSFML* app)
	:
	State(app),
	mInputString{ "localhost","8888" }
{
}

void NetworkState::Init()
{
	for (auto& t : mText)
	{
		t.setFont(mpApp->resources.GetFontAt(Resources::FONT_MAIN));
	}

	mText.at(TEXT_INPUT_IP).setString(mInputString.at(TEXT_INPUT_IP));
	mText.at(TEXT_INPUT_IP).setPosition({ 500,100 });

	mText.at(TEXT_INPUT_PORT).setString(mInputString.at(TEXT_INPUT_PORT));
	mText.at(TEXT_INPUT_PORT).setPosition({ 500,140 });

	mText.at(TEXT_PROMPT).setString("Host IP Address:\nPort:\n\nType IP address and Port\nEnter key to switch between inputs\nClick Play Icon to Start");
	mText.at(TEXT_PROMPT).setPosition({ 200,100 });

	mText.at(TEXT_TITLE).setPosition({ 150,50 });

	// default selected input
	mText.at(mActiveText).setFillColor(sf::Color::Green);

	mButtons.at(BTN_PLAY).setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_ICON_PLAY));
	mButtons.at(BTN_PLAY).setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_BC] + sf::Vector2f{ 0.0f,-128.0f });

}

void NetworkState::Update(float dt)
{
}

void NetworkState::Render(float dt)
{
	for (auto& t : mText)
	{
		mpApp->window.draw(t);
	}

	for (auto& btn : mButtons)
	{
		mpApp->window.draw(btn);
	}
}

void NetworkState::MouseInput(const sf::Vector2f& mos)
{
	if (mButtons.at(BTN_PLAY).getGlobalBounds().contains(mos))
	{

		switch (mpApp->gameType)
		{
		case ReversiSFML::GameType::HOST:
			//todo host setup
			break;
		case ReversiSFML::GameType::JOIN:
			//todo client setup
			break;
		default:;
		}

		mpApp->resources.Play(Resources::SOUND_CLICK, mpApp->masterVolume);
		mpApp->stateManager.ChangeState(gc::STATE_INDEX_PLAYER_SELECTION, true);
	}
}

void NetworkState::KeyInput(sf::Keyboard::Key key)
{
}

void NetworkState::TextEntered(unsigned int key)
{
	//todo validate strings

	switch (key)
	{
	case '\b':// backspace
		if (!mInputString.at(mActiveText).empty())
		{
			mInputString.at(mActiveText).erase(mInputString.at(mActiveText).size() - 1, 1);
		}
		break;
		// 
	case '\r': // switch input string
		mText.at(mActiveText).setFillColor(sf::Color::White);
		mActiveText = 1 - mActiveText;
		mText.at(mActiveText).setFillColor(sf::Color::Green);
		break;
	default:
		mInputString.at(mActiveText) += key;
		break;
	}

	mText.at(mActiveText).setString(mInputString.at(mActiveText));

}


void NetworkState::Reset()
{
	switch (mpApp->gameType)
	{
	case ReversiSFML::GameType::HOST:
		mText.at(TEXT_TITLE).setString("Host Game");
		break;
	case ReversiSFML::GameType::JOIN:
		mText.at(TEXT_TITLE).setString("Join Game");
		break;
	default:
		mText.at(TEXT_TITLE).setString("You are not supposed to be here.");
		break;
	}

}

