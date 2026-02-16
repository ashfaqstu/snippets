#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

enum State { EMPTY, OCCUPIED, DELETED };

struct Slot {
    int process_id;
    State state;
    
    Slot() : process_id(-1), state(EMPTY) {}
};

class MemoryManager {
private:
    vector<Slot> table;
    int N;              // Current table size
    int initial_N;      // Initial table size (limit for shrinking)
    int P_MAX;          // Max probe limit
    int occupied_count; // Number of OCCUPIED slots
    int deletion_count; // Tracks total deletions to trigger checks
    int max_probe_count;// Tracks the maximum probe sequence length in current table

    // --- Helper: Prime Number Logic ---
    bool isPrime(int n) {
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        for (int i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }
        return true;
    }

    int nextPrime(int n) {
        while (!isPrime(n)) n++;
        return n;
    }

    int prevPrime(int n) {
        while (n >= 2 && !isPrime(n)) n--;
        return (n < 2) ? 2 : n; // Safety fallback
    }

    // --- Helper: Stats Reporting ---
    void printRehashReport(bool before, int size, int occupied, int max_probe) {
        double lf = (double)occupied / size;
        if (before) {
            cout << "Rehash Triggered: BEFORE ---" << endl;
        } else {
            cout << "--- Rehash Triggered: AFTER ---" << endl;
        }
        cout << "Table Size: " << size << endl;
        cout << "Active Processes: " << occupied << endl;
        cout << "Load Factor: " << fixed << setprecision(6) << lf << endl;
        cout << "Max Probe Sequence: " << max_probe << endl;
    }

    // --- Core: Rehash Function ---
    void performRehash(int new_size) {
        // 1. Print BEFORE stats
        printRehashReport(true, N, occupied_count, max_probe_count);

        vector<Slot> old_table = table;
        int old_occupied = occupied_count;
        
        // 2. Reset Table
        table.clear();
        table.resize(new_size);
        N = new_size;
        occupied_count = 0;
        max_probe_count = 0; // Reset for the new layout
        deletion_count = 0;  // Reset deletion counter as per hint

        // 3. Re-insert only OCCUPIED slots
        for (const auto& slot : old_table) {
            if (slot.state == OCCUPIED) {
                insertInternal(slot.process_id, false); // false = don't print insert msgs
            }
        }

        // 4. Print AFTER stats
        printRehashReport(false, N, occupied_count, max_probe_count);
    }

    // --- Internal Insert (Shared by Allocate and Rehash) ---
    // Returns number of probes used
    int insertInternal(int pid, bool printOutput) {
        int probes = 0;
        int idx = -1;
        
        // Quadratic Probing: (hash(k) + j^2) % N
        // j starts at 0
        for (int j = 0; ; j++) {
            probes = j + 1;
            int hash_val = pid % N; // hash(k) = k
            int curr_idx = (hash_val + (long long)j * j) % N;

            // Stop at first EMPTY or DELETED
            if (table[curr_idx].state != OCCUPIED) {
                idx = curr_idx;
                break;
            }
            
            // Safety break to prevent infinite loops in full tables (though logic shouldn't allow full)
            if (j > N + 100 && probes > P_MAX) break; 
        }

        // Perform Insertion
        table[idx].process_id = pid;
        table[idx].state = OCCUPIED;
        occupied_count++;

        // Update Max Probe Count
        if (probes > max_probe_count) {
            max_probe_count = probes;
        }

        if (printOutput) {
            cout << "ALLOCATE " << pid << ": Inserted at index " << idx 
                 << " (probes: " << probes << ")" << endl;
        }
        
        return probes;
    }

public:
    MemoryManager(int n, int p_max) : N(n), initial_N(n), P_MAX(p_max) {
        table.resize(N);
        occupied_count = 0;
        deletion_count = 0;
        max_probe_count = 0;
    }

    void allocate(int pid) {
        // 1. Insert
        int probes = insertInternal(pid, true);

        [cite_start]// 2. Check Scale-Up Trigger [cite: 150, 158]
        if (probes > P_MAX) {
            [cite_start]// Resize to next prime >= 2 * current size [cite: 159]
            int target_size = nextPrime(2 * N);
            performRehash(target_size);
        }
    }

    void free(int pid) {
        int probes = 0;
        int idx = -1;
        bool found = false;

        [cite_start]// 1. Search using Quadratic Probing [cite: 197]
        for (int j = 0; j < N + 100; j++) { // Limit search reasonable bounds
            probes = j + 1;
            int hash_val = pid % N;
            int curr_idx = (hash_val + (long long)j * j) % N;

            if (table[curr_idx].state == EMPTY) {
                // Not found if we hit EMPTY
                break;
            }

            if (table[curr_idx].state == OCCUPIED && table[curr_idx].process_id == pid) {
                idx = curr_idx;
                found = true;
                break;
            }
        }

        if (found) {
            table[idx].state = DELETED; [cite_start]// [cite: 153]
            occupied_count--;
            deletion_count++; [cite_start]// [cite: 154]
            cout << "FREE " << pid << ": Freed from index " << idx << endl;

            [cite_start]// 2. Check Scale-Down Trigger (Every 5 deletions) [cite: 154, 162]
            if (deletion_count % 5 == 0) {
                double load_factor = (double)occupied_count / N;
                if (load_factor < 0.2) {
                    [cite_start]// Resize to closest smaller prime <= current size / 2 [cite: 163]
                    int target_size = prevPrime(N / 2);
                    
                    [cite_start]// Skip if new size < initial size [cite: 163]
                    if (target_size >= initial_N) {
                         performRehash(target_size);
                    }
                }
            }
        } else {
            // "If a group is not found, report it" - applied generally here for safety
            cout << "FREE " << pid << ": Not Found" << endl;
        }
    }
};

int main() {
    int N, P_MAX, Q;
    if (!(cin >> N >> P_MAX >> Q)) return 0;

    MemoryManager mm(N, P_MAX);

    string command;
    int pid;
    while (Q--) {
        cin >> command >> pid;
        if (command == "ALLOCATE") {
            mm.allocate(pid);
        } else if (command == "FREE") {
            mm.free(pid);
        }
    }

    return 0;
}
