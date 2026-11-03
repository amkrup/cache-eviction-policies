#pragma once
#include <unordered_map>
#include <list>
#include "Cache.h"
using namespace std;

// LRU cache using a hash table + a doubly linked list
// The list holds (key, value) pairs. front = most recently used, back = least recently used
// The hash table maps each key to its spot in the list, so get/put are fast (O(1))
class LRUHashDLL : public Cache
{
    int capacity;
    list <pair <int, int>> dll;
    unordered_map <int, list<pair<int, int>> :: iterator> cacheMap;

    // Move a key to the front of the list (most recently used) with the given value
    void touch(int key, int value)
    {
        dll.erase(cacheMap[key]);
        dll.push_front(make_pair(key, value));
        cacheMap[key] = dll.begin();
    }

public:
    LRUHashDLL(int cap)
    {
        capacity = cap;
    }

    int get(int key)
    {
        if (cacheMap.find(key) == cacheMap.end()) return -1;
        int value = cacheMap[key]->second;
        touch(key, value);
        return value;
    }

    void put(int key, int value)
    {
        if (capacity <= 0) return;

        if (cacheMap.find(key) != cacheMap.end())
        {
            touch(key, value);
            return;
        }

        if ((int)cacheMap.size() == capacity)
        {
            int lruKey = dll.back().first;
            cacheMap.erase(lruKey);
            dll.pop_back();
        }
        dll.push_front(make_pair(key, value));
        cacheMap[key] = dll.begin();
    }

    void show()
    {
        cout << "MRU -> LRU: ";
        for (list<pair<int, int> >::iterator it = dll.begin(); it != dll.end(); it++)
        {
            cout << "[" << it->first << ":" << it->second << "] ";
        }
        cout << "\n";
    }
};