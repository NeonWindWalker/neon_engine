#pragma once

#include "hashTable.h"

namespace Base
{

template<class t_Pair, class t_AllocatorWrapper = SystemAllocatorWrapper, uint t_nReservedCells = 16, uint t_nFillFactor = 4 >
class HashArray : public t_AllocatorWrapper
{
public:
	typedef t_Pair Pair_t;
	typedef t_AllocatorWrapper AllocatorWrapper_t;

protected:

	static_assert( t_nReservedCells >= t_nFillFactor && (t_nFillFactor == 2 || t_nFillFactor == 4 || t_nFillFactor == 8 || t_nFillFactor == 16) && (t_nReservedCells == 4 || t_nReservedCells == 8 || t_nReservedCells == 16 || t_nReservedCells == 32 || t_nReservedCells == 64 || t_nReservedCells == 128 || t_nReservedCells == 256), "HashArray invalid template params");

	struct Cell;

	struct PairEx
	{
		Pair_t pair;
		Cell* cell;

		PairEx() : cell(0) {}
		PairEx(const PairEx& other) : pair(other.pair), cell(other.cell) {}

		INLINE void assignMove(PairEx& other);
	};

	struct Cell
	{
		Hash hash;
		PairEx* element;

		INLINE Cell() : hash(0), element(0) {}

		INLINE boolean isUsed()const
		{
			return element != 0;
		}

		template<class t_Key>
		INLINE boolean isMatchKey(const t_Key& k)const
		{
			return element->pair == k;
		}

		INLINE void assignMove(Cell& other)
		{
			hash = other.hash;
			if(element)
				element->cell = 0;
			element = other.element;
			if(element)
				element->cell = this;
			other.element = 0;
		}
	};

	Cell* cells;
	uint cellsCount;
	uint minimalCellsCount;
	PairEx* pairs;
	uint pairsCount;

	Cell cellBank[t_nReservedCells];
	byte pairBank[(t_nReservedCells / t_nFillFactor) * sizeof(PairEx) + ALLOCATION_ALIGNMENT(sizeof(PairEx)) - DEFAULT_IMPLY_ALIGMENT];

	INLINE void internal_resize(uint s)
	{
		for(uint i=0; i < this->cellsCount; ++i)
			if(this->cells[i].element)
				Assert(this->cells[i].element->cell == &this->cells[i]);
		
		Assert(s == saturate(s));
		maximize(s, minimalCellsCount);
		if(s == this->cellsCount)
			return;

		Cell* old_cells = this->cells;
		uint old_cellsCount = this->cellsCount;
		PairEx* old_pairs = this->pairs;

		if(s > t_nReservedCells)
		{
			this->cells = (Cell*)this->allocator().alloc(sizeof(Cell) * s, ALLOCATION_ALIGNMENT(sizeof(Cell)), 0);
			for(uint i=0; i < s; ++i)
				new(this->cells + i) Cell();
			this->cellsCount = s;
			this->pairs = (PairEx*)this->allocator().alloc(sizeof(PairEx) * s, ALLOCATION_ALIGNMENT(sizeof(PairEx)), 0);
		}
		else
		{
			if(old_cells == this->cellBank)
				return;
			this->cells = this->cellBank;
			this->cellsCount = t_nReservedCells;
			this->pairs = alignMemoryAddresAndCast<PairEx>(this->pairBank);
		}
		Assert(this->cellsCount / t_nFillFactor >= this->pairsCount);

		for(uint i=0; i < old_cellsCount; ++i)
			if(old_cells[i].element)
				Assert(old_cells[i].element->cell == &old_cells[i]);
		
		//TODO Optimize
		for(uint i=0; i < old_cellsCount; ++i)
			if(old_cells[i].isUsed())
				HashTableOperations::reinsert(this->cells, this->cellsCount, old_cells[i]);

		//TODO Optimize
		for(uint i=0; i < this->pairsCount; ++i){
			PairEx* dest = &this->pairs[i];
			PairEx* src = &old_pairs[i];
			construct_destroy(dest, *src);
			dest->cell->element = dest;
		}

		if(old_cells != this->cellBank)
		{
			this->allocator().free(old_cells);
			this->allocator().free(old_pairs);
		}
		else
		{
			for(uint i=0; i < old_cellsCount; ++i)
			{
				old_cells[i].hash = 0;
				old_cells[i].element = 0;
			}
		}
		
		for(uint i=0; i < this->cellsCount; ++i)
			if(this->cells[i].element)
				Assert(this->cells[i].element->cell == &this->cells[i]);
	}

public:
	INLINE HashArray()
	{
		this->cells = this->cellBank;
		this->cellsCount = t_nReservedCells;
		this->minimalCellsCount = t_nReservedCells;
		this->pairs = alignMemoryAddresAndCast<PairEx>(this->pairBank);
		this->pairsCount = 0;
	}

