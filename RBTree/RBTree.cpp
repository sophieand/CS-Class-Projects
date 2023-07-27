#include <iostream>
#include <algorithm>
#include <stdio.h>

using namespace std;

//Black is 0, Red is 1
//not gonna test code for duplicates (multiple of the same integer), will test for unique keys, 
//key: used for ordering the nodes
//val: the value stored at each node
//color: the color of the node

/**
 * TODO: NOTES
 * check if a destructor, a copy constructor, and a copy assignment operator is needed
 * check if locations of size variables is correct
 * check if rank in selectSorter() is correct
 * check if issues in rank() can be fixed
 * fix size() function
 * LOCATION OF SIZE DECREMENT IN REMOVE() IS MOST LIKELY WRONG
 * finish annotating
 * issues: PREORDER AND POSTORDER ARENT PRINTING OUT IN CORRECT ORDER, SELECT IS WRONG
 * SIZE: need to edit size? confused on what I'm supposed to put in constructor
 * also current size is possibly wrong?
 */

enum color {
    black, //or 0
    red    //or 1
} color;

class keytype{};
class valuetype{};

//class for Node
template <class keytype, class valuetype> class Node {
public:
    keytype key;
    valuetype value;
    int color;
    Node *leftChild; //pointer to the left child of a parent node
    Node *rightChild; //pointer to the right child of a parent node
    Node *parent; //pointer to the parent node of two children nodes
    int size;

    Node() {                
        color = black; //or black = 0
        size = 1;
    }
    Node(keytype k, valuetype v, int color) { //constructor
        color = color;
        key = k;
        value = v;
        size = 1;
    }
};

template <class keytype, class valuetype> class RBTree {
private:
    Node<keytype, valuetype> *NIL;
    Node<keytype, valuetype> *root;
    int current_size; //TODO:

    /**
     * @brief a helper to the constructor with s items
     *        buildSubTree creates the left and right subtree by finding the median of a group of elements
     *        result will have the lowest level nodes as red and the rest black
     * 
     * @param k - used for ordering the nodes
     * @param v - the value stored at each node
     * @param s - number of items(keytype, valuetype)
     * @param parent - parent to each node
     * @return Node<keytype, valuetype>* 
     */
    Node<keytype, valuetype> *buildSubTree(keytype k[], valuetype v[], int s, Node<keytype, valuetype>* parent) {
        if (s == 0) return this->NIL; //if s is now empty, the tree is finished
        Node<keytype, valuetype> *newNode = new Node<keytype, valuetype>;
        newNode->parent = parent;
        newNode->leftChild = this->NIL;
        newNode->rightChild = this->NIL;
        newNode->color = black;
        newNode->size = 1;
        if (s == 1) { //the subtree is at the end nodes
            newNode->key = k[0];
            newNode->value = v[0];
            newNode->color = red;
        } 
        else { //keep building the tree
            newNode->key = k[s/2-1 + (s % 2)]; //find median by 2k-1 elements
            newNode->value = v[s/2-1 + (s % 2)]; //find median by 2k-1 elements
            newNode->leftChild = buildSubTree(k, v, (s/2-1 + (s%2)), newNode); //create left subtree
            newNode->size += newNode->leftChild->size;
            newNode->rightChild = buildSubTree(k+(s/2 + (s % 2)), v+(s/2 + (s%2)), s-(s/2 + (s%2)), newNode); //create right subtree
            newNode->size += newNode->rightChild->size;
        }
        return newNode;
    }
    int deleteNodeHelper(Node<keytype,valuetype> *node, keytype key);
    void rbTransplant(Node<keytype,valuetype> *u, Node<keytype,valuetype> *v);
    void fixDelete(Node<keytype,valuetype> *x);
public:
    RBTree(); 
    RBTree(keytype k[], valuetype V[], int s);
    Node<keytype, valuetype>* copy(Node<keytype, valuetype> *curr, Node<keytype, valuetype> *oldNILL);
    RBTree(const RBTree &other) {
        NIL = new Node<keytype,valuetype>();
        NIL->leftChild = nullptr;
        NIL->rightChild = nullptr;
        NIL->parent = nullptr;
        root = copy(other.root, other.NIL);
    }
    RBTree& operator=(const RBTree& other) {
        NIL = new Node<keytype,valuetype>();
        NIL->leftChild = nullptr;
        NIL->rightChild = nullptr;
        NIL->parent = nullptr;
        root = copy(other.root, other.NIL);
        return *this;
    }
    void deleteNodes(Node<keytype, valuetype> *curr);
    ~RBTree();
    void preorderSorter(Node<keytype, valuetype> *node);
    void postorderSorter(Node<keytype, valuetype> *node);
    void inorderSorter(Node<keytype, valuetype> *node); 
    Node<keytype, valuetype>* minimum(Node<keytype, valuetype> *curr);
    Node<keytype, valuetype>* maximum(Node<keytype, valuetype> *curr);
    Node<keytype, valuetype>* searchingTheTree(Node<keytype, valuetype> *node, keytype key);
    valuetype* search(keytype k);
    void leftRotation(Node<keytype,valuetype> *x);
    void rightRotation(Node<keytype,valuetype> *x);
    void insertFix(Node<keytype, valuetype> *k);
    void insert(keytype k, valuetype v);
    int rank(keytype k);
    Node<keytype, valuetype>* selectSorter(Node<keytype, valuetype> *x, int pos);
    keytype select(int pos);
    Node<keytype, valuetype>* findPredecessor(Node<keytype, valuetype> *x);
    Node<keytype, valuetype>* findSuccessor(Node<keytype, valuetype> *x);
    keytype *successor(keytype k);
    keytype *predecessor(keytype k);
    int size();
    void preorder();
    void inorder();
    void postorder();
    int remove(keytype k);
};

