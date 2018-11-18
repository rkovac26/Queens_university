import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Arrays;
import java.util.LinkedList;

public class Uber_alg {

    public static int[][] findShortestPaths(String network[][])throws FileNotFoundException, IOException{
        int graph[][] = new int[network.length][network.length];
        int shortestPaths[][] = new int[network.length][network.length];
        //Change strings in network to integers
        for (int i = 0; i < network.length; i++) {
            for (int j = 0; j < network.length; j++) {
                graph[i][j] = Integer.parseInt(network[i][j]);
            }
        }
        //Finds the shortest path for each node
        for (int i = 0; i < network.length; i++) {
            ShortestPath t = new ShortestPath();
            int dist[] = new int[network.length];
            dist = t.dijkstra(graph, i);
            for (int j = 0; j < network.length; j++) {
                shortestPaths[i][j]=dist[j];
            }
        }
        return shortestPaths;
        
    }
    
    public static int totalTime(String requests[][], int network[][]){
        int currentTime = 0;
        int waitTime = 0;
        int requestsInt[][] = new int[requests.length][requests[0].length];
        for (int i = 0; i < requests.length; i++) {
            for (int j = 0; j < requests[0].length; j++) {
                requestsInt[i][j] = Integer.parseInt(requests[i][j]);
            }
        }
        int i = 0;
        
        //Assuming the uber starts at first vertex, i.e. graph[0][0]
        waitTime = network[0][requestsInt[0][1]-1];
        currentTime = waitTime;

        while(true){
            currentTime+= network[requestsInt[i][1]-1][requestsInt[i][2]-1];
            if(i==requests.length-1){
                break;
            }
            i++;
            if(currentTime<requestsInt[i][0]){
                currentTime = requestsInt[i][0];
            }
            currentTime+= network[requestsInt[i-1][2]-1][requestsInt[i][1]-1];
            waitTime += currentTime - requestsInt[i][0]; 
        }
        
        return waitTime;
    }
    

    public static void main(String args[]) throws FileNotFoundException, IOException {
        String[][] requests = new String[300][3];
        requests = CsvParser.readCSV("c:\\Users\\Rastiq\\Downloads\\requests.csv");
        String[][] network = new String[50][50];
        network = CsvParser.readCSV("c:\\Users\\Rastiq\\Downloads\\network.csv");
        String[][] newrequests = new String[300][3];
        newrequests = CsvParser.readCSV("c:\\Users\\Rastiq\\Downloads\\newrequests.csv");
        String[][] newnetwork = new String[50][50];
        newnetwork = CsvParser.readCSV("c:\\Users\\Rastiq\\Downloads\\newnetwork.csv");

        int result = totalTime(requests,(findShortestPaths(network)));
        int new_result = totalTime(newrequests,(findShortestPaths(newnetwork)));
        System.out.printf("The combined waiting time of all customers of the first dataset is %d time units.\n", result);
        System.out.printf("The combined waiting time of all customers of the second dataset is %d time units.\n", new_result);
    }
}
