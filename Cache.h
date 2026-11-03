#pragma once
#include <iostream> // not required in this file but is in other ones.
using namespace std;

// Since every LRU/LFU version has get, put and show methods, this is a universal blueprint that all of them follow.
// "virtual" lets main.cpp use any version through one Cache pointer.
class Cache {
public:
    // all are made = 0 to signify every cache must write its own functions
    virtual int get(int key) = 0;              // returns the value / -1 if not found
    virtual void put(int key, int value) = 0;  // adds/updates, removes an old item if full
    virtual void show() {}                     // prints the cache items
    virtual ~Cache() {}
};