/**
 * @brief Construct a new RBTree<keytype, valuetype>::RBTree object
 * 
 * @tparam keytype 
 * @tparam valuetype 
 */
template <class keytype, class valuetype>
RBTree<keytype, valuetype>::RBTree() {
    NIL = new Node<keytype, valuetype>();
    NIL->color = black;
    NIL->parent = nullptr;
    NIL->leftChild = nullptr;
    NIL->rightChild = nullptr;
    NIL->size = 0;
    current_size = 0; //TODO:
    root = NIL;
}

/**
 * @brief Construct a new RBTree<keytype, valuetype>::RBTree object
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param k - used for ordering a node into the tree
 * @param v - value stored at each node
 * @param s - s items stored in tree
 */
template <class keytype, class valuetype>
RBTree<keytype, valuetype>::RBTree(keytype k[], valuetype v[], int s) {
    NIL = new Node<keytype, valuetype>();
    NIL->color = black;
    NIL->parent = nullptr;
    NIL->leftChild = nullptr;
    NIL->rightChild = nullptr;
    NIL->size = 0;
    bool inorder = true;
    for (int i = 0; i < s-1; i++) {
        if (k[i] > k[i+1]) inorder = false;
    }
    if (inorder) {
        root = buildSubTree(k, v, s, NIL);
    } else {
        root = new Node<keytype, valuetype>();
        root->color = black;
        root->parent = NIL;
        root->leftChild = NIL;
        root->rightChild = NIL;
        root->size = 1;
        root->key = k[0];
        root->value = v[0];
        for (int i = 1; i < s; i++) {
            insert(k[i], v[i]);
        }
    }
    insertFix(root);
}

/**
 * @brief destructor assistant
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param curr - current node (and its children) to be deleted
 */
template <class keytype, class valuetype>
void RBTree<keytype, valuetype>::deleteNodes(Node<keytype, valuetype> *curr) {
    if (curr == this->NIL) return; //if tree is now empty

    deleteNodes(curr->leftChild); //check left child's left and right child
    if (curr->leftChild != this->NIL) { //free left child if not already deleted
        delete curr->leftChild;
    }
    deleteNodes(curr->rightChild);
    if (curr->rightChild != this->NIL) {
        delete curr->rightChild;
    }
}

