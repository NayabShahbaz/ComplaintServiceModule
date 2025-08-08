# Complaint Service Module

This project provides implementations of several fundamental data structures in C++ using templates. It is designed as a foundational module for managing complaints or similar records, and can be extended for broader use.

## Files
- `source.cpp`: Main source file containing implementations and usage examples for all data structures.
- `sll.h`: Header file defining the singly linked list and its iterator.

## Data Structures & Features

## Data Structures & Features

### 🔗 Singly Linked List (SLL)

- Generic (template-based) implementation
- Operations: insert at start/end, delete, search, print, copy constructor
- Custom iterator class for traversal (supports standard iterator operations)

### 🗃️ Universal Hash Table

- Generic (template-based) implementation
- Supports insertion, deletion, search
- Dynamic resizing (doubles capacity when load factor ≥ 0.75)
- Universal hashing and double hashing for collision resolution
- Hash functions for integers and customizable universal hash

### 🌳 AVL Tree

- Self-balancing binary search tree
- Supports insert, delete, search, rotations for balancing
- Template-based for flexible key/value types

### 🥇 Priority Queue

- Generic (template-based) implementation
- Supports enqueue, dequeue, heap-based operations
- Used for managing complaints or tasks by priority

### 🧩 Templates

- All major data structures are implemented as C++ templates for type flexibility

### 🧭 Iterators

- Custom iterator for singly linked list, supporting traversal and comparison

## Requirements
- C++ compiler (e.g., g++, MSVC)
``

## Customization
You can extend the singly linked list and hash table to include more fields relevant to complaints, such as complaint ID, description, status, etc. The template-based design allows easy adaptation to other data types.

## License
This project is provided for educational purposes. You may modify and use it as needed.
This project is provided for educational purposes. You may modify and use it as needed.
