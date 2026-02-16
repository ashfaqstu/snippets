#include <bits/stdc++.h>

using namespace std;

// Enum for the specific probing types requested in the PDF
enum ProbingType { LINEAR, QUADRATIC };

template <typename K, typename V>
class HashTable {
private:
    struct Node {
        K key;
        V value;
        bool isDeleted; // To handle DELETE operations lazily
        Node(K k, V v) : key(k), value(v), isDeleted(false) {}
    };

    int N;
    int n;
    ProbingType type; // Stores whether this table uses LINEAR or QUADRATIC
    int hashFuncChoice; // 1 for Outer (Hash1), 2 for Inner (Hash2)
    vector<Node*> table;

    // Helper to convert Int keys to String for the hash functions
    string keyToString(K key) {
        stringstream ss;
        ss << key;
        return ss.str();
    }

    // Hash Function 1 (For Outer Table)
    unsigned long long calculateHash1(string keyStr) {
        unsigned long long hash = 0;
        unsigned long long p = 31;
        for (char c : keyStr) {
            hash = (hash * p + (c - 'a' + 1));
            hash %= N;
        }
        return hash;
    }

    // Hash Function 2 (For Inner Table)
    unsigned long long calculateHash2(string keyStr) {
        unsigned long long hash = 5381;
        for (char c : keyStr) {
            hash = ((hash << 5) + hash) + c; 
        }
        return hash % N;
    }

    unsigned long long getHash(K key) {
        string keyStr = keyToString(key);
        if (hashFuncChoice == 1) return calculateHash1(keyStr);
        return calculateHash2(keyStr);
    }

public:
    HashTable(int size, ProbingType pt, int hc) {
        N = size;
        type = pt;
        hashFuncChoice = hc;
        n = 0;
        table.assign(N, nullptr);
    }

    ~HashTable() {
        for (int i = 0; i < N; i++) {
            if (table[i]) delete table[i];
        }
    }

    // Insert function
    void insert(K key, V value) {
        unsigned long long h = getHash(key);

        for (int i = 0; i < N; i++) {
            unsigned long long idx;
            
            // Logic strictly from PDF:
            // Outer (Linear): (Hash1 + i) % N
            // Inner (Quadratic): (Hash2 + i*i) % N
            if (type == LINEAR) {
                idx = (h + i) % N;
            } else {
                idx = (h + i * i) % N;
            }

            // If empty slot or deleted slot, insert here
            if (table[idx] == nullptr || table[idx]->isDeleted) {
                if (table[idx]) delete table[idx]; // Clean up old deleted node
                table[idx] = new Node(key, value);
                n++;
                return;
            }
            // If key already exists, update value
            if (table[idx]->key == key) {
                table[idx]->value = value;
                table[idx]->isDeleted = false;
                return;
            }
        }
    }

    // Search function: returns a pointer to Value (or nullptr if not found)
    V* search(K key) {
        unsigned long long h = getHash(key);

        for (int i = 0; i < N; i++) {
            unsigned long long idx;
            if (type == LINEAR) idx = (h + i) % N;
            else idx = (h + i * i) % N;

            if (table[idx] == nullptr) return nullptr; // Stop if we hit empty
            
            if (!table[idx]->isDeleted && table[idx]->key == key) {
                return &table[idx]->value;
            }
        }
        return nullptr;
    }

    // Delete function: Marks node as deleted and returns true if found
    bool remove(K key, V* outVal = nullptr) {
        unsigned long long h = getHash(key);

        for (int i = 0; i < N; i++) {
            unsigned long long idx;
            if (type == LINEAR) idx = (h + i) % N;
            else idx = (h + i * i) % N;

            if (table[idx] == nullptr) return false;

            if (!table[idx]->isDeleted && table[idx]->key == key) {
                table[idx]->isDeleted = true;
                if(outVal) *outVal = table[idx]->value; // Copy value out before "deleting"
                n--;
                return true;
            }
        }
        return false;
    }

    // Helper to print all items (For SEARCH <group_id>)
    void printAll() {
        vector<pair<K, V>> items;
        for(int i=0; i<N; i++) {
            if(table[i] && !table[i]->isDeleted) {
                items.push_back({table[i]->key, table[i]->value});
            }
        }
        
        // Output format: (1, read), (2, write)
        for(size_t i=0; i<items.size(); i++) {
            cout << "(" << items[i].first << ", " << items[i].second << ")";
            if(i != items.size() - 1) cout << ", ";
        }
        cout << endl;
    }
};

int main() {
    int N, Q;
    cin >> N >> Q;

    // Outer Table: Key = int (Group ID), Value = Pointer to Inner Table
    // Uses LINEAR probing (Type 0) and Hash1 (1)
    HashTable<int, HashTable<int, string>*> outerTable(N, LINEAR, 1);

    while(Q--) {
        string command;
        cin >> command;

        if (command == "INSERT") {
            int gid, uid;
            string perm;
            cin >> gid >> uid >> perm;

            // 1. Find or Create Group in Outer Table
            HashTable<int, string>** innerTablePtr = outerTable.search(gid);
            HashTable<int, string>* innerTable = nullptr;

            if (innerTablePtr == nullptr) {
                // Group doesn't exist, create new Inner Table
                // Inner Table: Key = int (User ID), Value = string (Permission)
                // Uses QUADRATIC probing (Type 1) and Hash2 (2)
                innerTable = new HashTable<int, string>(N, QUADRATIC, 2);
                outerTable.insert(gid, innerTable);
            } else {
                innerTable = *innerTablePtr;
            }

            // 2. Insert User into Inner Table
            innerTable->insert(uid, perm);
        }
        else if (command == "SEARCH") {
            // FIX: Use getline to handle 1 vs 2 arguments robustly
            string line;
            getline(cin, line); 
            stringstream ss(line);
            
            int gid;
            ss >> gid; // Read Group ID
            
            int uid;
            if (ss >> uid) { 
                // CASE 1: Two arguments found -> SEARCH <gid> <uid>
                HashTable<int, string>** innerPtr = outerTable.search(gid);
                if (innerPtr == nullptr) {
                    cout << "Group not found" << endl;
                } else {
                    string* perm = (*innerPtr)->search(uid);
                    if (perm) cout << *perm << endl;
                    else cout << "User not found in group " << gid << endl;
                }
            } 
            else {
                // CASE 2: Only one argument found -> SEARCH <gid>
                HashTable<int, string>** innerPtr = outerTable.search(gid);
                if (innerPtr == nullptr) {
                    cout << "Group not found" << endl;
                } else {
                    (*innerPtr)->printAll();
                }
            }
        }
        else if (command == "DELETE") {
            int gid, uid;
            cin >> gid >> uid;
            
            HashTable<int, string>** innerPtr = outerTable.search(gid);
            if (innerPtr == nullptr) {
                cout << "Group not found" << endl;
            } else {
                string deletedPerm;
                bool success = (*innerPtr)->remove(uid, &deletedPerm);
                if (success) {
                    cout << "(" << uid << ", " << deletedPerm << ") deleted" << endl;
                } else {
                    cout << "User not found" << endl; // Or specific error if needed
                }
            }
        }
    }

    return 0;
}