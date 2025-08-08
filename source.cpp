#include<iostream>
#include"sll.h"
#include<vector>
#include <cstdlib> // For rand() and srand()
#include<ctime>
#include<random>
using namespace std;

template <class T>
class UniversalHashtable
{
    
private:
    template<class T>
    class HashItem {
    public:
        int key;
        T value;
        short status;

        HashItem(int k = 0, T val = 0, short stat = 0)
        {
            key = k;
            value = 0;
            status = stat;

        }

        ~HashItem()
        {

        }
    };

    HashItem<T>* hashTable;
    int capacity;
    int currentElements;
    int p;
    int a;
    int b;

    // to bouble the size, when load factor is>=0.75
    void doubleCapacity()
    {
        HashItem<T>* oldArray = hashTable;;
        int oldCapacity = capacity;

        // Double the capacity
        capacity *= 2;
        hashTable = new HashItem<T>[capacity];
        currentElements = 0;

        // Rehash all elements
        for (int i = 0; i < oldCapacity; i++)
        {
            if (oldArray[i].status == 2) // Only rehash non-empty items
            {
                insert(oldArray[i].key, oldArray[i].value);
            }
        }

        // Clean up old array
        delete[] oldArray;
    }
   
    // Hash function for integers
    int HashFunction(int k)
    {
        return k;
    }

    // Universal hash function
    int universalHash(int k)
    {
       
        return ((a * k + b) % p) % capacity;
    }

public:
    UniversalHashtable()
    {
        srand(time(0));
        hashTable = new HashItem<T>[11];
        capacity = 11;
        currentElements = 0;
        p = 103; ; //p>=no of keys hypothetically rn
        a = rand() % p;// SEE how to use this 
        b = rand() % p;
    }
    UniversalHashtable(int const cap)
    {
        srand(time(0));
        if (cap < 1)
        {
            cout << "Capacicyt<1 error" << endl;
            return;
        }

        capacity = cap;
        p = 103;
        a = rand() % p;
        b = rand() % p;
        hashTable = new HashItem<T>[capacity];
        currentElements = 0;
    }
    int primaryHash(int k)
    {
        return k % capacity;
    }

    int secondaryHash(int k)
    {
        
        return 2 + (k % (capacity - 2));
    }

    int doubleHash(int k, int i) //to resolve the collisiosn
    {
        return (primaryHash(k) + i * secondaryHash(k)) % capacity;
    }

    void insert(int const k, T const val)
    {
        double loadFactor = (double)currentElements / capacity;

        // Check if resizing (load factor >= 0.75)
        if (loadFactor >= 0.75)
        {
            doubleCapacity();  // Double the capacity and rehash
        }

        int i = 0;
        int index = universalHash(k); // Find the initial candidate index


        while (hashTable[index].status == 2) {
            i++;
            index = doubleHash(k, i); //Resolve the collsion using double hash
            cout << "Probing index for key ,"<<k<<" iteration " << i << ": " << index << endl;
        }

        hashTable[index].status = 2; //occupied
        hashTable[index].value = val;
        hashTable[index].key = k;
        currentElements++;


    }

    bool deleteKey(int k)
    {
        int i = 0;
       
        int index = universalHash(k);

        while (hashTable[index].status != 0)
        {
            if (hashTable[index].status == 2 && hashTable[index].key == k)
            {
                hashTable[index].status = 1;//deleted
                currentElements--;
                return true;
            }

            i++;
            index = doubleHash(k, i); //find based on collision occureance
        }

        return false;
    }

    // Search for an element by key
    T search(int k)
    {
        int index = universalHash(k);
        for (int i = 0; i < capacity; ++i)
        {
            
            if (hashTable[index].status==2 && hashTable[index].key== k)
            {
                return hashTable[index].value; // Return iterator saved to the element in DLL
            }
            
             index = doubleHash(k, i); 
         
        }

        return T(NULL); // Not found so return an It = 0
    }

    void display()
    {
        for (int i = 0; i < capacity; i++)
        {
            if (hashTable[i].status == 2) //if occupied, then Display
            {
                cout << "Index " << i << ":" << hashTable[i].key << endl;
                (*hashTable[i].value)->Print();
            }
        }

    }

    ~UniversalHashtable ()
    {
        delete[] hashTable;
    }
};

template<class T>
class StrHashTable {
private:

    // A HashItem structure without status as not needed in this 
    template<class T>
    class HashItem {
    public:
        int key;
        T value;
        string data;


        // Default constructor
        HashItem() : key(0), value(T()), data("") {}

        HashItem(int k, T val, string str="") : key(k), value(val)
        {
            data = str;
        }
//Used by SLL in comparisons
        bool operator==(HashItem& other) const
        {
            return data==other.data;
        }

        bool operator!=(HashItem& other) const
        {
            return !(*this == other);
        }
    };

    //The hash table as an array of lists
    List<HashItem<T>>* hashTable;
    int currentElements;
    int capacity;

    // Double the size of the hash table and rehash all elements when lof>=0.5
    void doubleCapacity() 
    {
        int oldCapacity = capacity;
       List<HashItem<T>>* oldTable = hashTable;

        // Double the capacity
        capacity *= 2;
        hashTable = new List<HashItem<T>>[capacity];

        // Rehash all elements
        for (int i = 0; i < oldCapacity; ++i) 
        {
            for (typename List<HashItem<T>>::IteratorS it = oldTable[i].begin(); it != oldTable[i].end(); ++it)
            {
                insert((*it).data, (*it).value);
            }
        }

        delete[] oldTable;
    }

    // Convert a string key to an integer index
   unsigned int hash(const string& k)
    {
        unsigned int hashVal = 0;
        for (int i = 0; i < k.length(); ++i) {
            hashVal = 37 * hashVal + k[i];
        }
        return hashVal % capacity;
    }

public:
    // Constructor
    StrHashTable(int initialCapacity = 11)   : capacity(initialCapacity), currentElements(0) 
    {
        hashTable = new List<HashItem<T>>[capacity];
    }

    //Destructor
    ~StrHashTable() 
    {
        delete[] hashTable;
    }

    //Insert a key-value pair into the hash table
    void insert(string str, T val) 
    {
        double loadFactor = currentElements / capacity;
        // Check if resizing is needed
        if (loadFactor >= 0.75)
        {
            doubleCapacity();
        }

        int index = hash(str);
        // insert the new key-value pair
        HashItem<T>* newNode = new HashItem<T>(index, val, str);
        hashTable[index].insertAtEnd(*newNode);
        currentElements++;
    }

