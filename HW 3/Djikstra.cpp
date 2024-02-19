#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

// Function to implement Dijkstra's algorithm
void dijkstra(int src, const vector<vector<pair<int, int>>>& adjList, int V, vector<int>& dist) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    dist.assign(V, numeric_limits<int>::max());
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
                pq.push({dist[v], v});
            }
        }
    }
}

int main() {
    int V = 9; // Number of vertices
    vector<vector<pair<int, int>>> adjList(V);

    // Adding edges to the graph for example
    adjList[0].push_back({1, 4});
    adjList[0].push_back({7, 8});
    adjList[1].push_back({2, 8});
    adjList[1].push_back({7, 11});
    adjList[2].push_back({1, 8});
    adjList[2].push_back({3, 7});
    adjList[2].push_back({5, 4});
    adjList[2].push_back({8, 2});
    adjList[3].push_back({2, 7});
    adjList[3].push_back({4, 9});
    adjList[3].push_back({5, 14});
    adjList[4].push_back({3, 9});
    adjList[4].push_back({5, 10});
    adjList[5].push_back({2, 4});
    adjList[5].push_back({3, 14});
    adjList[5].push_back({4, 10});
    adjList[5].push_back({6, 2});
    adjList[6].push_back({5, 2});
    adjList[6].push_back({7, 1});
    adjList[6].push_back({8, 6});
    adjList[7].push_back({0, 8});
    adjList[7].push_back({1, 11});
    adjList[7].push_back({6, 1});
    adjList[7].push_back({8, 7});
    adjList[8].push_back({2, 2});
    adjList[8].push_back({6, 6});
    adjList[8].push_back({7, 7});

    // Vector to store distances
    vector<int> dist(V);

    // Run Dijkstra's algorithm from vertex 0
    dijkstra(0, adjList, V, dist);

    // Output the results
    cout << "Vertex\tDistance from Source" << endl;
    for (int i = 0; i < V; ++i) {
        cout << i << "\t\t\t\t" << dist[i] << endl;
    }

    return 0;
}