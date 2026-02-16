#include <bits/stdc++.h>

using namespace std;

// --- 1. Define the Data Structure for a Citizen ---
struct Citizen {
    string name;
    string nid;
    string phone;
};

// --- 2. Keep the HashTable Class (Standard Implementation) ---
enum Method { CHAINING, DOUBLE_HASHING, CUSTOM_PROBING };

template <typename K, typename V>
class HashTable {
private:
    struct Node {
        K key;
        V value;
        bool isDeleted;
        Node(K k, V v) : key(k), value(v), isDeleted(false) {}
    };

    int N;                  
    int n;                  
    int lastResizeN;        
    int opCountSinceResize; 
    
    Method method;
    int hashChoice;             
    
    vector<Node*> openTable;       
    const int INITIAL_SIZE = 1009; // Use a prime > 1000 as requested
    const double MAX_LOAD = 0.5;
    const double MIN_LOAD = 0.25;
    const int C1 = 3; 
    const int C2 = 5;

    string keyToString(K key) {
        stringstream ss;
        ss << key;
        return ss.str();
    }

    bool isPrime(int num) {
        if (num <= 1) return false;
        if (num <= 3) return true;
        if (num % 2 == 0 || num % 3 == 0) return false;
        for (int i = 5; i * i <= num; i += 6) {
            if (num % i == 0 || num % (i + 2) == 0) return false;
        }
        return true;
    }

    int nextPrime(int num) {
        if (num <= 2) return 2;
        if (num % 2 == 0) num++;
        while (!isPrime(num)) num += 2;
        return num;
    }

    int prevPrime(int num) {
        if (num < 2) return 2;
        if (num % 2 == 0) num--;
        while (num >= 13 && !isPrime(num)) num -= 2;
        return (num < 13) ? 13 : num;
    }

    unsigned long long calculateHash1(string keyStr) {
        unsigned long long hash = 0;
        unsigned long long p = 31;
        for (char c : keyStr) {
            hash = (hash * p + (c - 'a' + 1));
            hash %= N; 
        }
        return hash;
    }

    unsigned long long calculateHash2(string keyStr) {
        unsigned long long hash = 5381;
        for (char c : keyStr) {
            hash = ((hash << 5) + hash) + c; 
        }
        return hash % N;
    }

    unsigned long long calculateAuxHash(string keyStr) {
        unsigned long long hash = 0;
        for (char c : keyStr) {
            hash = c + (hash << 6) + (hash << 16) - hash;
        }
        if (N <= 1) return 1;
        return 1 + (hash % (N - 1));
    }

    unsigned long long getHash(K key) {
        string keyStr = keyToString(key);
        if (hashChoice == 1) return calculateHash1(keyStr);
        return calculateHash2(keyStr);
    }
    
    unsigned long long getAuxHash(K key) {
        return calculateAuxHash(keyToString(key));
    }

    void insertInternal(K key, V value) {
        unsigned long long h = getHash(key);
        unsigned long long aux = getAuxHash(key);
        
        for (int i = 0; i < N; i++) {
            unsigned long long idx;
            if (method == DOUBLE_HASHING) idx = (h + i * aux) % N;
            else idx = (h + C1 * i * aux + C2 * i * i) % N;

            if (openTable[idx] && !openTable[idx]->isDeleted) {
                // Check if updating existing key (not strictly needed for this problem but good practice)
                if (openTable[idx]->key == key) {
                    openTable[idx]->value = value;
                    return;
                }
            } else {
                if (openTable[idx]) delete openTable[idx];
                openTable[idx] = new Node(key, value);
                n++;
                return;
            }
        }
    }

    void performResize(int newSize) {
        vector<Node> oldElements;
        for (int i = 0; i < N; i++) {
            if (openTable[i] && !openTable[i]->isDeleted) {
                oldElements.push_back(*openTable[i]);
            }
        }

        int oldN = N;
        N = newSize;
        n = 0; 

        for (int i = 0; i < oldN; i++) {
            if (openTable[i]) delete openTable[i];
        }
        openTable.clear();
        openTable.assign(N, nullptr);

        for (int i = 0; i < oldElements.size(); i++) {
            insertInternal(oldElements[i].key, oldElements[i].value);
        }
        lastResizeN = oldElements.size();
        opCountSinceResize = 0;
    }

public:
    HashTable(Method m, int hc) {
        method = m;
        hashChoice = hc;
        N = INITIAL_SIZE;
        n = 0;
        lastResizeN = 0;
        opCountSinceResize = 0;
        openTable.assign(N, nullptr);
    }

    ~HashTable() {
        for (int i = 0; i < openTable.size(); i++) {
            if (openTable[i]) delete openTable[i];
        }
    }

    void insert(K key, V value) {
        double loadFactor = (double)n / N;
        int minOps = (lastResizeN > 0) ? lastResizeN / 2 : 1;

        if (loadFactor > MAX_LOAD && opCountSinceResize >= minOps) {
            performResize(nextPrime(N * 2));
        }
        insertInternal(key, value);
        opCountSinceResize++;
    }

    // Returns -1 if not found, otherwise returns the Value (Index)
    int search(K key) {
        unsigned long long h = getHash(key);
        unsigned long long aux = getAuxHash(key);
        
        for (int i = 0; i < N; i++) {
            unsigned long long idx;
            if (method == DOUBLE_HASHING) idx = (h + i * aux) % N;
            else idx = (h + C1 * i * aux + C2 * i * i) % N;

            if (!openTable[idx]) break; 
            
            if (!openTable[idx]->isDeleted && openTable[idx]->key == key) {
                return openTable[idx]->value;
            }
        }
        return -1;
    }
};

// --- 3. Main Application Logic ---

int main() {
    // Database to store the actual records
    vector<Citizen> database;
    
    // Index to map Strings -> Database Index
    // Key: Name/NID/Phone, Value: Index in 'database' vector
    HashTable<string, int> index(DOUBLE_HASHING, 1);

    int choice;
    // Interactive Menu Loop
    while (true) {
        cout << "Enter choice: 1. Enter Data, 2. Search Index" << endl;
        if (!(cin >> choice)) break;
        
        // Clear input buffer after reading integer
        string dummy; 
        getline(cin, dummy); 

        if (choice == 1) {
            string name, nid, phone;
            
            cout << "Enter name: ";
            getline(cin, name);
            
            cout << "Enter NID no: ";
            getline(cin, nid);
            
            cout << "Enter phone no: ";
            getline(cin, phone);

            // 1. Store in Database
            Citizen c = {name, nid, phone};
            database.push_back(c);
            int id = database.size() - 1; // The index of the new record

            // 2. Update Index for ALL 3 fields
            // This ensures searching by any of them points to this record
            index.insert(name, id);
            index.insert(nid, id);
            index.insert(phone, id);
        }
        else if (choice == 2) {
            string searchStr;
            cout << "Enter search string: ";
            getline(cin, searchStr);

            // 1. Search the index
            int id = index.search(searchStr);

            // 2. Display result
            if (id != -1) {
                cout << "Match Found:" << endl;
                cout << "Name: " << database[id].name << endl;
                cout << "NID no: " << database[id].nid << endl;
                cout << "Phone no: " << database[id].phone << endl;
            } else {
                cout << "No match found" << endl;
            }
        }
    }

    return 0;
}