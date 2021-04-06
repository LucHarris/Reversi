#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <array>
#include <random>

struct SoundResource
{
	sf::SoundBuffer buffer;
	sf::Sound sound;
};

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


	std::array<sf::Texture, 14> mTextures;
	std::array<sf::Font, 1> mFonts;
	std::array<SoundResource, 4> mSounds;
	std::default_random_engine generator;


	void LoadTextures();
	void LoadFonts();
	void LoadSounds();
public:
	void Load();

	const sf::Texture& GetTextureAt(size_t p) const;
	const sf::Font& GetFontAt(size_t p) const;
	const sf::Vector2f GetTextureCenter(size_t p) const;
	void Play(size_t p, float vol);
	enum {
		FONT_MAIN = 0,
		TEXTURE_BG = 0,
		TEXTURE_BOARD,
		TEXTURE_DISC,
		TEXTURE_STRETCH,
		TEXTURE_LOGO,
		TEXTURE_HELP_ICON,
		TEXTURE_HELP_1,

		TEXTURE_ICON_PLAYER_PLUS,
		TEXTURE_ICON_REMOVE_ONE,
		TEXTURE_ICON_CPU_PLUS,
		TEXTURE_ICON_REMOVE_ALL,
		TEXTURE_ICON_PLAY,
		TEXTURE_ICON_MENU,
		TEXTURE_ICON_AUDIO,


		SOUND_PLACE = 0,
		SOUND_CLICK ,
		SOUND_WIN,
		SOUND_ERROR
	};
};

