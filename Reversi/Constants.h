#pragma once


#include <array>
#include <string>
#include <SFML/Graphics.hpp>


namespace gc
{
	const std::string AppTitle = "Reversi";


	const unsigned int
		VIEWPORT_WIDTH_U = 1280,
		VIEWPORT_HEIGHT_U = 720,

		CHAR_SIZE_REG = 24,

		STATE_INDEX_MAIN_MENU = 0,
		STATE_INDEX_GAME_SAMPLE = 1,

		BOARD_DISC_ROW_SIZE = 8,

		CHAT_SIZE = 6
		;
	const float
		VIEWPORT_WIDTH_F = 1280.0f, // 800.0f
		VIEWPORT_HEIGHT_F = 720.0f, // 600.0f
		HALF = 0.5f,
		BUTTON_SPACING = 0.15f;
		;
	const sf::Vector2f
		VIEWPORT_CENTER{VIEWPORT_WIDTH_F * HALF, VIEWPORT_HEIGHT_F * HALF},

		SPRITE_DISC_ORIGIN{24.0f,24.0f}
	;


	const sf::Vector2f VIEWPORT_PIVOT[9]{
		{0,0},
		{VIEWPORT_CENTER.x,0},
		{VIEWPORT_WIDTH_F,0},
		{0,VIEWPORT_CENTER.y},
		{VIEWPORT_CENTER.x,VIEWPORT_CENTER.y},
		{VIEWPORT_WIDTH_F,VIEWPORT_CENTER.y},
		{0,VIEWPORT_HEIGHT_F},
		{VIEWPORT_CENTER.x,VIEWPORT_HEIGHT_F},
		{VIEWPORT_WIDTH_F,VIEWPORT_HEIGHT_F},
	};

	// T - Top, C - Center, B - Bottom, L - Left, R - Right
	enum {
		PIVOT_TL,		PIVOT_TC,		PIVOT_TR,		
		PIVOT_CL,		PIVOT_CC,		PIVOT_CR,
		PIVOT_BL,		PIVOT_BC,		PIVOT_BR,
	};

	const sf::IntRect
		SPRITE_RECT_DISC_EMPTY{ 0, 0, 48, 48 },
		SPRITE_RECT_DISC_ACTIVE{ 48, 144,48, 48 },
		SPRITE_RECT_DISC_SHADOW{ 144, 384 ,48, 48 }
	;


}