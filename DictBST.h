/**
* @file DictBST.h - Binary Search Tree Implementation of DictBST ADT
* @author Ben Gruher
* @see "Seattle University, CPSC2430, Spring 2018"
*/

#pragma once
#include "adt/Dictionary.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>

/**
* @class DictBST - collection of KeyType objects and ValueType data
*
* An element is either in the set or not, solely as determined
* by the equality operator (operator==). There is no
* concept in this class of multiple equivalent elements.
*
* Implementations of this ADT are also expected to support
* a const_iterator and methods begin() and end(). Order of
* iteration can be arbitrary.
*
* @tparam KeyType  elements of the set
*                  must support equality operator
* @tparam ValueType cannot be Node*
*/
template <typename KeyType, typename ValueType>
class DictBST : public Dictionary<KeyType, ValueType> {
public:
	DictBST() : root(nullptr) {}
	~DictBST() {
		Node::freeNodes(root);
	}
	DictBST(const DictBST<KeyType, ValueType>& other) : DictBST<KeyType, ValueType>() {
		*this = other;
	}
	DictBST(DictBST<KeyType, ValueType>&& temp) : DictBST<KeyType, ValueType>() {
		*this = temp;
	}
	DictBST<KeyType, ValueType>& operator=(const DictBST<KeyType, ValueType>& other) {
		if (this != &other) {
			Node::freeNodes(root);
			root = other.root->copy();
		}
		return *this;
	}
	DictBST<KeyType, ValueType>& operator=(DictBST<KeyType, ValueType>&& temp) {
		std::swap(root, temp.root);
		return *this;
	}

	/**
	* Determine if the given key is currently in this DictBST
	*
	* @param key  possible element of this DictBST
	* @return     true if key is an element, false otherwise
	*/
	bool has(const KeyType& key) const {
		Node* p = root;
		while (p != nullptr) {
			if (p->data == key)
				return true;
			if (key < p->data)
				p = p->left;
			else
				p = p->right;
		}
		return false;
	}

	/**
	* Add the given key into this DictBST
	*
	* May or may not already be an element of this DictBST.
	* @param key  an element of this DictBST
	* @post       has(key) is true
	*/
	
	void add(const KeyType& key, const ValueType& value) {
		Node* p = root;
		if (p == nullptr)
			root = new Node(key, value);
		else
			while (!(p->data == key)) {
				if (key < p->data) {
					if (p->left == nullptr)
						p->left = new Node(key, value, p);
					p = p->left;
				}
				else {
					if (p->right == nullptr)
						p->right = new Node(key, value, p);
					p = p->right;
				}
			}
	}

	/**
	* Remove the given key from this DictBST
	*
	* It is irrelevant how many times this key has been added to the DictBST
	* or even if it is currently an element or not.
	* @param key  an element (possibly) of this DictBST
	* @post       has(key) is false
	*/
	void remove(const KeyType& key) {
		Node* remove;
		Node** pRemove;
		pRemove = &this->root;
		while (*pRemove != nullptr && (*pRemove)->data != key) {
			if (key < (*pRemove)->data)
				pRemove = &(*pRemove)->left;
			else
				pRemove = &(*pRemove)->right;
			remove = *pRemove;
		}
		if (remove == nullptr)
			return;
		if (!(remove->isLeaf())) {
			KeyType replacement = (remove->left != nullptr) ? remove->left->maxKey()->data : remove->right->minKey()->data;
			DictBST<KeyType, ValueType>::remove(replacement);
			remove->data = replacement;
		}
		else {
			delete remove;
			*pRemove = nullptr;
		}
	}

	ValueType& get(const KeyType& key) {
		Node* p = root;
		if (this->has(key)) {
			while (p != nullptr) {
				if (p->data == key)
					return p->value;
				if (key < p->data)
					p = p->left;
				else
					p = p->right;
			}
		}
		else {
			while (p != nullptr) {
				if (key < p->data) {
					if (p->hasLeft())
						p = p->left;
					else {
						p->left = new Node(key, p);
						return p->right->value;
					}
				}
				else {
					if (p->hasRight())
						p = p->right;
					else {
						p->right = new Node(key, p);
						return p->right->value;
					}
				}
			}
		}
		return p->value;
	}

