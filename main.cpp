#include <iostream>
#include <string>
#include "LFUFreqDLL.h"
#include "LFUHeap.h"
#include "LRUHashDLL.h"
#include "LRUHeap.h"
using namespace std;

string names[4] = {
    "LRU - Hash Table + Doubly Linked List",
    "LRU - Hash Table + Min-Heap",
    "LFU - Hash Table + Frequency Linked Lists",
    "LFU - Hash Table + Min-Heap"
};

Cache* makeCache(int choice, int capacity) {
    if(choice == 1) return new LRUHashDLL(capacity);
    if(choice == 2) return new LRUHeap(capacity);
    if(choice == 3) return new LFUFreqDLL(capacity);
    return new LFUHeap(capacity);
}

int readInt(string prompt) {
    int x;
    cout << prompt;
    while(!(cin >> x)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Enter a number: ";
    }
    return x;
}

int main() {
    string bar(44, '=');
    cout << bar << "\n   LRU / LFU CACHE SIMULATOR\n" << bar << "\n";
    for(int i = 0; i < 4; i++) cout << "  " << i + 1 << ". " << names[i] << "\n";

    int choice = readInt("Choose implementation: ");
    while(choice < 1 || choice > 4) choice = readInt("Invalid. Choose again: ");
    int capacity = readInt("Cache capacity: ");
    while(capacity < 1) capacity = readInt("Must be at least 1: ");

    Cache* cache = makeCache(choice, capacity);
    string msg = "Ready.";

    while(true) {
        cout << "\n" << bar << "\n " << names[choice - 1] << "  (capacity " << capacity << ")\n" << bar << "\n ";
        cache->show();
        cout << " > " << msg << "\n";
        cout << "----------------------------------------------\n";
        cout << "  1. Put   2. Get   3. Clear   0. Exit\n";

        int op = readInt("Select: ");
        if(op == 0) break;

        if(op == 1) {
            int key = readInt("  key: ");
            int value = readInt("  value: ");
            cache->put(key, value);
            msg = "PUT " + to_string(key) + " = " + to_string(value);
        } else if(op == 2) {
            int key = readInt("  key: ");
            int value = cache->get(key);
            if(value == -1) msg = "GET " + to_string(key) + " -> MISS";
            else msg = "GET " + to_string(key) + " -> HIT, value " + to_string(value);
        } else if(op == 3) {
            delete cache;
            cache = makeCache(choice, capacity);
            msg = "Cache cleared.";
        } else {
            msg = "Invalid option.";
        }
    }
    delete cache;
}
