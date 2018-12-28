package ivanov.boris.tree;

public class LeafNode extends TreeNode {

    private String response;

    public LeafNode(String response) {
        this.response = response;
    }

    @Override
    public String toString() {
        return response;
    }
}
