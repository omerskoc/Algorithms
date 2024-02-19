#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <utility>
#include <stack>
#include <iomanip>  // For setw and left, used in formatting the output

using namespace std;

// Structure to represent an edge in a graph. It contains a source, a destination, and a weight
struct Edge {
    int src, dest, weight;
};

// Structure to represent a graph.
struct Graph {
    int V, E;  // Number of vertices (V) and edges (E)
    vector<Edge> edges;  // List of all edges in the graph
    vector<vector<pair<int, int>>> adjList;  // Adjacency list for the graph

    // Function to add an edge to the graph
    void addEdge(int u, int v, int w) {
        edges.push_back({u, v, w});
    }

    // Bellman-Ford algorithm to find the shortest path from a single source to all other vertices
    bool bellmanFord(int src, vector<int>& dist) {
        dist.assign(V, numeric_limits<int>::max());  // Initialize distances as infinite
        dist[src] = 0;  // Distance from the source to itself is zero

        // Relaxation of all edges V-1 times
        for (int i = 0; i < V - 1; i++) {
            for (auto& edge : edges) {
                int u = edge.src;
                int v = edge.dest;
                int weight = edge.weight;
                if (dist[u] != numeric_limits<int>::max() && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                }
            }
        }

        // Check for negative weight cycles
        for (auto& edge : edges) {
            int u = edge.src;
            int v = edge.dest;
            int weight = edge.weight;
            if (dist[u] != numeric_limits<int>::max() && dist[u] + weight < dist[v]) {
                return false;  // Graph contains a negative weight cycle
            }
        }

        return true;
    }

    // Function to create an adjacency list from the edge list for efficient traversal
    void createAdjList() {
        adjList.resize(V);
        for (auto& edge : edges) {
            adjList[edge.src].push_back({edge.dest, edge.weight});
        }
    }

    // Dijkstra's algorithm to find the shortest path from a single source to all other nodes
    void dijkstra(int src, vector<int>& dist, vector<int>& prev) {
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        dist.assign(V, numeric_limits<int>::max());
        prev.assign(V, -1);
        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            for (auto& p : adjList[u]) {
                int v = p.first;
                int weight = p.second;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    prev[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }
    }

    // Function to calculate the total weight of a path from source to destination
    int calculatePathWeight(int src, int dest, const vector<int>& prev) {
        int totalWeight = 0;
        int current = dest;

        while (current != src && current != -1) {
            int u = prev[current];
            // Find the weight of the edge from u to current
            for (auto &p : adjList[u]) {
                if (p.first == current) {
                    totalWeight += p.second;
                    break;
                }
            }
            current = u;
        }

        return totalWeight;
    }
};

int main() {
    Graph g;
    g.V = 5;  // Set number of vertices
    g.E = 9;  // Set number of edges

    // Adding edges to the graph
    g.addEdge(0, 4, -4);
    g.addEdge(0, 1, 3);
    g.addEdge(0, 2, 8);
    g.addEdge(1, 4, 7);
    g.addEdge(1, 3, 1);
    g.addEdge(2, 1, 4);
    g.addEdge(3, 2, -5);
    g.addEdge(3, 0, 2);
    g.addEdge(4, 3, 6);

    vector<int> h(g.V); // Vector to store distances from the source vertex
    vector<vector<int>> distanceMatrix(g.V, vector<int>(g.V, numeric_limits<int>::max())); // Matrix to store shortest path distances

    // Run Bellman-Ford algorithm to check for negative weight cycles and prepare for Dijkstra
    if (g.bellmanFord(0, h)) {
        g.createAdjList(); // Create adjacency list for efficient traversal

        // Run Dijkstra's algorithm for each vertex as the source
        for (int src = 0; src < g.V; src++) {
            vector<int> dist, prev;
            g.dijkstra(src, dist, prev);

            // Calculate the shortest path distances for each destination
            for (int dest = 0; dest < g.V; dest++) {
                if (dist[dest] != numeric_limits<int>::max()) {
                    distanceMatrix[src][dest] = g.calculatePathWeight(src, dest, prev);
                }
            }
        }

        // Print the shortest path distances matrix
        cout << "\nShortest Path Distances Matrix:\n";
        const int cellWidth = 5;  // Cell width for formatting output
        for (int i = 0; i < g.V; i++) {
            for (int j = 0; j < g.V; j++) {
                cout << setw(cellWidth) << left;  // Set cell width and alignment
                if (distanceMatrix[i][j] == numeric_limits<int>::max()) {
                    cout << "INF";
                } else {
                    cout << distanceMatrix[i][j];
                }
            }
            cout << endl;
        }
    } else {
        cout << "Graph contains negative weight cycle" << endl;
        return 1;
    }

    return 0;
}