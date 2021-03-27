#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

class ReversiSFML;


class ToggleButton
	{
		ReversiSFML* mpApp;
	public:
		sf::Sprite icon;
		sf::Sprite show;
		bool mDisplay = false;

		ToggleButton(ReversiSFML* app);
		void Render(float dt);
		// when mouse hovers over the icon a sprite will appear
		void MouseInput(const sf::Vector2f& pos);
		
	};
