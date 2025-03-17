#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

#include "dijkstras.h"

using namespace std;

struct Node {
    int vertex, weight;
    Node(int v, int w) : vertex(v), weight(w) {}
    bool operator>(const Node& other) const { 
        return weight > other.weight; 
    }
};

vector<int> dijkstra_shortest_path(const Graph& G, int source, vector<int>& previous) {
    int n = G.numVertices;
    vector<int> distances(n, numeric_limits<int>::max());
    previous.assign(n, -1);
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    distances[source] = 0;
    pq.push(Node(source, 0));

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();
        int u = current.vertex;

        if (distances[u] < current.weight) 
            continue;

        for (const Edge& edge : G[u]) {
            int v = edge.dst, weight = edge.weight;

            if (distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                previous[v] = u;
                pq.push(Node(v, distances[v]));
            }
        }
    }

    return distances;
}

vector<int> extract_shortest_path(const vector<int>& distances, const vector<int>& previous, int destination) {
    vector<int> path;

    for (int v = destination; v != -1; v = previous[v])
        path.push_back(v);

    reverse(path.begin(), path.end());
    return path;
}

void print_path(const vector<int>& v, int total) {
    for (size_t i = 0; i < v.size(); ++i) {
        if (i > 0)
            cout << " ";

        cout << v[i];
    }

    cout << " \nTotal cost is " << total << endl;
}