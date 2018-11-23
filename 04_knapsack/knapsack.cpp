#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <queue>
#include <cassert>
#include <algorithm>

using namespace std;

//==============================================================================
// CONFIGURATION
//==============================================================================

const int KNAPSACK_CAPACITY = 5000;

const int POPULATION_SIZE = 300;
const int MAX_GENERATIONS = 500;
const float PMUTATION = 0.9;

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

//==============================================================================
// IMPLEMENTATION
//==============================================================================

// Generate random number in the interval [first, last]
int rand(int first, int last) {
    return rand() % (last - first + 1) + first;
}

using Chromosome = vector<bool>;
using Population = vector<Chromosome>;

int getWeight(Chromosome chromosome) {
    int weight = 0;

    for (int i = 0; i < chromosome.size(); i++) {
        if (chromosome[i]) {
            weight += items[i].weight;
        }
    }

    return weight;
}

int getFitness(Chromosome chromosome) {
    int fitness = 0;

    for (int i = 0; i < chromosome.size(); i++) {
        if (chromosome[i]) {
            fitness += items[i].value;
        }
    }

    return fitness;
}

void sortPopulation(Population& population) {
    sort(population.begin(), population.end(),
            [](const Chromosome& a, const Chromosome& b) {
                return getFitness(a) > getFitness(b);
            });
}

Population generatePopulation() {
    Population population;
    int chromosomeLength = items.size();

    for (int i = 0; i < POPULATION_SIZE; i++) {
        Chromosome chromosome(chromosomeLength, false);

        for (int i = 0; i < chromosomeLength * 2; i++) {
            int itemIndex = rand(0, chromosomeLength - 1);
            int currentWeight = getWeight(chromosome);
            int newWeight = currentWeight + items[itemIndex].weight;

            if (!chromosome[itemIndex] && newWeight <= KNAPSACK_CAPACITY) {
                chromosome[itemIndex] = true;
            }
        }

        population.push_back(chromosome);
    }

    return population;
}


// This function expects population to be sorted in descending order
Population selectParents(const Population& population) {
    Population parents;

    int parent1 = rand(0, population.size() / 10);
    int parent2 = rand(0, population.size() / 10);

    while (parent1 == parent2) {
        parent2 = rand(0, population.size() / 10);
    }

    parents.push_back(population[parent1]);
    parents.push_back(population[parent2]);

    return parents;
}

Population crossover(const Population& parents) {
    const int chromosomeLength = items.size();
    Population offspring(2, Chromosome(chromosomeLength, false));

    int idx;

    for (int i = 0; i < chromosomeLength; i++) {
        idx = rand(0, 1);
        if (idx == 1) {
            offspring[0][i] = parents[0][i];
            offspring[1][i] = parents[1][i];
        }
        else {
            offspring[0][i] = parents[1][i];
            offspring[1][i] = parents[0][i];
        }
    }

    return offspring;
}

void mutate(Population& offspring) {
    int chromosomeLength = offspring[0].size();

    for (int i = 0; i < offspring.size(); i++) {
        int gene = rand(0, chromosomeLength - 1);

        float pmutation = rand(0, 1000);

        if (pmutation <= 1000 * PMUTATION) {
            offspring[i][gene] = !offspring[i][gene];
        }

    }
}

void printPopulation(Population& population) {
    for (Chromosome& c : population) {
        cout << "Value: " << getFitness(c) << endl;
        cout << "Weight: " << getWeight(c) << "\n\n";
    }
}

int main() {
    srand(time(0));

    Population population = generatePopulation();

    sortPopulation(population);
    Chromosome bestSolution = population[0];

    cout << "Initial population best solution:" << endl;
    cout << "\t" << getFitness(bestSolution) << endl;
    cout << "\t" << getWeight(bestSolution) << endl;

    for (int i = 0; i < MAX_GENERATIONS; i++) {
        Population parents = selectParents(population);
        Population offspring = crossover(parents);
        mutate(offspring);

        bool hasValidOfspring = false;
        // replace the less fittest with the offspring
        if (getWeight(offspring[0]) <= KNAPSACK_CAPACITY) {
            population[population.size() - 1] = offspring[0];
            hasValidOfspring = true;
        }
        if (getWeight(offspring[1]) <= KNAPSACK_CAPACITY) {
            population[population.size() - 2] = offspring[1];
            hasValidOfspring = true;
        }

        if (hasValidOfspring) {
            sortPopulation(population);
            if (getFitness(population[0]) > getFitness(bestSolution)) {
                bestSolution = population[0];
            }
        }
    }

    cout << "Best solution found:" << endl;
    cout << "\t" << getFitness(bestSolution) << endl;
    cout << "\t" << getWeight(bestSolution) << endl;

    return 0;
}