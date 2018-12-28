package ivanov.boris.dataset;

import java.util.ArrayList;
import java.util.List;

public class DatasetEntry {
    private List<String> attributes;
    private String response;

    public DatasetEntry(List<String> attributes, String response) {
        this.attributes = attributes;
        this.response = response;
    }

    public DatasetEntry(DatasetEntry other) {
        this.response = other.response;
        attributes = new ArrayList<>(other.attributes);
    }

    public List<String> getAttributes() {
        return attributes;
    }

    public String getResponse() {
        return response;
    }
}
