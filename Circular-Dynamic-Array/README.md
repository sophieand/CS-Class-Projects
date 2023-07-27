#Circular Dynamic Array

A program that creates a circular dynamic array.

CircularDynamicArray(): Default Constructor. The array should be of capacity 2.

CircularDynamicArray(int s): For this constructor the array should be of capacity and size s.

~CircularDynamicArray(): Destructor

elmtype& operator[](int i): Traditional [] operator. Should print a message if i is out of bounds and return a reference to value of type elmtype stored in the class for this purpose..

void addEnd(elmtype v): Increases the size of the array by 1 and stores v at the end of the array. Should double the capacity when the new element doesn't fit.

void addFront(elmtype v): increases the size of the array by 1 and stores v at the beginning of the array. Should double the capacity when the new element doesn't fit. The new element should be the item returned at index 0.


void delEnd(): reduces the size of the array by 1 at the end. Should shrink the capacity when only 25% of the array is in use after the delete.

void delFront(): reduces the size of the array by 1 at the beginning of the array. Should shrink the capacity when only 25% of the array is in use after the delete.

int length(): returns the size of the array.

int capacity(): returns the capacity of the array.

int clear(): Frees any space currently used and starts over with an array of size 2.

Elmtype QuickSelect(int k): returns the kth smallest element in the array using the quickselect algorithm.

Elmtype WCSelect(int k): returns the kth smallest element in the array using the worst case O(N) time algorithm.

void stableSort(): Sorts the values in the array using a comparison based O(N lg N) algorithm. The sort must be stable.

void radixSort(int i): Sorts the values in the array using a radix based sort on the low order i bits of elmtype.

int linearSearch(elmtype e): Performs a linear search of the array looking for the item e. Returns the index of the item if found or -1 otherwise.

int binSearch(elmtype e): Performs a binary search of the array looking for the item e. Returns the index of the item if found or -1 otherwise. Assumes that the array is in sorted order.


