#include "Resources.h"
#include <algorithm>
#include "Constants.h"
#include <assert.h>

void Resources::LoadTextures()
{
	// define texture data for iteration
	const TextureData textureData[5]
	{
		{"Data/Textures/patternBackground.png",true,true },
		{"Data/Textures/board.png",true,true },
		{"Data/Textures/disc.png",true,true },
		{"Data/Textures/logoStretch.png",true,true },
		{"Data/Textures/logo.png",false,true }
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
	const FontData fontData[1]
	{
		{"Data/Fonts/Montserrat-Regular.ttf"}
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

void Resources::Load()
{
	LoadTextures();
	LoadFonts();
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