    // Delete a key from the hash table
    bool deleteKey(string k) 
    {
        int index = hash(k);
        typename List<HashItem<T>>::IteratorS it = hashTable[index].begin();
        // Iterate through the list at the index

        for (;it != hashTable[index].end(); ++it) 
        {
            if ((*it).data == k)
            {
                hashTable[index].Delete(*it);
                currentElements--;
                return true;
            }
        }

        if (it == hashTable[index].end()) //only one node in the chain
        {
            if ((*it).data == k)
            {
                hashTable[index].Delete(*it);
                currentElements--;
                return true;
            }

        }

        return false; // Key not found
    }

    // searching for a value by key
    T search(string k) 
    {
        int index = hash(k);


        typename List<HashItem<T>>::IteratorS it = hashTable[index].begin();
        // Iterate through the list at the index
        for (; it != hashTable[index].end(); ++it) 
        {
            if ((*it).data == k) {
                return (*it).value; //Return iterator saved to the element in DLL
            }
        }
        //if head== tail
        if ((*it).data == k) {
            return (*it).value; //Return iterator saved to the element in DLL
        }

        return T(NULL); // Key not found, return default value
    }

    // Display the contents of the hash table
    void display() {
        for (int i = 0; i < capacity; ++i) {
            cout << "Index " << i << ": " << endl;
            if (hashTable[i].isEmpty()) {
                cout << "Empty";
            }
            else
            {
                typename List <HashItem<T>>::IteratorS it = hashTable[i].begin();
                for (; it != hashTable[i].end(); ++it)
                {
                    cout << "(Key: " << (*it).key <<")"<< endl;
                    (*(*it).value)->Print();
                }
                cout << "Key: " << (*it).key << endl; " Value: "; (*(*it).value)->Print(); //the tail itself to be printed
            }
            cout << endl;
        }
    }
};

template<class U, class T>
class GroupHashTable
{
    template< class U, class T>
    class HashItem {
    public:
        U key;
        List<T>* userPtr;// iterator based sll to all the users belonging to the same group
        short status;

        HashItem(U k= U(), short stat = 0)
        {
            key = k;
            userPtr = NULL;
            status = stat;

        }

        ~HashItem()
        {
            delete userPtr;
        }
    };


protected:

    HashItem<U, T>* Hashtable;
    int currentElements;
    int capacity;

private:
    // to double the size, when load factor is>=0.5
    void doubleCapacity()
    {
        HashItem<U, T>* oldArray = Hashtable;
        int oldCapacity = capacity;

        // Double the capacity
        capacity *= 2;
        Hashtable = new HashItem<U, T>[capacity];

        // Rehash all elements
        for (int i = 0; i < oldCapacity; i++)
        {
            if (oldArray[i].status == 2) // Only rehash non-empty items
            {
                List<T>* users = oldArray[i].userPtr;
                if (users)//if list exists, copy and paste it
                {
                    typename List<T>::IteratorS it = users->begin();

                    while (it != users->end())
                    {
                        insert(oldArray[i].key, *it); //Rehash 
                        ++it;
                    }
                }
            }
        }
        // Clean up old array
        delete[] oldArray;
    }


public:

    GroupHashTable()
    {
        Hashtable = new HashItem<U, T>[11];
        capacity = 11;
        currentElements = 0;
    }

    GroupHashTable(int cap)
    {
        capacity = cap;
        currentElements = 0;
        Hashtable = new HashItem<U, T>[capacity];

    }

    ~GroupHashTable()
    {
        for (int i = 0; i < capacity; ++i)
        {
            if (Hashtable[i].userPtr) {
                delete Hashtable[i].userPtr;
                Hashtable[i].userPtr = nullptr;
            }
        }

        delete[] Hashtable;

    }

    virtual int getNextCandidate(U k, int i)
    {
        int ind = hash(k);
        return ((ind % capacity) + i) % capacity; //linear probing to find vacant cell
    }

    int hash(int k)
    {
        return k % capacity;
    }

    unsigned int hash(const string& k)
    {
        unsigned int hashVal = 0;
        for (int i = 0; i < k.length(); ++i) {
            hashVal = 37 * hashVal + k[i];
        }
        return hashVal % capacity;
    }

    void insert(U k, T const val)
    {
        double loadFactor = (double)currentElements / capacity;

        // Check if resizing (load factor > 0.75)
        if (loadFactor >= 0.75)
        {
            doubleCapacity();  // Double the capacity and rehash
        }

        int i = 0;
        int index = hash(k); //string to int

        // Resolve collisions using linear probing or quadratic depnding on data type
        while (Hashtable[index].status == 2 && Hashtable[index].key != k) //some other group
        {
            i++;
            index = getNextCandidate(k, i);
        }

        if (Hashtable[index].status != 2)
        {
            // Initialize a new list if the group(cell is initially) is empty
            Hashtable[index].userPtr = new List<T>();
            Hashtable[index].userPtr->insertAtEnd(val);
            Hashtable[index].key = k;
            Hashtable[index].status = 2;
            currentElements++;
        }
        else
            //Vacant cell found based on linear probling so fill it
        {
            Hashtable[index].status = 2; //occupied
            Hashtable[index].userPtr->insertAtEnd(val);
            Hashtable[index].key = k;
            currentElements++;
        }

    }


    //Deletes the Whole group
    bool deleteWholeKey(U k)
    {
        int i = 0;
        int index = hash(k); //string to int 

        while (Hashtable[index].status != 0)
        {
            if (Hashtable[index].status == 2 && Hashtable[index].key == k)
            {
                Hashtable[index].status = 1;//deleted
                int size = Hashtable[index].userPtr->GetSize();
                delete Hashtable[index].userPtr;
                Hashtable[index].userPtr = nullptr; // Preventing double deletion
                Hashtable[index].key = string();
                currentElements = currentElements - size;
                return true;
            }

            i++;
            index = getNextCandidate(k, i);
        }

        return false;
    }

