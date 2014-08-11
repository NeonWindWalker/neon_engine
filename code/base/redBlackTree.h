#pragma once

#include "seed.h"

namespace Base
{

enum ERedBlackTreeColor { ERedBlackTreeColor_RED, ERedBlackTreeColor_BLACK };

template<class t_Node>
class RedBlackTreeFields
{
public:
	t_Node* _root;
	t_Node* _leftest;
	t_Node* _rightest;
	RedBlackTreeFields() : _root(0), _leftest(0), _rightest(0) {}
}; 

template<typename t_Pair>
class RedBlackTreeNode
{
public:
	typedef t_Pair Pair_t;

	RedBlackTreeFields<RedBlackTreeNode>& redBlackTreeFields;
	RedBlackTreeNode* left;
	RedBlackTreeNode* right;
	RedBlackTreeNode* parent;
	ERedBlackTreeColor color;
	t_Pair pair;

	INLINE RedBlackTreeNode(RedBlackTreeFields<RedBlackTreeNode>& fld) : redBlackTreeFields(fld), left(0), right(0), parent(0), color(ERedBlackTreeColor_RED)
	{}

	INLINE ~RedBlackTreeNode()
	{
#ifdef DEBUG
		left = (RedBlackTreeNode*)1;
		right = (RedBlackTreeNode*)2;
		parent = (RedBlackTreeNode*)3;
#endif
	}

	INLINE RedBlackTreeNode(RedBlackTreeNode& other, MoveInMemoryConstructorExtension flag) : redBlackTreeFields(other.redBlackTreeFields), left(other.left),  right(other.right), parent(other.parent), color(other.color)
	{
		move(pair, other.pair);

		RedBlackTreeFields<RedBlackTreeNode>* tree = &redBlackTreeFields;

		if(this->left)
			this->left->parent = this;
		if(this->right)
			this->right->parent = this;
		if(this->parent){
			if(this->parent->left == &other)
				this->parent->left = this;
			else{
				Assert(this->parent->right == &other);
				this->parent->right = this;
			}
		}

		if(this->parent == 0){
			Assert(tree->_root == &other);
			tree->_root = this;
		}
		if(tree->_leftest == &other)
			tree->_leftest = this;
		if(tree->_rightest == &other)
			tree->_rightest = this;
	}

	INLINE RedBlackTreeNode* next()
	{
		if(this->right){
			RedBlackTreeNode* n = this->right;
			while(n->left)
				n = n->left;
			return n;
		}

		RedBlackTreeNode* n = this;
		while(n->parent){
			if(n->parent->left == n)
				return n->parent;
			n = n->parent;
		}
		return 0;
	}

	INLINE RedBlackTreeNode* prev()
	{
		if(this->left){
			RedBlackTreeNode* n = this->left;
			while(n->right)
				n = n->right;
			return n;
		}
		RedBlackTreeNode* n = this;
		while(n->parent){
			if(n->parent->right == n)
				return n->parent;
			n = n->parent;
		}
		return 0;
	}

private:
	RedBlackTreeNode(){}
	void operator = (const RedBlackTreeNode& other){}
};

template<typename t_Value>
static INLINE void construct_destroy(RedBlackTreeNode<t_Value>* newObj, RedBlackTreeNode<t_Value>& oldObj) 
{
	new(newObj) RedBlackTreeNode<t_Value>(oldObj, MoveInMemoryConstructorExtension());
	oldObj.~RedBlackTreeNode<t_Value>();
}

template<typename t_Pair, class t_AllocatorWrapper = SystemAllocatorWrapper, typename t_Comparator = DefaultComparator<const t_Pair, const t_Pair> >
class RedBlackTree : protected RedBlackTreeFields< RedBlackTreeNode<t_Pair> >, public t_AllocatorWrapper
{
public:
	typedef t_Pair Pair_t;
	typedef const t_Comparator& Compare_t;
	typedef t_AllocatorWrapper AllocatorWrapper_t;
	typedef RedBlackTreeNode<t_Pair> Node_t;
public:
	RedBlackTree() {}
	RedBlackTree(typename t_AllocatorWrapper::Data_t& ad) : t_AllocatorWrapper(ad) {}
	~RedBlackTree() { clear(); }
	
	template<class t_OtherKey, class t_OtherComparator>
	INLINE Node_t* find(const t_OtherKey& key, t_OtherComparator cmp)
	{
		Node_t* n = this->_root;
		while (n != NULL) {
			int comp_result = cmp(n->pair, key);
			if (comp_result == 0) {
				return n;
			} if (comp_result > 0) {
				n = n->left;
			} else {
				n = n->right;
			}
		}
		return n;
	}

	template<class t_OtherKey>
	INLINE Node_t* find(const t_OtherKey& key)
	{
		return find(key, t_Comparator());
	}

