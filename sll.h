
#pragma once
#include<iostream>
using namespace std;

#pragma once
template <class T>
class List {

    template<class T>
    class Node {
    public:
        T data;
        Node<T>* next;

        Node()
        {
            data = 0;
            next = 0;
        }
        Node(T val=0, Node<T>* nextPtr = 0)
        {
            data = val;
            next = nextPtr;
        }
        ~Node()
        {
            next = NULL;
        }
    };
    Node<T>* head;
    Node<T>* tail;
    int size;

    template <class T>
    class SLLIterator {
    private:


        typename List::Node<T>* current;  // Current node the iterator is pointing to
    public:
        friend class List;
        // Default constructor (points to null initially)
        SLLIterator(Node<T>* start = nullptr) : current(start) {}

        // Dereference operator
        T& operator*() const
        {
            if (current)
                return current->data;
        }

        // Pre-increment operator to move the iterator to the next node
        SLLIterator& operator++()
        {
            if (current) {
                current = current->next;
            }
            return *this;
        }

        // Post-increment operator (returns the iterator before moving)
        SLLIterator operator++(int) {
            SLLIterator temp = *this;
            if (current) {
                current = current->next;
            }
            return temp;
        }

        // Comparison operators for equality and inequality
        bool operator==(const SLLIterator& other) const {
            return current == other.current;
        }

        bool operator!=(const SLLIterator& other) const {
            return current != other.current;
        }
    };

public:
    typedef SLLIterator<T> IteratorS;

    IteratorS begin()
    {
        IteratorS I(head);
        return I;
    }
    IteratorS end()
    {
        IteratorS I(tail);
        return I;
    }
    List()
    {
        head = 0;
        tail = 0;
        size = 0;
    }
    void InsertAtstart(T const val)
    {
        Node<T>* temp = new Node<T>(val, head);
        head = temp;

        if (tail == 0) // empty list before adding the node, then move tail to the same node as head(only one node )
        {
            tail = head;
        }
        size++;
    }
    void insertAtEnd(T const val)
    {

        Node<T>* temp = new Node<T>(val);
        if (tail != 0)
        {
            tail->next = temp;
        }
        else
        {
            head = temp;  // empty list, introduced one node and head is pointing to it
        }

        tail = temp; //this will happen in any case, empty or non empty list
        size++;

    }

    void Print()
    {
        Node<T>* temp = head;
        for (; temp != 0; temp = temp->next)
        {
            (*temp->data)->Print();
            cout << endl << endl;
        }


    }

    bool Search(T& val)
    {
        bool found = false;
        Node<T>* temp = head;

        for (; temp != 0 && found == false; temp = temp->next) {
            if (temp->data == val)
            {
                found = true;
            }
        }

        return found;
    }

    int GetSize()
    {
        return size;
    }
    bool isEmpty()
    {
        if (tail == 0 && head == 0)
            return true;
        return false;
    }

    void InsertBefore(T v1, T v2)
    {
        Node<T>* temp1 = head;

        bool found = false;
        bool inserted = false;//for extreme case of insering before first node
        for (; temp1 != NULL && found == false; temp1 = temp1->next)
        {
            if (head->data == v2)// found at first node
            {
                InsertAtstart(v1);
                found = true;
                inserted = true;
              
            }

            else if (temp1->next->data == v2)
            {
                found = true;
            }



        }
        if (found && !inserted)
        {
            Node<T>* newNode = new Node<T>(v1, temp1->next);
            temp1->next = newNode;
            size++;
        }


        return found;
    }

    void Delete(T val)
    {
        // Handle empty list case
        if (head == nullptr) {
            return; // List is empty, nothing to delete
        }

        Node<T>* temp = head;

        // Check if the head node contains the value to be deleted
        if (head->data == val) {
            Node<T>* toDel = head;
            head = head->next; // Move the head to the next node
            if (head == nullptr) { // If the list becomes empty, update tail
                tail = nullptr;
            }
            delete toDel; // Delete the original head
            size--;
            return;
        }

        // Traverse the list to find the node with the value to be deleted
        while (temp->next != nullptr && temp->next->data != val)
        {
            temp = temp->next;
        }

        // If we found the node to delete (temp->next is the node to be deleted)
        if (temp->next != nullptr) 
        {
            Node<T>* toDel = temp->next; // Node to delete
            temp->next = toDel->next; // Bypass the node to delete
            if (toDel == tail) { // If we're deleting the tail node
                tail = temp; // Update the tail pointer
            }
            delete toDel; // Delete the node
            size--;
        }
    }

    ~List()
    {
        Node<T>* tmp = head;
        while (!isEmpty())
        {
            if (head == tail)
            {
                head = tail = NULL;
            }
            if (tmp)
            {
                head = tmp->next;
                delete tmp;
                tmp = head;
            }
        }
    }
    //Copy constructor to copy one list into another in case a node with twp kids is deleted
    List(const List& other)
    {
        // Initialize head and tail for the new list
        head = nullptr;
        tail = nullptr;

        // Temporary pointer to traverse the other list
        Node<T>* current = other.head;

        // Copy each node from the other list
        while (current != nullptr)
        {
            insertAtEnd(current->data);
            current = current->next;
        }
    }
    void DeleteAtstart()
    {
        if (head != 0) // non empty
        {
            Node<T>* temp = head;
            if (head == tail)// only one node
                head = tail = NULL;
            else  // more than one node
            {
                head = head->next;
            }

            delete temp;
        }
    }
};