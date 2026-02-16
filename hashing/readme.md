This code implements a robust **Generic Hash Table** in C++ that supports multiple collision resolution strategies and dynamic resizing. Here is a breakdown of the strategies, how to manipulate the functions, and what everything produces.

---

## 1. The Collision Resolution Strategies

The code provides three primary "methods" to handle cases where two keys hash to the same index:

| Strategy | Logic | Pros/Cons |
| --- | --- | --- |
| **Chaining** | Each bucket is a `std::list`. Multiple items at one index. | **Pro:** Never gets "full." **Con:** Cache inefficient. |
| **Double Hashing** | Uses a second hash function to find the next skip interval. | **Pro:** Eliminates "clustering." **Con:** More computation. |
| **Custom Probing** | Uses a quadratic formula: . | **Pro:** Avoids primary/secondary clustering. |

---

## 2. Function Reference: Create, Call, and Output

To use this class, you first need to specify the types (Key and Value) and the strategy.

### **Initialization (The Constructor)**

```cpp
HashTable<string, int> ht(CHAINING, 1);

```

* **Inputs:** `Method` (CHAINING, DOUBLE_HASHING, or CUSTOM_PROBING) and `hashChoice` (1 for Polynomial Rolling, 2 for DJB2).
* **Action:** Sets the initial table size to 13 (a prime number).

### **Inserting Data**

```cpp
ht.insert("Apple", 50);

```

* **Logic:** 1. Checks if the key exists (avoids duplicates).
2. Checks the **Load Factor** ().
3. If Load > 0.5, it triggers `performResize` to the next prime of .
4. If Load < 0.25 (and it has resized before), it shrinks.
* **Output:** None (void), but internal state updates.

### **Searching for Data**

```cpp
int hits = 0;
int val = ht.search("Apple", &hits);

```

* **Input:** The key to find and an optional pointer to an integer to track "probes" (hits).
* **Output:** Returns the value associated with the key. If not found, returns `-1`.
* **Manipulation:** The `hits` pointer tells you exactly how many steps the algorithm took to find the item (useful for performance analysis).

---

## 3. Key Manipulation & Logic Flow

The code uses several helper functions to ensure the math behind the hash table remains efficient:

* **`nextPrime` / `prevPrime`:** Used during resizing. Keeping the table size () as a prime number is critical to reducing collisions, especially in Double Hashing.
* **`keyToString`:** Uses a `stringstream` to convert any type `K` into a string so it can be hashed.
* **Internal Hashing:** * **Hash 1:** Polynomial rolling (standard for strings).
* **Hash 2:** DJB2 (very fast, low collision rate).
* **AuxHash:** Used only for Open Addressing (Double/Custom Probing) to determine the "step size."



---

## 4. Understanding the `main()` Output

When you run this code, it simulates a real-world scenario with 10,000 random words. It outputs a comparison table:

1. **Method:** Which strategy was used.
2. **Hash1/Hash2 Colls:** Total number of times an insertion hit an occupied slot.
3. **Hash1/Hash2 Hits:** The average number of steps (probes) required to find a key during a search.

> **Insight:** You will notice that **Chaining** usually has lower "hits" (closer to 1.0) because it doesn't have to probe other slots, but **Open Addressing** (Double Hashing) is often more memory-compact.

---

## 5. Summary of Manipulations

If you want to change the behavior of the table, modify these constants in the `private` section:

* **`MAX_LOAD`**: Change from `0.5` to `0.8` to make the table more memory efficient but slower.
* **`C1` and `C2**`: Change these in Custom Probing to see how it affects the "clustering" of data.
* **`INITIAL_SIZE`**: Set this higher if you know you have a massive dataset to avoid early resizes.

**Would you like me to help you modify the `search` function to return a boolean instead, or perhaps implement a `remove` function for this table?**