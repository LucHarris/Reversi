#pragma once
#include <list>
#include <memory>
#include "Board.h"

struct Node
{
	enum class BoardState{CAN_MOVE = 0, WIN = 1, LOSE, DRAW};
	typedef std::list<Node> Children;
	int move = -1; // default invalid
	BoardState boardState = BoardState::CAN_MOVE;
	std::pair<int, int> scores;
	Children children;

	Node() {};

	// Simulates moves with scores
	// @param agent: current side agent is playing. 0/1
	// @param move: 1d position on board.
	// @param depth: base condition
	// @param board: copy for simulating 
	Node(int agent, int& m, int depth, ReversiBoard board);
	
};


class NodeManager
{
	Node base;
	Node* pNodeLocation = nullptr;

public:

};