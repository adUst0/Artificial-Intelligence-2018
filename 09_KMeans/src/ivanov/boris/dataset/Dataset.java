package ivanov.boris.dataset;

import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Dataset {
    private List<Point2D> entries = new ArrayList<>();

    public Dataset() {}

    public static Dataset fromFile(String fileName) {
        Dataset dataset = new Dataset();

        try (BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(new FileInputStream(fileName)))) {
            String line = bufferedReader.readLine();
            while (line != null) {
                if (line.isEmpty()) {
                    line = bufferedReader.readLine();
                    continue;
                }

                List<String> entry = Arrays.asList(line.split("\\s+"));
                dataset.addEntry(Double.parseDouble(entry.get(0)), Double.parseDouble(entry.get(1)));

                line = bufferedReader.readLine();
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return dataset;
    }

    public void addEntry(double x, double y) {
        entries.add(new Point2D(x, y));
    }

    public int size() {
        return entries.size();
    }

    public List<Point2D> getEntries() {
        return entries;
    }
}
