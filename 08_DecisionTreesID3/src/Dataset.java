import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class Dataset {
    private List<DatasetEntry> entries = new ArrayList<>();
    private String positiveResponse;
    private String negativeResponse;

    static final int POSITIVE_VALUE = -1;
    static final int NEGATIVE_VALUE = -2;

    private static double log2(double x) {
        return Math.log(x) / Math.log(2);
    }

    private static double entropy(int x, int y) {
        if (x == 0 || y == 0) {
            return 0;
        }

        double pX = (double) x / (x + y);
        double pY = (double) y / (x + y);
        double entropy = 0;

        entropy = - (pX * log2(pX)) - (pY * log2(pY));

        return entropy;
    }

    // This method calculates the Entropy of E(Response, attributeIndex)
    private double entropy(int attributeIndex) {

        List<String> possibleValues = new ArrayList<>(); // Possible values for this attribute
        List<Integer> positiveCount = new ArrayList<>(); // Positive responses for a particular value of the attribute
        List<Integer> negativeCount = new ArrayList<>(); // Negative responses for a particular value of the attribute

        for (DatasetEntry entry : entries) {
            String attributeValue = entry.getAttributes().get(attributeIndex);
            int valueIndex = possibleValues.indexOf(attributeValue);

            if (valueIndex == -1) {
                possibleValues.add(attributeValue);
                positiveCount.add(0);
                negativeCount.add(0);
                valueIndex = (possibleValues.indexOf(attributeValue));
            }

            if (entry.getResponse().equals(positiveResponse)) {
                positiveCount.set(valueIndex, positiveCount.get(valueIndex) + 1);
            }
            else {
                negativeCount.set(valueIndex, negativeCount.get(valueIndex) + 1);
            }
        }

        double entropy = 0;
        int totalExamples = entries.size();

        for (int i = 0; i < possibleValues.size(); i++) {
            int positive = positiveCount.get(i);
            int negative = negativeCount.get(i);
            entropy += (double) (positive + negative) / totalExamples * entropy(positive, negative);
        }

        return entropy;
    }

    public Dataset(String positive, String negative) {
        this.positiveResponse = positive;
        this.negativeResponse = negative;
    }

    public static Dataset fromFile(String fileName, String positiveResponse, String negativeResponse) {
        Dataset dataset = new Dataset(positiveResponse, negativeResponse);

        try (BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(new FileInputStream(fileName)))) {
            String line = bufferedReader.readLine();
            while (line != null) {
                if (line.isEmpty() || line.charAt(0) == '@' || line.charAt(0) == '%') {

                    line = bufferedReader.readLine();
                    continue;
                }

                //line = line.replaceAll("'", "");

                List<String> entry = Arrays.asList(line.split(","));
                String response = entry.get(entry.size() - 1);
                dataset.addEntry(new DatasetEntry(entry.subList(0, entry.size() - 1), response));

                line = bufferedReader.readLine();
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return dataset;
    }

    // This method calculates the Gain of G(Response, attributeIndex)
    public double gain(int attributeIndex) {
        int totalPositiveResponses = (int) entries.stream().filter(x -> x.getResponse().equals(positiveResponse)).count();
        int totalNegativeResponses = (int) entries.stream().filter(x -> x.getResponse().equals(negativeResponse)).count();

        return entropy(totalNegativeResponses, totalPositiveResponses) - entropy(attributeIndex);
    }

    public void addEntry(DatasetEntry entry) {
        entries.add(entry);
    }

    public boolean areAllEntriesPositive() {
        return entries.stream().filter(x -> x.getResponse().equals(positiveResponse)).count() == entries.size();
    }

    public boolean areAllEntriesNegative() {
        return entries.stream().filter(x -> x.getResponse().equals(negativeResponse)).count() == entries.size();
    }

    public int getMostCommonValue() {
        int totalPositiveResponses = (int) entries.stream().filter(x -> x.getResponse().equals(positiveResponse)).count();
        int totalNegativeResponses = (int) entries.stream().filter(x -> x.getResponse().equals(negativeResponse)).count();

        return (totalPositiveResponses > totalNegativeResponses) ? POSITIVE_VALUE : NEGATIVE_VALUE;
    }

    public int getBestAttribute(List<Integer> attributes) {
        if (entries.isEmpty() || attributes.isEmpty()) {
            return -1;
        }

        int bestAttributeIndex = attributes.get(0);
        double bestGain = 0;

        for (int index : attributes) {
            double currentGain = gain(index);
            if (currentGain > bestGain) {
                bestAttributeIndex = index;
                bestGain = currentGain;
            }
        }

        return bestAttributeIndex;
    }

    public List<String> getPossibleValues(int attribute) {
        List<String> possibleValues = new ArrayList<>();

        for (DatasetEntry entry : entries) {
            List<String> attributes = entry.getAttributes();

            if (!possibleValues.contains(attributes.get(attribute))) {
                possibleValues.add(attributes.get(attribute));
            }
        }

        return possibleValues;
    }

    public int size() {
        return entries.size();
    }

    public Dataset getSubsetWithValue(int selectedAttribute, String value) {
        Dataset dataset = new Dataset(positiveResponse, negativeResponse);
        for (DatasetEntry entry : entries) {
            List<String> attributes = entry.getAttributes();

            if (attributes.get(selectedAttribute).equals(value)) {
                dataset.addEntry(new DatasetEntry(entry));
            }
        }

        return dataset;
    }

    public int getAttributesCount() {
        if (entries.size() == 0) {
            return 0;
        }

        return entries.get(0).getAttributes().size();
    }

    public static void main(String[] args) throws IOException {
        //Dataset.fromFile("Data/breast-cancer.arff");
        Dataset dataset = Dataset.fromFile("Data/playgolf.data", "Yes", "No");
        List<Integer> attributes =
                IntStream.range(0, dataset.getAttributesCount()).boxed().collect(Collectors.toList());
        TreeNode tree = ID3DecisionTree.createTree(dataset, attributes);

        tree.printBFS();
    }
}
