#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

template <typename T>
class AVLNode {
public:
    T key;
    int height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(T k) : key(k), height(1), left(nullptr), right(nullptr) {}
};

template <typename T>
class AVLTree {
private:
    AVLNode<T>* root;

    int height(AVLNode<T>* node) {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    int getBalance(AVLNode<T>* node) {
        if (node == nullptr)
            return 0;
        return height(node->left) - height(node->right);
    }

    AVLNode<T>* rightRotate(AVLNode<T>* y) {
        AVLNode<T>* x = y->left;
        AVLNode<T>* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    AVLNode<T>* leftRotate(AVLNode<T>* x) {
        AVLNode<T>* y = x->right;
        AVLNode<T>* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    AVLNode<T>* insertHelper(AVLNode<T>* node, T key, bool& success) {
        if (node == nullptr) {
            success = true;
            return new AVLNode<T>(key);
        }

        if (key < node->key) {
            node->left = insertHelper(node->left, key, success);
        } else if (key > node->key) {
            node->right = insertHelper(node->right, key, success);
        } else {
            // Duplicate key
            success = false;
            return node;
        }

        node->height = 1 + max(height(node->left), height(node->right));

        int balance = getBalance(node);

        // Left Left Case
        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    AVLNode<T>* minValueNode(AVLNode<T>* node) {
        AVLNode<T>* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    AVLNode<T>* deleteHelper(AVLNode<T>* node, T key, bool& success) {
        if (node == nullptr) {
            success = false;
            return node;
        }

        if (key < node->key) {
            node->left = deleteHelper(node->left, key, success);
        } else if (key > node->key) {
            node->right = deleteHelper(node->right, key, success);
        } else {
            // Node found
            success = true;

            if ((node->left == nullptr) || (node->right == nullptr)) {
                AVLNode<T>* temp = node->left ? node->left : node->right;

                if (temp == nullptr) {
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }
                delete temp;
            } else {
                AVLNode<T>* temp = minValueNode(node->right);
                node->key = temp->key;
                node->right = deleteHelper(node->right, temp->key, success);
                success = true; // Reset since recursive call might have changed it
            }
        }

        if (node == nullptr)
            return node;

        node->height = 1 + max(height(node->left), height(node->right));

        int balance = getBalance(node);

        // change here >= to > ok baad
        // Left Left Case
        if (balance > 1 && getBalance(node->left) >= 0)
            return rightRotate(node);

        // Left Right Case
        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Right Case
        if (balance < -1 && getBalance(node->right) <= 0)
            return leftRotate(node);

        // Right Left Case
        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    void preOrderHelper(AVLNode<T>* node, vector<T>& result) {
        if (node != nullptr) {
            result.push_back(node->key);
            preOrderHelper(node->left, result);
            preOrderHelper(node->right, result);
        }
    }

    void inOrderHelper(AVLNode<T>* node, vector<T>& result) {
        if (node != nullptr) {
            inOrderHelper(node->left, result);
            result.push_back(node->key);
            inOrderHelper(node->right, result);
        }
    }

    void postOrderHelper(AVLNode<T>* node, vector<T>& result) {
        if (node != nullptr) {
            postOrderHelper(node->left, result);
            postOrderHelper(node->right, result);
            result.push_back(node->key);
        }
    }

    vector<T> levelOrderHelper() {
        vector<T> result;
        if (root == nullptr)
            return result;

        queue<AVLNode<T>*> q;
        q.push(root);

        while (!q.empty()) {
            AVLNode<T>* node = q.front();
            q.pop();
            result.push_back(node->key);

            if (node->left != nullptr)
                q.push(node->left);
            if (node->right != nullptr)
                q.push(node->right);
        }

        return result;
    }

public:
    AVLTree() : root(nullptr) {}

    bool insert(T key) {
        bool success = false;
        root = insertHelper(root, key, success);
        return success;
    }

    bool deleteNode(T key) {
        bool success = false;
        root = deleteHelper(root, key, success);
        return success;
    }

    vector<T> preOrder() {
        vector<T> result;
        preOrderHelper(root, result);
        return result;
    }

    vector<T> inOrder() {
        vector<T> result;
        inOrderHelper(root, result);
        return result;
    }

    vector<T> postOrder() {
        vector<T> result;
        postOrderHelper(root, result);
        return result;
    }

    vector<T> levelOrder() {
        return levelOrderHelper();
    }
};

int main() {
    ifstream infile("input_avl.txt");
    ofstream outfile("output_avl.txt");

    int n;
    infile >> n;

    vector<string> results;
    AVLTree<int> avl;

    for (int i = 0; i < n; i++) {
        int ei, xi;
        infile >> ei >> xi;

        if (ei == 0) {
            // Delete
            int ri = avl.deleteNode(xi) ? 1 : 0;
            results.push_back(to_string(ei) + " " + to_string(xi) + " " + to_string(ri));
        } else if (ei == 1) {
            // Insert
            int ri = avl.insert(xi) ? 1 : 0;
            results.push_back(to_string(ei) + " " + to_string(xi) + " " + to_string(ri));
        } else if (ei == 2) {
            // Traverse
            vector<int> traversal;
            
            if (xi == 1) {
                traversal = avl.preOrder();
            } else if (xi == 2) {
                traversal = avl.inOrder();
            } else if (xi == 3) {
                traversal = avl.postOrder();
            } else if (xi == 4) {
                traversal = avl.levelOrder();
            }

            string traversalStr = "";
            for (size_t j = 0; j < traversal.size(); j++) {
                traversalStr += to_string(traversal[j]);
                if (j < traversal.size() - 1)
                    traversalStr += " ";
            }
            results.push_back(traversalStr);
        }
    }

    outfile << results.size() << endl;
    for (const string& result : results) {
        outfile << result << endl;
    }

    infile.close();
    outfile.close();

    return 0;
}

// g++ -o avl avl.cpp -std=c++11
// avl.exe
