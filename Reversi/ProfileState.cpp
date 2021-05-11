#include "ProfileState.h"
#include "ReversiSFML.h"
#include <sstream>
#include "Utility.h"

ProfileState::ProfileState(ReversiSFML* app)
	:
	State(app)
{
}

void ProfileState::Init()
{
	mText.at(TEXT_USER_OUT).setFont(mpApp->resources.GetFontAt(Resources::FONT_MAIN));
	mText.at(TEXT_USER_OUT).setFillColor({ 0,0,0,200 });

	mText.at(TEXT_INPUT).setFont(mpApp->resources.GetFontAt(Resources::FONT_MAIN));


}

void ProfileState::Update(float dt)
{
}

void ProfileState::Render(float dt)
{
	for (auto& text : mText)
	{
		mpApp->window.draw(text);
	}
}							

void ProfileState::MouseInput(const sf::Vector2f& pos)
{
}

void ProfileState::KeyInput(sf::Keyboard::Key key)
{
}

void ProfileState::TextEntered(unsigned int key)
{

	if (mpApp->gameType == ReversiSFML::GameType::SINGLE)
	{

		const bool canType = mInputString.length() < UserData::GetMaxNameSize();

		ClientSendData clientSend;

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
			if (mpApp->gameType != ReversiSFML::GameType::JOIN)
			{
				// host or local
				if (mInputString.length() > 0)
				{
					std::copy(mInputString.begin(), mInputString.end(), mpApp->localPlayer.userData.name);

					util::saveFile(gc::PATH_LOCAL_USER, mpApp->localPlayer.userData);
				}
			}
			else
			{
				// client
				std::copy(mInputString.begin(), mInputString.end(), clientSend.msg);

				mpApp->threadPool.PushInputQueue(clientSend);
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


}

void ProfileState::Reset()
{
	// updates local data display
	UserData ud = mpApp->localPlayer.userData;

	std::ostringstream oss;

	oss << ud.name
		<< "\n White Wins: " << ud.whiteWin
		<< "\n Black Wins: " << ud.blackWin
		<< "\n Draws: " << ud.draw
		<< "\n Played: " << ud.gamesPlayed;


	mText.at(TEXT_USER_OUT).setString(oss.str().c_str());

}

