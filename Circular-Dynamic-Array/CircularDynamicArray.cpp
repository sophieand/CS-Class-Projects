#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

template <class E>
class CircularDynamicArray {
	private:
	bool reversed;                  //boolean to check if state is reversed or not
	int size;                       //size of array
	int cap;                        //capacity of array
	E *circle;                      //stores array
	E store;                        //kept in class to return a reference to in elmtype
	int front;                  
 
	E partition(E arr[], int l, int r) {
		E x = arr[r];
		E i = l;
		for (int j = l; j <= r - 1; j++) {
			if (arr[j] <= x) {
				swap(arr[i], arr[j]);
				i++;
			}
		}
		swap(arr[i], arr[r]);
		return i;
	}
	int QuickSelectRecursive(E argh[], int l, int r, int k) {
		if (k > 0 && k <= r - l + 1) {
			int index = partition(argh, l, r);
			if (index - l == k - 1)
				return argh[index];
			if (index - l > k - 1)
				return QuickSelectRecursive(argh, l, index - 1, k);
			return QuickSelectRecursive(argh, index + 1, r, k - index + l - 1);
		}
		return store;
	}
	void mergeSortRecursive(E argh[], int l, int r) {
		if (r <= l) return;
		
		int mid = l + (r-l)/2;
		mergeSortRecursive(argh, l, mid);
		mergeSortRecursive(argh, mid+1, r);
	
		int size = r - l + 1;
		vector<E> temp(size,0);

		int i = l;
		int j = mid + 1;

		for (int k = 0; k <= r-l; k++) {
			if (i == mid + 1) {
				temp[k] = argh[j];
				j++;
			} else if (j == r + 1) {
				temp[k] = argh[i];
				i++;
			} else if (argh[i] < argh[j]) {
				temp[k] = argh[i];
				i++;
			} else {
				temp[k] = argh[j];
				j++;
			}
		}
		for (int k = 0; k <= r-l; k++) {
			argh[k+l] = temp[k];
		}
	}
	

