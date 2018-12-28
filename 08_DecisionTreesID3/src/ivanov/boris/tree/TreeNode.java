package ivanov.boris.tree;

import java.util.*;

public class TreeNode {
    private List<TreeNode> children = new ArrayList<>();
    private int value = -1; // index of the attribute
    private String edge = null; // value of the edge between parent node and this node

    public TreeNode() {}

    public TreeNode(int value) {
        this.value = value;
    }

    @Override
    public String toString() {
        return String.valueOf(value);
    }

    public TreeNode getChild(int index) {
        return children.get(index);
    }

    public List<TreeNode> getChildren() {
        return children;
    }

    public TreeNode addChild(TreeNode node) {
        children.add(node);
        return node; // return reference to the child
    }

    public String getEdge() {
        return edge;
    }

    public void setEdge(String edge) {
        this.edge = edge;
    }

    public int getValue() {
        return value;
    }

    public void setValue(int value) {
        this.value = value;
    }

    public void printDFS() {
        Stack<TreeNode> s = new Stack<>();
        List<TreeNode> visited = new ArrayList<>();

        s.push(this);

        while (!s.empty()) {
            TreeNode node = s.pop();
            System.out.println(node.getValue());

            if (!visited.contains(node)) {
                visited.add(node);

                for (TreeNode child : node.getChildren()) {
                    if (!visited.contains(child)) {
                        s.push(child);
                    }
                }
            }
        }
    }

    public void printBFS() {
        Queue<TreeNode> q = new LinkedList<>();
        Queue<TreeNode> parents = new LinkedList<>();
        Queue<Integer> level = new LinkedList<>();
        List<TreeNode> visited = new ArrayList<>();

        q.add(this);
        parents.add(new TreeNode());
        level.add(0);

        while (!q.isEmpty()) {
            TreeNode node = q.poll();
            TreeNode parent = parents.poll();
            int currentLevel = level.poll();
            visited.add(node);
            System.out.println("Level: " + currentLevel + ": " + "Value: " +
                    node.toString() + " (Parent: " + parent.getValue() + "; Edge: " + node.getEdge() + ")");

            for (TreeNode child : node.getChildren()) {
                q.add(child);
                parents.add(node);
                level.add(currentLevel + 1);
            }
        }
    }
}