	template<class t_OtherKey, class t_OtherComparator>
	INLINE Node_t* insert(boolean& outIsInserted, const t_OtherKey& key, t_OtherComparator cmp)
	{
		Node_t* inserted_node = 0;
		if (this->_root == NULL) {
			inserted_node = new(this->allocator()) Node_t(*this);
			inserted_node->pair = key;
			this->_root = this->_leftest = this->_rightest = inserted_node;
		} else {
			Node_t* n = this->_root;
			while (1) {
				int comp_result = cmp(n->pair, key);
				if (comp_result == 0) {
					outIsInserted = false;
					return n;
				} else if (comp_result > 0) {
					if (n->left == NULL) {
						inserted_node = new(this->allocator()) Node_t(*this);
						inserted_node->pair = key;
						n->left = inserted_node;
						if(this->_leftest == n)
							this->_leftest = inserted_node;
						break;
					} else {
						n = n->left;
					}
				} else {
					Assert (comp_result < 0);
					if (n->right == NULL) {
						inserted_node = new(this->allocator()) Node_t(*this);
						inserted_node->pair = key;
						n->right = inserted_node;
						if(this->_rightest == n)
							this->_rightest = inserted_node;
						break;
					} else {
						n = n->right;
					}
				}
			}
			inserted_node->parent = n;
		}
		insert_case1(inserted_node);
		//verify_properties();
		outIsInserted = true;
		return inserted_node;
	}

	template<class t_OtherKey>
	INLINE Node_t* insert(boolean& outIsInserted, const t_OtherKey& key)
	{
		return insert(outIsInserted, key, t_Comparator());
	}

	INLINE void erase(Node_t& node)
	{
		Node_t* child;
		Node_t* n = &node;

		if(n == this->_leftest){
			if(this->_leftest->right){
				this->_leftest = this->_leftest->right;
				while(this->_leftest->left)
					this->_leftest = this->_leftest->left;
			}
			else if(this->_leftest->parent)
				this->_leftest = this->_leftest->parent;
			else
				this->_leftest = 0;
		}

		if(n == this->_rightest){
			if(this->_rightest->left){
				this->_rightest = this->_rightest->left;
				while(this->_rightest->right)
					this->_rightest = this->_rightest->right;
			}
			else if(this->_rightest->parent)
				this->_rightest = this->_rightest->parent;
			else
				this->_rightest = 0;
		}


		if (n->left != NULL && n->right != NULL) {
			Node_t* pred = maximum_node(n->left);

			boolean nLeft = n->parent ? n->parent->left == n : false;
			boolean predLeft = pred->parent ? pred->parent->left == pred : false;

			swap(pred->parent, n->parent);
			swap(pred->left, n->left);
			swap(pred->right, n->right);
			swap(pred->color, n->color);

			if(pred->parent == pred)
				pred->parent = n;
			if(pred->left == pred)
				pred->left = n;
			if(pred->right == pred)
				pred->right = n;

			if(n->parent == n)
				n->parent = pred;
			if(n->left == n)
				n->left = pred;
			if(n->right == n)
				n->right = pred;

			if(pred->left)
				pred->left->parent = pred;
			if(pred->right)
				pred->right->parent = pred;
			if(pred->parent){
				if(nLeft)
					pred->parent->left = pred;
				else
					pred->parent->right = pred;
			}

			if(n->left)
				n->left->parent = n;
			if(n->right)
				n->right->parent = n;
			if(n->parent){
				if(predLeft)
					n->parent->left = n;
				else
					n->parent->right = n;
			}

			if(n == this->_root)
				this->_root = pred;

			swap(pred->pair, n->pair);
			verify_properties();
			swap(pred->pair, n->pair);

		
			//n->key = pred->key;
			//n->val = pred->val;
			//n = pred;
		}

		Assert(n->left == NULL || n->right == NULL);
		child = n->right == NULL ? n->left  : n->right;
		if (node_color(n) == ERedBlackTreeColor_BLACK) {
			n->color = node_color(child);
			delete_case1(n);
		}
		replace_node(n, child);
		if (n->parent == NULL && child != NULL)
			child->color = ERedBlackTreeColor_BLACK;

		delete n;
		verify_properties();
	}

	template<class t_OtherKey, class t_OtherComparator>
	INLINE boolean erase(const t_OtherKey& key, t_OtherComparator cmp) {
		Node_t* n = find(key, cmp);
		if (n == 0) return false;
		erase(*n);
		return true;
	}

	template<class t_OtherKey>
	INLINE boolean erase(const t_OtherKey& key) {
		Node_t* n = find(key);
		if (n == 0) return false;
		erase(*n);
		return true;
	}

