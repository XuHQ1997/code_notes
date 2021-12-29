#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <memory>
#include <random>
#include <ctime>
#include <set>
#include <cassert>

int node_count = 0;

struct Node {
    int count;
    std::unique_ptr<Node> children[26];

    Node() : count(0) { ++node_count; }
    ~Node() { --node_count; }
};


Node* insert(const std::string& str, Node* root) {
    for (char c : str) {
        int idx = c - 'a';
        if (!root->children[idx]) {
            root->children[idx] = std::make_unique<Node>();
        }

        root = root->children[idx].get();
    }
    ++root->count;
    return root;
}

Node* find(const std::string& str, Node* root) {
    for (char c : str) {
        int idx = c - 'a';
        root = root->children[idx].get();
        if (root == nullptr) {
            break;
        }
    }
    return root;
}

int count(const std::string& str, Node* root) {
    Node* node = find(str, root);
    if (node == nullptr) return 0;
    else return node->count;
}


int main() {
    const int n_strings = 1000;
    const int n_tests = 1000;
    std::default_random_engine e(time(0));
    std::uniform_int_distribution<char> char_u('a', 'z');
    std::uniform_int_distribution<int> size_u(1, 50);

    auto generate_string = [&e, &char_u, &size_u]()->std::string {
        int length = size_u(e);
        std::string res(length, 'x');

        for (int i = 0; i < res.length(); ++i) {
            res[i] = char_u(e);
        }
        return res;
    };

    {
        Node root;
        std::multiset<std::string> data;
        for (int i = 0; i < n_strings; ++i) {
            std::string str = generate_string();
            data.insert(str);
            insert(str, &root);
        }

        for (const std::string& str : data) {
            assert(count(str, &root) != 0);
        }

        for (int i = 0; i < n_tests; ++i) {
            std::string str = generate_string();
            int multiset_count = data.count(str);
            int trietree_count = count(str, &root);
            assert(multiset_count == trietree_count);
        }
    }
    assert(node_count == 0);
    std::cout << "test pass" << std::endl;
    return 0;
}