    //Deletes part of sll(for string parameters)
    bool deletePartKey(string k, string str1, string str2) //str1 is usname and str2 is email
    {
        int i = 0;
        int index = hash(k); //string to int 

        while (Hashtable[index].status != 0)
        {
            if (Hashtable[index].status == 2 && Hashtable[index].key == k)
            {
                typename List<T>::IteratorS it = Hashtable[index].userPtr->begin();
                typename List<T>::IteratorS it2(0);
                while (it != it2)
                {
                    if ((*(*it))->GetUsername() == str1 || ((*(*(it)))->GetEmail() == str2))
                    {
                        Hashtable[index].userPtr->Delete(*it);
                        currentElements--;
                        return true;
                    }

                    it++;
                }
            }
            i++;
            index = getNextCandidate(k, i);
        }

        return false;
    }

    //Delets part of sll(for int parameter)
    bool deleteKey(int id)
    {
        int i = 0;

        int index = hash(id);

        while (Hashtable[index].status != 0)
        {
            if (Hashtable[index].status == 2 && Hashtable[index].key == id)
            {
                typename List<T>::IteratorS it = Hashtable[index].userPtr->begin();
                typename List<T>::IteratorS it2(0);
                while (it != it2)
                {
                    if ((*(*it))->Getid() == id)
                    {
                        Hashtable[index].userPtr->Delete(*it);
                        currentElements--;
                        return true;
                    }

                    it++;
                }
            }

            i++;
            index = getNextCandidate(id, i); //find based on collision occureance
        }

        return false;

    }

    //Returns ptr to list of the groupings made
    List<T>* Search(U k)
    {
        int i = 0;
        int index = hash(k);

        while (Hashtable[index].status != 0)
        {
            if (Hashtable[index].status == 2 && Hashtable[index].key == k)
                return Hashtable[index].userPtr; //pointer to the list with all the iterators 

            i++;
            index = getNextCandidate(k, i);
        }

        return nullptr;
    }

    //Printing 
    void display()
    {
        for (int i = 0; i < capacity; i++)
        {
            if (Hashtable[i].status == 2) //if occupied, then Display
            {
                cout << "Index " << i << ": (" << Hashtable[i].key << ")" << endl;
                Hashtable[i].userPtr->Print(); //Print the SLL of pointers
                
            }
        }

    }

};

template<class U,class T>
class GroupHashTableQ: public GroupHashTable<U,T>
{
private:
    //Overriding Function to Get next candidate based on Quadratic probing
    int getNextCandidate(U k, int i)//Quadratic probling for resolving the collision
    {
        int ind = (this)->hash(k);
        return (ind % this->capacity + i * i) % this->capacity;// Quadratic probing formula
    }
public:
    GroupHashTableQ() :GroupHashTable<U,T>(){}
    GroupHashTableQ(int cap): GroupHashTable<U,T>(cap){}
    ~GroupHashTableQ(){}//will use parent class destructor

};

class Complaint {
private:
    int complaintId;
    int userID;
    string text;
    int type;
    static int serialCounter;
    int SerialNum;// for fifo

public:
    class IteratorToObject {
    private:
        typename Complaint::Complaint* ptr;
    public:

        IteratorToObject()
        {

        }

        IteratorToObject(Complaint* iptr)
        {
            ptr = iptr;
        }

        Complaint*& operator* ()
        {
            if (ptr)
                return ptr;
        }

        bool operator== (const IteratorToObject& it)
        {
            return ptr == it.ptr;
        }

        bool operator!=(const IteratorToObject& it)
        {
            return !(ptr == it.ptr);
        }
    };

    typedef IteratorToObject IterO;

    Complaint(int _id1, int iduser, string _text, string _type)
    {
        complaintId = _id1;
        userID = iduser;
        text = _text;
        if (_type == "platinum")
            type = 1;
        else if (_type == "gold")
            type = 2;
        else if (_type == "silver")
            type = 3;
        else if (_type == "new")
            type = 4;
        else if (_type == "regular")
            type = 5;

        SerialNum = ++serialCounter;

    }
    void Settype(int ty)
    {
        type = ty;
    }

    int Getid()
    {
        return complaintId;
    }

    int GetUserId()
    {
        return userID;
    }
    ~Complaint()
    {

    }

    bool isComplaintID(int id)
    {
        if (complaintId == id)
            return true;
        return false;
    }
    void Print()
    {
        cout << "Complaint id:\t " << complaintId << endl;
        cout << "User ID:\t " << userID << endl;
        cout << "Complaint made:\t" << text << endl;

    }
    bool operator<(const Complaint& obj)
    {
        // Compare by type first
        if (type < obj.type)
            return true;

        // If types are equal, compare by SerialNum for FIFO order (smaller serial number comes first)
        if (type == obj.type)
            return SerialNum < obj.SerialNum;

        // Otherwise, return false to maintain heap properties
        return false;

    }
    bool operator==(const Complaint& obj)
    {
        if (type == obj.type)
            return true;
        return false;

    }
};

int Complaint::serialCounter = 0;


template<class T>
class Priorityqueue {
public:
    Priorityqueue() : currentSize(0), capacity(100)
    {
    }

    Priorityqueue(T* arr, int N)
    {
        currentSize = 0;
        capacity = N;

        for (int i = 0; i < N; i++)
            data.push_back(arr[i]);

        currentSize = data.size();

        buildMinHeap(N);

    }
    T Search(int val)
    {
        Complaint* ptr = 0;
        for (int i = 0; i < currentSize && ptr == 0; i++)
        {
            if ((*data[i]).isComplaintID(val))
                ptr = data[i];

        }

        return ptr;
    }
    void buildMinHeap(int N)
    {
        int index;
        for (index = N / 2 - 1; index >= 0; index--)
        {
            bubble_down(index);
        }
    }

    void insert(const T& x)
    {
        if (currentSize < capacity)
        {
            data.push_back(x);  // Dynamically grow the vector
            currentSize++;
            bubble_up(currentSize - 1);
        }
    }
    bool IncreasePriority(T complaintPtr)
    {
        // Locate the complaint in the heap
        int index = -1;
        for (int i = 0; i < currentSize; i++)
        {
            if (data[i] == complaintPtr)
            {
                index = i;
                break;
            }
        }

        if (index == -1)
        {
            cout << "Complaint not found in the priority queue." << endl;
            return false;
        }

        //  Increase the priority of the complaint (set type to highest priority)
        complaintPtr->Settype(1);  // Assuming "platinum" is the highest priority 

        // Bubble up the complaint to restore heap order
        bubble_up(index);

        return true;
    }

    bool isEmpty() const {
        if (currentSize == 0)
            return true;
        return false;
    }


