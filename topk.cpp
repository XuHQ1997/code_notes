#include <iostream>
#include <vector>
#include <ctime>
#include <random>
#include <algorithm>
#include <cassert>

using namespace std;

namespace bubble {
int topk(vector<int>& nums, int begin, int end, int k) {
    for(int i = begin; i <= begin + k; ++i) {
        for(int j = end - 1; j > i; --j) {
            if(nums[j] > nums[j-1])
                swap(nums[j], nums[j-1]);
        }
    }
    return nums[begin + k];
}
}  // namespace bubble

namespace partition {
int topk(vector<int>& nums, int begin, int end, int k) {
    int i = begin;
    int j = end - 1;
    int key = nums[begin];

    while(i < j) {
        while(i < j && nums[j] <= key)
            --j;
        if(i < j)
            nums[i] = nums[j];

        while(i < j && nums[i] >= key)
            ++i;
        if(i < j)
            nums[j] = nums[i];
    }
    nums[i] = key;

    if(i == k)
        return nums[i];
    else if(i > k)
        return topk(nums, begin, i, k);
    else
        return topk(nums, i + 1, end, k);
}
}  // namespace partition

namespace heap {
void adjust_heap(vector<int>& heap, int root, int end) {
    for(;;) {
        int min_child = root;
        int lchild = root * 2 + 1;
        int rchild = root * 2 + 2;
        if(lchild < end && heap[lchild] < heap[min_child])
            min_child = lchild;
        if(rchild < end && heap[rchild] < heap[min_child])
            min_child = rchild;

        if(min_child == root)
            break;
        swap(heap[min_child], heap[root]);
        root = min_child;
    }
}

int topk(vector<int>& nums, int begin, int end, int k) {
    assert(begin == 0);

    int heap_end = k + 1;
    for(int i = heap_end/2; i >= 0; --i)
        adjust_heap(nums, i, heap_end);
    
    for(int i = heap_end; i < end; ++i) {
        if(nums[i] > nums[0]) {
            swap(nums[i], nums[0]);
            adjust_heap(nums, 0, heap_end);
        }
    }
    swap(nums[0], nums[k]);
    return nums[k];
}
}  // namespace heap


int main() {
    constexpr int n_samples = 1000;
    constexpr int n_tests = 100;

    default_random_engine e(time(nullptr));
    uniform_int_distribution<int> u(0, n_samples);
    uniform_int_distribution<int> qu(0, n_samples-1);
    vector<int> nums(n_samples);

    using topk_fn = int(*)(vector<int>&, int, int, int);
    vector<pair<string, topk_fn>> handlers {
        {"partition", partition::topk},
        {"heap", heap::topk},
        {"bubble", bubble::topk},
    };

    for(auto& named_fn : handlers) {
        for(int i = 0; i < n_tests; ++i) {
            // sample data
            for(int& n : nums) 
                n = u(e);
            int k = qu(e);

            // run topk function
            int res = named_fn.second(nums, 0, nums.size(), k);

            // assert
            for(int j = 0; j < k; ++j)
                assert(nums[j] >= nums[k]);
            for(int j = k + 1; j < nums.size(); ++j)
                assert(nums[j] <= nums[k]);
            assert(res == nums[k]);
        }
        cout << named_fn.first << " Pass Test" << endl;
    }

    cout << "All Pass Test" << endl;
    return 0;
}