	public:
	CircularDynamicArray() {        //default constructor
		this->reversed = false;
		this->cap = 2;
		this->size = 0;
		this->circle = new E[cap];
		this->front = 0;
	} 
	~CircularDynamicArray() {       //destructor										
	    delete[] circle;
	}
	CircularDynamicArray(int s) {   //default constructor of size s
		this->reversed = false;
		this->cap = s;
		this->size = s;
		this->circle = new E[cap];
		this->front = 0;
	}
	CircularDynamicArray(const CircularDynamicArray<E>& old) { // copy constructor
		this->cap = old.cap;
		this->size = old.size;
		this->front = old.front;
		this->store = old.store;
		this->circle = new E[cap];
		copy(&old.circle[0], &old.circle[0] + old.cap, &circle[0]); //copies elements into new array
	}
	E& operator[](int i) { //i finds an index in bounds and prints the value of it
		if (i < 0 || i > this->size-1) {
			cout << "Input is out of bounds" << endl;
			return this->store;
		} 
        else {
			if (reversed) { //checks if reversed to start from the back of the array
				return circle[((front + size) - 1 - i) % cap];
			} 
            else {
				return circle[(front + i) % cap]; //searches for the index put in by the user and returns it
			}
		}
	}
	CircularDynamicArray<E>& operator=(const CircularDynamicArray<E>& old) { //copy assignment operator - needed b/c of rule of 3
		if (this != &old) {
			this->cap = old.cap;
			this->size = old.size;
			this->front = old.front;
			this->store = old.store;
			delete[] circle;
			this->circle = new E[cap];
			copy(&old.circle[0], &old.circle[0] + old.cap, &circle[0]); //copies elements into new array
		}
		return *this;
	}
	void addEnd(E v) {
		if (reversed) { //checks if reversed, if so, addFront's code is triggered
			if (this->size == this->cap) {
				E* newArray = new E[cap * 2];
				newArray[0] = v;
				for (int i = 0; i < cap; i++) {
					newArray[i + 1] = circle[(front + i) % cap];
				}
				size++;
				front = 0;
				cap *= 2;
				delete[] circle;
				circle = newArray;
			}
			else {
				front = (front - 1) % cap;
				if (front < 0) front = cap-1;
				circle[front] = v;
				size++;
			}
			return;
		}

		if (this->size == this->cap) { //checks if the array is full, if full, copies the old array into a new array then performs function
			E* newArray = new E[cap * 2];
			for (int i = 0; i < cap; i++) {
				newArray[i] = circle[(front + i) % cap];
			}
			newArray[cap] = v;
			cap = cap * 2;
			front = 0;
			delete[] circle;
			circle = newArray;
		}
		else { //else, the new addEnd is created
			circle[(front + size) % cap] = v; 
		}
		size++;
	}
	void addFront(E v) {
		if (reversed) { //addEnd's code is used if reversed is triggered
			if (this->size == this->cap) {
				E* newArray = new E[cap * 2];
				for (int i = 0; i < cap; i++) {
					newArray[i] = circle[(front + i) % cap];
				}
				newArray[cap] = v;
				cap = cap * 2;
				front = 0;
				delete[] circle;
				circle = newArray;
			}
			else {
				circle[(front + size) % cap] = v; 
			}
			size++;
			return;
		}
		if (this->size == this->cap) { //checks if full, if so, a new array is created and old array is copied into new, then addFront
			E* newArray = new E[cap * 2];
			newArray[0] = v;
			for (int i = 0; i < cap; i++) {
				newArray[i + 1] = circle[(front + i) % cap];
			}
			cap *= 2;
			size++;
			front = 0;
			delete[] circle;
			circle = newArray;
		}
		else { //front is moved to index before, or circled around, new front becomes the new value of v
			front = (front - 1) % cap;
			if (front < 0) front = cap-1;
			circle[front] = v;
			size++;
		}
	}
	void delEnd() {
		if (reversed) { //check if reversed, if so, delFront is triggered instead
			if (this->size <= cap/4) {
				E* newArray = new E[cap/2];
				for (int i = 1; i < size; i++) {
					newArray[i] = circle[(front + i) % cap];
				}
				size = size - 1;
				cap = cap / 2;
				delete[] circle;
				circle = newArray;
				front = 0;
			}
			else {
				front = (front + 1) % cap;
				size = size - 1;
			}   
			return;
		}

		if (this->size <= cap/4) { // check if size is 1/4 of capacity, if so, a new array is created
			E* newArray = new E[cap/2];
			for (int i = 0; i < size - 1; i++) {
				newArray[i] = circle[(front + i) % cap];
			}
			size = size - 1;
			cap = cap / 2;
			delete[] circle;
			circle = newArray;
			front = 0;
		}
		else { //back is deleted
			size = size - 1;
		}
	}
	void delFront() {
		if (reversed) { //check if reversed, if so, delEnd is triggered instead
			if (this->size <= cap/4) {
				E* newArray = new E[cap/2];
				for (int i = 0; i < size - 1; i++) {
					newArray[i] = circle[(front + i) % cap];
				}
				size = size - 1;
				cap = cap / 2;
				delete[] circle;
				circle = newArray;
				front = 0;
			}
			else {
				size = size - 1;
			}
			return;
		}

		if (this->size <= cap/4) { //check if size is 1/4 capacity, if so, new array is created
			E* newArray = new E[cap/2];
			for (int i = 1; i < size; i++) {
				newArray[i] = circle[(front + i) % cap];
			}
			size = size - 1;
			cap = cap / 2;
			delete[] circle;
			circle = newArray;
			front = 0;
		}
		else { //change to the new front then downsize
			front = (front + 1) % cap;
			size = size - 1;
		}
	}
	int length() { //print current size
		return this->size;
	}
	int capacity() { //print current capacity
		return this->cap;
	}
	void clear() { //overwrite variables, destroy old array, and create new array
		this->size = 0;
		this->cap = 2;
		this->front = 0;
		delete[] circle;
		this->circle = new E[cap];
	}
	E QuickSelect(int k) { //
		E* newArray = new E[cap];
		for (int i = 0; i < size; i++) {
			newArray[i] = circle[(front + i) % cap];
		}
		int first = 0;
		int last = size - 1;
		E nth = QuickSelectRecursive(newArray, first, last, k);
		return nth;
	}
	void stableSort() { //create a new array, initiate mergesort
		E* newArray = new E[cap];
		for (int i = 0; i < size; i++) {
			newArray[i] = circle[(front + i) % cap];
		}
		int first = 0;
		int last = size - 1;
		mergeSortRecursive(newArray, first, last);
		delete[] circle;
		circle = newArray;
		front = 0;

		return;
	}
	int linearSearch(E e) {
		for (int i = 0; i < this->size; i++) { //iterate through entire list
			if (e == this->circle[(front + i) % cap]) { //if the value is equal to any part of the list, then return the index
				if (reversed) return size-1-i;
				return i;
			}
		}
		return -1; //else, return false
	}
	int binSearch(E e) { //assume array is already sorted
		int front = 0;	//set front
		int back = this->size - 1; //set back
		int mid = (back - front) / 2; //set middle

		while (back - front > 1) { //while not end of array
			mid = (back + front) / 2; //set new middle to keep chopping off parts of the array that aren't needed
			if (circle[mid] < e) { //if the value at the current middle is less than one, set a new front to be above it
				front = mid + 1;
			} else { 				//
				back = mid;
			}
		}
		if (circle[front] == e) {
			if (reversed) {
				return size-1-front;
			}
			return front;
		} else if (circle[back] == e) {
			if (reversed) return size-1-back;
			return back;
		} else {
			return -1;
		}
	}
	void reverse() {
		this->reversed = !reversed;

		//set a new front
		//create a boolean in addfront, addback, delEnd, delFront that calls the opposite function

	}
};