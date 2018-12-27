import java.util.List;

public class ID3DecisionTree {

    public static TreeNode createTree(Dataset dataset, List<Integer> attributes) {
        TreeNode currentNode = new TreeNode();

        if (dataset.areAllEntriesPositive()) {
            currentNode.setValue(Dataset.POSITIVE_VALUE);
            return currentNode;
        }
        if (dataset.areAllEntriesNegative()) {
            currentNode.setValue(Dataset.NEGATIVE_VALUE);
            return currentNode;
        }
        if (attributes.size() == 0) {
            currentNode.setValue(dataset.getMostCommonValue());
            return currentNode;
        }

        int selectedAttribute = dataset.getBestAttribute(attributes);
        currentNode.setValue(selectedAttribute);

        for (String value : dataset.getPossibleValues(selectedAttribute)) {
            Dataset subset = dataset.getSubsetWithValue(selectedAttribute, value);
            if (subset.size() != 0) {
                attributes.remove((Integer)selectedAttribute);
                TreeNode child = currentNode.addChild(createTree(subset, attributes));
                child.setEdge(value);
            }
        }

        return currentNode;
    }
}
