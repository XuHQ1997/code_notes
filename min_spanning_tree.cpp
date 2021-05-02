#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <climits>
#include <cassert>
using namespace std;

struct Edge {
    int start, end, value;
};

namespace kruscal {
class UnionFindSet {
public:
    UnionFindSet(int n_points)
            : parents_(n_points) {
        for(int i = 0; i < parents_.size(); ++i)
            parents_[i] = i;
    }

    int find(int x) {
        int r = x;
        while(r != parents_[r])
            r = parents_[r];
        while(x != r) {
            int temp = parents_[x];
            parents_[x] = r;
            x = temp;
        }
        return r;
    }

    void connect(int x, int y) {
        x = find(x);
        y = find(y);
        if(x != y)
            parents_[x] = y;
    }

    bool is_connected(int x, int y) {
        return find(x) == find(y);
    }
private:
    vector<int> parents_;
};

vector<Edge> mst(int n_points, vector<Edge>& edges) {
    UnionFindSet ufset(n_points);
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.value < b.value;
    });

    vector<Edge> ret;
    ret.reserve(n_points - 1);
    for(auto& e : edges) {
        if(!ufset.is_connected(e.start, e.end)) {
            ufset.connect(e.start, e.end);
            ret.push_back(e);
        }
        if(ret.size() == n_points - 1)
            break;
    }
    // the graph isn't connected.
    if(ret.size() != n_points - 1)
        ret.clear();
    return ret;
}
}  // namespace kruscal

namespace prime {
vector<vector<int>> build_adjacency_matrix(int n_points, const vector<Edge>& edges) {
    vector<vector<int>> dist(
        n_points, vector<int>(n_points, INT_MAX)
    );
    for(int i = 0; i < n_points; ++i)
        dist[i][i] = 0;
    for(auto& e : edges) {
        dist[e.start][e.end] = e.value;
        dist[e.end][e.start] = e.value;
    }
    return dist;
}

vector<Edge> mst(int n_points, const vector<Edge>& edges) {
    auto dist = build_adjacency_matrix(n_points, edges);
    vector<bool> is_added(n_points, false);
    vector<int> connect_to(n_points, 0);
    vector<int> dist_to(n_points);

    // add point 0
    is_added[0] = true;
    for(int i = 1; i < n_points; ++i) {
        // connect_to[i] = 0;
        dist_to[i] = dist[0][i];
    }

    vector<Edge> ret;
    ret.reserve(n_points - 1);
    for(int i = 1; i < n_points; ++i) {
        int min_idx = -1;
        int min_dist = INT_MAX;
        for(int j = 0; j < n_points; ++j) {
            if(!is_added[j] && dist_to[j] < min_dist) {
                min_dist = dist_to[j];
                min_idx = j;
            }
        }
        // the graph isn't connected.
        if(min_dist == INT_MAX)
            return {};

        is_added[min_idx] = true;
        ret.push_back({min_idx, connect_to[min_idx], 
                       dist[min_idx][connect_to[min_idx]]});

        for(int j = 0; j < n_points; ++j) {
            if(!is_added[j] && dist_to[j] > dist[min_idx][j]) {
                dist_to[j] = dist[min_idx][j];
                connect_to[j] = min_idx;
            }
        }
    }
    return ret;
}
}  // namespace prime


int main() {
    constexpr int n_tests = 10;
    constexpr int n_points = 1000;

    default_random_engine e(time(nullptr));
    uniform_int_distribution<int> cost_u(1, 100);
    uniform_real_distribution<double> prob_u(0.01, 0.7);

    auto build_graph = [&]()->vector<Edge> {
        vector<Edge> ret;
        bernoulli_distribution b(prob_u(e));

        for(int i = 0; i < n_points; ++i) {
            for(int j = i + 1; j < n_points; ++j) {
                if(b(e))
                    ret.push_back({i, j, cost_u(e)});         
            }
        }
        return ret;
    };

    auto compute_cost = [](const vector<Edge>& edges) {
        int cost = 0;
        for(auto& e : edges)
            cost += e.value;
        return cost;
    };

    for(int i = 0; i < n_tests; ++i) {
        vector<Edge> edges = build_graph();
        vector<Edge> ret1 = kruscal::mst(n_points, edges);
        vector<Edge> ret2 = prime::mst(n_points, edges);

        if(ret1.empty())
            cout << "not connected" << endl;

        assert(!(ret1.empty()^ret2.empty()));
        assert(compute_cost(ret1) == compute_cost(ret2));
    }
    cout << "Two algorithm output the same answer" << endl;
    return 0;
}