    const T& getMin() const
    {
        if (currentSize != 0)
        {
            return data[0];
        }

        return 0;
    }
    void deleteMin()
    {
        T element = data[currentSize - 1]; //root el
        delete data[0];
        data[0] = element;
        data.pop_back();
        currentSize--; //del last element hypothetically
        bubble_down(0);
    }

    bool deleteAll(T key)
    {
        bool found = false;
        for (int i = 0; i < currentSize; ++i)
        {
            if (data[i] == key) {
                data[i] = data[--currentSize]; // Replace with the last element
                bubble_down(i);               // Restore heap property
                bubble_up(i);                 // Restore heap property
                found = true;
                --i; // Recheck the current index after shifting elements
            }
        }
        return found;

    }
    void Print()
    {
        for (int i = 0; i < currentSize; i++)
        {
            cout << data[i] << " ";
        }
        cout << endl;
    }
    ~Priorityqueue()
    {
        for (int i = 0; i < currentSize; i++)
            delete data[i];

        currentSize = 0;
    }


private:
    vector <T> data;
    int capacity;
    int currentSize;
    void Swap(int i, int j)
    {
        T temp = data[i];
        data[i] = data[j];
        data[j] = temp;

    }
    void bubble_up(int i)
    {
        if (i == 0)
            return;
        else
        {
            int parent = (i - 1) / 2;
            if (*data[i] < *data[parent])
            {
                Swap(i, parent);
                bubble_up(parent);
            }
        }
    }

    void bubble_down(int i)
    {
        int minChild, rightChild, leftchild; //indexes in the array

        leftchild = 2 * i + 1;
        rightChild = 2 * i + 2;

        if (leftchild <= currentSize - 1)
        {
            if (leftchild == currentSize - 1)
                minChild = leftchild;
            else
            {
                if (*data[rightChild] < *data[leftchild])
                    minChild = rightChild;
                else
                    minChild = leftchild;
            }

            if (*data[minChild] < *data[i])
            {
                Swap(i, minChild);
                bubble_down(minChild);
            }
        }
    }
};

class User {
private:
    int UserID;
    string UserName;
    string email;
    string country;
    string type;


public:
    User()
    {

    }
    User(int id, string usName, string Em, string _country, string _ty)
    {
        UserID = id;
        UserName = usName;
        email = Em;
        country = _country;
        type = _ty;
    }
    int GetId() const
    {
        return UserID;
    }
    string GetCountry() const
    {
        return country;
    }
    string GetType() const
    {
        return type;
    }
    string GetEmail() const
    {
        return email;
    }
    string GetUsername() const
    {
        return UserName;
    }
    ~User()
    {
        //useless
    }
    bool CompareEmail(string str)
    {
        if (email == str)
            return true;
        return false;
    }
    bool CompareUserName(string str)
    {
        if (UserName == str)
            return true;
        return false;
    }
    void Print() const
    {
        cout << "User Id:\t" << UserID << endl;
        cout << "Username:\t" << UserName << endl;
        cout << "Email:\t" << email << endl;
        cout << "Country:\t" << country << endl;
        cout << "Type:\t" << type << endl;
    }


};

template <typename T, typename U>
class AVLTree
{
private:
    template<class T, class U>
    class AVLNode {
    public:
        T data;
        U userPtr; //will be iterator based
        AVLNode<T, U>* left;
        AVLNode<T, U>* right;
        int height;

        AVLNode(T d, U ptr = 0, AVLNode<T, U>* l = 0, AVLNode<T, U>* r = 0, int h = 0)
        {
            data = d;
            left = l;
            userPtr = ptr;
            right = r;
            height = h;
        }
        ~AVLNode()
        {

        }
    };

    AVLNode<T, U>* root;

    // Helper functions
    int height(AVLNode<T, U>* nodeptr)
    {

        if (nodeptr == nullptr) {
            return -1;
        }

        int left = -1, right = -1;
        if (nodeptr->left)
            left = height(nodeptr->left);
        if (nodeptr->right)
            right = height(nodeptr->right);

        return max(left, right) + 1;
    }

    AVLNode<T, U>* findMin(AVLNode<T, U>* t)
    {
        if (t == nullptr) {
            return nullptr; // Tree is empty
        }

        // Traverse to the leftmost node
        while (t->left != nullptr) {
            t = t->left;
        }

        return t; // Return the node with the minimum element
    }

    void remove(const T& x, AVLNode<T, U>*& t)
    {

        if (t == nullptr)

            return; // Item not found; do nothing

        if (x < t->data)

            remove(x, t->left);

        else if (t->data < x)

            remove(x, t->right);

        else if (t->left != nullptr && t->right != nullptr) {//Two children

            AVLNode<T, U>* successor = findMin(t->right);
            t->data = successor->data;
            t->userPtr = successor->userPtr;

            remove(t->data, t->right);

        }

        else
        {

            AVLNode<T, U>* oldNode = t;

            if (t->left != nullptr)
                t = t->left;
            else
                t = t->right; // Explicitly set t to nullptr if it's a leaf

            delete oldNode;

        }

        balance(t); // same as insertion

    }
    void rightRotate(AVLNode<T, U>*& x) {
        // 1. Save orphan Y right subtree
        AVLNode<T, U>* orphan = x->left->right;

        // 2. Y(left child) is the new parent
        AVLNode<T, U>* y = x->left;

        // 3. attach x to right of new parent
        y->right = x;

        // 4. attach orphan
        x->left = orphan;

        x->height = height(x);  // update nodes'
        y->height = height(y);  // height values

        x = y;
    }
    void leftRotate(AVLNode<T, U>*& x)
    {
        // 1. Save orphan Y left subtree
        AVLNode<T, U>* orphan = x->right->left;

        // 2. Y(left child) is the new parent
        AVLNode<T, U>* y = x->right;

        // 3. attach x to right of new parent
        y->left = x;

        // 4. attach orphan
        x->right = orphan;

        x->height = height(x);  // update nodes'
        y->height = height(y);  // height values

        x = y;

    }
    void doubleLeftRightRotation(AVLNode<T, U>*& X)
    {
        leftRotate(X->left);
        rightRotate(X);

    }

    void doubleRightLeftRotation(AVLNode<T, U>*& X)
    {
        rightRotate(X->right);
        leftRotate(X);
    }

