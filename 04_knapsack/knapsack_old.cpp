#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <queue>
#include <cassert>
#include <algorithm>

using namespace std;

struct item {
    int weight;
    int value;
};

//==============================================================================
// CONFIGURATION
//==============================================================================

const int KNAPSACK_CAPACITY = 5000;

const int POPULATION_SIZE = 50;
const int SELECTED_INDIVIDS_COUNT = static_cast<int>(POPULATION_SIZE * 0.5);
const int MAX_GENERATIONS = 100;

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

int getKnapsackWeight(const string& bag) {
    int weight = 0;

    for (int i = 0; i < bag.size(); i++) {
        if (bag[i]) {
            weight += items[i].weight;
        }
    }

    return weight;
}

int getKnapsackFitness(const string& isInside) {
    int fitness = 0;

    for (int i = 0; i < isInside.size(); i++) {
        if (isInside[i]) {
            fitness += items[i].value;
        }
    }

    return fitness;
}

vector<string> generatePopulation(int populationSize) {
    vector<string> population(populationSize, string(items.size(), '0'));

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
    }

    return population;
}

void testGenerataionOfPopulation() {
    vector<string> population = generatePopulation(5);

    for (int i = 0; i < population.size(); i++) {
        cout << getKnapsackWeight(population[i]) << ": ";
        for (int j = 0; j < items.size(); j++) {
            cout << population[i][j] << " ";
        }
        cout << endl;
    }
}

vector<string> selection(const vector<string >& population) {
    vector<string> selected;

    // <fitness, index>
    priority_queue<pair<int, int> > pq;

    for (int i = 0; i <= population.size(); i++) {
        pq.push(make_pair(getKnapsackFitness(population[i]), i));
    }

    for (int i = 0; i < SELECTED_INDIVIDS_COUNT; i++) {
        selected.push_back(population[pq.top().second]);
        pq.pop();
    }

    return selected;
}

int main() {
    srand(time(0));

    // // TODO: keep best solution
    // string bestSolution(items.size(), true);

    
    vector<string> population = generatePopulation(POPULATION_SIZE);
    sort(population.begin(), population.end(), 
        [](const string& a, const string& b) {
            return getKnapsackFitness(a) > getKnapsackFitness(b);
        });

    for (int i = 0; i < population.size(); i++) {
        cout << "Population:\n\tfitness: " << getKnapsackFitness(population[i]) << "\n\tweight: " << getKnapsackWeight(population[i]) << endl;
    }

    population = selection(population);
    cout << "\n\n\n" << endl;
    for (int i = 0; i < population.size(); i++) {
        cout << "Selection:\n\tfitness: " << getKnapsackFitness(population[i]) << "\n\tweight: " << getKnapsackWeight(population[i]) << endl;
    }

    // cout << getKnapsackFitness(bestSolution) << endl;



    return 0;
}