	INLINE void clear()
	{
		if(!this->_root)
			return;
		if(this->_root->left)
			clear_req(*this->_root->left);
		if(this->_root->right)
			clear_req(*this->_root->right);

		delete this->_root;
		this->_root = this->_leftest = this->_rightest = 0;
	}
	INLINE boolean empty()const { return this->_root == 0; }

	INLINE Node_t* root() { return this->_root; }
	INLINE Node_t* leftest() { return this->_leftest; }
	INLINE Node_t* rightest() { return this->_rightest; }
	
protected:

	void clear_req(Node_t& node)
	{
		if(node.left)
			clear_req(*node.left);
		if(node.right)
			clear_req(*node.right);

		delete &node;
	}

	INLINE Node_t* grandparent(Node_t* n) {
		Assert (n != NULL);
		Assert (n->parent != NULL); /* Not the root Node_t* */
		Assert (n->parent->parent != NULL); /* Not child of root */
		return n->parent->parent;
	}

	INLINE Node_t* sibling(Node_t* n) {
		Assert (n != NULL);
		Assert (n->parent != NULL); /* Root Node_t* has no sibling */
		if (n == n->parent->left)
			return n->parent->right;
		else
			return n->parent->left;
	}

	INLINE Node_t* uncle(Node_t* n) {
		Assert (n != NULL);
		Assert (n->parent != NULL); /* Root Node_t* has no uncle */
		Assert (n->parent->parent != NULL); /* Children of root have no uncle */
		return sibling(n->parent);
	}

	INLINE void verify_properties() {
		return;
		verify_property_1(this->_root);
		verify_property_2(this->_root);
		/* Property 3 is implicit */
		verify_property_4(this->_root);
		verify_property_5(this->_root);
	}

	void verify_property_1(Node_t* n) {
		Assert(node_color(n) == ERedBlackTreeColor_RED || node_color(n) == ERedBlackTreeColor_BLACK);
		if (n == NULL) return;
		verify_property_1(n->left);
		verify_property_1(n->right);
	}

	void verify_property_2(Node_t* r) {
		Assert(node_color(r) == ERedBlackTreeColor_BLACK);
	}

	INLINE ERedBlackTreeColor node_color(Node_t* n) {
		return n == NULL ? ERedBlackTreeColor_BLACK : n->color;
	}

	void verify_property_4(Node_t* n) {
		if (node_color(n) == ERedBlackTreeColor_RED) {
			Assert (node_color(n->left)   == ERedBlackTreeColor_BLACK);
			Assert (node_color(n->right)  == ERedBlackTreeColor_BLACK);
			Assert (node_color(n->parent) == ERedBlackTreeColor_BLACK);
		}
		if (n == NULL) return;
		verify_property_4(n->left);
		verify_property_4(n->right);
	}

	void verify_property_5(Node_t* r) {
		int black_count_path = -1;
		verify_property_5_helper(r, 0, &black_count_path);
	}

	void verify_property_5_helper(Node_t* n, int black_count, int* path_black_count) {
		if (node_color(n) == ERedBlackTreeColor_BLACK) {
			black_count++;
		}
		if (n == NULL) {
			if (*path_black_count == -1) {
				*path_black_count = black_count;
			} else {
				Assert (black_count == *path_black_count);
			}
			return;
		}
		verify_property_5_helper(n->left,  black_count, path_black_count);
		verify_property_5_helper(n->right, black_count, path_black_count);
	}

	INLINE void rotate_left(Node_t* n) {
		Node_t* r = n->right;
		replace_node(n, r);
		n->right = r->left;
		if (r->left != NULL) {
			r->left->parent = n;
		}
		r->left = n;
		n->parent = r;
	}

	INLINE void rotate_right(Node_t* n) {
		Node_t* L = n->left;
		replace_node(n, L);
		n->left = L->right;
		if (L->right != NULL) {
			L->right->parent = n;
		}
		L->right = n;
		n->parent = L;
	}

	INLINE void replace_node(Node_t* oldn, Node_t* newn) {
		if (oldn->parent == NULL) {
			this->_root = newn;
		} else {
			if (oldn == oldn->parent->left)
				oldn->parent->left = newn;
			else
				oldn->parent->right = newn;
		}
		if (newn != NULL) {
			newn->parent = oldn->parent;
		}
	}

	inline void insert_case1(Node_t* n) {
		if (n->parent == NULL)
			n->color = ERedBlackTreeColor_BLACK;
		else
			insert_case2(n);
	}

	INLINE void insert_case2(Node_t* n) {
		if (node_color(n->parent) == ERedBlackTreeColor_BLACK)
			return; /* Tree is still valid */
		else
			insert_case3(n);
	}

