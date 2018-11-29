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

const int POPULATION_SIZE = 500;
const int MAX_GENERATIONS = 1500;
const float PMUTATION = 0.9;

struct item {
    int weight;
    int value;
};

// short example
vector<item> items {
    {5000, 24},
    {90, 150},
    {130, 35},
    {1530, 200},
    {500, 160},
    {150, 60},
    {680, 45},
    {270, 60},
    {390, 40},
    {230, 30},
    {520, 10},
    {110, 70},
    {320, 30},
    {240, 15},
    {480, 10},
    {730, 40},
    {420, 70},
    {430, 75},
    {220, 80},
    {70, 20},
    {180, 12},
    {40, 50},
    {300, 10},
    {900, 1},
    {2000, 150}
};

// // long example
// vector<item> items {
//     {5000, 200},
//     {1681, 447},
//     {1398, 178},
//     {4558, 233},
//     {829, 464},
//     {1249, 446},
//     {2154, 280},
//     {3742, 109},
//     {3445, 108},
//     {4305, 462},
//     {3274, 251},
//     {3194, 114},
//     {1250, 136},
//     {4873, 381},
//     {4222, 461},
//     {3030, 255},
//     {44, 252},
//     {752, 385},
//     {1699, 152},
//     {978, 330},
//     {3186, 212},
//     {4091, 159},
//     {3153, 215},
//     {469, 279},
//     {1301, 75},
//     {4661, 231},
//     {2847, 86},
//     {4124, 395},
//     {1351, 249},
//     {4434, 481},
//     {3934, 379},
//     {4383, 14},
//     {4849, 188},
//     {667, 485},
//     {2679, 146},
//     {1044, 372},
//     {4226, 440},
//     {4521, 458},
//     {1892, 75},
//     {2243, 143},
//     {1982, 205},
//     {4434, 312},
//     {3391, 195},
//     {3553, 255},
//     {2634, 217},
//     {1285, 160},
//     {4039, 267},
//     {259, 177},
//     {4782, 293},
//     {415, 10},
//     {484, 68},
//     {2940, 335},
//     {4022, 65},
//     {817, 150},
//     {426, 477},
//     {428, 130},
//     {1355, 342},
//     {1347, 310},
//     {3430, 417},
//     {1127, 440},
//     {4566, 129},
//     {1959, 200},
//     {3257, 98},
//     {230, 92},
//     {2622, 36},
//     {3166, 488},
//     {3401, 349},
//     {2624, 461},
//     {3454, 183},
//     {1217, 285},
//     {2322, 480},
//     {270, 245},
//     {3016, 165},
//     {3160, 472},
//     {632, 479},
//     {944, 46},
//     {3357, 276},
//     {564, 262},
//     {1864, 18},
//     {3029, 243},
//     {4416, 100},
//     {3915, 275},
//     {2695, 293},
//     {1315, 53},
//     {1809, 486},
//     {932, 417},
//     {2764, 340},
//     {3130, 455},
//     {2597, 18},
//     {729, 308},
//     {4516, 199},
//     {751, 285},
//     {3199, 116},
//     {4167, 428},
//     {328, 371},
//     {2348, 435},
//     {2475, 150},
//     {3264, 256},
//     {1012, 173},
//     {339, 386},
//     {4552, 121},
//     {3060, 350},
//     {3882, 495},
//     {1516, 276},
//     {2303, 442},
//     {1356, 26},
//     {1686, 327},
//     {3684, 431},
//     {498, 261},
//     {278, 92},
//     {609, 114},
//     {4714, 490},
//     {1412, 313},
//     {997, 18},
//     {3755, 105},
//     {4686, 363},
//     {1207, 134},
//     {4358, 191},
//     {2099, 487},
//     {344, 334},
//     {904, 73},
//     {899, 356},
//     {1420, 405},
//     {4483, 321},
//     {3541, 192},
//     {1385, 317},
//     {1982, 309},
//     {2692, 257},
//     {4058, 240},
//     {4099, 87},
//     {1561, 496},
//     {2151, 58},
//     {1870, 328},
//     {3249, 149},
//     {2501, 468},
//     {3280, 305},
//     {1464, 434},
//     {3832, 422},
//     {3022, 247},
//     {97, 91},
//     {2771, 168},
//     {85, 484},
//     {670, 304},
//     {3370, 382},
//     {351, 53},
//     {3903, 147},
//     {803, 250},
//     {1931, 476},
//     {2740, 154},
//     {855, 344},
//     {1264, 193},
//     {1516, 226},
//     {2718, 199},
//     {1323, 443},
//     {1582, 209},
//     {4357, 404},
//     {1362, 167},
//     {2541, 298},
//     {2217, 471},
//     {2093, 205},
//     {1917, 148},
//     {1238, 296},
//     {3740, 155},
//     {3682, 492},
//     {1362, 482},
//     {2589, 308},
//     {481, 404},
//     {2888, 496},
//     {887, 2},
//     {671, 419},
//     {2519, 219},
//     {1730, 104},
//     {1116, 1},
//     {2655, 48},
//     {4769, 51},
//     {3859, 465},
//     {2997, 126},
//     {143, 187},
//     {3030, 437},
//     {763, 137},
//     {874, 207},
//     {1294, 454},
//     {224, 215},
//     {4433, 322},
//     {2268, 344},
//     {3146, 482},
//     {2130, 281},
//     {4672, 329},
//     {1649, 392},
//     {3611, 345},
//     {4708, 70},
//     {4347, 253},
//     {1152, 7},
//     {3114, 499},
//     {373, 153},
//     {2348, 443},
//     {3769, 460},
//     {1192, 258},
//     {1055, 237},
//     {3028, 403},
//     {362, 430}
// };

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
    cout << "\tFitness: " << getFitness(bestSolution) << endl;
    cout << "\tWeight: " << getWeight(bestSolution) << endl;

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
    cout << "\tFitness: " << getFitness(bestSolution) << endl;
    cout << "\tWeight: " << getWeight(bestSolution) << endl;

    return 0;
}