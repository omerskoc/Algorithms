#include <iostream>
#include <vector>
#include <limits>

using namespace std;

// Assume we have an Edge structure defined as follows:
struct Edge {
    int src, dest, weight;
};

class Graph {
public:
    int V; // Number of vertices
    vector<Edge> edges; // All edges in the graph

    // Function to add an edge to the graph
    void addEdge(int src, int dest, int weight) {
        Edge edge = {src, dest, weight};
        edges.push_back(edge);
    }

    // Function to implement Bellman-Ford algorithm
    bool bellmanFord(int src, vector<int>& dist) {
        dist.assign(V, numeric_limits<int>::max());
        dist[src] = 0;

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
                return false; // Graph contains a negative weight cycle
            }
        }
        return true; // No negative weight cycles found
    }
};

int main() {
    Graph g;
    g.V = 5; // Example number of vertices

    // Adding edges according to the example graph
    g.addEdge(0, 1, -1); // A to B
    g.addEdge(0, 2, 4);  // A to C
    g.addEdge(1, 2, 3);  // B to C
    g.addEdge(1, 3, 2);  // B to D
    g.addEdge(1, 4, 2);  // B to E
    g.addEdge(3, 2, 5);  // D to C
    g.addEdge(3, 1, 1);  // D to B
    g.addEdge(4, 3, -3); // E to D


    vector<int> dist(g.V);
    if (g.bellmanFord(0, dist)) { // Run Bellman-Ford from vertex 0
        cout << "Vertex\tDistance from Source" << endl;
        for (int i = 0; i < g.V; i++) {
            cout << i << "\t\t\t\t" << dist[i] << endl;
        }
    } else {
        cout << "Graph contains a negative weight cycle" << endl;
    }

    return 0;
}