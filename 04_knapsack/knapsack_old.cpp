// START
// Generate the initial population
// Compute fitness
// REPEAT
//     Selection
//     Crossover
//     Mutation
//     Compute fitness
// UNTIL population has converged
// STOP

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const int KNAPSACK_CAPACITY = 5000;

const int MAX_GENERATIONS = 100;
const int POPULATION_SIZE = 15;
const int MAX_SELECTED = 6;

struct item {
    int weight;
    int value;
};

vector<item> items {
    {90, 150}, // map 
    {130, 35}, // compass 
    {1530, 200}, // water 
    {500, 160}, // sandwich 
    {150, 60}, // glucose 
    {680, 45}, // tin 
    {270, 60}, // banana 
    {390, 40}, // apple 
    {230, 30}, // cheese 
    {520, 10}, // beer 
    {110, 70}, // suntan cream 
    {320, 30}, // camera 
    {240, 15}, // T-shirt 
    {480, 10}, // trousers 
    {730, 40}, // umbrella 
    {420, 70}, // waterproof trousers 
    {430, 75}, // waterproof overclothes 
    {220, 80}, // note-case 
    {70, 20}, // sunglasses 
    {180, 12}, // towel 
    {40, 50}, // socks 
    {300, 10}, // book 
    {900, 1}, // notebook 
    {2000, 150} // tent 
};

// TODO: keep best solution
vector<bool> bestSolution(items.size(), true);

int getKnapsackWeight(const vector<bool>& bag) {
    int weight = 0;

    for (int i = 0; i < bag.size(); i++) {
        if (bag[i]) {
            weight += items[i].weight;
        }
    }

    return weight;
}

int getFitnessOfPopulation(const vector<bool>& isInside) {
    int fitness = 0;

    for (int i = 0; i < isInside.size(); i++) {
        if (isInside[i]) {
            fitness += items[i].weight;
        }
    }

    return fitness;
}

vector<vector<bool>> generatePopulation(int populationSize) {
    vector<vector<bool>> population(populationSize, vector<bool>(items.size()));

    for (int i = 0; i < populationSize; i++) {
        for (int j = 0; j < items.size(); j++) {
            bool canAdd = rand() % 2;

            if (canAdd) {
                if (getKnapsackWeight(population[i]) + items[j].weight <= KNAPSACK_CAPACITY) {
                    population[i][j] = true;
                }
                else {
                    break;
                }
            }
        }

        if (getFitnessOfPopulation(population[i]) < getFitnessOfPopulation(bestSolution)) {
            bestSolution = population[i];
        }
    }

    return population;
}

void testGenerataionOfPopulation() {
    vector<vector<bool>> population = generatePopulation(5);

    for (int i = 0; i < population.size(); i++) {
        cout << getKnapsackWeight(population[i]) << ": ";
        for (int j = 0; j < items.size(); j++) {
            cout << population[i][j] << " ";
        }
        cout << endl;
    }
}

vector<int> selection(const vector<vector<bool> >& population) {
    vector<int> selected(MAX_SELECTED);

    // <fitness, index>
    priority_queue<pair<int, int> > pq;

    for (int i = 0; i <= population.size(); i++) {
        pq.push(make_pair(getFitnessOfPopulation(population[i]), i));
    }

    for (int i = 0; i < MAX_SELECTED; i++) {
        selected[i] = pq.top().first;
        pq.pop();
    }

    return selected;
}

int main() {
    srand(time(0));
    
    vector<vector<bool>> population = generatePopulation(POPULATION_SIZE);

    for (int i = 0; i < MAX_GENERATIONS; i++) {
        vector<int> selection = 
    }

    cout << getFitnessOfPopulation(bestSolution) << endl;



    return 0;
}