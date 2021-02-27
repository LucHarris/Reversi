#pragma once
#include <SFML/Graphics.hpp>
#include <array>
class Resources
{
	struct TextureData
	{
		std::string filename;
		bool isRepeated = true;
		bool isSmooth = true;
	};

	// expandable if necessary
	struct FontData
	{
		std::string filename;
	};


	std::array<sf::Texture, 5> mTextures;
	std::array<sf::Font, 1> mFonts;
	void LoadTextures();
	void LoadFonts();
public:
	void Load();

	const sf::Texture& GetTextureAt(size_t p) const;
	const sf::Font& GetFontAt(size_t p) const;

	enum {
		FONT_MAIN = 0,
		TEXTURE_BG = 0,
		TEXTURE_BOARD,
		TEXTURE_DISC,
		TEXTURE_STRETCH,
		TEXTURE_LOGO
	};
};

