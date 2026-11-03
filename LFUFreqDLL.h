#pragma once
#include <list>
#include <unordered_map>
#include "Cache.h"
using namespace std;

// LFU cache using hash tables + one doubly linked list per use count
// freqMap: use count -> list of items with that count (front = most recently used)
// keyMap: key -> position of its item in one of those lists
// minFreq is the smallest use count in the cache, so the item to remove is at the
// back of freqMap[minFreq] (least recently used among the least frequently used)
class LFUFreqDLL : public Cache
{
    struct Node
    {
        int key;
        int value;
        int freq;

        Node(int k, int v)
        {
            key = k;
            value = v;
            freq = 1;
        }
    };

    int capacity;
    int minFreq;
    unordered_map<int, list<Node>::iterator> keyMap;   // key -> node
    unordered_map<int, list<Node> > freqMap;           // use count -> list of nodes

    // Record one more use of a node: move it to the list for the next use count.
    void updateFreq(list<Node>::iterator it)
    {
        int key = it->key;
        int value = it->value;
        int freq = it->freq;

        freqMap[freq].erase(it);   // remove from the old list

        if (freqMap[freq].empty())
        {
            freqMap.erase(freq);
            if (minFreq == freq) minFreq++;
        }

        freq++;
        Node n(key, value);
        n.freq = freq;
        freqMap[freq].push_front(n);   // add to the front of the new list
        keyMap[key] = freqMap[freq].begin();
    }

public:
    LFUFreqDLL(int cap)
    {
        capacity = cap;
        minFreq = 0;
    }

    int get(int key)
    {
        if (keyMap.find(key) == keyMap.end()) return -1;
        list<Node>::iterator it = keyMap[key];
        int value = it->value;
        updateFreq(it);
        return value;
    }

    void put(int key, int value)
    {
        if (capacity <= 0) return;

        if (keyMap.find(key) != keyMap.end())
        {
            list<Node>::iterator it = keyMap[key];
            it->value = value;
            updateFreq(it);
            return;
        }

        if ((int)keyMap.size() == capacity)
        {
            list<Node>& lst = freqMap[minFreq];
            int keyToRemove = lst.back().key;
            lst.pop_back();
            keyMap.erase(keyToRemove);
            if (lst.empty()) freqMap.erase(minFreq);
        }

        freqMap[1].push_front(Node(key, value));
        keyMap[key] = freqMap[1].begin();
        minFreq = 1;
    }

    void show()
    {
        cout << "key:value(count): ";
        for (unordered_map<int, list<Node> >::iterator f = freqMap.begin(); f != freqMap.end(); f++)
        {
            for (list<Node>::iterator it = f->second.begin(); it != f->second.end(); it++)
            {
                cout << "[" << it->key << ":" << it->value << "(" << it->freq << ")] ";
            }
        }
        cout << "\n";
    }
};