#include <iostream>
#include <random>
#include <ctime>
#include <memory>
#include <cassert>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
};

int get_list_length(ListNode* head) {
    int ret = 0;
    while(head) {
        ++ret;
        head = head->next;
    }
    return ret;
}

ListNode* reverse(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* cur = head;

    while(cur) {
        ListNode* next = cur->next;

        cur->next = prev;

        prev = cur;
        cur = next;
    }
    return prev;
}

ListNode* get_Kth_from_last(ListNode* head, int k) {
    ListNode* fast = head;
    ListNode* slow = head;
    while(k--)
        fast = fast->next;
    while(fast) {
        fast = fast->next;
        slow = slow->next;
    }
    return slow;
}

ListNode* get_mid_node(ListNode* head) {
    ListNode* fast = head;
    ListNode* slow = head;
    while(fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
    }
    return slow;
}


int main() {
    constexpr int n_tests = 10;
    constexpr int max_n_node = 1000;
    
    default_random_engine e(time(nullptr));
    uniform_int_distribution<int> n_node_dist(0, max_n_node);

    auto generate_linkedlist = [&]()->unique_ptr<ListNode[]> {
        int n_node = n_node_dist(e);
        unique_ptr<ListNode[]> dataptr(
            new ListNode[n_node]
        );
        auto data = dataptr.get();
        for(int i = 0; i < n_node - 1; ++i)
            data[i] = {i, &data[i+1]};
        data[n_node-1] = {n_node-1, nullptr};
        return dataptr;
    };

    /* ===============================================
        test for reverse
    =============================================== */
    for(int i = 0; i < n_tests; ++i) {
        auto dataptr = generate_linkedlist();
        ListNode* head = dataptr.get();
        int length = get_list_length(head);

        head = reverse(head);

        assert(get_list_length(head) == length);
        while(head) {
            assert(head->val == length - 1);
            --length;
            head = head->next;
        }
    }
    cout << "Reverse Pass." << endl;

    /* ===============================================
        test for get_Kth_from_last and get_mid_node
    =============================================== */
    for(int i = 0; i < n_tests; ++i) {
        auto dataptr = generate_linkedlist();
        ListNode* head = dataptr.get();
        int length = get_list_length(head);

        int k = (n_node_dist(e) % length) + 1;
        ListNode* ret = get_Kth_from_last(head, k);
        assert(ret->val == head[length-k].val);

        ret = get_mid_node(head);
        assert(ret->val == head[length/2].val);
    }
    cout << "getKthFromlast Pass." << endl;
    cout << "getMidNode Pass" << endl;
    return 0;
}
