#include "ProfileState.h"
#include "ReversiSFML.h"
#include <sstream>

ProfileState::ProfileState(ReversiSFML* app)
	:
	State(app)
{
}

void ProfileState::Init()
{
	mUserText.setFont(mpApp->resources.GetFontAt(Resources::FONT_MAIN));
	mUserText.setFillColor({ 0,0,0,200 });

}

void ProfileState::Update(float dt)
{
}

void ProfileState::Render(float dt)
{
	mpApp->window.draw(mUserText);
}

void ProfileState::MouseInput(const sf::Vector2f& pos)
{
}

void ProfileState::KeyInput(sf::Keyboard::Key key)
{
}

void ProfileState::TextEntered(unsigned int key)
{
}

void ProfileState::Reset()
{
	// updates local data display
	UserData ud = mpApp->localUser;

	std::ostringstream oss;

	oss << ud.name
		<< "\n White Wins: " << ud.whiteWin
		<< "\n Black Wins: " << ud.blackWin
		<< "\n Draws: " << ud.draw
		<< "\n Played: " << ud.gamesPlayed;

	mUserText.setString(oss.str().c_str());

}

