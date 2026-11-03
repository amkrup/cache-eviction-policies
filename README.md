# Modular Cache Eviction Simulator (LRU & LFU)

A modular, high-performance C++ implementation of **LRU (Least Recently Used)** and **LFU (Least Frequently Used)** caching policies, demonstrating different underlying data structure tradeoffs between $O(1)$ and $O(\log N)$ operations.

---

## Project Overview

This project provides an interactive Command-Line Interface (CLI) tool allowing users to test, compare, and visualize key caching mechanisms under custom capacities and interactive workloads. 

The architecture follows strict Object-Oriented Programming (OOP) principles using an abstract base class (`Cache.h`) to achieve modular polymorphism across all policy implementations.

---

## Repository Structure

```text
├── Cache.h        # Abstract base class defining common interface (get, put, show)
├── LRUHashDLL.h   # Optimal LRU using Hash Table + Doubly Linked List - O(1)
├── LRUHeap.h      # Alternative LRU using Hash Table + Min-Heap - O(log N)
├── LFUFreqDLL.h   # Optimal LFU using Hash Table + Frequency DLL Buckets - O(1)
├── LFUHeap.h      # Alternative LFU using Hash Table + Min-Heap / BST - O(log N)
├── main.cpp       # CLI application for policy selection, capacity set, and input testing
└── README.md      # Project documentation
