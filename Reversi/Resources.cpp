#include "Resources.h"
#include <algorithm>
#include "Constants.h"
#include <assert.h>

void Resources::LoadTextures()
{
	// define texture data for iteration
	const TextureData textureData[3]
	{
		{"Data/Textures/patternBackground.png",true,true },
		{"Data/Textures/board.png",true,true },
		{"Data/Textures/disc.png",true,true }
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

sf::Texture& Resources::GetTextureAt(size_t p)
{
	return mTextures.at(p);
}

sf::Font& Resources::GetFontAt(size_t p)
{
	return mFonts.at(p);
}