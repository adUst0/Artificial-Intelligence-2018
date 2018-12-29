package ivanov.boris;

import ivanov.boris.dataset.Dataset;
import ivanov.boris.dataset.Point2D;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

/* Pseudo-code for KMeans (http://stanford.edu/~cpiech/cs221/handouts/kmeans.html):
------------------------------------------------------------------------------------------------------------------------
# Function: K Means
# -------------
# K-Means is an algorithm that takes in a dataset and a constant
# k and returns k centroids (which define clusters of data in the
# dataset which are similar to one another).
def kmeans(dataSet, k):

    # Initialize centroids randomly
    numFeatures = dataSet.getNumFeatures()
    centroids = getRandomCentroids(numFeatures, k)

    # Initialize book keeping vars.
    iterations = 0
    oldCentroids = None

    # Run the main k-means algorithm
    while not shouldStop(oldCentroids, centroids, iterations):
        # Save old centroids for convergence test. Book keeping.
        oldCentroids = centroids
        iterations += 1

        # Assign labels to each datapoint based on centroids
        labels = getLabels(dataSet, centroids)

        # Assign centroids based on datapoint labels
        centroids = getCentroids(dataSet, labels, k)

    # We can get the labels too by calling getLabels(dataSet, centroids)
    return centroids
------------------------------------------------------------------------------------------------------------------------
# Function: Should Stop
# -------------
# Returns True or False if k-means is done. K-means terminates either
# because it has run a maximum number of iterations OR the centroids
# stop changing.
def shouldStop(oldCentroids, centroids, iterations):
    if iterations > MAX_ITERATIONS: return True
    return oldCentroids == centroids
------------------------------------------------------------------------------------------------------------------------
# Function: Get Labels
# -------------
# Returns a label for each piece of data in the dataset.
def getLabels(dataSet, centroids):
    # For each element in the dataset, chose the closest centroid.
    # Make that centroid the element's label.
------------------------------------------------------------------------------------------------------------------------
# Function: Get Centroids
# -------------
# Returns k random centroids, each of dimension n.
def getCentroids(dataSet, labels, k):
    # Each centroid is the geometric mean of the points that
    # have that centroid's label. Important: If a centroid is empty (no points have
    # that centroid's label) you should randomly re-initialize it.
------------------------------------------------------------------------------------------------------------------------
 */

// TODO
// This implementation should be refactored in order to work with any Dataset, not only the current assignment
public class KMeansAlgorithm {

    private static final int MAX_ITERATIONS = Short.MAX_VALUE;

    public static List<Point2D> KMeans(Dataset dataset, int k) {

        List<Point2D> centroids = getRandomCentroids(dataset, k);

        int iterations = 0;
        List<Point2D> oldCentroids = null;

        while (!shouldStop(oldCentroids, centroids, iterations)) {
            oldCentroids = centroids;
            iterations++;

            // Assign labels to each datapoint based on centroids
            List<Point2D> labels = getLabels(dataset, centroids);

            // Assign centroids based on datapoint labels
            centroids = getCentroids(dataset, labels, centroids);
        }

        return centroids;
    }

    private static List<Point2D> getCentroids(Dataset dataset, List<Point2D> labels, List<Point2D> oldCentroids) {
        List<Point2D> centroids = new ArrayList<>();

        List<Point2D> entries = dataset.getEntries();

        for (int i = 0; i < oldCentroids.size(); i++) {
            Point2D current = oldCentroids.get(i);
            double sumX = 0;
            double sumY = 0;
            double count = 0;

            for (int j = 0; j < labels.size(); j++) {
                if (labels.get(j).equals(current)) {
                    sumX += entries.get(j).x;
                    count++;
                    sumY += entries.get(j).y;
                }
            }

            if (count == 0) {
                centroids.add(new Point2D(Math.random(), Math.random()));
            } else {
                centroids.add(new Point2D(sumX / count, sumY / count));
            }
        }

        return centroids;
    }

    private static List<Point2D> getLabels(Dataset dataset, List<Point2D> centroids) {
        List<Point2D> labels = new ArrayList<>();

        List<Point2D> entries = dataset.getEntries();
        for (int i = 0; i < entries.size(); i++) {
            labels.add(getClosestCentroid(entries.get(i), centroids));
        }

        return labels;
    }

    private static Point2D getClosestCentroid(Point2D point, List<Point2D> centroids) {
        int closest = 0;
        double minDistance = Double.MAX_VALUE;

        for (int i = 0; i < centroids.size(); i++) {
            double distance = Math.sqrt(Math.pow((point.x - centroids.get(i).x), 2) +
                    Math.pow((point.y - centroids.get(i).y), 2));
            if (distance < minDistance) {
                minDistance = distance;
                closest = i;
            }
        }

        return centroids.get(closest);
    }

    private static boolean shouldStop(List<Point2D> oldCentroids, List<Point2D> centroids, int iterations) {
        if (iterations > MAX_ITERATIONS) {
            return true;
        }

        if (oldCentroids == null) {
            return false;
        }

        if (oldCentroids.equals(centroids)) {
            // System.out.println("Centroids have converged!");
            return true;
        }

        return false;
    }

    private static List<Point2D> getRandomCentroids(Dataset dataset, int centroidsCounts) {
        List<Point2D> centroids = new ArrayList<>();

        Random rand = new Random();

        for (int i = 0; i < centroidsCounts; i++) {

            Point2D point2D = dataset.getEntries().get(rand.nextInt(dataset.getEntries().size()));

            centroids.add(new Point2D(point2D));
        }

        return centroids;
    }
}
