#include "Discs.h"
#include "ReversiSFML.h"

Discs::Discs(ReversiSFML* app)
	:
	mpApp(app)
{
}

void Discs::Init()
{
	for (auto& s : mDiscSprites)
	{
		s.setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_DISC));
		s.setTextureRect(gc::SPRITE_RECT_DISC_ACTIVE);
		s.setOrigin(gc::SPRITE_DISC_ORIGIN);
	}
}

void Discs::Update(float dt)
{
}

void Discs::Render(float dt)
{
	for (auto& s : mDiscSprites)
	{
		mpApp->window.draw(s);
	}
}

void Discs::Set(int x, int y, const DiscState& ds)
{
	DiscState& discState = mDiscStates.at(x, y);

	// update if changed
	if (discState != ds)
	{
		sf::Sprite& discSprite(mDiscSprites.at(x, y));

		// change state
		discState = ds;

		// change sprite properties 
		switch (ds)
		{
		case DiscState::EMPTY:
			discSprite.setTextureRect(gc::SPRITE_RECT_DISC_EMPTY);
			discSprite.setColor(sf::Color::White);
				break;
		case DiscState::WHITE:
			discSprite.setTextureRect(gc::SPRITE_RECT_DISC_ACTIVE);
			discSprite.setColor(sf::Color::White);
				break;
		case DiscState::BLACK:
			discSprite.setTextureRect(gc::SPRITE_RECT_DISC_ACTIVE);
			discSprite.setColor(sf::Color::Black);
				break;
		case DiscState::SHADOW:
			discSprite.setTextureRect(gc::SPRITE_RECT_DISC_SHADOW);
			discSprite.setColor(sf::Color::White);
				break;
		default:
			break;
		}
	}

}
