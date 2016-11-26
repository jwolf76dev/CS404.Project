#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stack>
#include <algorithm>
#include <vector>
#include <iterator>
#include <functional>
#include <utility>
#include <list>
#include <queue>

using namespace std;

typedef int node;
typedef int dist;

// structure for nodes
struct ANode {
	node Node;
	dist Distance;

	ANode() {
		Node = 0;
		Distance = 0;
	}

	ANode(node v, dist d) {
		Node = v;
		Distance = d;
	}

	int getNode() const { return Node; }
	int getDist() const { return Distance; }
};

// overload comparison operator for min heap
struct Compare {
	int operator()(const ANode n1, const ANode n2) {
		return n1.getDist() > n2.getDist();
	}
};


int main() {

	/*	Input file format:
		n, a, b
		m, i, j, w
		...
		m, i, j, w

		Where:
		n = number of nodes
		a = start node
		b = end node
		m = matrix assignment (Edge or Flow)
		i = row index
		j = column index
		w = edge weight
	*/

	// open output file stream
	ofstream fout;
	fout.open("SneakyPathOutput.txt", ios::app);
		
	// open file stream and input file
	char delim;
	ifstream fin;
	string fileName = "CS404FS16SneakyPathInputN10d.txt";
	fin.open(fileName);
	string line;
	// exit if unable to open file
	if (fin.fail()) {
		cerr << "Unable to open input file. ";
		system("pause");
		return 1;
	}

	// initialize node variables
	char c; // placeholder for delimiter
	int n = 0;
	int startNode = 0;
	int endNode = 0;
	ANode SelMin;

	// read in first line of file
	getline(fin, line);
	istringstream data(line);
	data >> n >> c >> startNode >> c >> endNode;

	// verify input on console
	cout << "Matrix setup information:" << endl << endl;
	cout << n << ", " << startNode << ", " << endNode << endl << endl;

	// adjust nodes for index references
	startNode -= 1;
	endNode -= 1;

	// make number of nodes constant
	const int SIZE = n;

	// verify adjustments on console
	cout << "Index-adjusted values:" << endl << endl;
	cout << n << ", " << startNode << ", " << endNode << endl << endl;

	// initialize matrix variables
	char m;
	int i;
	int j;
	int w;
	const int INF = numeric_limits<int>::max() / 6;

	// build arrays dynamically
	// E[] = n x n array of edge weights
	int **E = new int *[SIZE];
	for (i = 0; i < SIZE; i++) {
		E[i] = new int[SIZE];
	}
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			E[i][j] = INF;  // unknown distance between nodes
		}
	}

	// F[] = n x n array of flow between nodes
	int **F = new int *[SIZE];
	for (i = 0; i < SIZE; i++) {
		F[i] = new int[SIZE];
	}
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			F[i][j] = 0; // no known flow between nodes
		}
	}

	// next[] = n x n array of vertex indices
	int **next = new int *[SIZE];
	for (i = 0; i < SIZE; i++) {
		next[i] = new int[SIZE];
	}
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			next[i][j] = -1; // next hop unknown
		}
	}

	// L[] = n x n array of total flow per edge
	int **L = new int *[SIZE];
	for (i = 0; i < SIZE; i++) {
		L[i] = new int[SIZE];
	}
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			L[i][j] = 0; // total flow per edge unknown
		}
	}

	// SneakyPath[] = 3 x n array of sneaky path data
	int **SneakyPath = new int *[3];
	for (i = 0; i < 3; i++) {
		SneakyPath[i] = new int[SIZE];
	}
	// initialize 1st row to "infinity"
	i = 0;
	for (j = 0; j < SIZE; j++) {
		SneakyPath[i][j] = INF; // distance between nodes unknnown
	}
	// initialize second row to "WHITE"
	i = 1;
	for (j = 0; j < SIZE; j++) {
		SneakyPath[i][j] = 0; // node color = "WHITE"
	}
	// initialize 3rd row to "NULL"
	i = 2;
	for (j = 0; j < SIZE; j++) {
		SneakyPath[i][j] = -1; // came-from nodes unknown
	}

	// verify initialization on console
	cout << "Initialize matrices:" << endl << endl;
	cout << "Matrix E" << endl;
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			cout << setw(11) << E[i][j];
		}
		cout << endl;
	}
	cout << endl;

	cout << "Matrix F" << endl;
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			cout << setw(11) << F[i][j];
		}
		cout << endl;
	}
	cout << endl;

	cout << "Matrix L" << endl;
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			cout << setw(11) << L[i][j];
		}
		cout << endl;
	}
	cout << endl;

	cout << "Matrix next" << endl;
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			cout << setw(11) << next[i][j];
		}
		cout << endl;
	}
	cout << endl;

	cout << "Matrix SneakyPath" << endl;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < SIZE; j++) {
			cout << setw(11) << SneakyPath[i][j];
		}
		cout << endl;
	}
	cout << endl;

	// build the E and F arrays from the input file
	while (fin.good()) {
		getline(fin, line);
		istringstream data(line);
		data >> m >> c >> i >> c >> j >> c >> w;

		// adjust node values to array indexes
		i -= 1;
		j -= 1;

		if (line.empty()) { ; }  // ignore empty lines
		else if (i == j) { ; }
		else if (m == 'E') {
			E[i][j] = w;
			next[i][j] = j;

			// verify input on console
			cout << m << ", " << i + 1 << ", " << j + 1 << ", " << E[i][j] << endl << endl;
			cout << "next[" << i + 1 << "," << j + 1 << "] = " << j + 1 << endl << endl;
		}
		else if (m == 'F') {
			F[i][j] = w;

			// verify input on console
			cout << m << ", " << i + 1 << ", " << j + 1 << ", " << F[i][j] << endl << endl;
		}
		else { ; }  // ignore invalid input
	}

	// verify initialization and input on console
	cout << "Index-adjusted values:" << endl << endl;
	cout << "Matrix E" << endl;
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			cout << setw(11) << E[i][j];
		}
		cout << endl;
	}
	cout << endl;

	cout << "Matrix F" << endl;
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			cout << setw(11) << F[i][j];
		}
		cout << endl;
	}
	cout << endl;

	cout << "Matrix next" << endl;
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			cout << setw(11) << next[i][j];
		}
		cout << endl;
	}
	cout << endl;

	// close file stream
	fin.close();

	// copy nodes without direct connections from E[] to L[]
	//   (used when calculating total flow-per-edge)
	// set those distances in E[] to "infinity"
	//   (used when calculating shortest paths)
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			if (E[i][j] == INF) {
				L[i][j] = -1; // no direct path between nodes
			}
		}
	}

	// verify initialization and assignment on console
	cout << "Matrix L" << endl;
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			cout << setw(11) << L[i][j];
		}
		cout << endl;
	}
	cout << endl;

	// find shortest paths to all nodes and build next hop array
	// with Floyd-Warshall's Algorithm
	for (int k = 0; k < SIZE; k++) {
		for (i = 0; i < SIZE; i++) {
			for (j = 0; j < SIZE; j++) {
				if (E[i][k] + E[k][j] < E[i][j]) {
					E[i][j] = E[i][k] + E[k][j];
					next[i][j] = next[i][k];
				}
			}
		}

		// verify each iteration on console
		cout << "Matrix E for k = " << k << endl;
		for (i = 0; i < SIZE; i++) {
			for (j = 0; j < SIZE; j++) {
				cout << setw(11) << E[i][j];
			}
			cout << endl;
		}
		cout << endl;

		cout << "Matrix next for k = " << k << endl;
		for (i = 0; i < SIZE; i++) {
			for (j = 0; j < SIZE; j++) {
				cout << setw(11) << next[i][j];
			}
			cout << endl;
		}
		cout << endl;
	}

	// verify assignment on console
	cout << "Final Matrix E" << endl;
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			cout << setw(11) << E[i][j];
		}
		cout << endl;
	}
	cout << endl;

	cout << "Final Matrix next" << endl;
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			cout << setw(11) << next[i][j];
		}
		cout << endl;
	}
	cout << endl;

	// initialize flow variables
	int flow = 0;
	int start = 0;  // starting node for edge being explored
	bool lastHop = false;
	int nextHop = 0;

	// calculate total flow-per-edge array
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			lastHop = false; 
			if (i == j) { ; }
			else if (!( F[i][j] == 0 )) {
				flow = F[i][j];
				start = i;
				while (!( lastHop )) {
					nextHop = ( ( next[start][j] ) );
					L[start][nextHop] += flow;
					if (nextHop != j) { start = next[start][j]; }
					else { lastHop = true; }
				}
			}
		}
	}

	//for (int i = 0; i < SIZE; i++) {
	//	for (int j = 0; j < SIZE; j++) {
	//		if (edgeMatrix[i][j] == BIG) {
	//			totalFlowMatrix[i][j] = BIG;
	//		}
	//	}
	//}

	// verify calculations on console
	cout << "Final Matrix L" << endl;
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			cout << setw(11) << L[i][j];
		}
		cout << endl;
	}
	cout << endl;

	// find sneaky path with Dijkstra's Algorithm
	// distance matrix L[] and path matrix SneakyPath[] initialized in an earlier step
	// enumerate SneakyPath[] rows for easier reference
	const int DISTANCE = 0;
	const int COLOR = 1;
	const int CAMEFROM = 2;
	int NodeIndex = 0;

	// node color represented as int values
	const int WHITE = 0;
	const int GRAY = 1;
	const int BLACK = 2;

	// MyMinheap = minheap.create()
	priority_queue <ANode, vector<ANode>, Compare> SneakyHeap;
	NodeIndex = startNode;

	// Distance(StartNode) = 0
	SneakyPath[DISTANCE][NodeIndex] = 0;

	// Color(StartNode) = GRAY
	SneakyPath[COLOR][NodeIndex] = 1;

	// MyMinheap.insert(StartNode)
	dist weight = SneakyPath[DISTANCE][NodeIndex];
	SneakyHeap.push(ANode(startNode, weight));

	// while NOT MyMinheap.IsEmpty do
	while (!( SneakyHeap.empty() )) {

		// SelMin = MyMinheap.delete( ) until Color(SelMin) == GRAY end do
		ANode SelMin = SneakyHeap.top();
		SneakyHeap.pop();
		NodeIndex = SelMin.getNode();
		if (SneakyPath[COLOR][NodeIndex] == GRAY) {

			// Color(SelMin) = BLACK
			SneakyPath[COLOR][NodeIndex] = BLACK;

			// if SelMin == StopNode then RETURN
			if (NodeIndex == endNode) { break; }

			// for all(AdjNode adjacent to SelMin and with Color(AdjNode) != BLACK) do
			for (j = 0; j < SIZE; j++) {
				if (L[NodeIndex][j] != -1 && SneakyPath[COLOR][j] != BLACK) {

					// if Distance(SelMin) + Cost(AdjNode) < Distance(AdjNode)
					if (SneakyPath[DISTANCE][NodeIndex] + L[NodeIndex][j] < SneakyPath[DISTANCE][j]) {

						// Distance(AdjNode) = Distance(SelMin) + Cost(AdjNode)
						SneakyPath[DISTANCE][j] = SneakyPath[DISTANCE][NodeIndex] + L[NodeIndex][j];

						// CameFrom(AdjNode) = NextNode
						SneakyPath[CAMEFROM][j] = NodeIndex;

						// MyMinheap.insert(AdjNode)
						SneakyHeap.push(ANode(j, ( SneakyPath[DISTANCE][j] )));

						// Color(AdjNode) = GRAY
						SneakyPath[COLOR][j] = 1;
					}
				}
			}
		}
	}

	// analyze sneaky path
	// initialize edge weight and path node variables
	int TotalTraffic = SneakyPath[DISTANCE][endNode];
	int hops = 0;
	int HopCost = 0;
	float wAverage = 0;
	int wMaxEdge = 0;
	int vMaxFrom = 0;
	int vMaxTo = 0;
	int wMinEdge = INF;
	int vMinFrom = 0;
	int vMinTo = 0;
	int CurrentNode = endNode;
	int CameFrom = 0;
	stack<int> HopPath;

	while(!(CurrentNode == startNode)) {
		CameFrom = SneakyPath[CAMEFROM][CurrentNode];
		HopCost = L[CameFrom][CurrentNode];
		if (HopCost < wMinEdge) {
			wMinEdge = HopCost;
			vMinFrom = CameFrom;
			vMinTo = CurrentNode;
		}
		if (HopCost > wMaxEdge) {
			wMaxEdge = HopCost;
			vMaxFrom = CameFrom;
			vMaxTo = CurrentNode;
		}
		hops++;
		HopPath.push(CurrentNode);
		wAverage = TotalTraffic / static_cast<float>(hops);
		CurrentNode = SneakyPath[CAMEFROM][CurrentNode];
	}
	HopPath.push(startNode);

	// output data
	cout << endl << fileName << endl << endl;

	cout << "The path with the least amount of traffic from "
		<< startNode + 1 << " to " << endNode + 1 << " is:" << endl
		<< "{ ";

	while ((HopPath.size() > 1)) {
		cout << HopPath.top() + 1 << ", ";
		HopPath.pop();
	}
	cout << HopPath.top() + 1 << " }" << endl;
	HopPath.pop();
	cout << "over a total of " << hops << " hops and total traffic flow of "
		<< TotalTraffic << "." << endl
		<< "The average traffic per edge is " << setprecision(5) << wAverage << "." << endl
		<< "The edge with the least amount of traffic on the path is "
		<< "( " << vMinFrom + 1 << ", " << vMinTo + 1 << " ) with flow of " << wMinEdge << "." << endl
		<< "The edge with the most amount of traffic on the path is "
		<< "( " << vMaxFrom + 1 << ", " << vMaxTo + 1 << " ) with flow of " << wMaxEdge << "." << endl << endl;


	
	fout.close();
	return 0;
}