#pragma once

#include "redBlackTree.h"
#include "cellAllocator.h"

namespace Base {


template<class t_Pair>
class RedBlackTreeMapNodeT
{
public:
	typedef RedBlackTreeNode<t_Pair> Type;
};

template<typename t_Pair, class t_AllocatorWrapper = SystemAllocatorWrapper, typename t_Comparator = DefaultComparator<const t_Pair, const t_Pair> >
class RedBlackTreeMap : private RedBlackTree<t_Pair, t_AllocatorWrapper, t_Comparator>
{
public:
	typedef RedBlackTreeNode<t_Pair> Node_t;
	typedef RedBlackTree<t_Pair, t_AllocatorWrapper, t_Comparator> Base_t;

	RedBlackTreeMap() {}
	RedBlackTreeMap(typename t_AllocatorWrapper::Data_t& ad) : Base_t(ad) {}

	INLINE typename t_AllocatorWrapper::Allocator_t& allocator() { return Base_t::allocator(); }
	 
	class Iterator
	{
	public:
		INLINE Iterator(){ this->_node = 0; }
		INLINE Iterator(Node_t * p) { this->_node = p; }
		INLINE Iterator operator ++ () { this->_node = this->_node->next(); return *this; }
		INLINE Iterator operator -- () { this->_node = this->_node->prev(); return *this; }
		INLINE Iterator next()const { return Iterator(this->_node->next()); }
		INLINE Iterator prev()const { return Iterator(this->_node->prev()); }
		friend INLINE boolean operator != (const Iterator& a, const Iterator& b) { return a._node != b._node; }
		friend INLINE boolean operator == (const Iterator& a, const Iterator& b) { return a._node == b._node; }
		INLINE t_Pair* operator -> ()const { return &this->_node->pair; }
		INLINE t_Pair& operator * ()const { return this->_node->pair; }
		INLINE Node_t* node() { return _node; }
	private:
		Node_t* _node;
	};

	class ConstIterator
	{
	public:
		INLINE ConstIterator(){ this->_node = 0; }
		INLINE ConstIterator(Node_t * p) { this->_node = p; }
		INLINE ConstIterator operator ++ () { this->_node = this->_node->next(); return *this; }
		INLINE ConstIterator operator -- () { this->_node = this->_node->prev(); return *this; }
		INLINE ConstIterator next()const { return ConstIterator(this->_node->next()); }
		INLINE ConstIterator prev()const { return ConstIterator(this->_node->prev()); }
		friend INLINE boolean operator != (const ConstIterator& a, const Iterator& b) { return a._node != b._node; }
		friend INLINE boolean operator == (const ConstIterator& a, const Iterator& b) { return a._node == b._node; }
		INLINE const t_Pair* operator -> ()const { return &this->_node->pair; }
		INLINE const t_Pair& operator * ()const { return this->_node->pair; }
		INLINE Node_t* node() { return _node; }
	private:
		Node_t* _node;
	};

	INLINE Iterator begin() { return Iterator(0); }
	INLINE Iterator first() { return Iterator(Base_t::leftest()); }
	INLINE Iterator last() { return Iterator(Base_t::rightest()); }
	INLINE Iterator end() { return Iterator(0); }

	template<class t_OtherKey, class t_OtherComparator>
	INLINE Iterator find(const t_OtherKey& key, t_OtherComparator cmp) { return Iterator(Base_t::find(key, cmp)); }
	template<class t_OtherKey>
	INLINE Iterator find(const t_OtherKey& key) { return Iterator(Base_t::find(key)); }
	template<class t_OtherKey, class t_OtherComparator>
	INLINE ConstIterator find(const t_OtherKey& key, t_OtherComparator cmp)const { return Iterator(Base_t::find(key, cmp)); }
	template<class t_OtherKey>
	INLINE ConstIterator find(const t_OtherKey& key)const { return Iterator(Base_t::find(key)); }
	template<class t_OtherKey, class t_OtherComparator>
	INLINE Iterator insert(boolean& outIsInserted, const t_OtherKey& key, t_OtherComparator cmp) { return Iterator(Base_t::insert(outIsInserted, key, cmp)); }
	template<class t_OtherKey>
	INLINE Iterator insert(boolean& outIsInserted, const t_OtherKey& key) { return Iterator(Base_t::insert(outIsInserted, key)); }
	template<class t_OtherKey, class t_OtherComparator>
	INLINE boolean erase(const t_OtherKey& key, t_OtherComparator cmp) { return Base_t::erase(key, cmp); }
	template<class t_OtherKey>
	INLINE boolean erase(const t_OtherKey& key) { return Base_t::erase(key); }
	INLINE void erase(Iterator it) { Assert(it.node()); Base_t::erase(*(it.node())); }
	INLINE Iterator erase_retNext(Iterator it)
	{
		Iterator ret = it;
		++ret;
		Base_t::erase(it);
		return ret;
	}
	INLINE Iterator erase_retPrev(Iterator it)
	{
		Iterator ret = it;
		--ret;
		Base_t::erase(it);
		return ret;
	}