    void balance(AVLNode<T, U>*& x)
    {
        if (x == nullptr) return;
        if (height(x->right) - height(x->left) < -1)
        {
            if (height(x->left->left) >= height(x->left->right))
                rightRotate(x);
            else
                doubleLeftRightRotation(x);
        }
        else if (height(x->right) - height(x->left) > 1)
        {
            if (height(x->right->right) >= height(x->right->left))
                leftRotate(x);
            else
                doubleRightLeftRotation(x);
        }

        x->height = max(height(x->left), height(x->right)) + 1;
    }

    AVLNode<T, U>* findParent(AVLNode<T, U>* node, T val)
    {
        if (node == NULL) {
            return nullptr;  // Node not found
        }

        // If we find the node, return the parent
        if ((node->left && node->left->data == val) || (node->right && node->right->data == val)) {
            return node;  // Return the parent node
        }

        // Otherwise, recursively search in the left and right subtrees
        if (val < node->data) {
            return findParent(node->left, val);  // Search in left subtree
        }
        else {
            return findParent(node->right, val);  // Search in right subtree
        }
    }

    AVLNode<T, U>*& search(AVLNode<T, U>*& ptr, T val)
    {
        if (ptr == NULL)
            return ptr; //ref to that null
        else if (ptr->data == val)
            return ptr;
        else if (ptr->data < val)
            return search(ptr->right, val);
        else if (ptr->data > val)
            return search(ptr->left, val);
    }

    bool Search(AVLNode<T, U>* ptr, T& val)
    {
        if (ptr == NULL)
            return false;
        else if (ptr->data == val)
            return true;
        else if (ptr->data < val)
            return Search(ptr->right, val);
        else if (ptr->data > val)
            return Search(ptr->left, val);
    }

    bool insert(AVLNode<T, U>*& ptr, T& val, U& ptrToNode)
    {
        if (ptr == NULL)
        {
            ptr = new AVLNode<T, U>(val, ptrToNode);
            return true;

        }
        else if (ptr->data < val)
            insert(ptr->right, val, ptrToNode);
        else if (ptr->data > val)
            insert(ptr->left, val, ptrToNode);

        balance(ptr);

    }

    void inorderPrint(AVLNode<T, U>* ptr) const
    {
        if (ptr)
        {
            inorderPrint(ptr->left);
            (*(ptr->userPtr))->Print();
            cout << endl;
            inorderPrint(ptr->right);
        }

    }

public:

    //Finds Parent for The insertion in DLL
    U findParent(T val)
    {
        return (findParent(root, val))->userPtr; //return Iterator 
    }


    bool searchandPrint(T& val)
    {
        AVLNode<T, U>* node = search(root, val);
        if (node)
        {
            (*(node->userPtr))->Print();
            return true;
        }
        return false;
    }

    //Returns string type data of the node
    string searchAndReturnData(T& val)
    {
        AVLNode<T, U>* node = search(root, val);

        if (node)
        {
            string type = (*(node->userPtr))->GetType();
            return type;
        }
    }

    bool search(T& key)
    {
        bool found = Search(root, key);
        if (found)
            return true;
        return false;

    }

    //Searches the ptr witht he val, deltes it and returns iterator to the node to delete that from dll too
    U searchandDelete(T val)
    {
        AVLNode<T, U>*& node = search(root, val);
        if (node)
        {
            U it = node->userPtr;
            remove(val, node);
            return it;

        }
        return U(NULL);

    }

    //Searches Id and then inserts Iterator to the DLl node 
    bool searchandAdd(T val, U ptrToNode)
    {
        AVLNode<T, U>* node = search(root, val);
        if (node)
        {
            node->userPtr = ptrToNode;
            return true;

        }
        return false;
    }

    bool insert(T v, U ptr)
    {
        if (search(v))
            return false;

        else
            return insert(root, v, ptr);

    }

    void Destroy(AVLNode<T, U>*& ptr)
    {
        if (ptr)
        {
            Destroy(ptr->left);
            Destroy(ptr->right);
            delete ptr;

        }

    }

    void inorderPrint() const
    {
        inorderPrint(root);
        cout << endl;
    }

    bool isEmpty()
    {
        if (root)
            return false;
        return true;
    }

    AVLTree()
    {
        root = NULL;
    }

    ~AVLTree()
    {
        Destroy(root);
        root = NULL;
    }

};

class UserList {
private:
    //node class
    class Node {
    public:
        User* data;
        Node* next;
        Node* prev;

        Node()
        {
            data = 0;
            prev = 0;
            next = 0;
        }
        Node(User* obj, Node* nextPtr = 0, Node* prevPtr = 0)
        {
            data = obj;
            next = nextPtr;
            prev = prevPtr;
        }
        ~Node()
        {
            delete data;
            data = NULL;
            next = NULL;
            prev = NULL;
        }
    };
    //iterator class
    class ListIterator {
    public:
        friend class UserList;

        ListIterator(Node* ptr = NULL)
        {
            iptr = ptr;
        }

        ListIterator& operator++()
        {//prefix ++
            if (iptr)
                iptr = iptr->next;
            return (*this);
        }

        ListIterator operator++(int)
        {
            // postfix ++
            ListIterator old = *this;
            ++(*this);
            return old;
        }
        ListIterator& operator--() //pre dec

        {
            if (iptr)
                iptr = iptr->prev;
            return (*this);
        }

        ListIterator operator--(int) //post dec
        {
            ListIterator old = *this;
            --(*this);
            return old;
        }

        bool operator== (const ListIterator& rhs)
        {
            if (iptr == rhs.iptr)
                return true;
            return false;
        }

        bool operator !=(const ListIterator& rhs)
        {
            return !(iptr == rhs.iptr);
        }

        User*& operator* () const
        {
            if (iptr)
                return iptr->data;
        }

    private:
        typename UserList::Node* iptr;

    };

    Node* head;
    Node* tail;
    int size;
    //Associated indexes 
    UniversalHashtable<ListIterator> hashtableforUserId; // hashtable that have indexes, and the iterators stored to the values
    AVLTree<int, ListIterator> Indexes; // one AVL for insertion in o logn to keep dll sorted
    StrHashTable<ListIterator> UsernameIndex;
    StrHashTable<ListIterator> emailIndex;
    GroupHashTable<string, ListIterator> CountryGroupIndex;
    GroupHashTableQ <string, ListIterator> TypeGroupIndex;
   


public:
    typedef ListIterator Iterator;
    Iterator begin()
    {
        Iterator I(head);
        return I;
    }
    Iterator end()
    {
        Iterator I(tail);
        return I;
    }

