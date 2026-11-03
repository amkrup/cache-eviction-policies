#pragma once
#include <queue>
#include <unordered_map>
#include "Cache.h"
using namespace std;

// LFU cache implemented using a hash table + a min-heap.
// The hash table (key -> value, use count, time of last use) gives fast lookup.
// The heap keeps the least used key on top, so it is the one removed when full.
// If two keys have the same use count, the one used longest ago is removed.

// Note: when a key is used again we push a NEW heap entry and leave the old one behind. 
//       Old entries are skipped when removing (see removeOne), so the heap can grow larger than the cache.
class LFUHeap : public Cache {
    struct Entry {            // what the hash table stores for each key
        int value;
        int count;            // how many times it was used
        int time;             // when it was last used
    };

    struct HeapItem {         // what the heap stores
        int count;
        int time;
        int key;
        // Reversed because a priority_queue normally gives the BIGGEST item, so we flip the comparison to get the SMALLEST count/time instead.
        bool operator<(const HeapItem& other) const {
            if (count != other.count) return count > other.count;
            return time > other.time;
        }
    };

    int capacity;
    int clock;                          // goes up by 1 on every use
    unordered_map<int, Entry> table;    // the hash table
    priority_queue<HeapItem> heap;      // the min-heap

    // Record one more use of a key
    void use(int key) {
        table[key].count++;
        clock++;
        table[key].time = clock;
        HeapItem item = {table[key].count, clock, key};
        heap.push(item);
    }

    // Remove the least frequently used key
    void removeOne() {
        while(!heap.empty()) {
            HeapItem top = heap.top();
            heap.pop();
            // Only trust this heap entry if it matches the table
            if(table.count(top.key) == 1 && table[top.key].count == top.count && table[top.key].time == top.time) {
                table.erase(top.key);
                return;
            }
        }
    }

public:
    LFUHeap(int cap) {
        capacity = cap;
        clock = 0;
    }

    int get(int key) {
        if(table.count(key) == 0) return -1;
        use(key);
        return table[key].value;
    }

    void put(int key, int value) {
        if(table.count(key) == 1) {   // key already exists: update it
            table[key].value = value;
            use(key);
            return;
        }
        if(table.size() == capacity) removeOne();
        Entry e = {value, 0, 0};
        table[key] = e;
        use(key);
    }

    void show() {
        cout << "key:value(count): ";
        for(auto kv : table) {
            cout << "[" << kv.first << ":" << kv.second.value << "(" << kv.second.count << ")] ";
        }
        cout << "\n";
    }
};
