#include <bits/stdc++.h>

using namespace std;

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
    
    vector<list<Node>> chainTable; 
    vector<Node*> openTable;       
    const int INITIAL_SIZE = 101; // Increased slightly for small inputs
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

        if (method == CHAINING) {
            // Modify: Check if exists and update value
            for (auto it = chainTable[h].begin(); it != chainTable[h].end(); ++it) {
                if (it->key == key) {
                    it->value = value;
                    return;
                }
            }
            chainTable[h].push_back(Node(key, value));
            n++;
        } 
        else {
            unsigned long long aux = getAuxHash(key);
            
            for (int i = 0; i < N; i++) {
                unsigned long long idx;
                
                if (method == DOUBLE_HASHING) {
                    idx = (h + i * aux) % N;
                } else { 
                    idx = (h + C1 * i * aux + C2 * i * i) % N;
                }

                if (openTable[idx] && !openTable[idx]->isDeleted) {
                     // Modify: Check for update
                     if (openTable[idx]->key == key) {
                         openTable[idx]->value = value;
                         return;
                     }
                    totalCollisions++; 
                } else {
                    if (openTable[idx]) delete openTable[idx];
                    openTable[idx] = new Node(key, value);
                    n++;
                    return;
                }
            }
        }
    }

    void performResize(int newSize) {
        vector<Node> oldElements;
        if (method == CHAINING) {
            for (int i = 0; i < N; i++) {
                for (auto it = chainTable[i].begin(); it != chainTable[i].end(); ++it) {
                    oldElements.push_back(*it);
                }
            }
        } else {
            for (int i = 0; i < N; i++) {
                if (openTable[i] && !openTable[i]->isDeleted) {
                    oldElements.push_back(*openTable[i]);
                }
            }
        }

        int oldN = N;
        N = newSize;
        n = 0; 

        if (method == CHAINING) {
            chainTable.clear();
            chainTable.resize(N);
        } else {
            for (int i = 0; i < oldN; i++) {
                if (openTable[i]) delete openTable[i];
            }
            openTable.clear();
            openTable.assign(N, nullptr);
        }

        for (int i = 0; i < oldElements.size(); i++) {
            insertInternal(oldElements[i].key, oldElements[i].value);
        }
        lastResizeN = oldElements.size();
        opCountSinceResize = 0;
    }

public:
    long long totalCollisions; 

    HashTable(Method m, int hc) {
        method = m;
        hashChoice = hc;
        N = INITIAL_SIZE;
        n = 0;
        lastResizeN = 0;
        opCountSinceResize = 0;
        totalCollisions = 0;

        if (method == CHAINING) chainTable.resize(N);
        else openTable.assign(N, nullptr);
    }

    ~HashTable() {
        for (int i = 0; i < openTable.size(); i++) {
            if (openTable[i]) delete openTable[i];
        }
    }

    void insert(K key, V value) {
        // We modified insertInternal to handle updates, so we call it directly
        // But we still need resize logic
        
        // Check if key exists first to avoid double counting n
        // (Optimized insert logic handles this inside insertInternal now)

        double loadFactor = (double)n / N;
        int minOps = (lastResizeN > 0) ? lastResizeN / 2 : 1;

        if (loadFactor > MAX_LOAD && opCountSinceResize >= minOps) {
            performResize(nextPrime(N * 2));
        }
        else if (loadFactor < MIN_LOAD && N > INITIAL_SIZE && opCountSinceResize >= minOps) {
            int newSize = prevPrime(N / 2);
            if (newSize < INITIAL_SIZE) newSize = INITIAL_SIZE;
            performResize(newSize);
        }

        insertInternal(key, value);
        opCountSinceResize++;
    }

    int search(K key, int* hits = nullptr) {
        unsigned long long h = getHash(key);
        int localHits = 0;

        if (method == CHAINING) {
            for (auto it = chainTable[h].begin(); it != chainTable[h].end(); ++it) {
                localHits++;
                if (it->key == key) {
                    if (hits) *hits = localHits;
                    return it->value;
                }
            }
        } else {
            unsigned long long aux = getAuxHash(key);
            for (int i = 0; i < N; i++) {
                localHits++;
                unsigned long long idx;
                
                if (method == DOUBLE_HASHING) idx = (h + i * aux) % N;
                else idx = (h + C1 * i * aux + C2 * i * i) % N;

                if (!openTable[idx]) break; 
                
                if (!openTable[idx]->isDeleted && openTable[idx]->key == key) {
                    if (hits) *hits = localHits;
                    return openTable[idx]->value;
                }
            }
        }
        if (hits) *hits = localHits;
        return -1;
    }
};

// Helper function: Logic to get Reverse Complement
string getReverseComplement(string s) {
    string complement = "";
    for (int i = s.length() - 1; i >= 0; i--) {
        char base = s[i];
        if (base == 'A') complement += 'T';
        else if (base == 'T') complement += 'A';
        else if (base == 'C') complement += 'G';
        else if (base == 'G') complement += 'C';
    }
    return complement;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, l;
    if (cin >> n >> l) {
        // Use Double Hashing for O(ln) performance
        // Map stores: Sequence -> Count
        HashTable<string, int> ht(DOUBLE_HASHING, 1);

        for (int i = 0; i < n; i++) {
            string currentSeq;
            cin >> currentSeq;

            // 1. Calculate Target (Reverse Complement)
            string target = getReverseComplement(currentSeq);

            // 2. Check if target exists in map
            int count = ht.search(target);
            
            // search returns -1 if not found
            if (count != -1) {
                // If found k times, print the pair k times
                for (int k = 0; k < count; k++) {
                    cout << target << " " << currentSeq << "\n";
                }
            }

            // 3. Insert current sequence into map (Increment count)
            int currentCount = ht.search(currentSeq);
            if (currentCount == -1) currentCount = 0;
            ht.insert(currentSeq, currentCount + 1);
        }
    }

    return 0;
}