    UserList()
    {
        head = new Node(NULL, tail);
        tail = new Node(NULL, NULL, head);
        head->next = tail;
    }

    //Insert in DLL
    typename UserList::Iterator InsertAfterPtr(Iterator ptr, User*& d)
    {
        Node* current = ptr.iptr;
        Node* newNode = new Node(d);
        if (current)
        {
            newNode->prev = current;
            newNode->next = current->next;
            current->next->prev = newNode;
            current->next = newNode;
            size++;
        }

        return Iterator(newNode);

    }

    //Insert in DLL
    typename UserList::Iterator InsertBeforePtr(Iterator ptr, User*& d)
    {

        Node* current = ptr.iptr;
        Node* newNode = new Node(d);
        if (current)
        {
            newNode->next = current;
            newNode->prev = current->prev;
            current->prev->next = newNode;
            current->prev = newNode;
            size++;
        }
        return Iterator(newNode);
    }

    //DLL based delete node 
    bool DeleteNode(Iterator& toDelete)
    {
        Node* to_Delete = toDelete.iptr;
        if (to_Delete)
        {
            to_Delete->next->prev = to_Delete->prev;
            to_Delete->prev->next = to_Delete->next;
            delete to_Delete;
            size--;
            return true;
        }

        return false;
    }

    //This function takes a user, inserts it into dll, returns ptr to it, which is comverted to iterator and passed to avl trees to insert withtin the indexed nodes
    bool InsertUser(int id, string usName, string Em, string _country, string _ty)
    {
        User* newUser = new User(id, usName, Em, _country, _ty);
        bool inserted = false;
        if (Indexes.isEmpty())
        {
            Iterator ptrToNode = InsertBeforePtr(end(), newUser); //insert befgore tail
            Indexes.insert(id, ptrToNode);
            hashtableforUserId.insert(id, ptrToNode); // Inserting the key into hash table too 
            UsernameIndex.insert(usName, ptrToNode);
            emailIndex.insert(Em, ptrToNode);
            CountryGroupIndex.insert(_country, ptrToNode);
            TypeGroupIndex.insert(_ty, ptrToNode);
            inserted = true;
        }
        else
        {
            Iterator it(NULL);
            bool IdalreadyinUse = Indexes.search(id);
            if (!IdalreadyinUse)
            {
                Indexes.insert(id, it);
                Iterator ptr = Indexes.findParent(id);
                if (id > ptr.iptr->data->GetId()) //if id of new USer is greater than parent attack next to parent
                {
                    Iterator ptrToNode = InsertAfterPtr(ptr, newUser);
                    Indexes.searchandAdd(id, ptrToNode); //logn to put the ptr to node into the avl node
                    hashtableforUserId.insert(id, ptrToNode);
                    UsernameIndex.insert(usName, ptrToNode);
                    emailIndex.insert(Em, ptrToNode);
                    CountryGroupIndex.insert(_country, ptrToNode);
                    TypeGroupIndex.insert(_ty, ptrToNode);

                    
                }
                else
                {
                    Iterator ptrToNode = InsertBeforePtr(ptr, newUser);
                    Indexes.searchandAdd(id, ptrToNode);
                    hashtableforUserId.insert(id, ptrToNode);
                    UsernameIndex.insert(usName, ptrToNode);
                    emailIndex.insert(Em, ptrToNode);
                    CountryGroupIndex.insert(_country, ptrToNode);
                    TypeGroupIndex.insert(_ty, ptrToNode);

                }

                inserted = true;
            }
            else
                inserted = false;
        }

        return inserted;
    }

    //Prints Based on userId( all the database)
    void PrintALL()
    {
        Indexes.inorderPrint();
    }

    //Searching user in database based on the unique id
    bool SearchUser(int id)
    {
        Iterator it = hashtableforUserId.search(id); //returns iterator by searching in o(1)
        if (it != 0)
        {
            cout << "User Found" << endl;
            (*it)->Print(); //Print the user detail 
            return true;
        }
        else
        {
            cout << "User not Found" << endl;
            return false;
        }
   
    }
    //deleting a specific index(index based on username) 
    bool DeleteIndexonUserName(string& usname)
    {
        bool deleted= UsernameIndex.deleteKey(usname);
        if (deleted)
        {
            cout << "User Deleted from Indexes of Username" << endl;
            return true;
        }
        else
        {
            cout << "User with this Username does not exist!" << endl;
            return false;
        }
    }

    //Deleting a specific index based on email
    bool DeleteIndexonEmail(string& em)
    {
        bool deleted = emailIndex.deleteKey(em);
        if (deleted)
        {
            cout << "User Deleted from Indexes of Email" << endl;
            return true;
        }
        else
        {
            cout << "User with this email does not exist!" << endl;
            return false;
        }


    }

    //helper function to find the string in data
    bool SearchandPrintinUserListbyUserName(string str)
    {
        Node* temp = head->next;//dummy based dll for minimal checks

        while (temp != tail)
        {
            bool found = temp->data->CompareUserName(str);
            if (found)
            {
                temp->data->Print();
                return true;
            }
            temp = temp->next;
        }

        return false;
    }

    //Searches and print the required user with the email
    bool SearchandPrintinUserListbyEmail(string str)
    {
        Node* temp = head->next;//dummy based dll for minimal checks

        while (temp != tail)
        {
            bool found = temp->data->CompareEmail(str);
            if (found)
            {
                temp->data->Print();
                return true;
            }
            temp = temp->next;
        }

        return false;
    }

    //Searches using the username in hash for username first, then in the dll
    bool SearchUsingUserName(string& usName)
    {
        Iterator it = UsernameIndex.search(usName);
        if (it != 0)
        {
            (*it)->Print();
            return true;
        }
        else
        {
            bool foundInDll = SearchandPrintinUserListbyUserName(usName);
            if (foundInDll)
            {
                cout << "Found in DLL but not in index " << endl;
                return true;
            }
            cout << "User Not Found!" << endl;
            return false;
        }


    }

    //Searches using email given first in avl index, then in dll if not found in first
    bool SearchUsingEmail(string& em)
    {
        Iterator itToobj = emailIndex.search(em);
        if (itToobj!=0)
        {
            (*itToobj)->Print();
            return true;
        }
        else
        {
            bool foundInDll = SearchandPrintinUserListbyEmail(em);
            if (foundInDll)
            {
                cout<< "Found in DLL but not in index " << endl;
                return true;
            }

            cout << "User Not Found!" << endl;
            return false;
        }
    }

