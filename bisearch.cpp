#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#include <vector>
#include <cassert>

using namespace std;

int find(const vector<int>& nums, int target) {
    int left = 0;
    int right = nums.size();

    while(left < right) {
        int mid = left + (right - left) / 2;
        if(nums[mid] == target) {
            return mid;
        } else if(nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return -1;
}

int lower_bound(const vector<int>& nums, int target) {
    int left = 0;
    int right = nums.size();

    while(left < right) {
        int mid = left + (right - left) / 2;
        if(nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

int upper_bound(const vector<int>& nums, int target) {
    int left = 0;
    int right = nums.size();

    while(left < right) {
        int mid = left + (right - left) / 2;
        if(nums[mid] <= target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}


int main() {
    constexpr int n_samples = 10000;
    constexpr int n_tests = 100;
    constexpr pair<int, int> sample_range = {-3000, 3000};
    constexpr pair<int, int> target_range = {-4000, 4000};

    vector<int> nums(n_samples);
    
    default_random_engine e(time(nullptr));
    uniform_int_distribution<int> u(sample_range.first, 
                                    sample_range.second);
    for(int i = 0; i < n_samples; ++i)
        nums[i] = u(e);
    sort(nums.begin(), nums.end());

    uniform_int_distribution<int> uu(target_range.first,
                                     target_range.second);
    for(int i = 0; i < n_tests; ++i) {
        int target = uu(e);

        int find_res = find(nums, target);
        int lb_res = lower_bound(nums, target);
        int ub_res = upper_bound(nums, target);

        auto lb_iter = std::lower_bound(nums.begin(), nums.end(), target);
        auto ub_iter = std::upper_bound(nums.begin(), nums.end(), target);

        assert(lb_res == std::distance(nums.begin(), lb_iter));
        assert(ub_res == std::distance(nums.begin(), ub_iter));
        assert((find_res == -1 && *lb_iter != target) || (nums[find_res] == target));
    } 
    cout << "Pass Test" << endl;
    return 0;
}