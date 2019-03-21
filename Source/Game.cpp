#include "Game.h"

bool Game::isWinner(State board, Blank player)
{
	if (board[0] == player && board[0] == board[1] && board[1] == board[2])
		return true;

	else if (board[3] == player && board[3] == board[4] && board[4] == board[5])
		return true;

	else if (board[6] == player && board[6] == board[7] && board[7] == board[8])
		return true;

	else if (board[0] == player && board[0] == board[4] && board[4] == board[8])
		return true;

	else if (board[2] == player && board[2] == board[4] && board[4] == board[6])
		return true;

	else if (board[0] == player && board[0] == board[3] && board[3] == board[6])
		return true;

	else if (board[1] == player && board[1] == board[4] && board[4] == board[7])
		return true;

	else if (board[2] == player && board[2] == board[5] && board[5] == board[8])
		return true;

	else
		return false;
}

bool Game::isAcceptableState(State board)
{
	int numberOf_X = 0, numberOf_O = 0;

	for (int i = 0; i < (int)board.size(); i++)
	{
		numberOf_X += (board[i] == X);
		numberOf_O += (board[i] == O);
	}

	if ((numberOf_X == numberOf_O) || (numberOf_X == (numberOf_O + 1)))
	{
		if (!isWinner(board, X) && !isWinner(board, O))
			return true;

		else if (isWinner(board, O) && !isWinner(board, X) && (numberOf_O == numberOf_X))
			return true;

		else if (isWinner(board, X) && !isWinner(board, O) && ((numberOf_O + 1) == numberOf_X))
			return true;

		else
			return false;
	}

	else
		return false;
}

State Game::getBaseCaseMatrix(State main)
{
	vector<State> states =
	{
		{ main[0], main[1], main[2], main[3], main[4], main[5], main[6], main[7], main[8] },
		{ main[6], main[3], main[0], main[7], main[4], main[1], main[8], main[5], main[2] },
		{ main[8], main[7], main[6], main[5], main[4], main[3], main[2], main[1], main[0] },
		{ main[2], main[5], main[8], main[1], main[4], main[7], main[0], main[3], main[6] },
		{ main[6], main[7], main[8], main[3], main[4], main[5], main[0], main[1], main[2] },
		{ main[8], main[5], main[2], main[7], main[4], main[1], main[6], main[3], main[0] },
		{ main[2], main[1], main[0], main[5], main[4], main[3], main[8], main[7], main[6] },
		{ main[0], main[3], main[6], main[1], main[4], main[7], main[2], main[5], main[8] }
	};

	int baseState = 0;

	for (int i = 1; i < (int)states.size(); i++)
		if (hasPriority(states[i], states[baseState]))
			baseState = i;

	return states[baseState];
}

Outcome Game::play(vector<int> player_1, vector<int> player_2)
{
	int step = 0;
	GameState state;

	while(true)
	{
		if(player_1[step] < INDIVIDUAL_SIZE)
		{
			state = baseStateMatrix[player_1[step]];
			step = player_1[step];
		}

		else
			return (Outcome)player_1[step];
		
		if (player_2[step] < INDIVIDUAL_SIZE)
		{
			state = baseStateMatrix[player_2[step]];
			step = player_2[step];
		}

		else
			return (Outcome)player_2[step];
	}
}

Game::Game()
{
	for (int i = 0; i < 3; i += 1)
	{
		for (int j = 0; j < 3; j += 1)
		{
			for (int k = 0; k < 3; k += 1)
			{
				for (int l = 0; l < 3; l += 1)
				{
					for (int m = 0; m < 3; m += 1)
					{
						for (int n = 0; n < 3; n += 1)
						{
							for (int p = 0; p < 3; p += 1)
							{
								for (int q = 0; q < 3; q += 1)
								{
									for (int r = 0; r < 3; r += 1)
									{
										int level = 0;
										State temp = { (Blank)i, (Blank)j, (Blank)k, (Blank)l, (Blank)m, (Blank)n, (Blank)p, (Blank)q, (Blank)r };

										for (int index = 0; index < (int)temp.size(); index++)
											level += (temp[index] != E);

										if (isAcceptableState(temp))
										{
											temp = getBaseCaseMatrix(temp);
											GameState newGameBoard = { temp, level };

											if (find(baseStateMatrix.begin(), baseStateMatrix.end(), newGameBoard) == baseStateMatrix.end())
												baseStateMatrix.push_back(newGameBoard);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	sort(baseStateMatrix.begin(), baseStateMatrix.end());
}

Game::Game(string filePath)
{
	ifstream fin(filePath);

	for (int i = 0; i < INDIVIDUAL_SIZE; i++)
	{
		int level = 0;
		vector<Blank> board;

		for (int j = 0; j < BOARD_SIZE; j++)
		{
			int block;
			fin >> block;
			board.push_back((Blank)block);

			if (block != 0)
				level++;
		}

		GameState state{ board, level };
		baseStateMatrix.push_back(state);
	}

	sort(baseStateMatrix.begin(), baseStateMatrix.end());
}
