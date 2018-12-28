package ivanov.boris.dataset;

import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Dataset {
    private List<DatasetEntry> entries = new ArrayList<>();

    private String positiveResponse;
    private String negativeResponse;

    public Dataset(String positive, String negative) {
        this.positiveResponse = positive;
        this.negativeResponse = negative;
    }

    public Dataset(List<DatasetEntry> entries, String positiveResponse, String negativeResponse) {
        this(positiveResponse, negativeResponse);
        this.entries = entries;
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

                line = line.replaceAll("'", "");

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

    public void addEntry(DatasetEntry entry) {
        entries.add(entry);
    }

    public int size() {
        return entries.size();
    }

    public int getAttributesCount() {
        if (entries.size() == 0) {
            return 0;
        }

        return entries.get(0).getAttributes().size();
    }

    public String getPositiveResponse() {
        return positiveResponse;
    }

    public String getNegativeResponse() {
        return negativeResponse;
    }

    public List<DatasetEntry> getEntries() {
        return entries;
    }

    public int getTotalPositiveResponses() {
        return (int) entries.stream().filter(x -> x.getResponse().equals(positiveResponse)).count();
    }
    public int getTotalNegativeResponses() {
        return (int) entries.stream().filter(x -> x.getResponse().equals(negativeResponse)).count();
    }

    public List<String> getAttributeAllPossibleValues(int attribute) {
        if (attribute < 0 || attribute >= getAttributesCount()) {
            throw new IllegalArgumentException();
        }

        List<String> possibleValues = new ArrayList<>();

        for (DatasetEntry entry : entries) {
            List<String> attributes = entry.getAttributes();

            if (!possibleValues.contains(attributes.get(attribute))) {
                possibleValues.add(attributes.get(attribute));
            }
        }

        return possibleValues;
    }

    public Dataset getSubsetWithAttributeValue(int selectedAttribute, String value) {
        Dataset dataset = new Dataset(positiveResponse, negativeResponse);
        for (DatasetEntry entry : entries) {
            List<String> attributes = entry.getAttributes();

            if (attributes.get(selectedAttribute).equals(value)) {
                dataset.addEntry(new DatasetEntry(entry));
            }
        }

        return dataset;
    }
}