	const ValueType& get(const KeyType& key) const {
		Node* p = root;
		if (this->has(key)) {
			while (p != nullptr) {
				if (p->data == key)
					return p->value;
				if (key < p->data)
					p = p->left;
				else
					p = p->right;
			}
		}
		else {
			throw std::invalid_argument("Not in Dictionary");
		}
		return p->value;
	}

	class const_iterator {
	public:
		const_iterator(typename DictBST<KeyType, ValueType>::Node* p) : current(p) {}
		const KeyType& operator*() const {
			return current->data;
		}
		const const_iterator& operator++() {
			if (current->hasRight())
				current = current->right->minKey();
			else if (current->back == nullptr)
				current = nullptr;
			else {
				while (current->back != nullptr && current == current->back->right)
					current = current->back;
				current = current->back;
			}
			return *this;
		}
		bool operator!=(const const_iterator& rhs) const {
			if (rhs.current == this->current)
				return false;
			return true;
		}
	private:
		typename DictBST<KeyType, ValueType>::Node * current;
		friend class DictBST<KeyType, ValueType>;
	};
	const_iterator begin() {
		Node *p = root;
		while (p->hasLeft()) {
			p = p->left;
		}
		const_iterator it(p);
		return it;
	}
	const_iterator begin(KeyType k) {
		Node* p = root;
		while (p != nullptr) {
			if (p->data == k) {
				const_iterator it(p);
				return p;
			}
			if (k < p->data)
				p = p->left;
			else
				p = p->right;
		}
		const_iterator it(p);
		return it;
	}
	const_iterator end() {
		const_iterator it(nullptr);
		return it;
	}

private:
	struct Node {
		KeyType data;
		ValueType value;
		Node* left;
		Node* right;
		Node* back;
		Node() : left(nullptr), right(nullptr), back(nullptr) {}
		Node(KeyType d) : data(d), left(nullptr), right(nullptr), back(nullptr) {}
		Node(KeyType d, ValueType v) : data(d), value(v), left(nullptr), right(nullptr), back(nullptr) {}
		Node(KeyType d, Node* l, Node* r) : data(d), left(l), right(r), back(nullptr) {}
		Node(KeyType d, Node* b) : data(d), left(nullptr), right(nullptr), back(b) {}
		Node(KeyType d, ValueType v, Node* b) : data(d), value(v), left(nullptr), right(nullptr), back(b) {}

		bool isLeaf() const {
			if (left == nullptr && right == nullptr)
				return true;
			else
				return false;
		}
		bool hasLeft() const {
			if (left == nullptr)
				return false;
			else
				return true;
		}
		bool hasRight() const {
			if (right == nullptr)
				return false;
			else
				return true;
		}
		Node* maxKey() {
			Node* p = this;
			while (p->right != nullptr)
				p = p->right;
			return p;
		}
		Node* minKey() {
			Node* p = this;
			while (p->left != nullptr)
				p = p->left;
			return p;
		}
		static void freeNodes(Node* &node) {
			if (!node->isLeaf()) {
				if (node->hasLeft())
					freeNodes(node->left);
				if (node->hasRight())
					freeNodes(node->right);
			}
			node = nullptr;
		}
		static std::ostream& printR(std::ostream& out, std::string prefix, const Node* node) {
			if (node != nullptr) {
				if (node->hasLeft()) {
					printR(out, " " + prefix + "0", node->left);
				}
				out << prefix << ": " << node->data << std::endl;
				if (node->hasRight()) {
					printR(out, " " + prefix + "1", node->right);
				}
			}
			return out;
		}
		friend std::ostream& operator<<(std::ostream& out, const Node* node) {
			printR(out, "", node);
			return out;
		}
		Node* copy() const {
			Node* nP = new Node(data, nullptr);
			if (left != nullptr) {
				nP->left = left->copy();
				nP->left->back = nP;
			}
			if (right != nullptr) {
				nP->right = right->copy();
				nP->right->back = nP;
			}
			return nP;
		}
	};
	Node * root;
};

