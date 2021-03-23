#include <vector>
#include <iostream>
#include <random>
#include <string>

#include <ctime>
#include <cassert>
#include <cstring>

using namespace std;

namespace select {
void sort(vector<int>& nums, int start, int end) {
    for(int i = start; i < end; ++i) {
        int min_value = nums[i];
        int min_idx = i;

        for(int j = i + 1; j < end; ++j) {
            if(min_value > nums[j]) {
                min_value = nums[j];
                min_idx = j;
            }
        }

        swap(nums[i], nums[min_idx]);
    }
}
}  // namespace select

namespace bubble {
void sort(vector<int>& nums, int start, int end) {
    for(int i = end; i > start; --i) {
        bool flag = false;
        for(int j = 1; j < i; ++j) {
            if(nums[j-1] > nums[j]) {
                swap(nums[j-1], nums[j]);
                flag = true;
            }
        }
    }
}
}  // namespace bubble

namespace heap {
inline int lchild(int i) { return i * 2 + 1; }
inline int rchild(int i) { return i * 2 + 2; }

void __adjust_heap(vector<int>& nums, int start, int end) {
    int root = start;
    for(;;) {
        int l = lchild(root);
        int r = rchild(root);
        int swap_to = root;

        // swap to the greater child
        if(l < end && nums[swap_to] < nums[l])
            swap_to = l;
        if(r < end && nums[swap_to] < nums[r])
            swap_to = r;
        
        // if root is greater than its two children,
        // it already satify the rule of heap.
        if(swap_to != root) {
            swap(nums[swap_to], nums[root]);
            root = swap_to;
        } else {
            break;
        }
    }
}

void sort(vector<int>& nums, int start, int end) {
    assert(start == 0);

    for(int i = start + (end - start) / 2; i >= 0; --i) {
        __adjust_heap(nums, i, end);
    }
    
    for(int i = end - 1; i > start; --i) {
        swap(nums[i], nums[start]);
        __adjust_heap(nums, start, i);
    }
}
}  // namespace heap

namespace quick {
void sort(vector<int>& nums, int start, int end) {
    if(start + 1 >= end)
        return ;
    
    int i = start;
    int j = end - 1;
    int key = nums[i];

    while(i < j) {
        while(i < j && nums[j] >= key)
            --j;
        if(i < j)
            nums[i++] = nums[j];
        
        while(i < j && nums[i] <= key)
            ++i;
        if(i < j)
            nums[j--] = nums[i];
    }
    nums[i] = key;

    sort(nums, start, i);
    sort(nums, i + 1, end);
}
}  // namespace quick


namespace merge {
void sort(vector<int>& nums, int start, int end) {
    if(start + 1 >= end)
        return ;
    
    int mid = start + (end - start) / 2;
    sort(nums, start, mid);
    sort(nums, mid, end);

    vector<int> buffer(end - start);
    int p = start;
    int q = mid;
    int i = 0;
    while(p < mid && q < end) {
        if(nums[p] <= nums[q])
            buffer[i++] = nums[p++];
        else    
            buffer[i++] = nums[q++];
    }
    while(p < mid)
        buffer[i++] = nums[p++];
    while(q < end)
        buffer[i++] = nums[q++];
    
    memcpy(nums.data() + start, buffer.data(), (end - start) * sizeof(int));
}
}  // namespace merge

namespace radix {
void sort(vector<int>& nums, int begin, int end) {
    vector<int> buf(end - begin);
    int max_value = INT_MIN;
    for(int i = begin; i < end; ++i)
        max_value = max(max_value, nums[i]);

    for(int exp = 1; exp <= max_value; exp *= 10) {
        vector<int> count(10, 0);
        for(int i = begin; i < end; ++i) {
            int digit = (nums[i] / exp) % 10;
            ++count[digit];
        }

        for(int i = 1; i < count.size(); ++i)
            count[i] += count[i-1];

        for(int i = end - 1; i >= begin; --i) {
            int digit = (nums[i] / exp) % 10;
            buf[--count[digit]] = nums[i];
        }

        for(int i = 0; i < buf.size(); ++i)
            nums[begin + i] = buf[i];
    }
}
}  // namespace radix


int main() {
    constexpr int n_tests = 100;
    constexpr int n_samples = 1000;

    using sort_fn = void(*)(vector<int>&, int, int);
    vector<pair<string, sort_fn>> handlers{
        {"select", select::sort},
        {"bubble", bubble::sort}, 
        {"heap", heap::sort},
        {"quick", quick::sort},
        {"merge", merge::sort},
        {"radix", radix::sort}  // only work on nonnegtive integers.
    };

    default_random_engine e(time(0));
    uniform_int_distribution<int> u(0, n_samples);
    auto init_nums = [&u, &e]() {
        vector<int> nums(n_samples, 0);
        for(int i = 0; i < nums.size(); ++i)
            nums[i] = u(e);
        return nums;
    };

    auto check_sort = [](vector<int>& nums) {
        int i = 1;
        for(; i < nums.size() && nums[i] >= nums[i-1]; ++i)
            ;
        assert(i == nums.size());
    };

    for(auto& named_fn: handlers) {
        auto name = named_fn.first;
        auto fn = named_fn.second;
        
        for(int i = 0; i < n_tests; ++i)  {
            auto nums = init_nums();
            fn(nums, 0, nums.size());
            check_sort(nums);
        }

        cout << name << " passing test" << endl;
    }

    cout << "All Passing Test" << endl;
    return 0;
}
