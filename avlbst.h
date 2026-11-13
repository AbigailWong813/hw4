#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);
    void insertFix(AVLNode<Key, Value>* node);
    void removeFix(AVLNode<Key, Value>* node, int8_t diff);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node){
    AVLNode<Key, Value>* right = node->getRight();
    AVLNode<Key, Value>* parent = node->getParent();
    node -> setParent(right);
    //if right has a left child set as right child of node, otherwise new right child is nullptr
    if(right->getLeft()!=nullptr){
        node->setRight(right->getLeft());
        right->getLeft()->setParent(node);
    }else{
        node->setRight(nullptr);
    }
    //adjust right node pointers
    right->setLeft(node);
    right->setParent(parent);
    //adjust parent's pointers unless no parent (root)
    if(parent!=nullptr){
        if(parent->getLeft()==node){
        parent->setLeft(right);
        }else{
            parent->setRight(right);
        }
    }else{ //if node is root update root
        this->root_ = right;
    }
    
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node){
    AVLNode<Key, Value>* left = node->getLeft();
    AVLNode<Key, Value>* parent = node->getParent();
    node -> setParent(left);
    //if left has a right child set as left child of node, otherwise new left child is nullptr
    if(left->getRight()!=nullptr){
        node->setLeft(left->getRight());
        left->getRight()->setParent(node);
    }else{
        node->setLeft(nullptr);
    }
    //adjust left node pointers
    left->setRight(node);
    left->setParent(parent);
    //adjust parent's pointers unless no parent (root)
    if(parent!=nullptr){
        if(parent->getRight()==node){
        parent->setRight(left);
        }else{
            parent->setLeft(left);
        }
    }else{ //if node is root update root
        this->root_ = left;
    }

}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* node){
    AVLNode<Key, Value>* parent = node->getParent();
    if(parent == nullptr){return;}
    if (parent->getParent()==nullptr){return;}
    AVLNode<Key, Value>* grand = parent->getParent();

    if (grand->getLeft()==parent){
        grand->updateBalance(-1);
    }else{
        grand->updateBalance(1);
    }
    if (grand->getBalance()==0){return;}
    else if (abs(grand->getBalance())==1){insertFix(parent);}
    else {
        //it's the left child + zigzig
        if((grand->getBalance()<0 && parent->getBalance()<0)){
            rotateRight(grand);
            grand->setBalance(0);
            parent->setBalance(0);
        } 
        //it's the right child + zigzig      
        else if((grand->getBalance()>0 && parent->getBalance()>0)){
            rotateLeft(grand);
            grand->setBalance(0);
            parent->setBalance(0);
        }
        //it's the right child + zigzag
        else if (grand->getBalance()<0 && parent->getBalance()>0){
            //AVLNode* middle = node;
            int8_t oldBalance = node->getBalance();
            rotateLeft(parent);
            rotateRight(grand);
            node->setBalance(0);
            if(oldBalance == -1){
                //node->updateBalance(1);
                parent->setBalance(0);
                grand->setBalance(1);
            }else if (oldBalance == 0){
                parent->setBalance(0);
                grand->setBalance(0);
            }else{
                parent->setBalance(-1);
                grand->setBalance(0);
            }
        }else{
            int8_t oldBalance = node->getBalance();
            rotateRight(parent);
            rotateLeft(grand);
            node->setBalance(0);
            if(oldBalance == 1){
                //node->updateBalance(1);
                parent->setBalance(0);
                grand->setBalance(-1);
            }else if (oldBalance == 0){
                parent->setBalance(0);
                grand->setBalance(0);
            }else{
                parent->setBalance(1);
                grand->setBalance(0);
            }
        }
    }
    //}

}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int8_t diff){
    if (node==nullptr){return;}
    AVLNode<Key, Value>* parent = node->getParent();
    int8_t nextDiff = 0;
    if(parent!=nullptr){
        if(parent->getLeft()==node){
            nextDiff = 1;
        }else{
            nextDiff = -1;
        }
    }

    //case 1
    if(node->getBalance()+diff == -2){
        AVLNode<Key, Value>* child = node->getLeft();
        if(child->getBalance()==-1){
            rotateRight(node);
            node->setBalance(0);
            child->setBalance(0);
            removeFix(parent, nextDiff);
        }else if (child->getBalance()==0){
            rotateRight(node);
            node->setBalance(-1);
            child->setBalance(1);
        }else{
            AVLNode<Key, Value>* grand = child->getRight();
            int8_t b = grand->getBalance();
            rotateLeft(child);
            rotateRight(node);
            if(b==1){
                node->setBalance(0);
                child->setBalance(-1);
                grand->setBalance(0);
            }else if(b==0){
                node->setBalance(0);
                child->setBalance(0);
                grand->setBalance(0);
            }else{
                node->setBalance(1);
                child->setBalance(0);
                grand->setBalance(0);
            }
            removeFix(parent, nextDiff);
        }
    }
    //case 1.2 (mirror of above case) 
    else if(node->getBalance()+diff == 2){
        AVLNode<Key, Value>* child = node->getRight();
        if(child->getBalance()==1){
            rotateLeft(node);
            node->setBalance(0);
            child->setBalance(0);
            removeFix(parent, nextDiff);
        }else if (child->getBalance()==0){
            rotateLeft(node);
            node->setBalance(1);
            child->setBalance(-1);
        }else{
            AVLNode<Key, Value>* grand = child->getLeft();
            int8_t b = grand->getBalance();
            rotateRight(child);
            rotateLeft(node);
            if(b==-1){
                node->setBalance(0);
                child->setBalance(1);
                grand->setBalance(0);
            }else if(b==0){
                node->setBalance(0);
                child->setBalance(0);
                grand->setBalance(0);
            }else{
                node->setBalance(-1);
                child->setBalance(0);
                grand->setBalance(0);
            }
            removeFix(parent, nextDiff);
        }
    }
    //case 2
    else if (node->getBalance()+diff == -1){
        node->setBalance(-1);
        return;
    }
    //case 2.2
    else if(node->getBalance()+diff == 1){
        node->setBalance(1);
        return;
    }
    //case 3
    else if (node->getBalance()+diff == 0){
        node->setBalance(0);
        removeFix(parent, nextDiff);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(this->root_ == nullptr){
        AVLNode<Key, Value>* node = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        this->root_ = node;
        return;
    }
    typename BinarySearchTree<Key, Value>::iterator it = this->find(new_item.first);
    if (it != this->end()) {
        (*it).second = new_item.second;
        return;
    }

    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = nullptr;

    while (current != nullptr) {
        parent = current;
        if (new_item.first < current->getKey()) {
            current = current->getLeft();
        } else {
            current = current->getRight();
        }
    }

    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    
    if (new_item.first < parent->getKey()) {
        parent->setLeft(newNode);
        parent->updateBalance(-1); // Left child grew
    } else {
        parent->setRight(newNode);
        parent->updateBalance(1);  // Right child grew
    }

    if (parent->getBalance() != 0) {
        this->insertFix(newNode);
    }
    
    return;

}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    if(this->root_==nullptr){return;}
    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));

    if(temp == nullptr){
        return;
    }
     //check if node has two children
    if (temp->getRight()!=nullptr && temp->getLeft()!=nullptr){
        AVLNode<Key, Value> *pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(temp));
        nodeSwap(temp, pred);
    }

    //at this point temp can now either have 1 left child or 0 children
    AVLNode<Key, Value>* child;
    // Determine which child exists (if any)
    if (temp->getLeft() != nullptr) {
        child = temp->getLeft();
    } else {
        child = temp->getRight();
    }

    //update parent and child linking
    AVLNode<Key, Value>* parent = temp->getParent();
    if (child != nullptr) {
        child->setParent(parent);
    }
    int8_t diff = 0;
    if (parent == nullptr) {
        this->root_ = child; // Target was the root
    } else if (parent->getLeft() == temp) {
        parent->setLeft(child);
        diff = 1;
    } else {
        parent->setRight(child);
        diff = -1;
    }

    delete temp;
    removeFix(parent, diff);
    
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