/**
 * @brief Destroy the RBTree<keytype, valuetype>::RBTree object
 * 
 * @tparam keytype 
 * @tparam valuetype 
 */
template <class keytype, class valuetype>
RBTree<keytype, valuetype>::~RBTree() {
    deleteNodes(root); //iterate through and get rid of each nodes
    delete(root); //finally delete the root
}

/**
 * @brief assistant to copy and assignment operator
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param curr - 
 * @param oldNIL 
 * @return Node<keytype, valuetype>* 
 */
template <class keytype, class valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::copy(Node<keytype, valuetype>* curr, Node<keytype, valuetype>* oldNIL) {
    if (curr == oldNIL) return NIL;
    Node<keytype, valuetype>* newNode = new Node<keytype,valuetype>(curr->key, curr->value, curr->color);
    newNode->rightChild = NIL;
    newNode->leftChild = NIL;

    if (curr->leftChild != oldNIL) {
        newNode->leftChild = copy(curr->leftChild, oldNIL);
    }

    if (curr->rightChild != oldNIL) {
        newNode->rightChild = copy(curr->rightChild, oldNIL);
    }
    newNode->size = curr->size;

    return newNode;
}

// template <class keytype, class valuetype>
// RBTree<keytype, valuetype>::RBTree(const RBTree &other) {
//     root = copy(other.root);
// } 

// template <class keytype, class valuetype>
// RBTree& RBTree<keytype, valutype>::operator=(const RBTree& other) {
// 	root = copy(other.root);
// 	return *this;
// }

/**
 * @brief preorder() assistant
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param node - current node being viewed and then printed
 */
template <class keytype, class valuetype>
void RBTree<keytype, valuetype>::preorderSorter(Node<keytype, valuetype> *node) {
    if (node != this->NIL) { //if node is not a leaf, then find the next leaf
        cout << node->key << " "; //print node
        preorderSorter(node->leftChild);
        preorderSorter(node->rightChild);
    }
}

/**
 * @brief postorder() assistant
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param node - current node being viewed and then printed
 */
template <class keytype, class valuetype>
void RBTree<keytype, valuetype>::postorderSorter(Node<keytype, valuetype> *node) {
    if (node != NIL) {
        postorderSorter(node->leftChild);
        postorderSorter(node->rightChild);
        cout << node->key << " ";
    }
}

/**
 * @brief inorder() assistant
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param node - current node being viewed and then printed
 */
template <class keytype, class valuetype>
void RBTree<keytype, valuetype>::inorderSorter(Node<keytype, valuetype> *node) {
    if (node != NIL) { 
        inorderSorter(node->leftChild);
        cout << node->key << " ";
        inorderSorter(node->rightChild);
    }
}

/**
 * @brief searches the tree recursively until a node is found
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param node - the current node that key is being compared against
 * @param key - what is being asked to find
 * @return Node<keytype,valuetype> 
 */
template <class keytype, class valuetype>
Node<keytype,valuetype>* RBTree<keytype,valuetype>::searchingTheTree(Node<keytype, valuetype> *node, keytype key) {
    if (node == NIL || key == node->key) { //if tree is empty || search item is found
        return node;
    }

    if (key < node->key) { //if search item is less than current node key then iterate to left child
        return searchingTheTree(node->leftChild, key); 
    }
    return searchingTheTree(node->rightChild, key); //if search item is greater, then iterate to right child
}

/**
 * @brief search for node
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param k - key to search for
 * @return Node<keytype,valuetype>* 
 */
template <class keytype, class valuetype>
valuetype* RBTree<keytype,valuetype>::search(keytype k) {
    Node<keytype, valuetype> *node = searchingTheTree(this->root, k);
    if (node != this->NIL) {
        return &node->value;
    } else {
        return nullptr;
    }
}

/**
 * @brief completes a left rotation at x,
 *        the arrangement of the nodes on the right is transformed into the arrangement on the left node
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param x - the parent node of y, and the node to be rotated down
 */
