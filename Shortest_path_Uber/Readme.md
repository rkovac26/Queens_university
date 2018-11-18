# File description
- CsvParser.java loads the data into a 2D string array
- ShortestPath.java contains the implementation of the Dijkstra algorithm
- .csv files holds the information about the grid, where the driver can move and the starting and goal destinations of customers

# Goal
Find a path such that the total amount of waiting time of all customers is minimal

# Execution time and accuracy
  Firstly, since the adjacency matrix has been used for the Dijkstra’s algorithm and we must go through the entire matrix to find the adjacent vertices, the time complexity of the implementation is O(V^2). This implementation is used in the final algorithm where the shortest paths of every vertex is found therefore the final complexity is O(V^3). However, since the map of cities does not change frequently, the matrix of shortest path need to be created only once and the time to get from one vertex to another can be found in O(1) time. Therefore, if there are n customers, we can get the total waiting time in O(n), and finding out for how long would 1 extra customer needed to wait would cost O(1).
	Secondly, since the Dijkstra’s algorithm has been proved to find the shortest path from a given source vertex and we operate with only one driver, it can be concluded that the total waiting time is the lowest possible.

