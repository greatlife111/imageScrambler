#include "chain.h"
#include <cmath>
#include <iostream>
#include <limits>


// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain() {
  clear();
}

/**
 * Inserts a new node after the node pointed to by p in the
 * chain (so p->next is the new node) and returns a pointer to
 * the newly created node.
 * If p is NULL, inserts a new head node to the chain.
 * This function **SHOULD** create a new Node and increase length_.
 *
 * @param p = The new node should be pointed to by p->next.
 *            If p is NULL, the new node becomes the head of the chain.
 * @param ndata = The data to be inserted.
 */
Chain::Node * Chain::insertAfter(Node * p, const Block &ndata) {
  Node * newNode = new Node(ndata);
  
  if (p == NULL) {
    if (empty()) {
      head_ = newNode;
      newNode->prev = NULL;
      newNode->next = NULL;
    } else {
      head_->prev = newNode;
      newNode->next = head_;
      head_=newNode;
      newNode->prev = NULL;
    }
  } else {
    if (p->next == NULL) {
      p->next = newNode;
      newNode->prev = p;
      newNode->next = NULL;
    } else {
      newNode->next = p->next;
      p->next->prev = newNode;
    
      p->next = newNode;
      newNode->prev = p;
    }
  }
  length_ ++;
  return newNode;

}

/**
 * Swaps the position in the chain of the two nodes pointed to
 * by p and q.
 * If p or q is NULL or p==q, do nothing.
 * Change the chain's head pointer if necessary.
 */
void Chain::swap(Node *p, Node *q) {
  if (p == NULL || q == NULL || p == q) {
    return;
  }

  if (p->next == q || q->next == p){
    Node* first = NULL;
    Node* second = NULL;

    if (p->next == q) {
      first = p;
      second = q;
    } 
    else {
      first = q;
      second = p;
    }

    Node* first_prev = first->prev;
    Node* second_next = second->next;

    second->prev = first_prev;
    first->next = second_next;
    second->next = first;
    first->prev = second;

    // if first is head, make second head
    if (first != head_) {
      first_prev->next = second;
    }
    else {
      head_ = second;
    }

    // if second is tail, do nothing
    if (second_next != NULL) {
      second_next->prev = first;
    }
  }
  else {
    Node* qPrev = q->prev;
    Node* qNext = q->next;
    Node* pPrev = p->prev;
    Node* pNext = p->next;

    p->next = qNext;
    p->prev = qPrev;

    q->next = pNext;
    q->prev = pPrev;

    // if q is tail, do nothing
    if (qNext != NULL) {
      qNext->prev = p;
    }
    // if q is head, make p head
    if (q != head_) {
      qPrev->next = p;
    }
    else {
      head_ = p;
    }

    // if p is tail, do nothing
    if (pNext != NULL) {
      pNext->prev = q;
    }
    // if p is head, make q head
    if (p != head_) {
      pPrev->next = q;
    } else {
      head_ = q;
    }
  }
  return;
}
  
  


/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class.
 */
void Chain::clear() {
  Node * curr = head_;

  while (curr != NULL) {
    Node * temp = curr;
    head_ = head_->next;
    curr = curr->next;
    delete temp;
    temp = NULL;
    length_--;
  }
  delete head_;
  head_ = NULL;
}
/**
 * Makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const &other) {
if (other.head_ == NULL) return;

Node head = *other.head_;
Node *othercurr = other.head_;
head_ = new Node(head.data);
Node * curr = head_;
length_ = 1;

othercurr = othercurr->next;
while (othercurr != NULL) {
  Block data = othercurr->data;
  insertAfter(curr, data);
  curr = curr->next;
  othercurr = othercurr->next;
}
return;
}



/* Modifies the current chain:
 * 1) Find the node with the first (leftmost) block in the unscrambled
 *    image and move it to the head of the chain.
 *	This block is the one whose closest match (to the left) is the
 *	largest.  That is, the distance (using distanceTo) to this block
 *	is big for all other blocks.
 *	For each block B, find the distanceTo B from every other block
 *	and take the minimum of these distances as B's "value".
 *	Choose the block B with the maximum value over all blocks and
 *	swap its node to the head of the chain.
 *
 * 2) Starting with the (just found) first block B, find the node with
 *    the block that is the closest match to follow B (to the right)
 *    among the remaining blocks, move (swap) it to follow B's node,
 *    then repeat to unscramble the chain/image.
 */
void Chain::unscramble() {
  if (size() <= 1) return;

  Node *curr = head_;
  double max_distance = curr->next->data.distanceTo(curr->data);
  Node *first = curr;

  while(curr != NULL) {
    double min_distance = numeric_limits<double>::max();
    Node *otherNode = head_;

    while(otherNode != NULL) {
      if (otherNode != curr) {
        min_distance = min(otherNode->data.distanceTo(curr->data), min_distance);
      } 
      otherNode = otherNode->next;
    }

    if (min_distance > max_distance) {
      max_distance = min_distance;
      first = curr;
    }
    curr = curr->next;
  }
  swap(head_, first);

  Node * determineBlock = head_;
  while(determineBlock->next->next != NULL) {
  curr = determineBlock->next;
  double min_distance = determineBlock->data.distanceTo(curr->data);
  Node * minNode = curr; // node after first node

  while (curr != NULL) {
    double distance = determineBlock->data.distanceTo(curr->data);
    if (distance < min_distance) {
      min_distance = distance;
      minNode = curr;
    }
    curr = curr->next;
  }
  swap(determineBlock->next, minNode);
  determineBlock = determineBlock->next;
  }
  return;
}
