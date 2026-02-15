#include<bits/stdc++.h>

using namespace std;

template <typename Key, typename Value>
struct Node {
    Key key;
    Value value;
    int height;
    Node *left, *right;

    Node(Key k, Value v) : key(k), value(v), height(1), left(nullptr), right(nullptr) {}
};

template <typename Key, typename Value>
class AVLTree {
private:
    Node<Key, Value> *root;

    int height(Node<Key, Value> *N) {
        if (N == nullptr) return 0;
        return N->height;
    }

    int getBalance(Node<Key, Value> *N) {
        if (N == nullptr) return 0;
        return height(N->left) - height(N->right);
    }

    Node<Key, Value>* rightRotate(Node<Key, Value> *y) {
        Node<Key, Value> *x = y->left;
        Node<Key, Value> *T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    Node<Key, Value>* leftRotate(Node<Key, Value> *x) {
        Node<Key, Value> *y = x->right;
        Node<Key, Value> *T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    Node<Key, Value>* insertHelper(Node<Key, Value>* node, Key key, Value value, bool &success) {
        if (node == nullptr) {
            success = true;
            return new Node<Key, Value>(key, value);
        }

        if (key < node->key)
            node->left = insertHelper(node->left, key, value, success);
        else if (key > node->key)
            node->right = insertHelper(node->right, key, value, success);
        else {
            success = false; 
            return node;
        }

        node->height = 1 + max(height(node->left), height(node->right));

        int balance = getBalance(node);

        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    Node<Key, Value>* minValueNode(Node<Key, Value>* node) {
        Node<Key, Value>* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    Node<Key, Value>* deleteHelper(Node<Key, Value>* root, Key key, bool &success) {
        if (root == nullptr) {
            success = false;
            return root;
        }

        if (key < root->key)
            root->left = deleteHelper(root->left, key, success);
        else if (key > root->key)
            root->right = deleteHelper(root->right, key, success);
        else {
            success = true;
            if ((root->left == nullptr) || (root->right == nullptr)) {
                Node<Key, Value> *temp = root->left ? root->left : root->right;
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                } else
                    *root = *temp;
                delete temp;
            } else {
                Node<Key, Value> *temp = minValueNode(root->right);
                root->key = temp->key;
                root->value = temp->value; 
                root->right = deleteHelper(root->right, temp->key, success);
            }
        }

        if (root == nullptr) return root;

        root->height = 1 + max(height(root->left), height(root->right));
        int balance = getBalance(root);

        if (balance > 1 && getBalance(root->left) >= 0)
            return rightRotate(root);

        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        if (balance < -1 && getBalance(root->right) <= 0)
            return leftRotate(root);

        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    void preOrder(Node<Key, Value> *root) {
        if(root != nullptr) {
            cout << root->key << " ";
            preOrder(root->left);
            preOrder(root->right);
        }
    }
    void inOrder(Node<Key, Value> *root) {
        if(root != nullptr) {
            inOrder(root->left);
            cout << root->key << " ";
            inOrder(root->right);
        }
    }
    void postOrder(Node<Key, Value> *root) {
        if(root != nullptr) {
            postOrder(root->left);
            postOrder(root->right);
            cout << root->key << " ";
        }
    }
    void levelOrder(Node<Key, Value> *root) {
        if (root == nullptr) return;
        queue<Node<Key, Value>*> q;
        q.push(root);
        while (!q.empty()) {
            Node<Key, Value> *temp = q.front();
            q.pop();
            cout << temp->key << " ";
            if (temp->left != nullptr) q.push(temp->left);
            if (temp->right != nullptr) q.push(temp->right);
        }
    }

public:
    AVLTree() : root(nullptr) {}

    bool insert(Key k, Value v) {
        bool success = false;
        root = insertHelper(root, k, v, success);
        return success;
    }

    bool remove(Key k) {
        bool success = false;
        root = deleteHelper(root, k, success);
        return success;
    }

    void traverse(int type) {
        if (type == 1) preOrder(root);
        else if (type == 2) inOrder(root);
        else if (type == 3) postOrder(root);
        else if (type == 4) levelOrder(root);
        cout << "\n";
    }
};


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int N;
    if (!(cin >> N)) return 0;
    
    AVLTree<int, int> avl;
    cout << N << "\n"; 

    for (int i = 0; i < N; ++i) {
        int e, x;
        cin >> e >> x;
        
        if (e == 0) { 
            bool res = avl.remove(x);
            cout << e << " " << x << " " << (res ? 1 : 0) << "\n";
        } else if (e == 1) { 
            bool res = avl.insert(x, x);
            cout << e << " " << x << " " << (res ? 1 : 0) << "\n";
        } else if (e == 2) { avl.traverse(x);
        }
    }
    return 0;
}