
// A Java program for Dijkstra's single source shortest path algorithm.
// The program is for adjacency matrix representation of the graph
import java.util.*;
import java.lang.*;
import java.io.*;

class ShortestPath {
    int minDistance(int dist[], Boolean sptSet[]) { // minimum distance value vertex
        int min = Integer.MAX_VALUE;
        int min_index = -1;

        for (int v = 0; v < dist.length; v++)
            if (sptSet[v] == false && dist[v] <= min) {
                min = dist[v];
                min_index = v;
            }

        return min_index;
    }

    //Dijkstra's algorithm
    int[] dijkstra(int graph[][], int source) {
        int distances[] = new int[graph.length];      //holds the shortest distance from src to i
        Boolean shortestPathTree[] = new Boolean[graph.length]; //True if vertex i is in the shortest path tree

        for (int i = 0; i < graph.length; i++) {
            distances[i] = Integer.MAX_VALUE;
            shortestPathTree[i] = false;
        }

        distances[source] = 0; //distance from source to source is 0

        // Find shortest path for all vertices
        for (int count = 0; count < graph.length - 1; count++) {
            int u = minDistance(distances, shortestPathTree);  // minimum distance vertex from the unprocessed vertices
            shortestPathTree[u] = true; //picked vertex

            // Update distance value of the adjacent vertices of the
            // picked vertex.
            for (int v = 0; v < graph.length; v++)

                // Update dist[v] only if is not in sptSet, there is an
                // edge from u to v, and total weight of path from src to
                // v through u is smaller than current value of dist[v]
                if (!shortestPathTree[v] && graph[u][v] != 0 && distances[u] != Integer.MAX_VALUE && distances[u] + graph[u][v] < distances[v])
                    distances[v] = distances[u] + graph[u][v];
        }
        return distances;
    }
}