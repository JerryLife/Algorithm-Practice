//
// Created by HUSTW on 2017/11/30.
//
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

typedef struct EdgePair {
    int dst_node;
    int weight;
    EdgePair(int dst, int w): dst_node(dst), weight(w) {}
    ~EdgePair() = default;
} Pair;

struct AdjacentListNode {
    int src_node;
    vector<EdgePair> out_nbrs;
    AdjacentListNode(int src, vector<EdgePair> edgePair): src_node(src), out_nbrs(edgePair) {}
    explicit AdjacentListNode(int src): src_node(src) {}
    ~AdjacentListNode() = default;
};

class Graph {
    vector<AdjacentListNode> adjacent_list;
    int max_node_id = -1;
public:
    Graph() = default;
    Graph(const Graph &g) = default;
    void add_node(int node);
    void add_edge(int src_node, int dst_node, int weight);
    vector<int> get_all_nodes();
    vector<EdgePair> get_adjacents(int src_node);
    int get_weight(int src_node, int dst_node);
    void change_weight(int src_node, int dst_node, int offset);
    bool greedy_bfs(int s, int t, int parent[]);
    static int max_flow(Graph &graph, int s, int t);
    ~Graph() = default;
};

void Graph::add_node(int node) {
    adjacent_list.emplace_back(AdjacentListNode(node));
    max_node_id = max(node, max_node_id);
}

void Graph::add_edge(int src_node, int dst_node, int weight) {
    max_node_id = max(max_node_id, max(dst_node, src_node));
    for (AdjacentListNode &node: adjacent_list) {
        if (node.src_node == src_node) {
            // exist node
            node.out_nbrs.emplace_back(EdgePair(dst_node, weight));
            return;
        }
    }
    // not exist
    EdgePair edgePair = EdgePair(dst_node, weight);
    adjacent_list.emplace_back(AdjacentListNode(src_node, vector<EdgePair> {edgePair}));
}

vector<EdgePair> Graph::get_adjacents(int src_node) {
    for (AdjacentListNode &node: adjacent_list) {
        if (node.src_node == src_node) {
            // exist node
            return node.out_nbrs;
        }
    }
    // if not exist. return empty
    vector<EdgePair> edgePair;
    return edgePair;
}

int Graph::get_weight(int src_node, int dst_node) {
    for (AdjacentListNode &node: adjacent_list) {
        if (node.src_node == src_node){
            for (EdgePair &edgePair: node.out_nbrs) {
                if (edgePair.dst_node == dst_node) {
                    return edgePair.weight;
                }
            }
        }
    }
    return -1;
}

void Graph::change_weight(int src_node, int dst_node, int offset) {
    for (AdjacentListNode &node: adjacent_list) {
        if (node.src_node == src_node){
            for (int i = 0; i < node.out_nbrs.size(); ++i) {
                EdgePair edgePair = node.out_nbrs[i];
                if (edgePair.dst_node == dst_node) {
                    edgePair.weight += offset;
                    if (edgePair.weight == 0)   // check if weight == 0
                        node.out_nbrs.erase(node.out_nbrs.begin() + i);
                    return;
                }
            }
            break;
        }
    }
    add_edge(src_node, dst_node, offset);
}

vector<int> Graph::get_all_nodes() {
    vector<int> nodes;
    for (AdjacentListNode &node: adjacent_list) {
        nodes.push_back(node.src_node);
    }
    return nodes;
}


//////////////////////////////////////////////////////////////////////////////////////////
// max flow

bool Graph::greedy_bfs(int s, int t, int parent[]) {
    bool visited[max_node_id+1] {0};

    queue<int> q;
    visited[s] = true;
    parent[s] = -1;

    q.push(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        // search for all neighbors
        for (EdgePair &edgePair: get_adjacents(u)) {
            int v = edgePair.dst_node;
            if (visited[v])
                break;
            q.push(v);
            parent[v] = u;
            visited[v] = true;
        }
    }

    return visited[t];  // return if destination is visited
}

int Graph::max_flow(Graph &graph, int s, int t) {

    // initial residualAdjacentList
    Graph residualGraph(graph);

    // store the path found by bfs
    int parent[residualGraph.max_node_id + 1];

    int maximum_flow = 0;
    while (residualGraph.greedy_bfs(s, t, parent)) {

        // find the min capacity on this path
        int flow = INT_MAX;
        for (int v = t; v != s; v = parent[v]) {
            int w = residualGraph.get_weight(parent[v], v);
            flow = min(flow, w);
        }

        // update and get residual graph
        for (int v = t; v != s; v = parent[v]) {
            residualGraph.change_weight(parent[v], v, -flow);
            residualGraph.change_weight(v, parent[v], flow);
        }

        maximum_flow += flow;
    }

    return maximum_flow;
}


/////////////////////////////////////////////////////////////////////////////////

int main() {
    vector<int> flows;
    int _size = 1;
    while (_size-- > 0) {
        int n_request, n_stu, n_prof, n_time;
        scanf("%d%d%d%d", &n_request, &n_stu, &n_prof, &n_time);
        // cin >> n_request >> n_stu >> n_prof >> n_time;
        int node_size = n_time * (n_prof + n_stu);

        Graph g;
        g.add_node(node_size + 1);  // s
        g.add_node(node_size + 2);  // t

        // vector<Pair> prof_time;
        int node_offset = n_prof * n_time;
        for (int i = 0; i < n_time; ++i) {
            for (int j = 0; j < n_prof; ++j) {
                // prof_time.emplace_back(Pair(j, i));
                g.add_node(i * n_prof + j);
                g.add_edge(i * n_prof + j, node_size + 2, 1);
            }
            for (int k = 0; k < n_stu; ++k) {
                // stu_time.emplace_back(Pair(k, i));
                g.add_node(node_offset + i * n_stu + k);
                g.add_edge(node_size + 1, node_offset + i * n_stu + k, 1);
            }
        }

        while (n_request-- > 0) {
            int stu, prof, time;
            scanf("%d%d%d", &stu, &prof, &time);
            // cin >> stu >> prof >> time;
            stu--;
            prof--;
            time--;
            g.add_edge(time * n_stu + stu + node_offset, time * n_prof + prof, 1);
        }

        int max_flow = Graph::max_flow(g, node_size + 1, node_size + 2);
        // cout << max_flow << endl;
        flows.emplace_back(max_flow);
    }
    cout << endl << endl;
    for (auto &i: flows) {
        cout << i << endl;
    }
    return 0;
}