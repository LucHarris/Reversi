#include "Discs.h"
#include "ReversiSFML.h"


Discs::Discs(ReversiSFML* app)
	:
	mpApp(app)
{
}

void Discs::Init()
{
	mCursor.setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_DISC));
	mCursor.setTextureRect(gc::SPRITE_RECT_DISC_ACTIVE);


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

			s.sprite.setTextureRect(gc::SPRITE_RECT_DISC_EMPTY);

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


	UpdateDiscs();
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

	mpApp->window.draw(mCursor);
}


// set properites of a disc: white, black, empty or shadow (potential move)
void Discs::Set(int a, const DiscState& ds)
{
	DiscSprite& discState = mSprites.at(a);

	// update if changed
	if (discState.state != ds)
	{
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
			discState.sprite.setColor(gc::COL_DISC_DARK);
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

void Discs::MoveByMousePos(const sf::Vector2f& pos)
{
	assert(false);
	// grid clicked
	auto end = std::find_if(mSprites.begin(), mSprites.end(), [&pos](DiscSprite& d) 
		{
			return	d.sprite.getGlobalBounds().contains(pos);
		});
	
	if (mpApp->reversiGame.CanMove())
	{
		// valid move
		if (end != mSprites.end())
		{

			const int move = std::distance(mSprites.begin(), end);

			if (mpApp->reversiGame.Move(move))
			{
				mpApp->reversiGame.UpdateBoardBackup();
				mpApp->reversiGame.ToConsole();
				UpdateDiscs();

				if (mpApp->reversiGame.GetPlayerIndex() == 0)
				{
					mCursor.setColor(sf::Color::White);
				}
				else
				{
					mCursor.setColor(sf::Color::Black);
				}

				// check for winner
				if (mpApp->reversiGame.CanMove())
				{
					mpApp->resources.Play(Resources::SOUND_PLACE, mpApp->masterVolume);
				}
				else
				{
					mpApp->resources.Play(Resources::SOUND_WIN, mpApp->masterVolume);
				}
			}
			else
			{
				mpApp->resources.Play(Resources::SOUND_ERROR, mpApp->masterVolume);

			}
			
		}

	}
	else
	{
		auto scores = mpApp->reversiGame.GetPlayerScores();

		mpApp->resources.Play(Resources::SOUND_WIN, mpApp->masterVolume);


		mpApp->debugLog.setString(
			"Game ended\nWhite: " + 
			std::to_string(scores.first) +
			"\nBlack: "+
			std::to_string(scores.second) + "\n"
		);
	}
}

int Discs::MoveByMoooouse(const sf::Vector2f& pos)
{
	// invalid until proven otherwise
	int move = -1;

	// find tile clicked based on mouse position
	auto end = std::find_if(mSprites.begin(), mSprites.end(), [&pos](DiscSprite& d)
		{
			return	d.sprite.getGlobalBounds().contains(pos);
		});

	// valid move
	if (end != mSprites.end())
	{
		move = std::distance(mSprites.begin(), end);
	}

	return move;
}

void Discs::UpdateDiscs()
{

	auto& scoreGrid = mpApp->reversiGame.GetScoreGrid();
	auto& discGrid = mpApp->reversiGame.GetDiscGrid();

	for (size_t i = 0; i < mSprites.size(); i++)
	{
		switch (discGrid.at(i))
		{
		case CELL_WHITE:
			Set(i,DiscState::WHITE);
			break;
		case CELL_BLACK:
			Set(i,DiscState::BLACK);
			break;
		default:
			if(scoreGrid.at(i) >= MIN_SCORE)
			{
				Set(i,DiscState::SHADOW);
			}
			else
			{
				Set(i,DiscState::EMPTY);
			}
			break;
		}
	}
}

bool Discs::Move(int move)
{
	if (mpApp->reversiGame.Move(move))
	{
		mpApp->reversiGame.UpdateBoardBackup();
		mpApp->reversiGame.ToConsole();
		UpdateDiscs();

		if (mpApp->reversiGame.GetPlayerIndex() == 0)
		{
			mCursor.setColor(sf::Color::White);
		}
		else
		{
			mCursor.setColor(sf::Color::Black);
		}

		// check for winner
		if (mpApp->reversiGame.CanMove())
		{
			mpApp->resources.Play(Resources::SOUND_PLACE, mpApp->masterVolume);
		}
		else
		{
			mpApp->resources.Play(Resources::SOUND_WIN, mpApp->masterVolume);
		}

		return true;

	}
	else
	{
		mpApp->resources.Play(Resources::SOUND_ERROR, mpApp->masterVolume);

		return false;
	}
}

