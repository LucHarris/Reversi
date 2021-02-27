#include "Button.h"
#include "ReversiSFML.h"
#include "Constants.h"

void MenuButton::Init( const std::string& prompt, float relY)
{
	
	mSprite.setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_STRETCH));
	mSprite.setTextureRect({ 0,0,gc::ViewportWidth,128 });
	mSprite.setOrigin({ gc::ViewportWidthF * 0.5f, 64.0f });
	mSprite.setPosition(gc::ViewportWidthF * 0.5f, gc::ViewportHeightF * relY);
	mSprite.setScale({ 1.0f,0.5f });


	mText.setFont(mpApp->resources.GetFontAt(Resources::FONT_MAIN));
	mText.setCharacterSize(gc::CharacterSizeStandard);
	mText.setFillColor(sf::Color::Black);
	mText.setString(prompt);
	mText.setOrigin({-100.0f,14.0f});
	mText.setPosition(mSprite.getPosition());
}

bool MenuButton::Contains(const sf::Vector2i& pos)
{
	sf::FloatRect fr = mSprite.getGlobalBounds();
	return fr.contains((sf::Vector2f)pos);
}

MenuButton::MenuButton(ReversiSFML* app)
	:
	mpApp(app)
{
}

void MenuButton::Render(float dt)
{
	mpApp->window.draw(mSprite);
	mpApp->window.draw(mText);
	
}