template <class keytype, class valuetype>
void RBTree<keytype, valuetype>::leftRotation(Node<keytype,valuetype> *x) {
		Node<keytype,valuetype> *y = x->rightChild; //y is the right child of x
		x->rightChild = y->leftChild; //switch x's right child to become y's left child
		if (y->leftChild != this->NIL) { //if y has a left subtree
			y->leftChild->parent = x;    //assign x as the parent of the left subtree of y
		}
		y->parent = x->parent; //make x's parent into y's parent
		if (x->parent == this->NIL) { //if the parent of x is NULL, 
			this->root = y;           //make y as the root of the tree
		} 
        else if (x == x->parent->leftChild) { //if x is the left child of its parent, make y as the left child
			x->parent->leftChild = y;
		} 
        else { //else, assign y as the right child of its parent
			x->parent->rightChild = y;
		}
		y->leftChild = x; //make x as the left child of y
		x->parent = y; //assign y as the parent of x

        y->size = x->size; //TODO:
        x->size = x->leftChild->size + x->rightChild->size + 1; //TODO:
	}

/**
 * @brief completes a right rotation at x,
 *        the arrangement of the nodes on the left is transformed into the arrangement on the right node
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param x -the parent of node y, and the node to be rotated down
 */
template <class keytype, class valuetype>
void RBTree<keytype, valuetype>::rightRotation(Node<keytype,valuetype> *x) {
    Node<keytype,valuetype> *y = x->leftChild; //y is the left child of x
    x->leftChild = y->rightChild; //switch x's left child to become y's right child
    if (y->rightChild != this->NIL) { //if y has a right subtree
        y->rightChild->parent = x;    //assign x as the parent of the right subtree of y
    }
    y->parent = x->parent; //make x's parent into y's parent
    if (x->parent == this->NIL) { //if the parent of x is NULL,
        this->root = y;           //make y as the root of the tree
    } 
    else if (x == x->parent->rightChild) { //if x is the right child of its parent, make y as the right child
        x->parent->rightChild = y; 
    } 
    else { //else, assign y as the left child of its parent
        x->parent->leftChild = y;
    }
    y->rightChild = x; //assign x as the right child of y
    x->parent = y; //assign y as the parent of x

    
    y->size = x->size; //TODO:
    x->size = x->rightChild->size + x->leftChild->size + 1; //TODO:
}

/**
 * @brief balances the tree after an insert
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param k - initially the new node, but also the node to be fixed
 */
template <class keytype, class valuetype>
void RBTree<keytype, valuetype>::insertFix(Node<keytype, valuetype> *k) {
    if (k->parent == this->NIL) { //if k is the root, color k as black
        k->color = black;
        return;
    }

    if (k->parent->color == black) return; //if k's parent node is black, then the tree is balanced

    Node<keytype, valuetype>* parent = k->parent;
    Node<keytype, valuetype>* grandparent = parent->parent;

    Node<keytype, valuetype>* uncle;

    //setting up the parent/grandparent/uncle nodes
    if (grandparent == this->NIL) {
        uncle = this->NIL;
    } 
    else if (grandparent->leftChild == parent) {
        uncle = grandparent->rightChild;
    } 
    else {
        uncle = grandparent->leftChild;
    }

    //CASE 1: if uncle is red then change k's parent and uncle nodes to black, then grandparent to red
    if (uncle != this->NIL && uncle->color == red) {
        parent->color = uncle->color = black;
        grandparent->color = red;
        insertFix(grandparent);
        return;
    }
    
    //CASE 2: if uncle is black and k is the right child, then perform a left rotation
    if (k == parent->rightChild && parent == grandparent->leftChild) {
        leftRotation(parent);
        k = parent;
        parent = k->parent;
    } 
    else if (k == parent->leftChild && parent == grandparent->rightChild) { //CASE 3: if uncle is black and k is the left child, then perform a right rotation
        rightRotation(parent);
        k = parent;
        parent = k->parent;
    }

    //if the color of the left child of the grandparent of k is red, set the color of both children of grandparent as black and the grandparent as red
    parent->color = black;
    grandparent->color = red;

    //if k is the left child of the parent, then right rotate, else, left rotate
    //BC CASE 2 LEADS INTO CASE 3
    if (k == parent->leftChild) {
        rightRotation(grandparent);
    } 
    else {
        leftRotation(grandparent);
    }
}