    bool DeleteUser(int id)
    { //delete from all  group indices and single indexed tree 
        string usName, email, country, type;
        Iterator toDelete = Indexes.searchandDelete(id);
        hashtableforUserId.deleteKey(id);


        Node* temp = toDelete.iptr;

        if (temp)
        {
            usName = temp->data->GetUsername();
            email = temp->data->GetEmail();
            country = temp->data->GetCountry();
            type = temp->data->GetType();
        }



        if (toDelete != 0)
        {
            emailIndex.deleteKey(email);//deletion of indexes from the secondary indexxing 
            UsernameIndex.deleteKey(usName);
            CountryGroupIndex.deletePartKey(country, usName,email);
            TypeGroupIndex.deletePartKey(type, usName, email);
            
            bool deleted = DeleteNode(toDelete);//deleting from dll

            cout << "User Deleted" << endl;
            return true;
        }
        else
        {
            cout << "User not Found" << endl;
            return false;
        }
    }

    void DisplayNameIndex()
    {
        UsernameIndex.display();
    }

    void DisplayEmailIndex()
    {
        emailIndex.display();
    }

    void DisplayGroupOnCountry(string country)
    {
        List<Iterator>* listTousers = CountryGroupIndex.Search(country);
        listTousers->Print();
    }
    void DisplayGroupOnType(string type)
    {
        List<Iterator>* listTousers = TypeGroupIndex.Search(type);
        listTousers->Print();
    }

    void DisplayAllGroupCountryIndex()
    {
        CountryGroupIndex.display();
    }

    void DisplayAllGroupTypeIndex()
    {
        TypeGroupIndex.display();
    }

    bool DeleteIndexOnCountryGroup(string country)
    {
        bool deleted = CountryGroupIndex.deleteWholeKey(country);

        return deleted;
    }

    bool DeleteIndexOnTypeGroup(string type)
    {
        bool deleted = TypeGroupIndex.deleteWholeKey(type);

        return deleted;
    }

    string GetUsersType(int id)
    {
        string type = Indexes.searchAndReturnData(id);

        return type;
    }

    List<Iterator>* SearchGroupOnCountry(string country)
    {
        List<Iterator>* ptrToList = CountryGroupIndex.Search(country);
       
        return ptrToList;
    }

    ~UserList()
    {
        Node* temp = head;
        for (int i = 0; i < size + 2; i++)
        {
            head = temp->next;
            delete temp;
            temp = head;

        }
        tail = 0;
    }


};
class CustomerServiceModule {
private:
    Priorityqueue<Complaint*> queue;
    UserList usersOnPortal;
    GroupHashTable<int, Complaint::IterO> userIndex; //maps user ids to sll of complaints

public:
    CustomerServiceModule()
    {

    }

    void RegisterComplaint(int id, int userId, string text)
    {
        string type = usersOnPortal.GetUsersType(userId);
        Complaint* IncomingComplaint = new Complaint(id, userId, text, type);
        queue.insert(IncomingComplaint); //inserting complain in the priority queue, make <operator

        userIndex.insert(userId, IncomingComplaint);

    }

    void PrintComplaintsBuUser(int id)
    {
        cout << "Printing Complaints by User ID " << id << endl;
        List<Complaint::IterO>* listptr = userIndex.Search(id);
        listptr->Print();

    }

    void MakeComplaint()
    {
        RegisterComplaint(314, 36, "Slow internet");
        RegisterComplaint(388, 12, "Login Issue");
        RegisterComplaint(332, 12, "Logout Issue");
        RegisterComplaint(221, 14, "Payment Issue");
        RegisterComplaint(372, 96, "Account hacked");
        RegisterComplaint(389, 96, "Account not updated to premium");
        RegisterComplaint(56, 58, "Payment is not processing");
        RegisterComplaint(421, 42, "Unable to update server");
    }

    bool ServiceComplaint()
    {
        Complaint* top = queue.getMin();
        if (top)
        {
            top->Print();
            int Cmplainid = top->Getid();
            int userid = top->GetUserId();

            cout << "This complaint has been serviced!" << endl << endl;
            userIndex.deleteKey(userid);

            queue.deleteMin();


            if (top)
                return true;
        }
        return false;
    }

    bool IncreasePriority(int complainId)
    {
        Complaint* ptr = queue.Search(complainId);
        if (queue.IncreasePriority(ptr))
            return true;
        return false;
    }

    void PrintComplaintsByCountry(string country)
    {
        UserList::Iterator user;
        List<UserList::Iterator>* ptrToUsers = usersOnPortal.SearchGroupOnCountry(country);
        // If no users are found for the country, exit
        if (ptrToUsers == nullptr || ptrToUsers->isEmpty()) {
            cout << "No users found in this country." << endl;
            return;
        }


        cout << "Complaints from users in country: " << country << endl;

        typename List<UserList::Iterator>::IteratorS userPtr = ptrToUsers->begin();

        while (userPtr != ptrToUsers->end())
        {
            user = *userPtr; // Dereferencing to access the user iterator

            // Retrieve and print complaints associated with the user
            List<Complaint::IterO>* listptr = userIndex.Search((*user)->GetId());  // This will print all complaints for this user
            listptr->Print();
            cout << endl;

            userPtr++;
        }
    }

