#include <iostream>

#include "dijkstras.h"

int main() {
    Graph G;
    file_to_graph("src/small.txt", G);  

    vector<int> previous(G.numVertices, -1);
    vector<int> distances = dijkstra_shortest_path(G, 0, previous);

    for (int destination = 0; destination < G.numVertices; ++destination) {
        vector<int> path = extract_shortest_path(distances, previous, destination);
        
        for (int i = 0; i < path.size(); ++i) {
            cout << path[i];

            if (i < path.size() - 1)
                cout << " ";
            }
            
        cout << "\nTotal cost is " << distances[destination] << endl;
    }
}