/**
 * @brief insert a new node into the tree
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param k - key, used for ordering node
 * @param v - value attached to the key
 */
template <class keytype, class valuetype>
void RBTree<keytype, valuetype>::insert(keytype k, valuetype v) { 
    current_size++; //TODO:
    Node<keytype,valuetype> *newNode = new Node<keytype, valuetype>();
    newNode->key = k;
    newNode->value = v;
    newNode->parent = this->NIL;
    newNode->leftChild = this->NIL;
    newNode->rightChild = this->NIL;
    newNode->color = black;
    Node<keytype,valuetype> *prev = this->NIL;     //leaf of the tree
    Node<keytype,valuetype> *curr = this->root;    //root of the tree

    while (curr != this->NIL) { //while tree is not empty
        prev = curr;
        if (newNode->key < curr->key) { //if less than root, then curr becomes left child
            curr = curr->leftChild;
        }
        else {
            curr = curr->rightChild; //if greater than root, then curr becomes right child
        }
    }

    newNode->parent = prev; //assign parent of the leaf as a parent of newNode
    if (prev == this->NIL) { //if the tree is empty, then set root to newNode
        root = newNode;
    }
    else if (newNode->key < prev->key) { //if newNode key is less than leaf key, make newNode as left child
        prev->leftChild = newNode;
    }
    else { //opposite if greater
        prev->rightChild = newNode;
    }

    if (newNode->parent == this->NIL) { //if newNode is the root, color it black
        newNode->color = black;
        return;
    }

    // if (newNode->parent->parent == this->NIL) { 
    //     return;
    // }

    newNode->color = red; //color new node as red
    if (!(newNode->parent->parent == this->NIL)) insertFix(newNode); //fix to balance tree

    curr = newNode;
    while (curr != this->NIL) {
        curr->size = curr->leftChild->size + curr->rightChild->size+1;
        curr = curr->parent;
    }
}

/**
 * @brief an assistant to delete
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param x - initially the new node, but also the node to be balanced
 */
template <class keytype, class valuetype>
void RBTree<keytype, valuetype>::fixDelete(Node<keytype,valuetype> *x) {
    if (x == NIL) return;
    Node<keytype,valuetype> *s;
    while (x != root && x->color == black) { //do until x is not the root of the tree and the color of x is black
        if (x == x->parent->leftChild) { //if x is the left child of its parent then,
            s = x->parent->rightChild; //x's sibling
            if (s->color == red) { //CASE 1: x's sibling is red
                s->color = black; //set the color of s to black
                x->parent->color = red; //set the color of x's parent to red
                leftRotation(x->parent); //perform left rotation on x's parent
                s = x->parent->rightChild; //assign the right child of the parent of x to w
            }

            if (s->leftChild->color == black && s->rightChild->color == black) { // CASE 2: the color of both the right and left child of the sibling is black
                s->color = red; //set s to red
                x = x->parent; //assign the parent of x to x
            } 
            else {
                if (s->rightChild->color == black) { //CASE 3: if the color of the right child of x's sibling is black
                    s->leftChild->color = black; //set the color of the leftchild of s as black
                    s->color = red; //set the color of s to red
                    rightRotation(s); //perform right rotation
                    s = x->parent->rightChild; //assign the rightchild of the parent of x to w
                } 

                //CASE 4: if all else:
                s->color = x->parent->color; //set the color of w as the color of the parent of x
                x->parent->color = black; //set the color of the parent of x as black
                s->rightChild->color = black; //set the color of the right child of w as black
                leftRotation(x->parent); //perform a left rotation
                x = root; //set x as the root of the tree
            }
        } 
        else {
            s = x->parent->leftChild;
            if (s->color == red) { //CASE 1
                s->color = black;
                x->parent->color = red;
                rightRotation(x->parent);
                s = x->parent->leftChild;
            }

            if (s->rightChild->color == black && s->rightChild->color == black) { //CASE 2
                s->color = red;
                x = x->parent;
            } 
            else {
                if (s->leftChild->color == black) { //CASE 3
                    s->rightChild->color = black;
                    s->color = red;
                    leftRotation(s);
                    s = x->parent->leftChild;
                } 

                //CASE 4
                s->color = x->parent->color;
                x->parent->color = black;
                s->leftChild->color = black;
                rightRotation(x->parent);
                x = root;
            }
        } 
    }
    x->color = black; 
}

