import java.util.*;

public class TreeNode {
    private List<TreeNode> children = new ArrayList<>();
    private int value = -1;
    private String edge = null;

    public TreeNode() {}

    public TreeNode(int value) {
        this.value = value;
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
        Queue<Integer> level = new LinkedList<>();
        List<TreeNode> visited = new ArrayList<>();

        q.add(this);
        level.add(0);

        while (!q.isEmpty()) {
            TreeNode node = q.poll();
            int currentLevel = level.poll();
            visited.add(node);
            System.out.println(currentLevel + ": " + node.getValue());

            for (TreeNode child : node.getChildren()) {
                q.add(child);
                level.add(currentLevel + 1);
            }
        }
    }

    public static void main(String[] args) {
        TreeNode treeNode = new TreeNode();
        treeNode.setValue(2);

        treeNode.addChild(new TreeNode(1));
        treeNode.addChild(new TreeNode(4));
        treeNode.addChild(new TreeNode(7));

        treeNode.getChild(1).addChild(new TreeNode(6));
        treeNode.getChild(2).addChild(new TreeNode(8));

        treeNode.getChild(2).getChild(0).addChild(new TreeNode(9));
        treeNode.getChild(2).getChild(0).addChild(new TreeNode(10));
        treeNode.getChild(2).getChild(0).addChild(new TreeNode(11));

        treeNode.printDFS();
        System.out.println("\n\n");
        treeNode.printBFS();
    }
}
