#include <iostream>
#include <ctime>
#include <vector>
#include <queue>
#include <cassert>

using namespace std;

using boolean = uint8_t;

std::ostream& operator<<(std::ostream& os, const boolean& value)
{
    return os << static_cast<bool>(value);
}

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

// One individ is one possible knapsack configuration
class Individ {
private:
    vector<boolean> isPresent; // flag if items[i] is inside the knapsack.

public:
    static const vector<item>& items;

    Individ() : isPresent(items.size()) {}

    Individ(const vector<boolean>& isPresent) : isPresent(isPresent) {}

    Individ(const Individ& other) : isPresent(other.isPresent) {}

    boolean& operator[](int i) {
        return isPresent[i];
    }

    int getFitness() const {
        int fitness = 0;

        for (int i = 0; i < isPresent.size(); i++) {
            if (isPresent[i]) {
                fitness += items[i].value;
            }
        }

        return fitness;
    }

    int getWeight() const {
        int weight = 0;

        for (int i = 0; i < isPresent.size(); i++) {
            if (isPresent[i]) {
                weight += items[i].weight;
            }
        }

        return weight;
    }
};

const vector<item>& Individ::items = ::items;

vector<Individ> generatePopulation(int populationSize) {
    vector<Individ> population();

    for (int i = 0; i < populationSize; i++) {
        for (int j = 0; j < Individ::items.size(); j++) {
            bool canAdd = rand() % 2;

            if (canAdd) {
                if (population[i].getWeight() + Individ::items[j].weight <= KNAPSACK_CAPACITY) {
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

vector<Individ> selection(const vector<Individ>& population) {
    vector<Individ> selected;

    // <fitness, index>
    priority_queue<pair<int, int> > pq;
    for (int i = 0; i <= population.size(); i++) {
        pq.push(make_pair(population[i].getFitness(), i));
    }

    for (int i = 0; i < SELECTED_INDIVIDS_COUNT; i++) {
        Individ individ(population[pq.top().second]);
        selected.push_back(individ);
        pq.pop();
    }

    return selected;
}

// vector<Individ> crossover(const vector<Individ>& population) {
//     // TODO
// }

// vector<Individ> mutation(const vector<Individ>& population) {
//     // TODO
// }


void testGenerataionOfPopulation();
void testSelection();
void testCrossover();
void testMutation();

int main() {
    srand(time(0));

    vector<Individ> population = generatePopulation(POPULATION_SIZE);
    Individ bestSolution();

    for (int i = 0; i < population.size(); i++) {
        cout << population[i].getFitness() << " " << population[i].getWeight() << endl;
    }

    population = selection(population);

    cout << "Selection: " << endl;
    for (int i = 0; i < population.size(); i++) {
        cout << population[i].getFitness() << " " << population[i].getWeight() << endl;
    }

    // for (int i = 0; i < MAX_GENERATIONS; i++) {
    //     // TODO: chech if better solution was found
    //     population = selection(population);
    //     // population = crossover(population);
    //     // population = mutation(population);
    // }

    return 0;
}

void testGenerataionOfPopulation() {
    vector<Individ> population = generatePopulation(5);

    for (int i = 0; i < population.size(); i++) {
        assert(population[i].getWeight() <= KNAPSACK_CAPACITY);
        cout << population[i].getWeight() << ": ";
        for (int j = 0; j < items.size(); j++) {
            cout << population[i][j] << " ";
        }
        cout << endl;
    }
}