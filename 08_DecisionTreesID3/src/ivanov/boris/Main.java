package ivanov.boris;

import ivanov.boris.dataset.Dataset;
import ivanov.boris.dataset.DatasetEntry;
import ivanov.boris.id3.ID3DecisionTree;
import ivanov.boris.tree.TreeNode;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class Main {

    private static final int VALIDATION_SUBSETS_COUNT = 10;

    public static void main(String[] args) throws IOException {
        Dataset dataset = Dataset.fromFile("Data/breast-cancer.arff",
                "recurrence-events", "no-recurrence-events");
        Collections.shuffle(dataset.getEntries());
        List<Integer> attributes =
                IntStream.range(0, dataset.getAttributesCount()).boxed().collect(Collectors.toList());

        double accuraciesSum = 0;
        int setSize = dataset.size() / VALIDATION_SUBSETS_COUNT;
        for (int i = 0; i < setSize * VALIDATION_SUBSETS_COUNT; i+= setSize) {
            int startIdx = i;
            int endIdx = (i + setSize >= dataset.size()) ? dataset.size() - 1 : i + setSize;
            List<DatasetEntry> testingSet = new ArrayList<>(dataset.getEntries().subList(startIdx, endIdx));

            List<DatasetEntry> trainingSet = new ArrayList<>(dataset.getEntries().subList(0, startIdx));
            trainingSet.addAll(dataset.getEntries().subList(endIdx, dataset.size()));

            TreeNode tree = ID3DecisionTree.createTree(new Dataset(trainingSet,
                    "recurrence-events", "no-recurrence-events"), attributes);

            int accurateCount = 0;
            for (DatasetEntry entry : testingSet) {
                String response = ID3DecisionTree.classify(tree, entry);
                if (entry.getResponse().equals(response)) {
                    accurateCount++;
                }
            }
            double accuracy = (double) accurateCount / testingSet.size() * 100;
            accuraciesSum += accuracy;
            System.out.println("Test" + (i / setSize) + 1 + " accuracy: " + accuracy + "%");
        }
        System.out.println("\n\nMean accuracy: " + accuraciesSum / VALIDATION_SUBSETS_COUNT + "%");

    }

}
