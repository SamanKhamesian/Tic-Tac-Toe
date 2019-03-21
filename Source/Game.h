#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <string>
#include <ctime>
#include <cmath>
#include <set>
#include <ctime>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>

#include "Config.h"

using namespace std;

enum Blank { E = 0, X = 1, O = 2 };
enum Outcome { X_is_Winner = 765, O_is_Winner = 766, No_One_Win = 767 };

typedef vector<Blank> State;

inline bool hasPriority(State board_1, State board_2)
{
	int sumOf_x_1 = 0, sumOf_o_1 = 0;
	int mulOf_x_1 = 1, mulOf_o_1 = 1;

	int sumOf_x_2 = 0, sumOf_o_2 = 0;
	int mulOf_x_2 = 1, mulOf_o_2 = 1;

	for (int i = 1; i < 10; i++)
	{
		sumOf_x_1 += (i * (board_1[i - 1] == X));
		sumOf_x_2 += (i * (board_2[i - 1] == X));

		sumOf_o_1 += (i * (board_1[i - 1] == O));
		sumOf_o_2 += (i * (board_2[i - 1] == O));

		mulOf_x_1 *= (i * (board_1[i - 1] == X));
		mulOf_x_2 *= (i * (board_2[i - 1] == X));

		mulOf_o_1 *= (i * (board_1[i - 1] == O));
		mulOf_o_2 *= (i * (board_2[i - 1] == O));
	}

	if (sumOf_x_1 == sumOf_x_2)
		if (sumOf_o_1 == sumOf_o_2)
			if (mulOf_x_1 == mulOf_x_2)
				if (mulOf_o_1 == mulOf_o_2)
					return lexicographical_compare(board_1.begin(), board_1.end(), board_2.begin(), board_2.end());

				else
					return mulOf_o_1 < mulOf_o_2;
			else
				return mulOf_x_1 < mulOf_x_2;
		else
			return sumOf_o_1 < sumOf_o_2;
	else
		return sumOf_x_1 < sumOf_x_2;
}

struct GameState
{
	State board;
	int gameLevel;

	inline bool operator== (const GameState& gameBoard) const
	{
		return (gameLevel == gameBoard.gameLevel && board == gameBoard.board);
	}

	inline bool operator< (const GameState& gameBoard) const
	{
		if (gameLevel != gameBoard.gameLevel)
			return gameLevel < gameBoard.gameLevel;

		else
			return hasPriority(board, gameBoard.board);
	}
};

class Game
{
public:

	vector<GameState> baseStateMatrix;

	Game();
	Game(string);

	bool isWinner(State, Blank);
	bool isAcceptableState(State);
	State getBaseCaseMatrix(State);
	Outcome play(vector<int>, vector<int>);
};