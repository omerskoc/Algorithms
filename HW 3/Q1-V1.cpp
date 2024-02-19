// Required libraries
#include <iostream>
#include <vector>
#include <climits>

using namespace std;

// Structure representing a vertex in the graph
struct Node {
    int key;     // Minimum weight to connect to the MST
    int parent;  // The vertex's parent in the MST
    bool inMST;  // Flag to check if the vertex is in the MST
};

// Function to extract the minimum key vertex from the set of vertices not yet included in MST
int extractMin(vector<Node>& nodes) {
    int minKey = INT_MAX;
    int minIndex = -1;

    // Loop through all vertices to find the minimum key vertex
    for (int i = 0; i < nodes.size(); ++i) {
        // If vertex is not in MST and its key is less than current minimum, update minimum
        if (!nodes[i].inMST && nodes[i].key < minKey) {
            minKey = nodes[i].key;
            minIndex = i;
        }
    }

    return minIndex;
}

// Function to construct and print the MST for a graph represented using adjacency matrix
void primMST(vector<vector<int>>& graph) {
    int numVertices = graph.size();
    vector<Node> nodes(numVertices);

    // Initialize all keys as INFINITE and MST as false
    for (int i = 0; i < numVertices; ++i) {
        nodes[i].key = INT_MAX;
        nodes[i].parent = -1;
        nodes[i].inMST = false;
    }

    // Start with the first vertex by setting its key to 0
    nodes[0].key = 0;

    // The MST will have numVertices vertices
    for (int count = 0; count < numVertices - 1; ++count) {
        // Pick the minimum key vertex from the set of vertices not yet included in MST
        int u = extractMin(nodes);
        nodes[u].inMST = true;

        // Update key and parent index of the adjacent vertices of the picked vertex
        for (int v = 0; v < numVertices; ++v) {
            // Update the key only if graph[u][v] is smaller than the key of v
            if (graph[u][v] && !nodes[v].inMST && graph[u][v] < nodes[v].key) {
                nodes[v].parent = u;
                nodes[v].key = graph[u][v];
            }
        }
    }

    // Print the constructed MST
    cout << "Edge   Weight" << endl;
    for (int i = 1; i < numVertices; ++i) {
        cout << nodes[i].parent << " - " << i << "    " << graph[i][nodes[i].parent] << endl;
    }
}

int main() {
    // Example graph represented as an adjacency matrix
    vector<vector<int>> graph = {
        {0, 4, 0, 0, 8, 0, 0},
        {4, 0, 6, 0, 11, 0, 0},
        {0, 6, 0, 7, 0, 2, 0},
        {0, 0, 7, 0, 0, 0, 14},
        {8, 11, 0, 0, 0, 7, 2},
        {0, 0, 2, 0, 7, 0, 1},
        {0, 0, 0, 14, 2, 1, 0}
    };

    // Run Prim's MST algorithm
    primMST(graph);

    return 0;
}