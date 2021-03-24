#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#include <cstring>
#include <cassert>

using namespace std;


bool next_permutation(vector<int>& nums) {
    int i = nums.size() - 1;
    for(; i > 0 && nums[i] <= nums[i-1]; --i)
        ;
    if(i == 0)
        return false;
    
    --i;
    int j = nums.size() - 1;
    for(; nums[j] <= nums[i]; --j)
        ;
    
    swap(nums[i], nums[j]);

    for(++i, j = nums.size() - 1; i < j; ++i, --j)
        swap(nums[i], nums[j]);
    return true;
}

bool prev_permutation(vector<int>& nums) {
    int i = nums.size() - 1;
    for(; i > 0 && nums[i] >= nums[i-1]; --i)
        ;
    if(i == 0)
        return false;
    
    --i;
    int j = nums.size() - 1;
    for(; nums[j] >= nums[i]; --j);
        ;
    
    swap(nums[i], nums[j]);

    for(++i, j = nums.size() - 1; i < j; ++i, --j)
        swap(nums[i], nums[j]);
    return true;
}


int main() {
    constexpr int n_samples = 10;

    default_random_engine e(time(nullptr));
    uniform_int_distribution<int> u(0, n_samples);
    vector<int> nums1(n_samples);
    vector<int> nums2(n_samples);
    bool res1, res2;

    /* ===============================================
        test for next_permutation
       =============================================== */
    for(int i = 0; i < n_samples; ++i)
        nums1[i] = nums2[i] = u(e);
    for(;;) {
        res1 = next_permutation(nums1);
        res2 = std::next_permutation(nums2.begin(), nums2.end());
        if(!res1 || !res2)
            break;

        assert(memcmp(nums1.data(), nums2.data(), sizeof(int) * n_samples) == 0);
    }
    assert(res1 == res2);

    /* ===============================================
        test for prev_permutation
       =============================================== */
    for(int i = 0; i < n_samples; ++i)
        nums1[i] = nums2[i] = u(e);
    for(;;) {
        res1 = prev_permutation(nums1);
        res2 = std::prev_permutation(nums2.begin(), nums2.end());
        if(!res1 || !res2)
            break;

        assert(memcmp(nums1.data(), nums2.data(), sizeof(int) * n_samples) == 0);
    }

    cout << "Passing Test." << endl;
    return 0;
}
