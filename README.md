# CS404.Project
Shortest Path implementations of Floyd-Warshall and Dijkstra's Algorithms

UMKC Fall 2016, Dr. Appie van de Liefvoort

**Summary**  
Finding an optimal path in a network is not always the shortest path, shortest distance, or cheapest cost, but rather: most
scenic, least traffic, get-away path, and so on. This project explores finding a *SneakyPath*.  

**Motivation**  
Commercial Airlines, roads, and computer and communication networks often model their routes as graphs. There are other
examples in social networks and cloud computing, and so on. In such a network, nodes are the cities, or intersections, or
server nodes (sources of demand, such as traffic or communication sites), and the links connect hem. If you want to go
from one node to another (by plane, by car, or routing internet traffic or information packet), then you need a plan: source,
destination, and a planned path in between. Let us explain this in terms of an road network between cities as the motivating
application, but keep in mind that this project might have additional uses. When planning a car-trip, say from *a* to *b*, most
people would want to find the shortest route, and there are several algorithms known that will help you find the shortest path.
But this may not always be the criterion used. Some people may be interested in the least travelled road, to avoid as much
congestion as possible. They want to avoid other cars as much as possible, they do not want to see other cars, they do not
other cars to see them, or whatever. May be you are towing a 12-foot wide boat, and you do not want to inconvenience other
drivers, so you look for the least travelled road. Even in computer networks this might be of interest: lowest probability of
dropped packets. We will call such a path a <i>SneakyPath</i>.</p>

You will be given a number of different input scenarios. Each scenario will have

* The size *n* of the system (the total number of cities), and  
* An adjacency matrix *E* with edge-weights. The (i,j)th entry E[i,j] represents the traveling time on the (direct) edge
between the two specific nodes, *i* and *j*.  

The matrix *E* will allow you to compute the shortest path from *a* to *b*, and several algorithms could be used for this. The
shortest path might take several edges (or links), and if you are the only one on the road, then this is the path you will take.
However, you are not the only person/car on the road. You will also be given

* A flow matrix *F*, whose (i,j)th entry F[i,j] represents the number of (other) cars that travel the entire path from node
*i* to node *j* every hour.</p>

For the sake of argument, say that there are F[21,47] = 115 cars traveling on the path from node v21 to node v47, then
there are 115 cars on each edge of that path. Suppose edge (v34, v38) is on the shortest path from node v21 to node v47,
then there are also 115 cars/hour on the edge (v34, v38) due to the flow F[21,47]. But is very well possible that the edge
(v34, v38) is also on the shortest path from node v17 to node v29 , and that F[17,29] = 75. This means that there are at least
190 cars/hour on the edge (v34, v38). Thus for each edge (i,j), you can calculate the actual carried traffic load from all
source-destination pairs on that edge, because everybody takes their shortest path. Put all this information in a new matrix
*L*, each entry representing the total load on the edge. Finally, you will be given

* the starting node *a* and the terminal node *b*  

and for this particular pair, you are asked to calculate (and print)

* The SneakyPath from *a* to *b*, such that the total number of other cars on the road encountered is as small as possible,  
* the edge on this SneakyPath with the lowest number of other cars,  
* the edge on this SneakyPath with the highest number of other cars, and  
* the average number of other cars on the SneakyPath, averaged over the number of links on the path.
