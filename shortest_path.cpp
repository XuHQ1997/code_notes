#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <random>
#include <ctime>
#include <cassert>

using namespace std;

namespace ChainForwardStar {
struct Edge {
    int end, cost, next;
};

vector<int> head;
vector<Edge> edges;

void init_chain_forward_star(int n_points) {
    head = vector<int>(n_points, -1);
}

void add_edge(int start, int end, int cost) {
    edges.push_back({end, cost, head[start]});
    head[start] = edges.size() - 1;
}

void reset_chain_forward_star() {
    head.clear();
    edges.clear();
}
}  // namespace ChainForwardStar
using namespace ChainForwardStar;

namespace dijkstra {
vector<int> shortest_path(int n_points, int start, int end) {
    vector<int> dist(n_points, INT_MAX);
    vector<bool> fixed(n_points, false);
    vector<int> prev(n_points, -1);

    // Node.first is index of point, and Node.second is dist from start.
    using Node = pair<int, int>;
    auto compare = [](const Node& a, const Node& b) {
        return a.second > b.second;
    };
    priority_queue<Node, vector<Node>, decltype(compare)> q(compare);

    // setup start
    dist[start] = 0;
    q.push({start, 0});

    while(!q.empty()) {
        Node cur = q.top();
        q.pop();
        if(cur.first == end) break;
        if(fixed[cur.first]) continue;

        fixed[cur.first] = true;
        for(int i = head[cur.first]; i != -1; i = edges[i].next) {
            auto& e = edges[i];
            if(fixed[e.end]) 
                continue;
            if(dist[e.end] > cur.second + e.cost) {
                dist[e.end] = cur.second + e.cost;
                prev[e.end] = cur.first;
                q.push({e.end, dist[e.end]});
            }
        }
    }

    vector<int> path;
    if(dist[end] == INT_MAX)
        return path;
    while(end != -1) {
        path.push_back(end);
        end = prev[end];
    }
    reverse(path.begin(), path.end());
    return path;
}
} // namespace dijkstra

namespace spfa {
vector<int> shortest_path(int n_points, int start, int end) {
    vector<int> in_queue(n_points, false);
    vector<int> dist(n_points, INT_MAX);
    vector<int> prev(n_points, -1);
    queue<int> q;

    q.push(start);
    dist[start] = 0;
    in_queue[start] = true;

    while(!q.empty()) {
        int cur = q.front();
        q.pop();
        in_queue[cur] = false;

        for(int i = head[cur]; i != -1; i = edges[i].next) {
            auto& e = edges[i];
            if(dist[e.end] > dist[cur] + e.cost) {
                dist[e.end] = dist[cur] + e.cost;
                prev[e.end] = cur;
                if(!in_queue[e.end]) {
                    q.push(e.end);
                    in_queue[e.end] = true;
                }
            }
        }
    }

    vector<int> path;
    if(dist[end] == INT_MAX)
        return path;
    while(end != -1) {
        path.push_back(end);
        end = prev[end];
    }
    reverse(path.begin(), path.end());
    return path;
}
}  // namespace spfa


int main() {
    constexpr int n_tests = 10;
    constexpr int n_points = 100;
    // Point i is only possible to connect with points in range of [i-20, i+20),
    // so that we can get longer path.
    constexpr pair<int, int> connect_range = {-20, 20};

    default_random_engine e(time(nullptr));
    uniform_int_distribution<int> cost_u(1, 100);
    uniform_real_distribution<double> prob_u(0.1, 0.7);

    // adjacent_matrix is only used to compute cost of paths output by these algorithms.
    vector<vector<int>> adjacent_matrix(n_points, vector<int>(n_points, -1));
    auto build_graph = [&]() {
        reset_chain_forward_star();
        init_chain_forward_star(n_points);
        bernoulli_distribution b(prob_u(e));

        for(int i = 0; i < n_points; ++i) {
            int range_start = max(0, i + connect_range.first);
            int range_end = min(n_points, i + connect_range.second);
            for(int j = range_start; j < range_end; ++j) {
                if(i != j && b(e)) {
                    int cost = cost_u(e);
                    adjacent_matrix[i][j] = cost;
                    add_edge(i, j, cost);
                }
            }
        }
    };
    auto compute_cost = [&](const vector<int>& path) {
        int cost = 0;
        for(int i = 1; i < path.size(); ++i) {
            int prev = path[i-1];
            int cur = path[i];
            assert(adjacent_matrix[prev][cur] != -1);
            cost += adjacent_matrix[prev][cur];
        }
        return cost;      
    };

    for(int i = 0; i < n_tests; ++i) {
        build_graph();
        vector<int> ret1 = dijkstra::shortest_path(n_points, 0, n_points-1);
        vector<int> ret2 = spfa::shortest_path(n_points, 0, n_points-1);

        assert(!(ret1.empty()^ret2.empty()));
        assert(compute_cost(ret1) == compute_cost(ret2));
    }
    cout << "Two algorithm output the same answer" << endl;
    return 0;
}