	template<class t_OtherKey>
	INLINE Iterator operator () (const t_OtherKey& key) { boolean outIsInserted; return Base_t::insert(outIsInserted, key); }

	INLINE ConstIterator begin()const { return ConstIterator(0); }
	INLINE ConstIterator first()const { return ConstIterator(Base_t::leftest()); }
	INLINE ConstIterator last()const { return ConstIterator(Base_t::rightest()); }
	INLINE ConstIterator end()const { return ConstIterator(0); }
	
	INLINE boolean empty()const { return Base_t::empty(); }
	INLINE void clear() { Base_t::clear(); }

private:
	RedBlackTreeMap(const RedBlackTreeMap& other){}
	void operator = (const RedBlackTreeMap& other){}
};

/*
template<class t_Key>
class BiMapNodeT
{
public:
	typedef RedBlackTreeNode<t_Key> Type;
};


template<typename t_Key0, typename t_Key1, typename t_Compare0 = DefaultComparator<t_Key0, t_Key0>, typename t_Compare1 = DefaultComparator<t_Key1, t_Key1>, class t_Allocator0 = SystemAllocator, class t_Allocator1 = SystemAllocator>
class BiMap
{
public:
	typedef RedBlackTreeNode<t_Key0, void*> Node0;
	typedef RedBlackTreeNode<t_Key1, void*> Node1;

	BiMap(t_Allocator0& a0 = getGlobalAllocator(), t_Allocator1& a1 = getGlobalAllocator()) : _tree0(a0), _tree1(a1) {}
	~BiMap(){ clear(); }
	class Iterator0
	{
	public:
		INLINE Iterator0(){ this->_node = 0; }
		INLINE Iterator0(Node0 * p) { this->_node = p; }
		INLINE Iterator0 operator ++ () { this->_node = this->_node->next(); return *this; }
		INLINE Iterator0 operator -- () { this->_node = this->_node->prev(); return *this; }
		friend INLINE boolean operator != (const Iterator0& a, const Iterator0& b) { return a._node != b._node; }
		friend INLINE boolean operator == (const Iterator0& a, const Iterator0& b) { return a._node == b._node; }
		INLINE const t_Key0& key0 () const { return *this->_node->key; }
		INLINE const t_Key1& key1 () const { return *reinterpret_cast<Node1*>(this->_node->val)->key; }
		INLINE const t_Key0& key () const { return *this->_node->key; }
		INLINE const t_Key1& val () const { return *reinterpret_cast<Node1*>(this->_node->val)->key; }
		INLINE const Node0& node()const { return *_node; }
	protected:
		Node0* _node;
		friend class Iterator;
	};

	class Iterator1
	{
	public:
		INLINE Iterator1(){ this->_node = 0; }
		INLINE Iterator1(Node1 * p) { this->_node = p; }
		INLINE Iterator1 operator ++ () { this->_node = this->_node->next(); return *this; }
		INLINE Iterator1 operator -- () { this->_node = this->_node->prev(); return *this; }
		friend INLINE boolean operator != (const Iterator1& a, const Iterator1& b) { return a._node != b._node; }
		friend INLINE boolean operator == (const Iterator1& a, const Iterator1& b) { return a._node == b._node; }
		INLINE const t_Key1& key1 () const { return *this->_node->key; }
		INLINE const t_Key0& key0 () const { return *reinterpret_cast<Node0*>(this->_node->val)->key; }
		INLINE const t_Key1& key () const { return *this->_node->key; }
		INLINE const t_Key0& val () const { return *reinterpret_cast<Node0*>(this->_node->val)->key; }
		INLINE const Node1& node()const { return *_node; }
	protected:
		Node1* _node;
		friend class Iterator;

	};

	class Iterator
	{
	public:
		INLINE Iterator(){ this->_node0 = 0; this->_node1 = 0; }
		INLINE Iterator(Node0 * p0, Node1 * p1) { this->_node0 = p0; this->_node1 = p1; }
		INLINE Iterator(Iterator0 i0) : _node0(i0._node), _node1(i0._node ? reinterpret_cast<Node1*>(i0._node->val) : 0) {}
		INLINE Iterator(Iterator1 i1) : _node0(i1._node ? reinterpret_cast<Node0*>(i1._node->val) : 0), _node1(i1._node) {}
		INLINE Iterator next0()const { Node0* n0 = this->_node0->next(); return Iterator(n0, reinterpret_cast<Node1*>(n0->val)); }
		INLINE Iterator prev0()const { Node0* n0 = this->_node0->prev(); return Iterator(n0, reinterpret_cast<Node1*>(n0->val)); }
		INLINE Iterator next1()const { Node1* n1 = this->_node1->next(); return Iterator(reinterpret_cast<Node0*>(n1->val), n1); }
		INLINE Iterator prev1()const { Node1* n1 = this->_node1->prev(); return Iterator(reinterpret_cast<Node0*>(n1->val), n1); }
		friend INLINE boolean operator != (const Iterator& a, const Iterator& b) { return a._node0 != b._node0; }
		friend INLINE boolean operator == (const Iterator& a, const Iterator& b) { return a._node0 == b._node0; }
		friend INLINE boolean operator != (const Iterator0& a, const Iterator& b) { return a._node != b._node0; }
		friend INLINE boolean operator != (const Iterator& a, const Iterator0& b) { return a._node0 != b._node; }
		friend INLINE boolean operator == (const Iterator0& a, const Iterator& b) { return a._node == b._node0; }
		friend INLINE boolean operator == (const Iterator& a, const Iterator0& b) { return a._node0 == b._node; }
		friend INLINE boolean operator != (const Iterator1& a, const Iterator& b) { return a._node != b._node1; }
		friend INLINE boolean operator != (const Iterator& a, const Iterator1& b) { return a._node1 != b._node; }
		friend INLINE boolean operator == (const Iterator1& a, const Iterator& b) { return a._node == b._node1; }
		friend INLINE boolean operator == (const Iterator& a, const Iterator1& b) { return a._node1 == b._node; }
		INLINE const t_Key0& key0 () const { return this->_node0->key; }
		INLINE const t_Key1& key1 () const { return this->_node1->key; }
		INLINE Iterator0 iterator0 () const { return Iterator0(_node0); }
		INLINE Iterator1 iterator1 () const { return Iterator1(_node1); }
		INLINE const Node0* node0()const { return _node0; }
		INLINE const Node1* node1()const { return _node1; }
	private:
		Node0* _node0;
		Node1* _node1;
	};

	INLINE Iterator begin() { return Iterator(0,0); }
	INLINE Iterator first() { return Iterator(this->_tree0.leftest(), this->_tree1.leftest()); }
	INLINE Iterator last() { return Iterator(this->_tree0.rightest(), this->_tree1.rightest()); }
	INLINE Iterator end() { return Iterator(0,0); }
	INLINE Iterator find0(const t_Key0& key)
	{ 
		Node0* n0 = this->_tree0.find(key);
		if(!n0)
			return Iterator(0,0);
		return Iterator(n0, reinterpret_cast<Node1*>(n0->val));
	}
	INLINE Iterator find1(const t_Key1& key)
	{ 
		Node1* n1 = this->_tree1.find(key);
		if(!n1)
			return Iterator(0,0);
		return Iterator(reinterpret_cast<Node0*>(n1->val), n1);
	}
	INLINE Iterator insert(boolean& outIsInserted, const t_Key0& key0, const t_Key1& key1)
	{ 
		boolean outIsInserted0,outIsInserted1;
		Node0* n0 = this->_tree0.insert(outIsInserted0, key0);
		Node1* n1 = this->_tree1.insert(outIsInserted1, key1);
		if(n0 == 0 || n1 == 0){
			if(n0 && outIsInserted0)
				this->_tree0.erase(*n0);
			if(n1 && outIsInserted1)
				this->_tree1.erase(*n1);
			outIsInserted = false;
			return Iterator();
		}
		assert(outIsInserted0 == outIsInserted1);
		outIsInserted = outIsInserted0;
		n0->val = n1;
		n1->val = n0;
		return Iterator(n0, n1);
	}
	INLINE void erase(Iterator i)
	{ 
		if(i.node0())
			return;
		this->_tree0.erase(*(i.node0()));
		this->_tree1.erase(*(i.node1()));
	}
	INLINE void erase0(const t_Key0& key)
	{ 
		Node0* n0 = this->_tree0.find(key);
		if(!n0)return;
		this->_tree1.erase(*reinterpret_cast<Node1*>(n0->val));
		this->_tree0.erase(*n0);
	}
	INLINE void erase1(const t_Key1& key)
	{ 
		Node1* n1 = this->_tree1.find(key);
		if(!n1)return;
		this->_tree0.erase(*reinterpret_cast<Node0*>(n1->val));
		this->_tree1.erase(*n1);
	}

	INLINE void clear()
	{
		this->_tree0.clear();
		this->_tree1.clear();
	}

	INLINE boolean empty()const { return _tree0.empty(); }

protected:
	RedBlackTree<t_Key0, void*, t_Compare0, t_Allocator0> _tree0;
	RedBlackTree<t_Key1, void*, t_Compare1, t_Allocator1> _tree1;
};
*/

/*
template<typename t_Pair, typename const t_Comparator& = DefaultComparator<t_Pair, t_Pair>, class t_AllocatorWrapper = SystemAllocatorWrapper>
class DMap : public RedBlackTreeMap<t_Pair, t_Comparator, AllocatorWrapper< TCellDefragmentAllocator< RedBlackTreeMapNodeT<t_Pair>, t_AllocatorWrapper > > >
{
public:
	INLINE DMap(){}
	INLINE DMap(typename t_AllocatorWrapper::Data_t& ad) : RedBlackTreeMap<t_Key, t_Value, t_Comparator, TCellDefragmentAllocator<RedBlackTreeNode<t_Key, t_Value>, t_AllocatorWrapper> >(ad) {}
	INLINE ~DMap(){ this->clear(); }
	void defragment() { allocator().defragment(this->forNodesMoveInMemory()); }
};

*/

}
