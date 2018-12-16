/*! Machine Learning: Supervised learning. Implement naive Bayes classifier.
 *
 * \brief       Homework 7
 * \author      Boris Ivanov
 *
 * 1) Download the data from http://archive.ics.uci.edu/ml/datasets/Congressional+Voting+Records
 *    or http://repository.seasr.org/Datasets/UCI/arff/vote.arff
 *
 * 2) Implement naive Bayes classifier to classify if republican / democrat
 *
 * 3) Validate the algorithm using 10-fold-cross-validation.
 *      - Divide the data into 10 random subsets.
 *      - Perform 10 trainings, using 9 sets for the training and 1 for testing
 *
 * 4) Print to stdout:
 *      - Accuracy for every of the 10 trainings and their arithmetic mean
 *
 *  The fundamental Naive Bayes assumption is that each feature makes an:
 *      - independent
 *      - equal
 *  contribution to the outcome.
 *
 *  To overcome the 'zero frequency problem', add one to the count for every attribute
**/

//==============================================================================
// INCLUDED FILES
//==============================================================================
#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <unordered_map>
#include <algorithm>

using namespace std;

//==============================================================================
// CONFIGURATION
//==============================================================================
static const int FEATURES_COUNT = 16; // the number of properties of one entry in the data set
static const int VALIDATION_SUBSETS_COUNT = 10;

//==============================================================================
// IMPLEMENTATION
//==============================================================================

enum FeatureValue {
    YES, NO, UNKNOWN
};
enum Response {
    REPUBLICAN, DEMOCRAT
};

struct FeatureValueHash {
    template<typename T>
    std::size_t operator()(T t) const {
        return static_cast<std::size_t>(t);
    }
};

Response string2Response(const string &entryClass) {
    if (entryClass == "republican") {
        return REPUBLICAN;
    } else if (entryClass == "democrat") {
        return DEMOCRAT;
    } else {
        printf("string2Response: Unknown class <%s>!", entryClass.c_str());
        exit(EXIT_FAILURE);
    }
}

string Response2string(Response response) {
    switch (response) {
        case REPUBLICAN:
            return string("republican");
        case DEMOCRAT:
            return string("democrat");
        default:
            cout << "Response2string: Unknown class!" << endl;
            exit(EXIT_FAILURE);
    }
}

struct Entry {
    vector<FeatureValue> raw;
    Response response;

    Entry(int propertiesCount) : raw(vector<FeatureValue>(propertiesCount)) {

    }

    friend istream &operator>>(istream &is, Entry &entry) {
        string entryClass;
        std::getline(is, entryClass, ',');
        entry.response = string2Response(entryClass);

        for (int i = 0; i < entry.raw.size(); i++) {
            char comma;
            char answer;

            is >> answer >> std::noskipws >> comma;

            if (answer == 'y') {
                entry.raw[i] = FeatureValue::YES;
            } else if (answer == 'n') {
                entry.raw[i] = FeatureValue::NO;
            } else {
                entry.raw[i] = FeatureValue::UNKNOWN;
            }
        }

        return is;
    }

    friend ostream &operator<<(ostream &os, Entry &entry) {
        for (int i = 0; i < entry.raw.size(); i++) {
            os << entry.raw[i] << ",";
        }
        os << Response2string(entry.response);
        return os;
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

        Entry entry(FEATURES_COUNT);
        inFile >> entry;

        dataset.push_back(entry);
    }

    inFile.close();
}

using PRepublican = unordered_map<FeatureValue, float, FeatureValueHash>;
using PDemocrat = unordered_map<FeatureValue, float, FeatureValueHash>;
using Probabilities = vector<pair<PRepublican, PDemocrat>>;

Probabilities calculateProbabilities(vector<Entry> &database) {
    Probabilities probabilities(FEATURES_COUNT);

    // this is to overcome the zero frequency problem
    for (int i = 0; i < FEATURES_COUNT; i++) {
        probabilities[i].first[YES] = 1;
        probabilities[i].first[NO] = 1;
        probabilities[i].first[UNKNOWN] = 1;

        probabilities[i].second[YES] = 1;
        probabilities[i].second[NO] = 1;
        probabilities[i].second[UNKNOWN] = 1;
    }

    int republicansCount = 1;
    int democratsCount = 1;

    for (auto entry : database) {
        if (entry.response == DEMOCRAT) {
            democratsCount++;
        } else {
            republicansCount++;
        }

        for (int i = 0; i < FEATURES_COUNT; i++) {
            if (entry.response == REPUBLICAN) {
                probabilities[i].first[entry.raw[i]]++;
            } else {
                probabilities[i].second[entry.raw[i]]++;
            }
        }
    }

    for (auto v : probabilities) {
        for (auto e : v.first) {
            e.second /= republicansCount++;
        }
        for (auto e : v.second) {
            e.second /= democratsCount++;
        }
    }

    return probabilities;
}

Response classify(Probabilities &probabilities, Entry &entry) {

    double PEntryDemocrat = 1;
    double PEntryRepublican = 1;
    for (int i = 0; i < FEATURES_COUNT; i++) {
        PEntryRepublican *= probabilities[i].first[entry.raw[i]];
        PEntryDemocrat *= probabilities[i].second[entry.raw[i]];
    }

    double PDemocrat = PEntryDemocrat / (PEntryDemocrat + PEntryRepublican);
    double PRepublican = PEntryRepublican / (PEntryDemocrat + PEntryRepublican);

    if (PDemocrat > PRepublican) {
        return DEMOCRAT;
    } else {
        return REPUBLICAN;
    }

}

int main() {
    srand(time(0));

    vector<Entry> database;
    ReadDatabase(database, "./../Data/house-votes-84.data");
    random_shuffle(database.begin(), database.end());

    double accuraciesSum = 0;
    int setSize = database.size() / VALIDATION_SUBSETS_COUNT;
    for (int i = 0; i < setSize * VALIDATION_SUBSETS_COUNT; i += setSize) {
        int startIdx = i;
        int endIdx = (i + setSize >= database.size()) ? database.size() - 1 : i + setSize;

        vector<Entry> testingSet(database.begin() + startIdx, database.begin() + endIdx);
        vector<Entry> trainingSet(database.begin(), database.begin() + startIdx);

        trainingSet.insert(trainingSet.end(), database.begin() + endIdx, database.end());

        Probabilities probabilities = calculateProbabilities(trainingSet);

        int accurateCount = 0;
        for (auto entry : testingSet) {
            Response response = classify(probabilities, entry);
            if (entry.response == response) {
                accurateCount++;
            }
        }
        double accuracy = (float) accurateCount / testingSet.size() * 100;
        accuraciesSum += accuracy;
        cout << "Test" << (i / setSize) + 1 << " accuracy: " << accuracy << "%" << endl;
    }

    cout << "\n\nMean accuracy: " << accuraciesSum / VALIDATION_SUBSETS_COUNT << "%" << endl;
    system("PAUSE");

    return 0;
}