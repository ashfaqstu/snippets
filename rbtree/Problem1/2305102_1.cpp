#include<bits/stdc++.h>

using namespace std;

enum Color { RED, BLACK };

template <typename Key, typename Value>
struct Node {
    Key key;
    Value value;
    Color color;
    Node *left, *right, *parent;
    int subtree_size; 

    Node(Key k, Value v) : key(k), value(v), color(RED), left(nullptr), right(nullptr), parent(nullptr), subtree_size(1) {}
};

template <typename Key, typename Value>
class RBTree {
private:
    Node<Key, Value> *root;
    Node<Key, Value> *TNULL; 

    void updateSize(Node<Key, Value>* node) {
        if (node != TNULL) {
            node->subtree_size = 1 +node->left->subtree_size +node->right->subtree_size;
        }
    }

    void leftRotate(Node<Key, Value> *x) {
        Node<Key,Value>*y =x->right;
        x->right =y->left;
        if (y->left != TNULL) y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
        
        y->subtree_size = x->subtree_size;
        updateSize(x); 
    }

    void rightRotate(Node<Key, Value> *y) {
        Node<Key, Value> *x = y->left;
        y->left = x->right;
        if (x->right != TNULL) x->right->parent = y;
        x->parent = y->parent;
        if (y->parent == nullptr) root = x;
        else if (y == y->parent->right) y->parent->right = x;
        else y->parent->left = x;
        x->right = y;
        y->parent = x;

        x->subtree_size = y->subtree_size;
        updateSize(y);
    }

    void insertFix(Node<Key, Value> *k) {
        Node<Key, Value> *u;
        while (k->parent->color == RED) {
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left;
                if (u->color == RED) {
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rightRotate(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    leftRotate(k->parent->parent);
                }
            } else {
                u = k->parent->parent->right;
                if (u->color == RED) {
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    rightRotate(k->parent->parent);
                }
            }
            if (k == root) break;
        }
        root->color = BLACK;
    }

    void rbTransplant(Node<Key, Value> *u, Node<Key, Value> *v) {
        if (u->parent == nullptr) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        v->parent = u->parent;
    }
    
    Node<Key, Value>* minimum(Node<Key, Value>* node) {
        while (node->left != TNULL) node = node->left;
        return node;
    }

    void deleteFix(Node<Key, Value> *x) {
        Node<Key, Value> *s;
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                s = x->parent->right;
                if (s->color == RED) {
                    s->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    s = x->parent->right;
                }
                if (s->left->color == BLACK && s->right->color == BLACK) {
                    s->color = RED;
                    x = x->parent;
                } else {
                    if (s->right->color == BLACK) {
                        s->left->color = BLACK;
                        s->color = RED;
                        rightRotate(s);
                        s = x->parent->right;
                    }
                    s->color = x->parent->color;
                    x->parent->color = BLACK;
                    s->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                s = x->parent->left;
                if (s->color == RED) {
                    s->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    s = x->parent->left;
                }
                if (s->right->color == BLACK && s->left->color == BLACK) {
                    s->color = RED;
                    x = x->parent;
                } else {
                    if (s->left->color == BLACK) {
                        s->right->color = BLACK;
                        s->color = RED;
                        leftRotate(s);
                        s = x->parent->left;
                    }
                    s->color = x->parent->color;
                    x->parent->color = BLACK;
                    s->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

public:
    RBTree() {
        TNULL = new Node<Key, Value>(Key(), Value());
        TNULL->color = BLACK;
        TNULL->subtree_size = 0;
        TNULL->left = nullptr;
        TNULL->right = nullptr;
        root = TNULL;
    }

    bool insert(Key key, Value value) {
        if (search(key)) return false; 

        Node<Key, Value> *node = new Node<Key, Value>(key, value);
        node->parent = nullptr;
        node->left = TNULL;
        node->right = TNULL;
        node->color = RED;

        Node<Key, Value> *y = nullptr;
        Node<Key, Value> *x = root;

        while (x != TNULL) {
            y = x;
            x->subtree_size++; 
            if (node->key < x->key) x = x->left;
            else x = x->right;
        }

        node->parent = y;
        if (y == nullptr) root = node;
        else if (node->key < y->key) y->left = node;
        else y->right = node;

        if (node->parent == nullptr) {
            node->color = BLACK;
            return true;
        }
        if (node->parent->parent == nullptr) return true;

        insertFix(node);
        return true;
    }

    bool search(Key k) {
        Node<Key, Value> *current = root;
        while (current != TNULL) {
            if (k == current->key) return true;
            if (k < current->key) current = current->left;
            else current = current->right;
        }
        return false;
    }

    int countLessThan(Key k) {
        int count = 0;
        Node<Key, Value> *curr = root;
        while (curr != TNULL) {
            if (k > curr->key) {
                count += curr->left->subtree_size + 1;
                curr = curr->right;
            } else {
                curr = curr->left;
            }
        }
        return count;
    }

    bool deleteNode(Key key) {
        Node<Key, Value> *z = root;
        Node<Key, Value> *x, *y;
        while (z != TNULL) {
            if (z->key == key) break;
            if (z->key > key) z = z->left;
            else z = z->right;
            //cout<<"jlrlrl"<<endl;
        }
        if (z == TNULL) return false; 
        y = z;
        Color y_original_color = y->color;
       
        if (z->left == TNULL) {
            x = z->right;
            rbTransplant(z, z->right);
        } else if (z->right == TNULL) {
            x = z->left;
            rbTransplant(z, z->left);
        } else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
            } else {
                rbTransplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            rbTransplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        
        Node<Key, Value> *trace = (x->parent == nullptr) ? root : x->parent; 
        while(trace != nullptr && trace != TNULL) {
            updateSize(trace);
            trace = trace->parent;
        }
      
        if (y_original_color == BLACK) {
            deleteFix(x);
        }
        
        if(root != TNULL) updateSize(root);
        
        delete z;
        return true;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int N;
    if (!(cin >> N)) return 0;

    RBTree<int, int> rbt;
    

    cout << N << "\n";

    for (int i = 0; i < N; ++i) {
        int e, x;
        cin >> e >> x;
        cout << e << " " << x << " ";

        if (e == 0) { 
            bool res = rbt.deleteNode(x);
            cout << (res ? 1 : 0) << "\n";
        } 
        else if (e == 1) {
            bool res = rbt.insert(x, x); 
            cout << (res ? 1 : 0) << "\n";
        } 
        else if (e == 2) { 
            bool res = rbt.search(x);
            cout << (res ? 1 : 0) << "\n";
        } 
        else if (e == 3) { 
            int count = rbt.countLessThan(x);
            cout << count << "\n";
        }
    }

    return 0;
}