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
	mText.at(TEXT_USER_OUT).setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_TC] + sf::Vector2f{0.0f,64.0f});

	mText.at(TEXT_INPUT).setFont(mpApp->resources.GetFontAt(Resources::FONT_MAIN));
	mText.at(TEXT_PROMPT).setFont(mpApp->resources.GetFontAt(Resources::FONT_MAIN));
	mText.at(TEXT_PROMPT).setString("Rename Profile: (Enter to apply) (Restart recommended)");
	mText.at(TEXT_PROMPT).setOrigin({ 0.0f,mText.at(TEXT_PROMPT).getGlobalBounds().height });

	auto pos = gc::VIEWPORT_PIVOT[gc::PIVOT_BL] + sf::Vector2f{ 128.0f,-128 };
	mText.at(TEXT_PROMPT).setPosition(pos);
	mText.at(TEXT_INPUT).setPosition(pos);
	mText.at(TEXT_INPUT).setFillColor(sf::Color::Green);

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
					// erase old name
					std::fill(mpApp->localPlayer.userData.name, mpApp->localPlayer.userData.name + UserData::GetMaxNameSize(), '\0');
					// assign new name
					std::copy(mInputString.begin(), mInputString.begin() + (mInputString.length() ), mpApp->localPlayer.userData.name);
					// update file
					util::saveFile(gc::PATH_LOCAL_USER[mpApp->profileIndex], mpApp->localPlayer.userData);
					// update Player list
					mpApp->playerSelection.PlayerUpdatesPlayerList(mpApp->localPlayer);
					// return to main menu
					mpApp->stateManager.ChangeState(gc::STATE_INDEX_MAIN_MENU, true);
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
	mInputString = mpApp->localPlayer.userData.name;
	mText.at(TEXT_INPUT).setString(mInputString);
	std::ostringstream oss;

	const auto& playerList = mpApp->playerSelection.GetHumanPlayers();

	// field names
	oss << "Player Profiles and Leaderboard\n\n#\tTW\tWW\tBW\tD\tPLY\tName\tid\n";

	int position = 0;

	for (auto& player : playerList)
	{
		// row entry
		oss
			<< ++position << "\t\t"
			<< player.userData.totalWins << "\t\t"
			<< player.userData.whiteWin << "\t\t"
			<< player.userData.blackWin <<		"\t\t"
			<< player.userData.draw <<			"\t\t"
			<< player.userData.gamesPlayed <<	"\t\t"
			<< player.userData.name << '\t' 
			<< player.userData.id << '\n'
			;
		
	}
	oss << "\n\n# - Position\t\tTW - Total Wins\t\tWW - WhtWin\nBW - BlkWin\t\tD-Draw\t\tPLY - Played";

	mText.at(TEXT_USER_OUT).setString(oss.str().c_str());

	// centre viewport
	auto rect = mText.at(TEXT_USER_OUT).getGlobalBounds();
	mText.at(TEXT_USER_OUT).setOrigin({ gc::HALF * rect.width,0 });

}

