#include "ReversiSFML.h"
#include "Constants.h"
#include "Utility.h"

ReversiSFML::ReversiSFML()
	:
	threadPool(this),
	stateManager(this),
	masterVolume(gc::VOL_TOGGLE),
	chat(this)
{
}

void ReversiSFML::Init()
{
	window.create(sf::VideoMode(gc::VIEWPORT_WIDTH_U, gc::VIEWPORT_HEIGHT_U), gc::AppTitle, sf::Style::Close);
	resources.Load();
	music.Init();
	music.SetMasterVolume(masterVolume * gc::HALF);
	music.PlayNext();
	reversiGame.Initialize();
	stateManager.Init();

	ResetPlayers();
	

	InitText(debugLog);
	debugLog.setString("Debug");

	chat.Init();

	chat.display = false;

	

}

void ReversiSFML::Run()
{
	sf::ContextSettings sc;
	sf::Clock clock;
	sf::Time deltaTime;
	float dt = 0.0f;

	while (window.isOpen())
	{
		sf::Event sfEvent;
		deltaTime = clock.restart();

		dt = deltaTime.asSeconds();

		stateManager.Update(dt);
		music.Update(dt);

		//todo remove
		const char msg[2][15] = { "Moo...\0\0\0\0\0","Woof!\0\0\0\0\0" };

		// sf events
		while (window.pollEvent(sfEvent))
		{
			switch (sfEvent.type)
			{
			case sf::Event::Closed: 
				window.close(); 
				break;
			case sf::Event::MouseButtonReleased:
				stateManager.MouseInput((sf::Vector2f)sf::Mouse::getPosition(window));
				
				break;
			case sf::Event::KeyReleased:
				stateManager.KeyInput(sfEvent.key.code);
				break;
			case sf::Event::TextEntered:
				// visable chat overrides state 
				if (chat.display)
				{
					chat.TextEntered(sfEvent.text.unicode);
				}
				else
				{
					stateManager.TextEntered(sfEvent.text.unicode);
				}

				/*if (sfEvent.text.unicode < 128 && sfEvent.text.unicode >= 32)
				{
					stateManager.TextEntered(sfEvent.text.unicode);
				}*/
				break;
			default:
				break;
			}
		}

		//todo remove network delay?
		networkDelay.Update(dt);
		if (networkDelay.HasElapsed())
		{
			networkDelay.Restart(0.5f);


			


			switch (gameType)
			{
			case ReversiSFML::GameType::SINGLE:
				break;
			case ReversiSFML::GameType::HOST:
				UpdateHost();
				break;
			case ReversiSFML::GameType::JOIN:
				SendDummyClientData();
				UpdateClient();
				break;
			default:
				break;
			};
		}
		
		//render
		Render(dt);
	}
}

void ReversiSFML::Render(float dt)
{
	window.clear(sf::Color::Black);

	stateManager.Render(dt);

	chat.Render();

	window.draw(debugLog);

	window.display();
}

void ReversiSFML::InitText(sf::Text& t)
{
	t.setFont(resources.GetFontAt(Resources::FONT_MAIN));
	t.setCharacterSize(gc::CHAR_SIZE_REG);
	t.setFillColor(sf::Color::Magenta);
}

void ReversiSFML::UpdateClient()
{

	// recv data
	ServerSendData updateClient = threadPool.GetServerData();

	updateClient(this);

	// get messages from client socket thread
	{
		OutputJob j;
		while (threadPool.PopOutputQueue(j)) // if
		{
			j(this);
		}
	}
	
}

void ReversiSFML::UpdateHost()
{
	// recv
	OutputJob j;
	while(threadPool.PopOutputQueue(j)) // if
	{
		j(this);
	}

	// send

	ServerSendData sendData;
	sendData.mBoard = reversiGame;
	sendData.mPlayerManagerSelect = playerSelection;
	sendData.mChatLog = chat.GetChatMessages();
	threadPool.UpdateServerData(sendData);

	ValidateSockets();

}

void ReversiSFML::SendDummyClientData()
{
	ClientSendData sendData;
	if (sentPlayerDataToHost)
	{
		// send dummy data for host to recv to keep windows updated
		sendData.dummy = true;
		threadPool.PushInputQueue(sendData);
	}
	else
	{
		sendData.dummy = false;
		// send profile
		sendData.player = localPlayer;
		sentPlayerDataToHost = true;
		threadPool.PushInputQueue(sendData);

		std::cout << "\n\n\n\n\n CLIENT SENDS USER DATA \n\n\n\n\n\n";
	}
}

void ReversiSFML::ValidateSockets()
{
	for (size_t i = 0; i < playerSelection.GetPlayerSize(); i++)
	{
		auto& pl = playerSelection.GetPlayerAt(i);

		if (pl.IsType(Player::Type::HUMAN) && pl.userData.id != localPlayer.userData.id)
		{
			if (!threadPool.SocketActive(pl.serverSocket))
			{
				// replace players assigned to a side with cpu player
				playerSelection.ReplacePlayersInSides(i, localAiPlayerIndex);

				// chat message
				std::ostringstream msg;
				msg << pl.userData.name << " left...";
				chat.AddMessage(msg.str().c_str());

				// set player in list as default
				Player p;
				pl = p;

			}
		}
	}

	if (playerSelection.HasGameEnded())
	{
		
	}
	
}

void ReversiSFML::UpdateGameType()
{
	switch (threadPool.socketType)
	{
	case ThreadPool::Type::END:
	case ThreadPool::Type::NONE:
		gameType = GameType::SINGLE;
		break;
	case ThreadPool::Type::CLIENT_SOCKET:
		gameType = GameType::JOIN;
		break;
	case ThreadPool::Type::SERVER_LISTEN:
	case ThreadPool::Type::SERVER_SOCKETS:
		gameType = GameType::HOST;
		break;
	default:
		break;
	}
}

void ReversiSFML::ResetPlayers()
{
	util::loadFile<UserData>(gc::PATH_LOCAL_USER, localPlayer.userData);
	localPlayer.type = Player::Type::HUMAN;

	if (!playerSelection.AddPlayer(localPlayer)) // index 0
	{
		assert(false);
	}

	// ai player
	Player ai(Player::Type::AI);

	ai.userData.name[0] = 'C';
	ai.userData.name[1] = 'P';
	ai.userData.name[2] = 'U';

	if (!playerSelection.AddPlayer(ai))// index 1
	{
		assert(false);
	}
}
