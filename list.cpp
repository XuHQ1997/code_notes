#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <random>
#include <ctime>
#include <cassert>

using namespace std;


class List {
public:
    struct ListNode {
        int val;
        ListNode* next;
        ListNode* prev;
    };
public:
    List() : fake_head{INT_MAX, &fake_tail, nullptr},
             fake_tail{INT_MAX, nullptr, &fake_head},
             n_node(0)
        {}

    ~List() {
        ListNode* cur = fake_head.next;
        ListNode* next;
        while(cur != &fake_tail) {
            next = cur->next;
            delete cur;
            cur = next;
        }
    }

    ListNode* begin(void) { return fake_head.next; }
    ListNode* end(void) { return &fake_tail; }
    int size(void) const { return n_node; }

    ListNode* get_node(int off) {
        if(off < 0 && off >= n_node)
            return &fake_tail;

        ListNode* cur = fake_head.next;
        while(off--)
            cur = cur->next;
        return cur;
    }

    ListNode* insert(ListNode* node, int val) {
        ListNode* baby = new ListNode{
            val, node, node->prev
        };
        baby->next->prev = baby;
        baby->prev->next = baby;
        ++n_node;
        return baby;
    }

    ListNode* push_front(int val) {
        return insert(fake_head.next, val);
    }

    ListNode* push_back(int val) {
        return insert(&fake_tail, val);
    }

    void remove_node(ListNode* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        delete node;
        --n_node;
    }

    void remove(int val) {
        ListNode* cur = fake_head.next;
        ListNode* next;
        while(cur != &fake_tail) {
            next = cur->next;
            if(cur->val == val)
                remove_node(cur);
            cur = next;
        }
    }
private:
    ListNode fake_head;
    ListNode fake_tail;
    int n_node;
};


int main() {
    constexpr int n_samples = 1000;
    constexpr int n_tests = 1000;

    default_random_engine e(time(nullptr));
    uniform_int_distribution<int> u(0, INT_MAX);

    list<int> stdlist;
    List mylist;

    auto do_insert = [&](void) {
        int size = stdlist.size();
        int off = u(e) % size;
        int val = u(e);
        
        auto stditer = stdlist.begin();
        std::advance(stditer, off);
        auto myiter = mylist.get_node(off);
        
        stdlist.insert(stditer, val);
        mylist.insert(myiter, val);
    };

    auto do_push_back = [&](void) {
        int val = u(e);
        stdlist.push_back(val);
        mylist.push_back(val);
    };

    auto do_push_front = [&](void) {
        int val = u(e);
        stdlist.push_front(val);
        mylist.push_front(val);
    };

    auto do_remove = [&](void) {
        int val = u(e);
        stdlist.remove(val);
        mylist.remove(val);
    };

    auto check = [&](void) {
        assert(stdlist.size() == mylist.size());

        auto stditer = stdlist.begin();
        auto myiter = mylist.begin();

        while(myiter != mylist.end() && myiter->val == *stditer) {
            myiter = myiter->next;
            ++stditer;
        }

        assert(myiter == mylist.end() && stditer == stdlist.end());
    };

    for(int i = 0; i < n_samples; ++i) {
        int val = u(e);
        stdlist.push_back(val);
        mylist.push_back(val);
    }

    for(int i = 0; i < n_tests; ++i) {
        int prob = u(e) % 100;
        if(prob < 25)
            do_insert();
        else if(prob < 50)
            do_push_back();
        else if(prob < 75)
            do_push_front();
        else
            do_remove();
        check();        
    }

    cout << "Pass Test" << endl;
    return 0;
}