    void setUp()
    {
        if (!usersOnPortal.InsertUser(42, "Calli", "calli@gmail.com", "UK", "platinum"))
            cout << "Id already in use." << endl;
        if (!usersOnPortal.InsertUser(36, "Ali", "alii@gmail.com", "Canada", "new"))
            cout << "Id already in use." << endl;
        if (!usersOnPortal.InsertUser(47, "Nidi", "nidi@gmail.com", "Pakistan", "silver"))
            cout << "Id already in use." << endl;
        if (!usersOnPortal.InsertUser(87, "Mahira", "mahira@gmail.com", "Canada", "new"))
            cout << "Id already in use." << endl;
        if (!usersOnPortal.InsertUser(96, "Momin", "momii@gmail.com", "Pakistan", "regular"))
            cout << "Id already in use." << endl;
        if (!usersOnPortal.InsertUser(14, "Nayab", "nayab05@gmail.com", "Dubai", "gold"))
            cout << "Id already in use." << endl;
        if (!usersOnPortal.InsertUser(12, "Ayub", "ayub00@gmail.com", "Dubai", "new"))
            cout << "Id already in use." << endl;
        if (!usersOnPortal.InsertUser(58, "Musa", "musa@gmail.com", "Dubai", "new"))
            cout << "Id already in use." << endl << endl;

        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        cout << "Printing ALL Users:" << endl;
        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        usersOnPortal.PrintALL();

        //Q1 , 2 and 3 
        //---------------------------------------------------------------------------------------------------------------------------



        //int id = 96;
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //cout << "Searching User with ID " << id << " : " << endl;
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //usersOnPortal.SearchUser(id);

        //id = 41;
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //cout << "Searching User with ID " << id << " : " << endl;
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //usersOnPortal.SearchUser(id);


        //id = 96;
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //cout << "Deleting and then Searching User with ID " << id << " : " << endl;
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //usersOnPortal.DeleteUser(id);
        //usersOnPortal.SearchUser(id);


        //string name = "Momin";
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //cout << "Searching User with Username " << name << " : " << endl;
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //usersOnPortal.SearchUsingUserName(name);

        //name = "Calli";
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //cout << "Searching User with Username " << name << " : " << endl;
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //usersOnPortal.SearchUsingUserName(name);


        //name = "Musa";

        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //cout << "Searching User with Username " << name << " : " << endl;
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //usersOnPortal.SearchUsingUserName(name);


        //name = "Ali";
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //cout << "Searching User with Username " << name << " : " << endl;
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //usersOnPortal.SearchUsingUserName(name);


        //string em = "momii@gmail.com";
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //cout << "Searching User with Email " << em << " : " << endl;
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //usersOnPortal.SearchUsingEmail(em);


        //em = "nayab05@gmail.com";
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //cout << "Searching User with Email " << em << " : " << endl;
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //usersOnPortal.SearchUsingEmail(em);


        //name = "Ali";
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //cout << "Deleting Index on Username only and then Searching User with name " << name << " : " << endl;
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //usersOnPortal.DeleteIndexonUserName(name);
        //usersOnPortal.SearchUsingUserName(name);
        //
        //name = "Momin"; //deleted this already from the database 
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //cout << "Deleting Index on Username only and then Searching User with name " << name << " : " << endl;
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //usersOnPortal.DeleteIndexonUserName(name);
        //usersOnPortal.SearchUsingUserName(name);


        //em = "ayub00@gmail.com";
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //cout << "Deleting Index on Email only and then Searching User with email " << em << " : " << endl;
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //usersOnPortal.DeleteIndexonEmail(em);
        //usersOnPortal.SearchUsingEmail(em);


        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //cout << "Displaying all Indexes on the Username:  " << endl;
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //usersOnPortal.DisplayNameIndex();

        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //cout << "Displaying all Indexes on the Email:  " << endl;
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //usersOnPortal.DisplayEmailIndex();

     
        ////Q3 testing group based hashes
        //string country = "Dubai";
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //cout << "Displaying all Users in the Country :  "<<country << endl;
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //usersOnPortal.DisplayGroupOnCountry(country);



        //country = "Pakistan";
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //cout << "Displaying all Users in the Country :  " << country << endl;
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //usersOnPortal.DisplayGroupOnCountry(country);

        //string ty = "platinum";
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //cout << "Displaying all Users with membership :  " << ty << endl;
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //usersOnPortal.DisplayGroupOnType(ty);


        //ty = "new";
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //cout << "Displaying all Users with membership :  " << ty << endl;
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //usersOnPortal.DisplayGroupOnType(ty);


        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //cout << "Deleting Index based on type and Displaying all Users Indexes made on membership "<< endl;
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //usersOnPortal.DeleteIndexOnTypeGroup(ty);
        //usersOnPortal.DisplayAllGroupTypeIndex();



        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //cout << "Deleting Index based on Country and then Displaying all indexes on Country  " << endl;
        //cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        //usersOnPortal.DeleteIndexOnCountryGroup(country);
        //usersOnPortal.DisplayAllGroupCountryIndex();



    }

    void Complaints()
    {
        //Q4
        MakeComplaint();


        string country = "Canada";
        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        cout << "Searching Complaints From Country : " << country << endl;
        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        PrintComplaintsByCountry(country);



        int userid = 12;
        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        cout << "Printing ALl Complaints by User: " << endl;
        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        PrintComplaintsBuUser(userid);


        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        cout << "Servicing Complaint: " << endl;
        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        ServiceComplaint();

        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        cout << "Servicing Complaint: " << endl;
        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        ServiceComplaint();

        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        cout << "Servicing Complaint: " << endl;
        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        ServiceComplaint();

        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        cout << "Servicing Complaint: " << endl;
        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        ServiceComplaint();


        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        cout << "Servicing Complaint: " << endl;
        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        ServiceComplaint();


        int complaintId = 372;
        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        cout << "Increasing priorty for Complaint id: " << complaintId << endl;
        cout << "-----------------------------------------------------------------------------------------------------------" << endl;

        if (IncreasePriority(complaintId)) //search this id, if found, increase its priority
            cout << "Priority Increased for the id" << endl;
        else
            cout << "Complaint id not found!" << endl;

        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        cout << "Servicing Complaint: " << endl;
        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        ServiceComplaint(); //should be this 372


        complaintId = 56;
        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        cout << "Increasing priorty for Complaint id: " << complaintId << endl;
        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        if (IncreasePriority(complaintId)) //search this id, if found, increase its priority
            cout << "Priority Increased for the id" << endl;
        else
            cout << "Complaint id not found!" << endl;

        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        cout << "Servicing Complaint: " << endl;
        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        ServiceComplaint();//should be 56


        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        cout << " Registering a new Complaint: " << endl;
        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        RegisterComplaint(58, 42, "Settings not working");



        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        cout << "Servicing Complaint: " << endl;
        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        ServiceComplaint();

        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        cout << "Servicing Complaint: " << endl;
        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        ServiceComplaint();

        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        cout << "Servicing Complaint: " << endl;
        cout << "-----------------------------------------------------------------------------------------------------------" << endl;
        if (!ServiceComplaint())
            cout << "All complaints have been registered!";


    }
    ~CustomerServiceModule()
    {

    }


};

int main()
{

    CustomerServiceModule myServer;
    myServer.setUp();
    myServer.Complaints();

    return 0;
}