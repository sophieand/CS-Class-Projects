#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class Node {
public:
    char c;
    int freq;
    Node *left;
    Node *right;
    int codelen;
    int ComPlusFreq;
    Node(char c, int freq);
    Node(int sum, Node *left, Node *right);
};

Node::Node(char c, int freq) {
    this->c = c;
    this->freq = freq;
    this->left = nullptr;
    this->right = nullptr;
    //this->ComPlusFreq = this->freq * this->codelen;
}

Node::Node(int sum, Node *left, Node *right) {
    this->freq = sum;
    this->left = left;
    this->right = right;
}

class MinHeap {
private:
    int MAX_CAP;
    int sizeOfHeap;
    int parent(int i) {return (i - 1) / 2;}
    int leftChild(int i) {return (2 * i + 1);}
    int rightChild(int i) {return (2 * i + 2);}
public:
    //vector<Node*> list;
    Node **list;
    MinHeap(int cap);
    void insert(Node* k);
    Node *removeMin();
    void heapify(int i);
    void printHeap();
    int getSize();
    int findUnc();
    int findComLen();
    void printHuff();
};

MinHeap::MinHeap(int cap) {
    sizeOfHeap = 0;
    MAX_CAP = 128;
    //list.resize(MAX_CAP);
    list = new Node*[MAX_CAP];
}

void MinHeap::insert(Node* k) {
    sizeOfHeap++; //help to find end of current list

    int i = sizeOfHeap - 1;
    list[i] = k; //new key is inserted at end

    //while i is not the root and while parent is greater than child
    while(i != 0 && list[parent(i)]->freq > list[i]->freq) {
        //swap parent and child
        swap(list[i], list[parent(i)]);
        //goes to parent func in class (i - 1)
        i = parent(i);
    }
}

void MinHeap::heapify(int i) {
    int l = leftChild(i);
    int r = rightChild(i);
    int tiny = i;

    //compares the left and right child with the current node
    //finding the smallest
    if ((l < sizeOfHeap) && (list[l]->freq < list[tiny]->freq)) {
        tiny = l;
    }
    if ((r < sizeOfHeap) && (list[r]->freq < list[tiny]->freq)) {
        tiny = r;
    }

    //if node doesnt equal the smallest, swap node with smallest, recursion to find smallest again
    if (tiny != i) {
        swap(list[i], list[tiny]);
        heapify(tiny);
    }
}

Node *MinHeap::removeMin() {
    if (sizeOfHeap == 0) {
        return nullptr;
    }
    else if (sizeOfHeap == 1) { //return current list, no change needed
        sizeOfHeap--;
        return list[0];
    }
    else {
        //get root
        Node *root = list[0];

        //extract current root and switch it with rightmost element, reduce size by 1, compare root's left and right child in heapify
        list[0] = list[sizeOfHeap - 1];
        sizeOfHeap--;
        heapify(0);

        return root; //returns previous root node
    }
}

int MinHeap::getSize() {
    return sizeOfHeap;
}

void MinHeap::printHeap() {
    // int missMyDog = 0;
    // int value = 1;
    // for (int i = 0; i < sizeOfHeap; i++) {
    //     if (i == value) {
    //         cout << endl;
    //         missMyDog += 1;
    //         value += (1 << missMyDog);
    //     }
    //     cout << list[i]->freq << " ";
    // }
    // cout << endl;

    for (int i = 0; i < sizeOfHeap; i++) {
        cout << list[i]->freq << ": " << list[i]->c << endl;
    }
}

void preorder(Node *root, int depth, Node* arr[]) {
    if (root == nullptr) { return; }
    if (root->left == nullptr && root->right == nullptr) {
        root->codelen = depth;
        arr[(unsigned char)root->c] = root;
    }

    preorder(root->left, depth + 1, arr);
    preorder(root->right, depth + 1, arr);
}

// int MinHeap::findUnc() {
//     int uncLen = sizeOfHeap * 8;
//     return uncLen;
// }

int MinHeap::findComLen() {
    int comLen = 0;
    //int i = 0;
    // while (1) {
    //     if (list)
    //     cout << "hi" << endl;
    //     comLen += list[i]->freq * list[i]->codelen;
    //     i++;
    // }
    cout << sizeOfHeap << endl;
    for (int i = 0; i < sizeOfHeap; i++) {
        cout << "hi" << endl;
        comLen += list[i]->freq * list[i]->codelen;
    }
    return comLen;
}

// void MinHeap::printHuff() {
//     //uncompressed length
//     int uncLen = findUnc();

//     //compressed length
//     int comLen = findComLen();

//     Node *root = removeMin();
//     //preorder(root, 0);
//     for (int i = 0; i < getSize(); i++) {
//         cout << "Uncompressed length: " << uncLen << " bits" << endl;
//         cout << "Compressed length: " << comLen << " bits" << endl;

//         cout << "CHAR|FREQ|CODE_LEN" << endl;
//         cout << "'" << list[i]->c << "'|" << list[i]->freq << "|" << preorder(root, 0);
//     }
// }

int main(int argc, char *argv[]) {
    ifstream file;
    file.open(argv[1]);

    char ch;
    int stringSize = 0;
    int charList[128] = { 0 };
    Node* arr[128] = { nullptr };
    
    //counting freqs in an array
    while (file.get(ch)) {
        stringSize++;
        charList[(int)ch]++;
    }

    MinHeap *H = new MinHeap(128);
    for (int j = 0; j < 128; j++) {
        if (charList[j] > 0) { 
            H->insert(new Node((char)j, charList[j]));
        }
    }
   // H->printHeap(); // Is valid heap?

    int sizeUniqueChars = 0;
   for (int i = 0; i < 128; i++) {
       if (charList[i] > 0) {
           sizeUniqueChars++;
       }
   }

    while (H->getSize() >= 2) {
        Node* left = H->removeMin();
        Node* right = H->removeMin();
        int sum = left->freq + right->freq;
        H->insert(new Node(sum, left, right));
    }

    Node *root = H->removeMin();

    

    preorder(root, 0, arr);

    //uncompressed length
    int uncLen = stringSize * 8;

    int comLen = 0;
    //compressed length
    for (int i = 0; i < 128; i++) {
        //cout << "hi" << endl;
        if (arr[i] == nullptr) continue;
        comLen += arr[i]->freq * arr[i]->codelen;
    }

    cout << "Uncompressed length: " << uncLen << " bits" << endl;
    cout << "Compressed length: " << comLen << " bits" << endl;
    cout << "CHAR|FREQ|CODE_LEN" << endl;

    // H->getSize()
    // off of arr
    for (int i = 0; i < 128; i++) {
        if (arr[i] == nullptr) continue;
        if (arr[i]->freq > 0) {
            //cout << "'" << H->list[i]->c << "'|" << H->list[i]->freq << "|" << H->list[i]->codelen << endl;
            cout << "'" << arr[i]->c << "'|" << arr[i]->freq << "|" << arr[i]->codelen << endl;
        }
    }

   return 0;
}
