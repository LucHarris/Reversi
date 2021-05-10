#include "SelectionState.h"
#include "ReversiSFML.h"
#include "Constants.h"

#include "Listener.h"
#include "client.h"

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
		d.setFont(mpApp->resources.GetFontAt(Resources::FONT_CHALK));
	}

	mPlayerDisplay.at(0).setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_CC] + sf::Vector2f{-190.0f,-190.0f});
	mPlayerDisplay.at(1).setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_CC] + sf::Vector2f{  20.0f,-190.0f});

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

	mButtons.at(BTN_WHITE_PLAYER_PLUS).setPosition(	gc::VIEWPORT_PIVOT[gc::PIVOT_TC] + sf::Vector2f{ -384.0f,128.0f });
	mButtons.at(BTN_WHITE_CPU_PLUS).setPosition(	gc::VIEWPORT_PIVOT[gc::PIVOT_TC] + sf::Vector2f{ -384.0f,192.0f });//192.0f 
	mButtons.at(BTN_WHITE_REMOVE_ONE).setPosition(	gc::VIEWPORT_PIVOT[gc::PIVOT_TC] + sf::Vector2f{ -384.0f,256.0f });//256.0f 
	mButtons.at(BTN_WHITE_REMOVE_ALL).setPosition(	gc::VIEWPORT_PIVOT[gc::PIVOT_TC] + sf::Vector2f{ -384.0f,320.0f });

	mButtons.at(BTN_BLACK_PLAYER_PLUS).setPosition(	gc::VIEWPORT_PIVOT[gc::PIVOT_TC] + sf::Vector2f{  384.0f,128.0f });
	mButtons.at(BTN_BLACK_CPU_PLUS).setPosition(	gc::VIEWPORT_PIVOT[gc::PIVOT_TC] + sf::Vector2f{ 384.0f,192.0f });
	mButtons.at(BTN_BLACK_REMOVE_ONE).setPosition(	gc::VIEWPORT_PIVOT[gc::PIVOT_TC] + sf::Vector2f{  384.0f,256.0f });
	mButtons.at(BTN_BLACK_REMOVE_ALL).setPosition(	gc::VIEWPORT_PIVOT[gc::PIVOT_TC] + sf::Vector2f{  384.0f,320.0f });

	mButtons.at(BTN_PLAY).setPosition(gc::VIEWPORT_PIVOT[gc::PIVOT_BC] + sf::Vector2f{ 0.0f,-128.0f });
	
	mButtons.at(BTN_BLACK_PLAYER_PLUS).setColor(gc::COL_ICON_DARK);
	mButtons.at(BTN_BLACK_CPU_PLUS).setColor(gc::COL_ICON_DARK);
	mButtons.at(BTN_BLACK_REMOVE_ONE).setColor(gc::COL_ICON_DARK);
	mButtons.at(BTN_BLACK_REMOVE_ALL).setColor(gc::COL_ICON_DARK);

	// set up blackboard
	const sf::Texture& tex = mpApp->resources.GetTextureAt(Resources::TEXTURE_BLACKBOARD);
	mBlackboard.setTexture(tex);
	mBlackboard.setOrigin((sf::Vector2f)tex.getSize() * gc::HALF);
	mBlackboard.setPosition(gc::VIEWPORT_CENTER);

}

void SelectionState::Update(float)
{

	UpdatePlayerList();


}

void SelectionState::Render(float)
{
	mpApp->window.draw(mBlackboard);

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
	if (mButtons.at(BTN_WHITE_PLAYER_PLUS).getGlobalBounds().contains(mos))
	{
		if (mpApp->PlayerSelection.AddPlayer(Player::Type::HUMAN, 0))
		{
			mpApp->resources.Play(Resources::SOUND_CHALK, mpApp->masterVolume);
		}
		else
		{
			mpApp->resources.Play(Resources::SOUND_ERROR, mpApp->masterVolume);
		}

	}

	if (mButtons.at(BTN_WHITE_REMOVE_ONE).getGlobalBounds().contains(mos))
	{
		if (mpApp->PlayerSelection.RemoveLast(0))
		{
			mpApp->resources.Play(Resources::SOUND_CLICK, mpApp->masterVolume);
		}
		else
		{
			mpApp->resources.Play(Resources::SOUND_ERROR, mpApp->masterVolume);
		}
	}

	if (mButtons.at(BTN_WHITE_CPU_PLUS).getGlobalBounds().contains(mos))
	{
		if (mpApp->PlayerSelection.AddPlayer(Player::Type::AI, 0))
		{
			mpApp->resources.Play(Resources::SOUND_CHALK, mpApp->masterVolume);
		}
		else
		{
			mpApp->resources.Play(Resources::SOUND_ERROR, mpApp->masterVolume);
		}

	}

	if (mButtons.at(BTN_WHITE_REMOVE_ALL).getGlobalBounds().contains(mos))
	{
		mpApp->PlayerSelection.ResetSide(0);
		mpApp->resources.Play(Resources::SOUND_CLICK, mpApp->masterVolume);
	}

	if (mButtons.at(BTN_BLACK_PLAYER_PLUS).getGlobalBounds().contains(mos))
	{
		if (mpApp->PlayerSelection.AddPlayer(Player::Type::HUMAN, 1))
		{
			mpApp->resources.Play(Resources::SOUND_CHALK, mpApp->masterVolume);
		}
		else
		{
			mpApp->resources.Play(Resources::SOUND_ERROR, mpApp->masterVolume);
		}

	}

	if (mButtons.at(BTN_BLACK_REMOVE_ONE).getGlobalBounds().contains(mos))
	{
		if(mpApp->PlayerSelection.RemoveLast(1))
		{
			mpApp->resources.Play(Resources::SOUND_CLICK, mpApp->masterVolume);
		}
		else
		{
			mpApp->resources.Play(Resources::SOUND_ERROR, mpApp->masterVolume);
		}
	}

	if (mButtons.at(BTN_BLACK_CPU_PLUS).getGlobalBounds().contains(mos))
	{
		if (mpApp->PlayerSelection.AddPlayer(Player::Type::AI, 1))
		{
			mpApp->resources.Play(Resources::SOUND_CHALK, mpApp->masterVolume);
		}
		else
		{
			mpApp->resources.Play(Resources::SOUND_ERROR, mpApp->masterVolume);
		}
	}

	if (mButtons.at(BTN_BLACK_REMOVE_ALL).getGlobalBounds().contains(mos))
	{
		mpApp->PlayerSelection.ResetSide(1);
		mpApp->resources.Play(Resources::SOUND_CLICK, mpApp->masterVolume);
	}

	if (mButtons.at(BTN_PLAY).getGlobalBounds().contains(mos))
	{
		// if the list is invalid ai players will be defaulted to play
		mpApp->PlayerSelection.ValidatePlayers();
		mpApp->stateManager.ChangeState(gc::STATE_INDEX_GAME_SAMPLE, true);
		mpApp->resources.Play(Resources::SOUND_CLICK, mpApp->masterVolume);

	}

	// todo remove dead code ... moved to update for networking
	//UpdatePlayerList();
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


	switch (mpApp->gameType)
	{
	case ReversiSFML::GameType::HOST:
		mpApp->threadPool.PushThreadQueue(ServerListener(&mpApp->threadPool), ThreadPool::Type::SERVER_LISTEN);
		break;
	case ReversiSFML::GameType::JOIN:
		mpApp->threadPool.PushThreadQueue(ClientSocket(&mpApp->threadPool), ThreadPool::Type::CLIENT_SOCKET);
		break;
	default:
		break;
	}
}