	INLINE HashArray(typename t_AllocatorWrapper::Data_t& ad) : t_AllocatorWrapper(ad)
	{
		this->cells = this->cellBank;
		this->cellsCount = t_nReservedCells;
		this->minimalCellsCount = t_nReservedCells;
		this->pairs = alignMemoryAddresAndCast<PairEx>(this->pairBank);
		this->pairsCount = 0;
	}

	INLINE ~HashArray()
	{
		this->minimalCellsCount = t_nReservedCells;
		clear();
	}

	class ConstIterator;

	class Iterator
	{
	public:
		INLINE Iterator(){}
		INLINE Iterator(PairEx* c) : element(c) {}
		INLINE Iterator(const Iterator& it) : element(it.element) {}
		INLINE Iterator operator = (const Iterator& it) { element = it.element; return *this; }
		INLINE Iterator operator ++ () { ++element; return *this; }
		INLINE Iterator operator -- () { --element; return *this; }
		INLINE Iterator next()const { return Iterator(element + 1); }
		INLINE Iterator prev()const { return Iterator(element - 1); }
		friend INLINE boolean operator != (const Iterator& a, const Iterator& b) { return a.element != b.element; }
		friend INLINE boolean operator == (const Iterator& a, const Iterator& b) { return a.element == b.element; }
		INLINE t_Pair* operator -> ()const { return &element->pair; }
		INLINE t_Pair& operator * ()const { return element->pair; }
		friend INLINE Iterator operator + (const Iterator& a, int b) { return Iterator(a.element + b); }
		friend INLINE Iterator operator - (const Iterator& a, int b) { return Iterator(a.element - b); }
	private:
		PairEx* element;

		friend class ConstIterator;
		friend class HashArray;
	};

	class ConstIterator
	{
	public:
		INLINE ConstIterator(){}
		INLINE ConstIterator(const PairEx* c) : element(c) {}
		INLINE ConstIterator(const Iterator& it) : element(it.element) {}
		INLINE ConstIterator operator = (const ConstIterator& it) { element = it->element; return *this; }
		INLINE ConstIterator operator = (const Iterator& it) { element = it->element; return *this; }
		INLINE ConstIterator operator ++ () { ++element; return *this; }
		INLINE ConstIterator operator -- () { --element; return *this; }
		INLINE ConstIterator next()const { return ConstIterator(element + 1); }
		INLINE ConstIterator prev()const { return ConstIterator(element - 1); }
		friend INLINE boolean operator != (const ConstIterator& a, const ConstIterator& b) { return a.element != b.element; }
		friend INLINE boolean operator == (const ConstIterator& a, const ConstIterator& b) { return a.element == b.element; }
		INLINE const t_Pair* operator -> ()const { return &element->pair; }
		INLINE const t_Pair& operator * ()const { return element->pair; }
		friend INLINE ConstIterator operator + (const ConstIterator& a, int b) { return ConstIterator(a.element + b); }
		friend INLINE ConstIterator operator - (const ConstIterator& a, int b) { return ConstIterator(a.element - b); }
	private:
		const PairEx* element;

		friend class Iterator;
		friend class HashArray;
	};

	INLINE Iterator begin() { return Iterator(this->pairs - 1); }
	INLINE Iterator first(){ return Iterator(this->pairs); }
	INLINE Iterator last() { return Iterator(this->pairs + this->pairsCount - 1); }
	INLINE Iterator end() { return Iterator(this->pairs + this->pairsCount); }

	INLINE ConstIterator begin()const { return ConstIterator(this->pairs - 1); }
	INLINE ConstIterator first()const { return ConstIterator(this->pairs); }
	INLINE ConstIterator last()const { return ConstIterator(this->pairs + this->pairsCount - 1); }
	INLINE ConstIterator end()const { return ConstIterator(this->pairs + this->pairsCount); }

	INLINE uint size()
	{
		return this->pairsCount;
	}

	INLINE void clear()
	{
		for(PairEx* p = this->pairs, *p_e = this->pairs + this->pairsCount; p != p_e; ++p){
			p->cell->element = 0;
			p->cell->hash = 0;
			p->~PairEx();
		}
		this->pairsCount = 0;
		
		for(uint i=0; i < this->cellsCount; ++i)
			Assert(this->cells[i].element == 0);

		internal_resize(0);
	}

	template<class t_Key>
	INLINE Iterator find(const t_Key& key)
	{
		int i = HashTableOperations::find(this->cells, this->cellsCount, DefaultHashGen<const t_Key>()(key), key);
		if(i >= 0)
			return Iterator(this->cells[i].element);
		else
			return end();
	}