/**
 * @brief switches two nodes
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param u 1st node
 * @param v 2nd node
 */
template <class keytype, class valuetype>
void RBTree<keytype, valuetype>::rbTransplant(Node<keytype,valuetype> *u, Node<keytype,valuetype> *v){
    if (v != NIL) v->size = u->size;
    if (u->parent == this->NIL) {
        root = v;
    } 
    else if (u == u->parent->leftChild){
        u->parent->leftChild = v;
    } 
    else {
        u->parent->rightChild = v;
    }
    v->parent = u->parent;
}

/**
 * @brief assistant to remove()
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param node - node helper
 * @param key - key to be deleted
 * @return int 
 */
template <class keytype, class valuetype>
int RBTree<keytype, valuetype>::deleteNodeHelper(Node<keytype,valuetype> *node, keytype key) {
    // find the node containing key
    Node<keytype,valuetype> *z, *x, *y, *curr;
    z = searchingTheTree(root, key);
    if (z == NIL) return 0;

    y = z;

    int y_original_color = z->color;
    if (z->leftChild == this->NIL) {
        x = z->rightChild;
        if (x == NIL) {
            curr = z->parent;
        } else {
            curr = x;
        }
        rbTransplant(z, z->rightChild);
    } else if (z->rightChild == this->NIL) {
        x = z->leftChild;
        rbTransplant(z, z->leftChild);
        curr = x;
    } else {
        y = maximum(z->leftChild); // y = Node to replace
        y_original_color = y->color;
        x = y->leftChild;
        if (y->leftChild != this->NIL) {
            curr = y->leftChild;
        } else if (y->parent != z) {
            curr = y->parent;
        } else {
            curr = y;
        }
        if (y->parent == z) {
            x->parent = y;
        } else {
            rbTransplant(y, y->leftChild);
            y->leftChild = z->leftChild;
            y->leftChild->parent = y;
        }
        rbTransplant(z, y);
        if (root == z) {
            root = y;
        }
        y->rightChild = z->rightChild;
        y->rightChild->parent = y;
        y->color = z->color;
        y->size = z->size - 1;
    }
    delete z;
    if (y_original_color == black) fixDelete(x);
    while (curr != NIL) {
        curr->size = curr->leftChild->size + curr->rightChild->size + 1;
        curr = curr->parent;
    }
    return 1;
}

/**
 * @brief find minimum node on left subtree
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param curr - current node
 * @return Node<keytype, valuetype>* 
 */
template <class keytype, class valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::minimum(Node<keytype, valuetype> *curr) {
    while (curr->leftChild != NIL) {
        curr = curr->leftChild;
    }
    return curr;
}

/**
 * @brief find maximum node on right subtree
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param curr - current node
 * @return Node<keytype, valuetype>* 
 */
template <class keytype, class valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::maximum(Node<keytype, valuetype> *curr) {
    while (curr->rightChild != NIL) {
        curr = curr->rightChild;
    }
    return curr;
}

/**
 * @brief given a key, find the node with that key and return its rank
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param k - key given
 * @return int 
 */
template <class keytype, class valuetype>
int RBTree<keytype, valuetype>::rank(keytype k) {
    Node<keytype, valuetype> *z = searchingTheTree(this->root, k); //TODO: use other search function??
    if (z == this->NIL) { //TODO: NIL instead??
        return 0;
    }
    int r = z->leftChild->size + 1;
    Node<keytype, valuetype> *y = z;
    while (y != root) {
        if (y == y->parent->rightChild) {
            r = r + y->parent->leftChild->size + 1;
        }
        y = y->parent;
    }
    return r;
}

