#include "Discs.h"
#include "ReversiSFML.h"

Discs::Discs(ReversiSFML* app)
	:
	mpApp(app)
{
}

void Discs::Init()
{
	// iteraters for discs
	int x = 0, y = 0;

	// for setting disc positions
	sf::Vector2f pos = gc::VIEWPORT_CENTER;

	// offset from centre of viewport
	const float start = gc::BOARD_DISC_ROW_SIZE * gc::HALF * 48.0f;

	// position of top left disc
	pos -= sf::Vector2f(start,start);

	// position each disc row by row
	while (y < gc::BOARD_DISC_ROW_SIZE)
	{
		while (x < gc::BOARD_DISC_ROW_SIZE)
		{
			// current sprite
			//sf::Sprite& s(mDiscSprites.at(x, y));
			Discs::DiscSprite& s(mSprites.at(x, y));

			// set properties
			s.sprite.setPosition(pos);
			s.sprite.setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_DISC));
			s.sprite.setTextureRect(gc::SPRITE_RECT_DISC_ACTIVE);

			// increment x
			pos.x += 48.0f;
			++x;
		}
		// reset x;
		x = 0;
		pos.x = gc::VIEWPORT_CENTER.x - start;

		// increment y
		pos.y += 48.0f;
		++y;
	}
}

void Discs::Update(float dt)
{
}

void Discs::Render(float dt)
{
	for (auto& s : mSprites)
	{
		mpApp->window.draw(s.sprite);
	}
}

void Discs::Set(int x, int y, const DiscState& ds)
{
	DiscSprite& discState = mSprites.at(x, y);

	// update if changed
	if (discState.state != ds)
	{
		//sf::Sprite& discSprite(mSprites.at(x, y));

		// change state
		discState.state = ds;

		// change sprite properties 
		switch (ds)
		{
		case DiscState::EMPTY:
			discState.sprite.setTextureRect(gc::SPRITE_RECT_DISC_EMPTY);
			discState.sprite.setColor(sf::Color::White);
				break;
		case DiscState::WHITE:
			discState.sprite.setTextureRect(gc::SPRITE_RECT_DISC_ACTIVE);
			discState.sprite.setColor(sf::Color::White);
				break;
		case DiscState::BLACK:
			discState.sprite.setTextureRect(gc::SPRITE_RECT_DISC_ACTIVE);
			discState.sprite.setColor(sf::Color::Black);
				break;
		case DiscState::SHADOW:
			discState.sprite.setTextureRect(gc::SPRITE_RECT_DISC_SHADOW);
			discState.sprite.setColor(sf::Color::White);
				break;
		default:
			break;
		}
	}

}

void Discs::MouseInput(const sf::Vector2f& pos)
{

	auto it = std::find_if(mSprites.begin(), mSprites.end(), [&pos](DiscSprite& d) 
		{
			return 
				(d.state == DiscState::EMPTY || d.state == DiscState::SHADOW) && 
				d.sprite.getGlobalBounds().contains(pos);
		});
}