	template<class t_Key>
	INLINE ConstIterator find(const t_Key& key)const
	{
		int i = HashTableOperations::find(this->cells, this->cellsCount, DefaultHashGen<const t_Key>()(key), key);
		if(i >= 0)
			return ConstIterator(this->cells[i].element);
		else
			return end();
	}

	template<class t_Key>
	INLINE Iterator insert(boolean01& isInserted, const t_Key& key)
	{
		if(this->pairsCount + 1 > this->cellsCount / t_nFillFactor)
			internal_resize(this->cellsCount << 1);

		uint i = HashTableOperations::insert(isInserted, this->cells, this->cellsCount, DefaultHashGen<const t_Key>()(key), key);
		if(isInserted)
		{
			new(this->pairs + this->pairsCount) PairEx();
			this->cells[i].element = &this->pairs[this->pairsCount ++];
			this->cells[i].element->cell = &this->cells[i];
			this->cells[i].element->pair = key;
		}
		return Iterator(this->cells[i].element);
	}

	template<class t_Key>
	INLINE boolean01 erase(const t_Key& key)
	{
		int i = HashTableOperations::find(this->cells, this->cellsCount, DefaultHashGen<const t_Key>()(key), key);
		if(i >= 0)
		{
			if(this->cells[i].element != &this->pairs[this->pairsCount - 1])
				this->cells[i].element->assignMove(this->pairs[this->pairsCount - 1]);
			
			this->pairs[-- this->pairsCount].~PairEx();

			this->cells[i].element = 0;
			this->cells[i].hash = 0;
			
			HashTableOperations::eraseShift(this->cells, this->cellsCount, i);
			
			for(uint i=0; i < this->cellsCount; ++i)
				if(this->cells[i].element)
					Assert(this->cells[i].element->cell == &this->cells[i]);

			if(this->pairsCount*t_nFillFactor*t_nFillFactor < this->cellsCount)
				internal_resize(this->cellsCount >> 1);
			return true;
		}
		return false;
	}

	template<class t_Key>
	INLINE Iterator operator () (const t_Key& key) { boolean outIsInserted; return insert(outIsInserted, key); }

	INLINE t_Pair& operator [] (uint n) { return this->pairs[n].pair; }
	INLINE const t_Pair& operator [] (uint n)const { return this->pairs[n].pair; }
	INLINE t_Pair& at(uint n) { return this->pairs[n].pair; }
	INLINE const t_Pair& at(uint n)const { return this->pairs[n].pair; }

	INLINE uint indexOf(const ConstIterator& it)const { return it.element - this->pairs; }

	INLINE void eraseByIndex(uint n)
	{
		uint i = this->pairs[n].cell - this->cells;

		Assert(this->pairs[n].cell == &this->cells[i]);

		if(this->cells[i].element != &this->pairs[this->pairsCount - 1])
			this->cells[i].element->assignMove(this->pairs[this->pairsCount - 1]);

		this->pairs[-- this->pairsCount].~PairEx();

		this->cells[i].element = 0;
		this->cells[i].hash = 0;
		
		HashTableOperations::eraseShift(this->cells, this->cellsCount, i);
		
		for(uint i=0; i < this->cellsCount; ++i)
				if(this->cells[i].element)
					Assert(this->cells[i].element->cell == &this->cells[i]);

		if(this->pairsCount*t_nFillFactor*t_nFillFactor < this->cellsCount)
			internal_resize(this->cellsCount >> 1);
	}

	INLINE void erase(const Iterator& it)
	{
		eraseByIndex(indexOf(it));
	}

	INLINE Iterator erase_retNext(const Iterator& it)
	{
		uint ind = indexOf(it);
		eraseByIndex(ind);
		return first() + ind;
	}

	INLINE Iterator erase_retPrev(const Iterator& it)
	{
		uint ind = indexOf(it);
		eraseByIndex(ind);
		return first() + ind - 1;
	}

private:
	//TODO implement
	HashArray(const HashArray& other){}
	void operator = (const HashArray& other){}
};

template<class t_Pair, class t_AllocatorWrapper, uint t_nReservedCells, uint t_nFillFactor >
INLINE void HashArray<t_Pair, t_AllocatorWrapper, t_nReservedCells, t_nFillFactor>::PairEx::assignMove(typename HashArray<t_Pair, t_AllocatorWrapper, t_nReservedCells, t_nFillFactor >::PairEx& other)
{
	pair = other.pair;
	cell->element = 0;
	cell = other.cell;
	cell->element = this;
	other.cell = 0;
}

}