	INLINE void insert_case3(Node_t* n) {
		if (node_color(uncle(n)) == ERedBlackTreeColor_RED) {
			n->parent->color = ERedBlackTreeColor_BLACK;
			uncle(n)->color = ERedBlackTreeColor_BLACK;
			grandparent(n)->color = ERedBlackTreeColor_RED;
			insert_case1(grandparent(n));
		} else {
			insert_case4(n);
		}
	}

	INLINE void insert_case4(Node_t* n) {
		if (n == n->parent->right && n->parent == grandparent(n)->left) {
			rotate_left(n->parent);
			n = n->left;
		} else if (n == n->parent->left && n->parent == grandparent(n)->right) {
			rotate_right(n->parent);
			n = n->right;
		}
		insert_case5(n);
	}

	INLINE void insert_case5(Node_t* n) {
		n->parent->color = ERedBlackTreeColor_BLACK;
		grandparent(n)->color = ERedBlackTreeColor_RED;
		if (n == n->parent->left && n->parent == grandparent(n)->left) {
			rotate_right(grandparent(n));
		} else {
			Assert (n == n->parent->right && n->parent == grandparent(n)->right);
			rotate_left(grandparent(n));
		}
	}



	INLINE Node_t* maximum_node(Node_t* n) {
		Assert (n != NULL);
		while (n->right != NULL) {
			n = n->right;
		}
		return n;
	}

	INLINE void delete_case1(Node_t* n) {
		if (n->parent == NULL)
			return;
		else
			delete_case2(n);
	}

	INLINE void delete_case2(Node_t* n) {
		if (node_color(sibling(n)) == ERedBlackTreeColor_RED) {
			n->parent->color = ERedBlackTreeColor_RED;
			sibling(n)->color = ERedBlackTreeColor_BLACK;
			if (n == n->parent->left)
				rotate_left(n->parent);
			else
				rotate_right(n->parent);
		}
		delete_case3(n);
	}

	inline void delete_case3(Node_t* n) {
		if (node_color(n->parent) == ERedBlackTreeColor_BLACK &&
			node_color(sibling(n)) == ERedBlackTreeColor_BLACK &&
			node_color(sibling(n)->left) == ERedBlackTreeColor_BLACK &&
			node_color(sibling(n)->right) == ERedBlackTreeColor_BLACK)
		{
			sibling(n)->color = ERedBlackTreeColor_RED;
			delete_case1(n->parent);
		}
		else
			delete_case4(n);
	}

	INLINE void delete_case4(Node_t* n) {
		if (node_color(n->parent) == ERedBlackTreeColor_RED &&
			node_color(sibling(n)) == ERedBlackTreeColor_BLACK &&
			node_color(sibling(n)->left) == ERedBlackTreeColor_BLACK &&
			node_color(sibling(n)->right) == ERedBlackTreeColor_BLACK)
		{
			sibling(n)->color = ERedBlackTreeColor_RED;
			n->parent->color = ERedBlackTreeColor_BLACK;
		}
		else
			delete_case5(n);
	}

	INLINE void delete_case5(Node_t* n) {
		if (n == n->parent->left &&
			node_color(sibling(n)) == ERedBlackTreeColor_BLACK &&
			node_color(sibling(n)->left) == ERedBlackTreeColor_RED &&
			node_color(sibling(n)->right) == ERedBlackTreeColor_BLACK)
		{
			sibling(n)->color = ERedBlackTreeColor_RED;
			sibling(n)->left->color = ERedBlackTreeColor_BLACK;
			rotate_right(sibling(n));
		}
		else if (n == n->parent->right &&
			node_color(sibling(n)) == ERedBlackTreeColor_BLACK &&
			node_color(sibling(n)->right) == ERedBlackTreeColor_RED &&
			node_color(sibling(n)->left) == ERedBlackTreeColor_BLACK)
		{
			sibling(n)->color = ERedBlackTreeColor_RED;
			sibling(n)->right->color = ERedBlackTreeColor_BLACK;
			rotate_left(sibling(n));
		}
		delete_case6(n);
	}

	INLINE void delete_case6(Node_t* n) {
		sibling(n)->color = node_color(n->parent);
		n->parent->color = ERedBlackTreeColor_BLACK;
		if (n == n->parent->left) {
			Assert (node_color(sibling(n)->right) == ERedBlackTreeColor_RED);
			sibling(n)->right->color = ERedBlackTreeColor_BLACK;
			rotate_left(n->parent);
		}
		else
		{
			Assert (node_color(sibling(n)->left) == ERedBlackTreeColor_RED);
			sibling(n)->left->color = ERedBlackTreeColor_BLACK;
			rotate_right(n->parent);
		}
	}

private:
	RedBlackTree(const RedBlackTree& other) {}
	void operator = (const RedBlackTree& other) {}
};

}
