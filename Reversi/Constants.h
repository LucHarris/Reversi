#pragma once


#include <array>
#include <string>
#include <SFML/Graphics.hpp>

namespace gc
{
	const std::string AppTitle = "Reversi";


	const unsigned int
		VIEWPORT_WIDTH_U = 800,
		VIEWPORT_HEIGHT_U = 600,

		CHAR_SIZE_REG = 24,

		STATE_INDEX_MAIN_MENU = 0,
		STATE_INDEX_GAME_SAMPLE = 1,

		BOARD_DISC_ROW_SIZE = 8
		;
	const float
		VIEWPORT_WIDTH_F = 800.0f,
		VIEWPORT_HEIGHT_F = 600.0f,
		HALF = 0.5f,
		BUTTON_SPACING = 0.15f;
		;
	const sf::Vector2f
		VIEWPORT_CENTER{VIEWPORT_WIDTH_F * HALF, VIEWPORT_HEIGHT_F * HALF},

		SPRITE_DISC_ORIGIN{24.0f,24.0f}
	;

	const sf::IntRect
		SPRITE_RECT_DISC_EMPTY{ 0, 0, 48, 48 },
		SPRITE_RECT_DISC_ACTIVE{ 48, 144,48, 48 },
		SPRITE_RECT_DISC_SHADOW{ 144, 384 ,48, 48 }
	;


}