/**
 * @brief assistant to select()
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param x - node used to search for key
 * @param pos - position to find
 * @return Node<keytype, valuetype> 
 */
template <class keytype, class valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::selectSorter(Node<keytype, valuetype> *x, int pos) {
    int rank = x->leftChild->size; //TODO: +1????
    if (rank == pos) return x;
    if (pos < rank) {
        return selectSorter(x->leftChild, pos);
    }
    else {
        return selectSorter(x->rightChild, pos - rank - 1);
    }
}

/**
 * @brief given a position in the tree, find its key
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param pos - position given
 * @return keytype 
 */
template <class keytype, class valuetype>
keytype RBTree<keytype, valuetype>::select(int pos) {
    if (pos > root->size) return NIL->key;
    Node<keytype, valuetype> *x = selectSorter(root, pos - 1);
    return x->key;
}

/**
 * @brief assistant to sucessor()
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param x - node used to find successor
 * @return Node<keytype, valuetype>* 
 */
template <class keytype, class valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::findSuccessor(Node<keytype, valuetype> *x) {
    if (x->rightChild != NIL) {
        return minimum(x->rightChild);
    }

    Node<keytype, valuetype> *y = x->parent;
    while (y != NIL && x == y->rightChild) {
        x = y;
        y = y->parent;
    }

    return y;
}

/**
 * @brief finds successor for a given node k
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param k - key to find successor of
 * @return keytype* 
 */
template <class keytype, class valuetype>
keytype* RBTree<keytype, valuetype>::successor(keytype k) {
    Node<keytype, valuetype> *node = searchingTheTree(this->root, k);

    if (node == NIL) {
        return nullptr;
    }

    Node<keytype, valuetype> *successor = findSuccessor(node);
    return &successor->key;
}

/**
 * @brief assistant to predecessor()
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param x - node used to search for predecessor
 * @return Node<keytype, valuetype>* 
 */
template <class keytype, class valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::findPredecessor(Node<keytype, valuetype> *x) {
    if (x->leftChild != NIL) {
        return maximum(x->leftChild);
    }

    Node<keytype,valuetype> *y = x->parent;
    while (y != NIL && x==y->leftChild) {
        x = y;
        y = y->parent;
    }

    return y;
}

/**
 * @brief finds predecessor to a given node k
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param k - node to find predecessor of
 * @return keytype* 
 */
template <class keytype, class valuetype>
keytype* RBTree<keytype, valuetype>::predecessor(keytype k) {
    Node<keytype, valuetype> *node = searchingTheTree(this->root, k);

    if (node == NIL) {
        return nullptr;
    }

    Node<keytype, valuetype> *predecessor = findPredecessor(node);
    return &predecessor->key;
}

/**
 * @brief returns current size of tree
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @return int 
 */
template <class keytype, class valuetype>
int RBTree<keytype, valuetype>::size() {
    return root->size;
}

/**
 * @brief remove node
 * 
 * @tparam keytype 
 * @tparam valuetype 
 * @param k - node to be removed
 * @return int 
 */
template <class keytype, class valuetype>
int RBTree<keytype, valuetype>::remove(keytype k) {
    return deleteNodeHelper(root, k);
}

/**
 * @brief preorder sorting
 * 
 * @tparam keytype 
 * @tparam valuetype 
 */
template <class keytype, class valuetype>
void RBTree<keytype, valuetype>::preorder() {
    preorderSorter(this->root);
    std::cout << std::endl;
}

/**
 * @brief postorder sorting
 * 
 * @tparam keytype 
 * @tparam valuetype 
 */
template <class keytype, class valuetype>
void RBTree<keytype, valuetype>::postorder() {
    postorderSorter(this->root);
    std::cout << std::endl;
}

/**
 * @brief inorder sorting
 * 
 * @tparam keytype 
 * @tparam valuetype 
 */
template <class keytype, class valuetype>
void RBTree<keytype, valuetype>::inorder() {
    inorderSorter(this->root);
    std::cout << std::endl;
}
