#include "Resources.h"
#include <algorithm>
#include "Constants.h"
#include <assert.h>

void Resources::LoadTextures()
{
	// define texture data for iteration
	// mTextures.size() must equal textureData size
	const TextureData textureData[17]
	{
		{"Data/Textures/patternBackground.png",true,true },
		{"Data/Textures/board.png",true,true },
		{"Data/Textures/disc.png",true,true },
		{"Data/Textures/logoStretch.png",true,true },
		{"Data/Textures/logo.png",false,true },
		{"Data/Textures/helpIcon.png",false,true },
		{"Data/Textures/help1.png",false,true },

		{"Data/Textures/iconPlayerPlus.png",false,true },
		{"Data/Textures/iconPlayerMinus.png",false,true },
		{"Data/Textures/iconCpuPlus.png",false,true },
		{"Data/Textures/iconCpuMinus.png",false,true },

		{"Data/Textures/iconPlay.png",false,true },
		{"Data/Textures/iconMenu.png",false,true },
		{"Data/Textures/iconAudio.png",false,true },
		{"Data/Textures/selection.png",false,true },
		{"Data/Textures/iconEndGame.png",false,true },
		{"Data/Textures/iconStats.png",false,true }



	};

	const size_t size = sizeof(textureData) / sizeof(textureData[0]);

	// validate paths match texture
	assert(mTextures.size() == size);

	for (size_t i = 0; i < size; ++i)
	{
		mTextures.at(i).loadFromFile(textureData[i].filename);
		mTextures.at(i).setSmooth(textureData[i].isSmooth);
		mTextures.at(i).setRepeated(textureData[i].isRepeated);
	}
}

void Resources::LoadFonts()
{
	// define list of font data for iteration
	const FontData fontData[2]
	{
		{"Data/Fonts/Montserrat-Regular.ttf"},
		{"Data/Fonts/Scaremate.otf"}
	};

	const size_t size = sizeof(fontData) / sizeof(fontData[0]); 

	// validate paths match texture
	assert(mFonts.size() == size);

	// load fonts
	for (size_t i = 0; i < size; ++i)
	{
		mFonts.at(i).loadFromFile(fontData[i].filename);
	}

}

void Resources::LoadSounds()
{
	const char soundPaths[5][128]
	{
		{"Data/Audio/Sfx/267931__anagar__knock-the-door.ogg"},
		{"Data/Audio/Sfx/331339__maurolupo__2-notes-octave-guitar.ogg"},
		{"Data/Audio/Sfx/511484__mattleschuck__success-bell.ogg"},
		{"Data/Audio/Sfx/537084__khenshom__guitar-string-snap-or-breaks-various-sounds.ogg"},
		{"Data/Audio/Sfx/178433__thavis360__writing-with-chalk-chalkboard.ogg"}
	};


	const size_t size = sizeof(soundPaths) / sizeof(soundPaths[0]);

	assert(mSounds.size() == size);

	for (size_t i = 0; i < size; i++)
	{
		if (mSounds.at(i).buffer.loadFromFile(soundPaths[i]))
		{
			mSounds.at(i).sound.setBuffer(mSounds.at(i).buffer);
		}
		else
		{
			assert(false);
		}
	}

}

void Resources::Load()
{
	LoadTextures();
	LoadFonts();
	LoadSounds();
}

const sf::Texture& Resources::GetTextureAt(size_t p) const
{
	return mTextures.at(p);
}

const sf::Font& Resources::GetFontAt(size_t p) const
{
	return mFonts.at(p);
}

const sf::Vector2f Resources::GetTextureCenter(size_t p) const
{
	return(sf::Vector2f)(mTextures.at(p).getSize()) * gc::HALF;
}


void Resources::Play(size_t p, float vol)
{
	std::uniform_real_distribution<float> distribution(0.8f, 1.2f);
	const float pitch = distribution(generator);
	mSounds.at(p).sound.play();
	mSounds.at(p).sound.setVolume(vol);
	mSounds.at(p).sound.setPitch(pitch);
}

