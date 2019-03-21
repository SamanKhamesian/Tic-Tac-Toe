#include "Genetic.h"

int nextMove(GameState cureState, Game* game)
{
	srand((unsigned)time(0));

	int diff = 0, empty = 0;

	for (int b = 0; b < BOARD_SIZE; b++)
	{
		if (cureState.board[b] == X)
			diff++;

		else if (cureState.board[b] == O)
			diff--;

		else if (cureState.board[b] == E)
			empty++;
	}

	if (game->isWinner(cureState.board, X))
		return X_is_Winner;

	else if (game->isWinner(cureState.board, O))
		return O_is_Winner;
	
	else if (empty == 0)
		return No_One_Win;

	else
	{
		int nextLevel = cureState.gameLevel + 1;
		set<int> possibleStates;

		for (int i = 0; i < BOARD_SIZE; i++)
		{
			State tempBoard = cureState.board;

			if (tempBoard[i] == E)
			{
				tempBoard[i] = (Blank)(diff + 1);
				tempBoard = game->getBaseCaseMatrix(tempBoard);
				GameState temp_state{ tempBoard, nextLevel };
				int index = find(game->baseStateMatrix.begin(), game->baseStateMatrix.end(), temp_state) - game->baseStateMatrix.begin();

				if (index != INDIVIDUAL_SIZE)
					possibleStates.insert(index);
			}
		}

		int step = rand() % (int)possibleStates.size();
		set<int>::iterator it = next(possibleStates.begin(), step);

		return *it;
	}
}

double Genetic::fitness(Individual candidate)
{
	double numberOfLose = 0;
	Outcome result;

	for (int i = 0; i < (int)population.size(); i++)
	{
		Individual player = population[i];

		result = game->play(player, candidate);
		
		if (result == O_is_Winner)
			numberOfLose++;

		result = game->play(candidate, player);

		if (result == X_is_Winner)
			numberOfLose++;
	}

	double f = numberOfLose / (2 * (int)population.size());

	return f;
}

pair<double, int> Genetic::choose(Rank rank)
{
	double result = ((rank == Best) ? double(POPULATION_SIZE * 2.0) : 0.0);
	int bestStrategy = 0;

	for (int i = 0; i < (int)population.size(); i++)
	{
		double f = fitness(population[i]);

		if (rank == Best)
		{
			if (f < result)
			{
				result = f;
				bestStrategy = i;
			}
		}

		else
		{
			if (f > result)
			{
				result = f;
				bestStrategy = i;
			}
		}
	}

	return make_pair(result, bestStrategy);
}

pair<Individual, Individual> Genetic::selection()
{
	srand((unsigned)time(0));
	vector< pair<double, Individual> > candidates;

	for (int i = 0; i < sqrt((int)population.size()); i++)
	{
		int index = rand() % population.size();
		pair<double, Individual> c = make_pair(fitness(population[index]), population[index]);
		candidates.push_back(c);
	}

	sort(candidates.begin(), candidates.end());
	return make_pair(candidates[0].second, candidates[1].second);
}

pair<Individual, Individual> Genetic::crossover(Individual parent_1, Individual parent_2)
{
	srand((unsigned)time(0));
	int random = rand() % 100;

	if (random < CROSSOVER_FRACTION * 100)
	{
		int rand_1, rand_2, diff;

		do {

			rand_1 = rand() % parent_1.size();
			rand_2 = rand() % parent_2.size();
			diff = abs(rand_2 - rand_1);

		} while (diff <= 5 || diff >= 255);

		int cutPoint_1 = min(rand_1, rand_2);
		int cutPoint_2 = max(rand_1, rand_2);

		for (int i = cutPoint_1; i < cutPoint_2; i++)
			swap(parent_1[i], parent_2[i]);
	}

	return make_pair(parent_1, parent_2);
}

Individual Genetic::mutation(Individual individual)
{
	srand((unsigned)time(0));
	int random = rand() % 100;

	if (random < MUTATION_FRACTION * 100)
	{
		int index = rand() % INDIVIDUAL_SIZE;
		GameState state = game->baseStateMatrix[index];
		int nxtMove = nextMove(state, game);
		individual[index] = nxtMove;
	}

	return individual;
}

Genetic::Genetic()
{
	game = new Game();

	for (int i = 0; i < POPULATION_SIZE; i++)
	{
		Individual individual(INDIVIDUAL_SIZE);

		for (int s = 0; s < INDIVIDUAL_SIZE; s++)
		{
			GameState cureState = game->baseStateMatrix[s];
			individual[s] = nextMove(cureState, game);
		}

		population.push_back(individual);
	}
}

Genetic::Genetic(Game* newGame)
{
	game = newGame;

	for (int i = 0; i < POPULATION_SIZE; i++)
	{
		Individual individual(INDIVIDUAL_SIZE);

		for (int s = 0; s < INDIVIDUAL_SIZE; s++)
		{
			GameState cureState = game->baseStateMatrix[s];
			individual[s] = nextMove(cureState, game);
		}

		population.push_back(individual);
	}
}

Genetic::~Genetic()
{
	delete game;
}
