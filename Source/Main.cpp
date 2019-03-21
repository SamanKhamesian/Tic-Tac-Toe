#include "Config.h"
#include "Genetic.h"

int main()
{
	srand((unsigned)time(0));

	Game* game = new Game(INPUT_PATH);
	Genetic genetic(game);

	for (int i = 0; i < GENERATIONS; i++)
	{
		pair<Individual, Individual> p = genetic.selection();
		pair<Individual, Individual> c = genetic.crossover(p.first, p.second);

		int random = rand() % 2;

		if(random == 0)
			c.first = genetic.mutation(c.first);

		else
			c.second = genetic.mutation(c.second);

		genetic.population[genetic.choose(Worst).second] = c.first;
		genetic.population[genetic.choose(Worst).second] = c.second;
	}

	pair<double, int> result = genetic.choose(Best);

	cout << "The Percentage of Lost Games is = " << result.first * 100 << endl;
	cout << "The Strategy for 765 Specific States is :" << endl;

	Individual bestAnswer = genetic.population[result.second];

	for (int i = 0; i < (int)bestAnswer.size(); i++)
		cout << setw(3) << i << " --> " << setw(3) << bestAnswer[i] << endl;

	cout << endl;
}