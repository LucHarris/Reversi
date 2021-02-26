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


	std::array<sf::Texture, 3> mTextures;
	std::array<sf::Font, 1> mFonts;
	void LoadTextures();
	void LoadFonts();
public:
	void Load();

	sf::Texture& GetTextureAt(size_t p);
	sf::Font& GetFontAt(size_t p);

};

