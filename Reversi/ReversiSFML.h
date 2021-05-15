#pragma once

#include <random>
#include "Resources.h"
#include "ThreadPool.h"
#include "StateManager.h"
#include "Board.h"
#include "MusicEngine.h"
#include "PlayerManager.h"
#include "User.h"
#include "DeltaTimer.h"
#include "Chat.h"

class ReversiSFML
{

public:
	ReversiSFML();
	ReversiSFML(const ReversiSFML& r) = delete;
	void Init();
	void Run();
	void Render(float);

	// sets up text elements with common state
	void InitText(sf::Text& t);
	
	ThreadPool threadPool;
	StateManager stateManager;
	sf::RenderWindow window;
	Resources resources;
	sf::Text debugLog;
	ReversiBoard reversiGame;
	MusicEngine music;
	// updates game player manager
	PlayerManager playerSelection;
	Player localPlayer;
	const int localAiPlayerIndex = 1;
	int clientPlayerIndex = -1;
	Chat chat;
	// local for comparasion
	// sf volume range from 0.0f - 100.0f
	float masterVolume;
	bool sentPlayerDataToHost = false;
	

	enum class GameType{SINGLE,HOST,JOIN};
	GameType gameType = GameType::SINGLE;

	void UpdateGameType();
	void ResetPlayers();
private:
	DeltaCountdownTimer networkDelay;
	void UpdateClient();
	void UpdateHost();
	// send dummy data if client profile hasn't been sent
	void SendDummyClientData();
	// host
	void ValidateSockets();
};

