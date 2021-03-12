#include <vector>
#include <iostream>
#include <queue>
#include <stack>
#include <memory>
#include <assert.h>

using namespace std;

const int NOT_EXIST = INT_MAX;

struct Node {
    int val;
    Node* lchild;
    Node* rchild;

    Node(int val_) : val(val_), lchild(nullptr), rchild(nullptr) {}
    Node() : Node(NOT_EXIST) {}
};

unique_ptr<Node[]> build_tree(const vector<int>& vals) {
    if(vals.size() <= 0 || vals[0] == NOT_EXIST)
        return nullptr;
    
    unique_ptr<Node[]> nodes(new Node[vals.size()]);
    for(int i = 0; i < vals.size(); ++i) {
        if(vals[i] == NOT_EXIST)
            continue;

        nodes[i].val = vals[i];
        int lchild_idx = i * 2 + 1;
        int rchild_idx = i * 2 + 2;
        if(lchild_idx < vals.size() && vals[lchild_idx] != NOT_EXIST)
            nodes[i].lchild = nodes.get() + lchild_idx;
        if(rchild_idx < vals.size() && vals[rchild_idx] != NOT_EXIST)
            nodes[i].rchild = nodes.get() + rchild_idx;
    }
    return nodes;
}

void bfs(vector<int>& res, Node* root) {
    queue<Node*> q;
    if(root) q.push(root);

    while(!q.empty()) {
        Node* cur = q.front();
        q.pop();

        res.push_back(cur->val);
        if(cur->lchild)
            q.push(cur->lchild);
        if(cur->rchild)
            q.push(cur->rchild);
    }
}

void preorder(vector<int>& res, Node* root) {
    if(root) {
        res.push_back(root->val);
        preorder(res, root->lchild);
        preorder(res, root->rchild);
    }
}

void preorder_nr(vector<int>& res, Node* root) {
    stack<Node*> s;

    while(!s.empty() || root) {
        while(root) {
            res.push_back(root->val);
            s.push(root);
            root = root->lchild;
        }

        root = s.top();
        s.pop();

        root = root->rchild;
    }
}

void inorder(vector<int>& res, Node* root) {
    if(root) {
        inorder(res, root->lchild);
        res.push_back(root->val);
        inorder(res, root->rchild);
    }
}

void inorder_nr(vector<int>& res, Node* root) {
    stack<Node*> s;

    while(!s.empty() || root) {
        while(root) {
            s.push(root);
            root = root->lchild;
        }

        root = s.top();
        s.pop();

        res.push_back(root->val);
        root = root->rchild;
    }
}

void postorder(vector<int>& res, Node* root) {
    if(root) {
        postorder(res, root->lchild);
        postorder(res, root->rchild);
        res.push_back(root->val);
    }
}

void postorder_nr(vector<int>& res, Node* root) {
    stack<pair<Node*, bool>> s;

    while(!s.empty() || root) {
        while(root) {
            s.push({root, false});
            root = root->lchild;
        }

        auto&& top = s.top();
        if(top.second) {
            s.pop();
            res.push_back(top.first->val);
        } else {
            top.second = true;
            root = top.first->rchild;
        }
    }
}


int main() {
    const int X = NOT_EXIST;
    auto nodes_ptr = build_tree({
        0, 
        1, 2, 
        X, 4, 5, X, 
        X, X, 9, 10, X, 12, X, X,
        X, X, X, X, 19, 20, X, 22, X, X, X, 26
    });
    auto root = nodes_ptr.get();

    vector<int> bfs_res;
    bfs(bfs_res, root);
    for(auto i: bfs_res)
        cout << i << " ";
    cout << endl;

    auto assert_and_output = [](const vector<int>& a, const vector<int>& b) {
        assert(a.size() == b.size());
        for(int i = 0; i < a.size(); ++i) {
            assert(a[i] == b[i]);
            cout << a[i] << " ";
        }
        cout << endl;
    };

    vector<int> pre_res, pre_res_nr;
    preorder(pre_res, root);
    preorder_nr(pre_res_nr, root);
    assert_and_output(pre_res, pre_res_nr);

    vector<int> in_res, in_res_nr;
    inorder(in_res, root);
    inorder_nr(in_res_nr, root);
    assert_and_output(in_res, in_res_nr);

    vector<int> post_res, post_res_nr;
    postorder(post_res, root);
    postorder_nr(post_res_nr, root);
    assert_and_output(post_res, post_res_nr);

    return 0;
}
