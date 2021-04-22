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

    void ToConsole() const;

	//bool IsPureNash() const;
	static bool IsPureNash(const Entry& e);
};

class NormalForm
{
	Matrix mMatrix;
	std::vector<Entry> entry;
	const std::array<int,64> mPayoffMultiplier;
	std::vector<Entry> pureNashEqui;
	std::vector<Entry> dominantStrategy;

public:

    NormalForm(int agent, ReversiBoard board, const std::array<int, 64>& );
	
	void ToConsole();

	// @return evaluated move. -1 for invalid option
    int Dominant();


private:
    void CalculateDominantMax();
	void CalcPureNashMax();
    
    void EliminateActions();
   
    void GeneratePayoffs(const ReversiBoard& board,int& p1,int& p2);


	void GenerateDominant();
	void GeneratePureNashEquilib();

	void GenerateMixedNashEqilib();
};