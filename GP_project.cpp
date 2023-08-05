#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <stack>
#include <unordered_set>
using namespace std;

struct Edge {
    int src, dest;
};

class Graph
{
public:
    // a vector of vectors to represent an adjacency list
    vector<vector<int>> adjList;

    Graph(vector<Edge> const &edges, int n)
    {
        // resizes the vector to hold `n` elements of type `vector<int>`
        adjList.resize(n);

        for (auto &edge: edges)
        {
            adjList[edge.src].push_back(edge.dest);
        }
    }
};



void graph_coloring(vector<vector<int>>& adj_list, vector<int>& colors)
{
    int n = adj_list.size();
    colors.assign(n, -1); // Initialize all vertices as uncolored (-1)
    for (int u = 0; u < n; u++)
    {
        // Check if vertex u has been colored already
        if (colors[u] == -1)
        {
            // Try to color vertex u with a new color (starting from 0)
            for (int color = 0; ; color++)
            {
                bool ok = true;
                for (int v : adj_list[u])
                {
                    if (colors[v] == color)
                    {
                        ok = false; // A neighbor has the same color
                        break;
                    }
                }
                if (ok) {
                    colors[u] = color; // Assign color to vertex u
                    break;
                }
            }
        }
    }
}

int edge_coloring(vector<vector<int>>& graph)
{
    int n = graph.size();
    vector<int> colors(n, -1);
    int num_colors = 0;
    for (int u = 0; u < n; u++)
    {
        vector<int> used_colors;
        // Vector to store the colors used by the neighbors of the current node.
        for (int v : graph[u])
        {
            if (colors[v] != -1)
            {
                used_colors.push_back(colors[v]);
            }
        }
        sort(used_colors.begin(), used_colors.end());
        int new_color = 0;
        for (int color : used_colors)
        {
            if (color == new_color)
            {
                new_color++;
            }
        }
        colors[u] = new_color;
        num_colors = max(num_colors, new_color + 1);
    }
    return num_colors;
}


typedef pair<int, int> pii;
vector<pii> prim(vector<vector<pii>>& graph)
{
    int n = graph.size();
    vector<pii> mst; // Empty vector that stores edges in the MST
    vector<bool> visited(n, false);
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    // Start with node 0
    pq.push(make_pair(0, 0));
    while (!pq.empty())
    {
        int curr = pq.top().second;
        int weight = pq.top().first;
        pq.pop();
        if (visited[curr]) // Checks whether current vertex has been visited
        {
            continue;
        }
        visited[curr] = true;
        mst.push_back(make_pair(curr, weight));
        for (auto& edge : graph[curr])
        {
            int neighbor = edge.first;
            int neighbor_weight = edge.second;
            if (!visited[neighbor])
            {
                pq.push(make_pair(neighbor_weight, neighbor));
                // Adds neighbour vertex and weight to priority queue
            }
        }
    }
    return mst;
}



typedef pair<int, pair<int, int>> edge; // <weight, <u, v>>
vector<pair<int, int>> kruskals(vector<pair<int, int>>& nodes, vector<edge>& edges)
{
    vector<pair<int, int>> mst; // Empty vector that stores edges in the MST
    int n = nodes.size();
    vector<int> parent(n);
    for (int i = 0; i < n; i++)
    {
        parent[i] = i;
    }

    // sort edges by weight
    sort(edges.begin(), edges.end());
    for (auto& e : edges)
    {
        int u = e.second.first;
        int v = e.second.second;
        int pu = parent[u];
        int pv = parent[v];
        if (pu != pv)
        {
            mst.push_back(make_pair(u, v));
            // update parent of all nodes in v's component to u's component
            for (int i = 0; i < n; i++)
            {
                if (parent[i] == pv)
                {
                    parent[i] = pu;
                }
            }
        }
    }
    return mst;
}


vector<int> iterative_dfs(vector<vector<int>>& graph, int start)
{
    stack<int> s;
    unordered_set<int> visited;
    vector<int> result;
    s.push(start);
    while (!s.empty())
    {
        int curr = s.top();
        s.pop();
        if (visited.count(curr) == 0)
        {
            visited.insert(curr);
            result.push_back(curr);
            for (int neighbor : graph[curr])
            {
                s.push(neighbor);
            }
        }
    }
    return result;
}

void printGraph(Graph const &graph, int n)
{
    for (int i = 0; i < n; i++)
    {
        // print the current vertex number
        cout << i << " ——> ";

        // print all neighboring vertices of a vertex `i`
        for (int v: graph.adjList[i]) {
            cout << v << " ";
        }
        cout << endl;
    }
}

// Graph Implementation using STL
int main()
{
    vector<Edge> edges =
    {
        {0, 1}, {1, 2}, {2, 0}, {2, 1}, {3, 2}, {4, 5}, {5, 4}
    };

    int n = 6;
    Graph graph(edges, n);
    printGraph(graph, n);
    // Graph coloring
    vector<int> colors;
    vector<vector<int>> adj_list = {
        {1, 2},
        {0, 2, 3, 4},
        {0, 1, 5},
        {1},
        {1},
        {2}
    };
    graph_coloring(adj_list, colors);
    for (int i = 0; i < n; i++)
    {
        cout << "Vertex " << i << " has color " << colors[i] << endl;
    }
    int num_colors = edge_coloring(adj_list);
        cout << "Minimum number of colors required: " << num_colors << endl;
    // Prim's algorithm for finding MST
    vector<vector<pii>> graph2 =
    {
        {{1, 1}, {2, 2}},
        {{0, 1}, {2, 3}},
        {{0, 2}, {1, 3}, {3, 1}},
        {{2, 1}, {4, 4}, {5, 5}},
        {{3, 4}},
        {{3, 5}}
    };
    vector<pii> mst = prim(graph2);
    for (auto& e : mst)
    {
        cout << e.first << " ——> " << e.second << endl;
    }
    // Kruskal's algorithm for finding MST
    vector<pair<int, int>> nodes(n);
    for (int i = 0; i < n; i++)
    {
        nodes[i] = make_pair(i, i);
    }
    vector<edge> edges2 =
    {
        {1, {0, 1}}, {2, {0, 2}}, {3, {1, 2}},
        {1, {2, 3}}, {4, {3, 4}}, {5, {4, 5}}
    };
    vector<pair<int, int>> mst2 = kruskals(nodes, edges2);
    for (auto& e : mst2)
    {
        cout << e.first << " ——> " << e.second << endl;
    }
    // Iterative DFS
    vector<int> result = iterative_dfs(adj_list, 0);
        for (int i = 0; i < result.size(); i++)
        {
            cout << result[i] << " ";
        }
        cout << endl;
    return 0;
}
