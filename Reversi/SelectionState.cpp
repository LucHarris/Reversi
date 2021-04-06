#include "SelectionState.h"
#include "ReversiSFML.h"
#include "Constants.h"

SelectionState::SelectionState(ReversiSFML* app)
	:
	State(app)
{
}

void SelectionState::Init()
{
	// player list
	for (auto& d : mPlayerDisplay)
	{
		d.setFont(mpApp->resources.GetFontAt(Resources::FONT_MAIN));
	}

	mPlayerDisplay.at(0).setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_TC] + sf::Vector2f{-200.0f,40.0f});
	mPlayerDisplay.at(1).setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_TC] + sf::Vector2f{ 200.0f,40.0f});

	UpdatePlayerList();

	// buttons
	mButtons.at(BTN_WHITE_PLAYER_PLUS).setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_ICON_PLAYER_PLUS));
	mButtons.at(BTN_WHITE_CPU_PLUS).setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_ICON_CPU_PLUS));
	mButtons.at(BTN_WHITE_REMOVE_ONE).setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_ICON_REMOVE_ONE));
	mButtons.at(BTN_WHITE_REMOVE_ALL).setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_ICON_REMOVE_ALL));

	mButtons.at(BTN_BLACK_PLAYER_PLUS).setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_ICON_PLAYER_PLUS));
	mButtons.at(BTN_BLACK_CPU_PLUS).setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_ICON_CPU_PLUS));
	mButtons.at(BTN_BLACK_REMOVE_ONE).setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_ICON_REMOVE_ONE));
	mButtons.at(BTN_BLACK_REMOVE_ALL).setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_ICON_REMOVE_ALL));

	mButtons.at(BTN_PLAY).setTexture(mpApp->resources.GetTextureAt(Resources::TEXTURE_ICON_PLAY));

	mButtons.at(BTN_WHITE_PLAYER_PLUS).setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_TC] +	sf::Vector2f{ -384.0f,128.0f });
	mButtons.at(BTN_WHITE_CPU_PLUS).setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_TC] +		sf::Vector2f{ -384.0f,192.0f });//192.0f 
	mButtons.at(BTN_WHITE_REMOVE_ONE).setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_TC] +	sf::Vector2f{ -384.0f,256.0f });//256.0f 
	mButtons.at(BTN_WHITE_REMOVE_ALL).setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_TC] +		sf::Vector2f{ -384.0f,320.0f });

	mButtons.at(BTN_BLACK_PLAYER_PLUS	).setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_TC] +		sf::Vector2f{  384.0f,128.0f });
	mButtons.at(BTN_BLACK_CPU_PLUS		).setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_TC] +		sf::Vector2f{ 384.0f,192.0f });
	mButtons.at(BTN_BLACK_REMOVE_ONE	).setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_TC] +		sf::Vector2f{  384.0f,256.0f });
	mButtons.at(BTN_BLACK_REMOVE_ALL		).setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_TC] +		sf::Vector2f{  384.0f,320.0f });

	mButtons.at(BTN_PLAY).setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_BC] + sf::Vector2f{ 0.0f,-128.0f });
	
	mButtons.at(BTN_BLACK_PLAYER_PLUS).setColor(gc::COL_ICON_DARK);
	mButtons.at(BTN_BLACK_CPU_PLUS).setColor(gc::COL_ICON_DARK);
	mButtons.at(BTN_BLACK_REMOVE_ONE).setColor(gc::COL_ICON_DARK);
	mButtons.at(BTN_BLACK_REMOVE_ALL).setColor(gc::COL_ICON_DARK);

}

void SelectionState::Update(float)
{
}

void SelectionState::Render(float)
{
	for (auto& d : mPlayerDisplay)
	{
		mpApp->window.draw(d);
	}

	for (auto& btn : mButtons)
	{
		mpApp->window.draw(btn);
	}
}

void SelectionState::MouseInput(const sf::Vector2f& mos)
{
	//todo add logic for clicks

	if (mButtons.at(BTN_WHITE_PLAYER_PLUS).getGlobalBounds().contains(mos))
	{
		mpApp->PlayerSelection.AddPlayer(Player::Type::HUMAN, 0);
	}

	if (mButtons.at(BTN_WHITE_REMOVE_ONE).getGlobalBounds().contains(mos))
	{
		mpApp->PlayerSelection.RemoveLast(0);
	}

	if (mButtons.at(BTN_WHITE_CPU_PLUS).getGlobalBounds().contains(mos))
	{
		mpApp->PlayerSelection.AddPlayer(Player::Type::AI, 0);
	}

	if (mButtons.at(BTN_WHITE_REMOVE_ALL).getGlobalBounds().contains(mos))
	{
		mpApp->PlayerSelection.ResetSide(0);
	}

	if (mButtons.at(BTN_BLACK_PLAYER_PLUS).getGlobalBounds().contains(mos))
	{
		mpApp->PlayerSelection.AddPlayer(Player::Type::HUMAN, 1);
	}

	if (mButtons.at(BTN_BLACK_REMOVE_ONE).getGlobalBounds().contains(mos))
	{
		mpApp->PlayerSelection.RemoveLast(1);
	}

	if (mButtons.at(BTN_BLACK_CPU_PLUS).getGlobalBounds().contains(mos))
	{
		mpApp->PlayerSelection.AddPlayer(Player::Type::AI, 1);
	}

	if (mButtons.at(BTN_BLACK_REMOVE_ALL).getGlobalBounds().contains(mos))
	{
		mpApp->PlayerSelection.ResetSide(1);
	}

	if (mButtons.at(BTN_PLAY).getGlobalBounds().contains(mos))
	{
		// if the list is invalid ai players will be defaulted to play
		mpApp->PlayerSelection.ValidatePlayers();
		mpApp->stateManager.ChangeState(gc::STATE_INDEX_GAME_SAMPLE, true);
	}

	UpdatePlayerList();
}

void SelectionState::KeyInput(sf::Keyboard::Key)
{
}

void SelectionState::TextEntered(unsigned int key)
{
}

void SelectionState::UpdatePlayerList()
{
	for (int i = 0; i < 2; i++)
	{
		mPlayerDisplay.at(i).setString(mpApp->PlayerSelection.GetPlayerList(i));
	}
}

void SelectionState::Reset()
{
}
