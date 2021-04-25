#pragma once
#include "Board.h"
#include <map>
#include <vector>

// move to cpp

typedef std::map<std::pair<int, int>, std::pair<int, int>> Matrix;

// tow player normal form payoffs
struct Entry
{
	// actions p1,p2
	int key[2]{ -1,-1 };
	// p1,p2
	int payoff[2]{ -1,-1 };
	// max for action
	int max[2]{ -1,-1 };
	// when key elements equal nash elements then
	int nash[2]{ -1,-1 };
	// expected utility for mixed strategy
	float expUtil[2]{ -1.0f,-1.0f };
    void ToConsole() const;

	//bool IsPureNash() const;
	static bool IsPureNash(const Entry& e);
};

class NormalForm
{
	ScoreGrid mProb;
	std::vector<Entry> mAllStrategies;
	std::array<int,64> mPayoffMultiplier;
	std::vector<Entry> mPureNash;
	std::vector<Entry> mDominantStrategies;
	std::map<int, int> mExpectedUtilTotal[2];
	std::vector <Entry> mPreviousMove;
	// invalid by default
	int mOpponentMove = -1;

public:
	NormalForm();
	void Init(const std::array<int, 64>& payoffMulti, const ScoreGrid& prob);
	int Evalualate(int agent, ReversiBoard board, int opponentMove);
	void Reset();
	void ToConsole();
private:
    void CalculateDominantMax();
	void CalcPureNashMax();
    void GeneratePayoffs(const ReversiBoard& board,int& p1,int& p2);
	void GenerateDominant();
	void GeneratePureNashEquilib();
	std::pair<float,float> CalcMixedNash(std::vector<Entry>& entries);
	int GetMixedMove(std::vector<Entry>& entries);
};