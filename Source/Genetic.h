#include "Game.h"

typedef vector<int> Individual;

enum Rank{Best = 0, Worst = 1};

class Genetic
{
public:

	Game* game = NULL;
	vector<Individual > population;

	Genetic();
	Genetic(Game* game);
	~Genetic();

	double fitness(Individual);
	pair<double, int> choose(Rank);
	pair<Individual, Individual> selection();
	pair<Individual, Individual> crossover(Individual, Individual);
	Individual mutation(Individual);
};
