#include "ToggleButton.h"
#include "ReversiSFML.h"

ToggleButton::ToggleButton(ReversiSFML* app)
	:
	mpApp(app)
{

}

void ToggleButton::Render(float dt)
{
	mpApp->window.draw(icon);
	if (mDisplay)
	{
		mpApp->window.draw(show);
	}
}

void ToggleButton::MouseInput(const sf::Vector2f& pos)
{
	if (icon.getGlobalBounds().contains(pos))
	{
		mDisplay = !mDisplay;
	}
	else
	{
		mDisplay = false;
	}

	// distinguish selected button
	if (mDisplay)
	{
		icon.setColor(sf::Color::Black);
	}
	else
	{
		icon.setColor(sf::Color::White);
	}

}