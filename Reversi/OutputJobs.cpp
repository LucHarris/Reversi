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
		UpdateHostButtons(d);
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
	// if the player is human a request has been made to add a player to list
	if (player.type == Player::Type::HUMAN)
	{
		// already there?
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
void ClientSendData::UpdateHostButtons(ReversiSFML* d)
{
	switch (op)
	{
	case ClientSendData::ButtonOp::SIDE_ADD_WHITE_PL:

		if (netPlayerIndex > 0)
		{
			d->playerSelection.PlayerToSide(netPlayerIndex, 0);
		}
		break;
	case ClientSendData::ButtonOp::SIDE_ADD_BLACK_PL:
		if (netPlayerIndex > 0)
		{
			d->playerSelection.PlayerToSide(netPlayerIndex, 1);
		}
		break;
	case ClientSendData::ButtonOp::SIDE_ADD_WHITE_AI:
		d->playerSelection.PlayerToSide(d->playerSelection.GetAiPlayerIndex(),0);
		break;
	case ClientSendData::ButtonOp::SIDE_ADD_BLACK_AI:
		d->playerSelection.PlayerToSide(d->playerSelection.GetAiPlayerIndex(),1);
		break;
	case ClientSendData::ButtonOp::SIDE_REMOVE_ONE_WHITE:
		d->playerSelection.RemoveLast(0);
		break;
	case ClientSendData::ButtonOp::SIDE_REMOVE_ONE_BLACK:
		d->playerSelection.RemoveLast(1);
		break;
	case ClientSendData::ButtonOp::SIDE_REMOVE_ALL_WHITE:
		d->playerSelection.ResetSide(0);
		break;
	case ClientSendData::ButtonOp::SIDE_REMOVE_ALL_BLACK:
		d->playerSelection.ResetSide(1);
		break;
	case ClientSendData::ButtonOp::START_GAME:
		d->stateManager.ChangeState(gc::STATE_INDEX_GAME_SAMPLE, true);
		break;
	case ClientSendData::ButtonOp::NO_OP:
		break;
	default:
		break;
	}
}
// updates client application 
void ServerSendData::operator()(ReversiSFML* pd)
{
	pd->playerSelection = mPlayerManagerSelect;
	pd->reversiGame = mBoard;

	// updates client index is neccessary
	if (pd->clientPlayerIndex < 0)
	{
		// search pm for id
		pd->clientPlayerIndex = mPlayerManagerSelect.GetPlayerIndex(pd->localPlayer);
	}

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