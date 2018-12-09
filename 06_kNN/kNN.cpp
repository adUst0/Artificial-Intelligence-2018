/*! Machine Learning: Supervised learning. K Nearest Neighbours.
 *
 * \brief       Homework 6
 * \author      Boris Ivanov
 *
 * 1) Download the data from http://archive.ics.uci.edu/ml/datasets/Iris
 *    or https://www.kaggle.com/uciml/iris,
 *    http://techlab.bu.edu/resources/data_view/iris_data_set/
 *
 * 2) Read the data in the program and divide it into 2 sets:
 *  - testing set (20 randomly selected entries)
 *  - training set (the rest entries)
 *
 * 3) Implement кNearestNeighbours
 *  - k - input parameter
 *  - Problem: classify the entries from the test set
 *
 * 4) Print to stdout:
 *  - the number of correctly classified entries
 *  - Accuracy (correctly classified / total testing entries)
**/

#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <algorithm>
#include <cmath>
#include <ctime>

using namespace std;

static const int DATASET_COLS = 5;
static const int CLASS_POS = 4; // last column is the class to which the entry belongs
static const int CLASSES_COUNT = 3; // all possible classes to which an entry can belong

static const int TESTING_SET_SIZE = 20;

using Entry = vector<float>;
using Dataset = vector<Entry>;

void ReadDatabase(Dataset &dataset, const char *name) {
    ifstream data(name);

    if (!data.is_open()) {
        cout << "Dataset file not found!" << endl;
        exit(EXIT_FAILURE);
    }

    while (true) {
        if (data.eof()) {
            break;
        }

        Entry entry(DATASET_COLS);

        for (int i = 0; i < DATASET_COLS - 1; i++) {
            float x;
            char comma;
            data >> x >> comma;
            entry[i] = x;
        }
        string entryClass;
        data >> entryClass;

        if (entryClass == "Iris-setosa") {
            entry[CLASS_POS] = 1;
        }
        else if (entryClass == "Iris-versicolor") {
            entry[CLASS_POS] = 2;
        }
        else if (entryClass == "Iris-virginica") {
            entry[CLASS_POS] = 3;
        }

        dataset.push_back(entry);
    }

    data.close();
}

class EntryComparator {
    const Entry& entry;

    double calculateDistance(const Entry& point) {
        double sum = 0;

        for (int i = 0; i < DATASET_COLS - 1; i++) {
            sum += pow(entry[i] - point[i], 2);
        }

        return sqrt(sum);
    }
public:
    EntryComparator(const Entry& entry_) : entry(entry_) {}

    bool operator() (const Entry& e1, const Entry& e2) {
        return calculateDistance(e1) < calculateDistance(e2);
    }
};

int classify(const Entry& entry, Dataset& dataset, int k) {
    sort(dataset.begin(), dataset.end(), EntryComparator(entry));

    vector<int> count(CLASSES_COUNT, 0);

    for (int i = 0; i < k; i++) {
        count[dataset[i][CLASS_POS]]++;
    }

    vector<int> candidates(dataset.size());
    int candidatesCount = 0;
    int maxOccurrences = 0;

    for (int i = 0; i < count.size(); i++) {
        if (count[i] > maxOccurrences) {
            candidatesCount = 0;
            candidates[candidatesCount++] = i;
            maxOccurrences = count[i];
        }
        else if (count[i] == maxOccurrences) {
            candidates[candidatesCount++] = i;
        }
    }

    if (candidatesCount == 1) {
        return candidates[0];
    }
    else {
        for (int i = 0; i < k; i++) {
            if (find(candidates.begin(), candidates.end(), (int)dataset[i][CLASS_POS]) != candidates.begin()) {
                return (int)dataset[i][CLASS_POS];
            }
        }
    }
};

int main() {
    srand(time(0));

    Dataset database;
    ReadDatabase(database, "./../Dataset/iris.data");

    Dataset trainingData;
    Dataset testingData;

    random_shuffle(database.begin(), database.end());

    for (int i = 0; i < database.size(); i++) {
        if (i < TESTING_SET_SIZE) {
            testingData.push_back(database[i]);
        }
        else {
            trainingData.push_back(database[i]);
        }
    }

    for (int i = 0; i < testingData.size(); i++) {
        int entryClass = classify(testingData[i], trainingData, (int)sqrt(trainingData.size()));

        cout << testingData[i][0] << ',' << testingData[i][1] << ',' << testingData[i][2] << ',' << testingData[i][3] << ',';

        switch (entryClass) {
            case 1:
                cout << "Iris-setosa";
                break;
            case 2:
                cout << "Iris-versicolor";
                break;
            case 3:
                cout << "Iris-virginica";
                break;
            default:
                cout << "Unknown entry class!";

        }
        cout << " -> ";
        switch ((int)testingData[i][4]) {
            case 1:
                cout << "Iris-setosa" << endl;
                break;
            case 2:
                cout << "Iris-versicolor" << endl;
                break;
            case 3:
                cout << "Iris-virginica" << endl;
                break;
            default:
                cout << testingData[i][4] << " " << (int)testingData[i][4] << " " <<"Unknown entry class!" << endl;

        }
    }

    return EXIT_SUCCESS;
}