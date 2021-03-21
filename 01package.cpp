#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <cassert>

using namespace std;

int package01_dfs(const vector<int>& volumes, const vector<int>& values, 
                 int capacity, int idx) {
    if(idx >= volumes.size())
        return 0;

    int value_w = INT_MIN;
    if(capacity >= volumes[idx])
        value_w = values[idx] + package01_dfs(volumes, values, capacity - volumes[idx], idx + 1);
    int value_wo = package01_dfs(volumes, values, capacity, idx + 1);

    return max(value_w, value_wo);
}

int package01_dp(const vector<int>& volumes, const vector<int>& values, int capacity) {
    vector<vector<int>> dp(volumes.size() + 1, vector<int>(capacity + 1, 0));
    /* Initialize dp[i][j] when (i == 0 || j == 0)
        dp[0][:] = 0; There is no item.
        dp[:][0] = 0; There is no space.
       Recursive equation:
        dp[i][j] = max(dp[i-1][j-volumes[i]], dp[i-1][j])
    */
    for(int i = 1; i <= volumes.size(); ++i) {
        // Do not use i to index volumes and values!
        // The indice of volumes and dp differ by one.
        int idx = i - 1;
        for(int j = 1; j <= capacity; ++j) {
            dp[i][j] = dp[i-1][j];
            if(j >= volumes[idx])
                dp[i][j] = max(dp[i-1][j-volumes[idx]] + values[idx],
                               dp[i][j]);
        }
    }

    return dp.back().back();
}


// space optimized dp
int package01_dp_opt(const vector<int>& volumes, const vector<int>& values, int capacity) {
    vector<int> dp(capacity + 1, 0);
    
    for(int i = 0; i < volumes.size(); ++i) {
        for(int j = capacity; j >= 0; --j) {
            // Before the assignment, dp[j] is equal to dp[i-1][j],
            // and dp[j-volumes[i]] is equal to dp[i-1][j-volumes[i]].
            // (To achieve this, we should iterate j in reverse.)
            //
            // After the assignment, dp[j] is equal to dp[i][j];
            if(j >= volumes[i])
                dp[j] = max(dp[j-volumes[i]] + values[i],
                            dp[j]);
        }
    }
    return dp.back();
}


int main() {
    constexpr int n_tests = 100;
    constexpr int n_items = 20;
    constexpr pair<int, int> capacity_range = {30, 200};
    constexpr pair<int, int> volume_range = {1, 40};

    default_random_engine e(time(nullptr));
    uniform_int_distribution<int> cap_u(capacity_range.first, 
                                        capacity_range.second);
    uniform_int_distribution<int> vol_u(volume_range.first,
                                        volume_range.second);
    uniform_int_distribution<int> val_u(1, 10);

    for(int i = 0; i < n_tests; ++i) {        
        int capacity = cap_u(e);
        vector<int> volumes(n_items);
        vector<int> values(n_items);
        for(int i = 0; i < n_items; ++i) {
            volumes[i] = vol_u(e);
            values[i] = val_u(e);
        }

        int res1 = package01_dfs(volumes, values, capacity, 0);
        int res2 = package01_dp(volumes, values, capacity);
        int res3 = package01_dp_opt(volumes, values, capacity);

        assert(res1 == res2 && res1 == res3);
    }

    cout << "Pass Test" << endl;
    return 0;
}