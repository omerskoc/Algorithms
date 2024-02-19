// Required libraries
#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

// Structure to represent a weighted edge in the graph
struct Edge {
    int to;     // Destination vertex of the edge
    int weight; // Weight of the edge
};

// Overload the operator to compare edges based on weight
// This will make the minimum weight edge to be on top of the heap
bool operator>(const Edge &a, const Edge &b) {
    return a.weight > b.weight;
}

// Function to construct and print the MST using adjacency list and min heap
void primMSTUsingHeap(vector<vector<Edge>>& graph) {
    int numVertices = graph.size();
    vector<bool> inMST(numVertices, false);
    vector<int> keys(numVertices, INT_MAX);
    vector<int> parent(numVertices, -1);

    // Min heap to store vertices based on the minimum 'key' value
    priority_queue<Edge, vector<Edge>, greater<Edge>> minHeap;

    // Start with the first vertex
    keys[0] = 0;
    minHeap.push({0, 0}); // Insert source vertex with weight 0

    while (!minHeap.empty()) {
        // Extract the vertex with minimum key value
        int u = minHeap.top().to;
        minHeap.pop();

        if (inMST[u]) continue; // Skip if vertex is already included in MST

        inMST[u] = true; // Include vertex in MST

        // Iterate over all the adjacent vertices of u
        for (auto &edge : graph[u]) {
            int v = edge.to;
            int weight = edge.weight;

            // If v is not in MST and weight of (u,v) is smaller than current key of v
            if (!inMST[v] && keys[v] > weight) {
                // Update key of v
                keys[v] = weight;
                parent[v] = u;
                minHeap.push({v, keys[v]});
            }
        }
    }

    // Print the edges of MST
    cout << "Edge   Weight\n";
    for (int i = 1; i < numVertices; ++i) {
        cout << parent[i] << " - " << i << "    " << keys[i] << "\n";
    }
}

int main() {
    // Example graph represented as an adjacency list
    vector<vector<Edge>> graph = {
        // Adjacency list for each vertex
        {{1, 4}, {4, 8}},       // Edges from vertex 0
        {{0, 4}, {2, 6}, {4, 11}}, // Edges from vertex 1
        {{1, 6}, {3, 7}, {5, 2}},  // Edges from vertex 2
        {{2, 7}, {6, 14}},      // Edges from vertex 3
        {{0, 8}, {1, 11}, {5, 7}, {6, 2}}, // Edges from vertex 4
        {{2, 2}, {4, 7}, {6, 1}}, // Edges from vertex 5
        {{3, 14}, {4, 2}, {5, 1}}  // Edges from vertex 6
    };

    primMSTUsingHeap(graph);

    return 0;
}