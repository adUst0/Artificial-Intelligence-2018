/*! Machine Learning: Supervised learning. Implement кNearestNeighbours.
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

//==============================================================================
// INCLUDED FILES
//==============================================================================
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <ctime>

using namespace std;

//==============================================================================
// CONFIGURATION
//==============================================================================
static const int PROPERTIES_COUNT = 4; // the number of properties of one entry in the data set
static const int CLASSES_COUNT = 3; // the number of different classes in the data set
static const int TESTING_SET_SIZE = 20;

string class_int2string(int entryClass) {
    switch (entryClass) {
        case 0:
            return string("Iris-setosa");
        case 1:
            return string("Iris-versicolor");
        case 2:
            return string("Iris-virginica");
        default:
            cout << "class_int2string: Unknown class!" << endl;
            exit(EXIT_FAILURE);
    }
}

int class_string2int(const string& entryClass) {
    if (entryClass == "Iris-setosa") {
        return 0;
    }
    else if (entryClass == "Iris-versicolor") {
        return 1;
    }
    else if (entryClass == "Iris-virginica") {
        return 2;
    }
    else {
        cout << "class_string2int: Unknown class!" << endl;
        exit(EXIT_FAILURE);
    }
}

//==============================================================================
// IMPLEMENTATION
//==============================================================================
struct Entry {
    vector<float> raw;
    int entryClass;

    Entry(int propertiesCount) : raw(vector<float>(propertiesCount)) {

    }

    friend istream& operator>>(istream &is, Entry& entry) {
        for (int i = 0; i < entry.raw.size(); i++) {
            char comma;
            is >> entry.raw[i] >> comma;
        }

        string entryClass;
        is >> entryClass;
        entry.entryClass = class_string2int(entryClass);

        return is;
    }

    friend ostream& operator<<(ostream &os, Entry& entry) {
        for (int i = 0; i < entry.raw.size(); i++) {
            os << entry.raw[i] << ",";
        }
        os << class_int2string(entry.entryClass);
        return os;
    }
};

class EntryComparator {
    const Entry& entry;

    double calculateDistance(const Entry& point) {
        double sum = 0;

        for (int i = 0; i < entry.raw.size(); i++) {
            sum += (entry.raw[i] - point.raw[i]) * (entry.raw[i] - point.raw[i]);
        }

        return sqrt(sum);
    }
public:
    EntryComparator(const Entry& entry_) : entry(entry_) {}

    bool operator() (const Entry& e1, const Entry& e2) {
        return calculateDistance(e1) < calculateDistance(e2);
    }
};

void ReadDatabase(vector<Entry> &dataset, const char *name) {
    ifstream inFile(name);
    if (!inFile.is_open()) {
        cout << "ReadDatabase: Dataset file \"" << name << " \" not found!" << endl;
        exit(EXIT_FAILURE);
    }

    while (true) {
        if (inFile.eof()) {
            break;
        }

        Entry entry(PROPERTIES_COUNT);
        inFile >> entry;

        dataset.push_back(entry);
    }

    inFile.close();
}

int classify(const Entry& entry, vector<Entry> &dataset, int k) {
    sort(dataset.begin(), dataset.end(), EntryComparator(entry));

    // find first K neighbours
    vector<int> classesCount(CLASSES_COUNT, 0);
    for (int i = 0; i < k; i++) {
        classesCount[dataset[i].entryClass]++;
    }

    // if several with maxOccurrences, choose the class with the nearest entry
    vector<int> candidates(CLASSES_COUNT);
    int candidatesCount = 0;
    int maxOccurrences = -1;

    for (int i = 0; i < CLASSES_COUNT; i++) {
        if (classesCount[i] > maxOccurrences) {
            maxOccurrences = classesCount[i];
            candidatesCount = 0;
            candidates[candidatesCount++] = i;
        }
        else if (classesCount[i] == maxOccurrences) {
            candidates[candidatesCount++] = i;
        }
    }

    for (int i = 0; i < k; i++) {
        if (find(candidates.begin(), candidates.begin() + 1, dataset[i].entryClass) != candidates.end()) {
            return dataset[i].entryClass;
        }
    }
    // this line can not be reached
    cout << "classify: Unknown class!" << endl;
    exit(EXIT_FAILURE);
};

int main(int argc, char *argv[]) {
    srand(time(0));

    vector<Entry> database;
    ReadDatabase(database, "./../Dataset/iris.data");

    vector<Entry> trainingData;
    vector<Entry> testingData;

    random_shuffle(database.begin(), database.end());

    for (int i = 0; i < database.size(); i++) {
        if (i < TESTING_SET_SIZE) {
            testingData.push_back(database[i]);
        }
        else {
            trainingData.push_back(database[i]);
        }
    }

    int K = (argc == 1) ? (int)sqrt(trainingData.size()) : atoi(argv[1]);

    int correctPredictions = 0;

    for (auto entry : testingData) {
        int entryClass = classify(entry, trainingData, K);
        // cout << entry << endl;
        // cout << "\tPrediction: " << class_int2string(entryClass) << endl;
        if (entryClass == entry.entryClass) {
            correctPredictions++;
        }
    }

    cout << "Correct predictions: " << correctPredictions << endl;
    cout << "Incorrect predictions: " << TESTING_SET_SIZE - correctPredictions << endl;
    cout << "Accuracy: " << (float)correctPredictions / TESTING_SET_SIZE * 100 << "%" << endl;

    return EXIT_SUCCESS;
}