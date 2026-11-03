#pragma once
#include <vector>
#include <unordered_map>
#include "Cache.h"
using namespace std;

// LRU cache using a hash table + a min-heap.

class LRUHeap : public Cache {
    struct Node {
        int key;
        int value;
        long long time;   // when it was last used
    };

    int capacity;
    long long timer;                // goes up by 1 on every use
    vector<Node> heap;              
    unordered_map<int, int> pos;    

    
    void swapNodes(int i, int j) {
        swap(heap[i], heap[j]);
        pos[heap[i].key] = i;
        pos[heap[j].key] = j;
    }

    // Move the node at index i up until its parent is not larger.
    void siftUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (heap[parent].time <= heap[i].time) break;
            swapNodes(i, parent);
            i = parent;
        }
    }

    // Move the node at index i down until both children are not smaller.
    void siftDown(int i) {
        int n = heap.size();
        while (true) {
            int smallest = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;

            if (left < n && heap[left].time < heap[smallest].time) smallest = left;
            if (right < n && heap[right].time < heap[smallest].time) smallest = right;

            if (smallest == i) break;
            swapNodes(i, smallest);
            i = smallest;
        }
    }

    // Remove the top of the heap (LRU).
    void removeMin() {
        pos.erase(heap[0].key);

        if (heap.size() == 1) {
            heap.pop_back();
            return;
        }

        heap[0] = heap.back();       // move the last node to the top
        pos[heap[0].key] = 0;
        heap.pop_back();
        siftDown(0);
    }

    
    void updateTime(int i) {
        timer++;
        heap[i].time = timer;
        siftDown(i);
    }

public:
    LRUHeap(int cap) {
        capacity = cap;
        timer = 0;
    }

    int get(int key) {
        if (pos.find(key) == pos.end()) return -1;  
        int i = pos[key];
        int value = heap[i].value;
        updateTime(i);
        return value;
    }

    void put(int key, int value) {
        if (capacity <= 0) return;

        if (pos.find(key) != pos.end()) {   // key already exists: update it
            int i = pos[key];
            heap[i].value = value;
            updateTime(i);
            return;
        }

        if ((int)heap.size() == capacity) removeMin();   

        timer++;
        Node n = {key, value, timer};
        heap.push_back(n);
        int i = heap.size() - 1;
        pos[key] = i;
        siftUp(i);
    }

    void show() {
        cout << "key:value(time): ";
        for (int i = 0; i < heap.size(); i++) {
            cout << "[" << heap[i].key << ":" << heap[i].value << "(" << heap[i].time << ")] ";
        }
        cout << "\n";
    }
};


