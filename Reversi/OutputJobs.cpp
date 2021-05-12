#include "OutputJobs.h"
#include "ReversiSFML.h"
#include <WS2tcpip.h>
#include <string>
#include <iostream>
#include <cassert>

// called in host to update main game
void ClientSendData::operator()(ReversiSFML* d)
{
	if (!dummy)
	{
		UpdateHostChat(d);
		UpdateHostPlayers(d);
		UpdateHostMoves(d);
		//todo stretch UpdateHostMousePos(d)... 
		ToConsole();
	}
}

void ClientSendData::ToConsole()
{
	if (!dummy)
	{
	std::cout <<'\t'
		<< dummy << '\t'
		<< msg << '\t'
		<< mouse[0] << '\t'
		<< mouse[1] << '\t'
		<< player.userData.id << '\t'
		<< move << '\t'
		;
	}
	else
	{
		std::cout << "\t\t\t\t\t\t\t";
	}
}

void ClientSendData::UpdateHostChat(ReversiSFML* d)
{
	if (msg[0] != '\0')
	{
		d->chat.AddMessage(msg);
	}
}

void ClientSendData::UpdateHostPlayers(ReversiSFML* d)
{
	if (player.type == Player::Type::HUMAN)
	{
		// player not present
		if (!d->playerSelection.PlayerPresent(player))
		{
			if (!d->playerSelection.AddPlayer(player))
			{
				d->chat.AddMessage("Can't add Plyr");
				std::cout << "\ncan't add player";
			}
		}
		else
		{
			std::cout << "\nplayer already present";
		}
	}
	else
	{
		std::cout << "\ncan't add non human player";
	}
}

void ClientSendData::UpdateHostMoves(ReversiSFML* d)
{
	if (move >= 0 && move < 64)
	{
		if (d->reversiGame.CanMove())
		{
			d->reversiGame.Move(move);
			//todo further operations required?
		}
	}
}
// updates client application 
void ServerSendData::operator()(ReversiSFML* pd)
{
	pd->playerSelection = mPlayerManagerSelect;
	pd->reversiGame = mBoard;
	// chat update
	const auto& localRecentMsg = pd->chat.GetRecentChatEntry();
	// validate string length matches
	assert(mChatLog.back().size() == localRecentMsg.size());
	// if last elements dont match then update client chat
	if (localRecentMsg != mChatLog.back())
	{
		pd->chat.SetChatLog(mChatLog);
		pd->chat.UpdateLog();
	}

	//std::cout << "\nServerSendData::operator()(ReversiSFML* pd)";
}

ServerSendData::ServerSendData(const char buffer[], size_t size)
{
	assert(size == sizeof(*this));

	memcpy_s(this, sizeof(ServerSendData), buffer, size);
}