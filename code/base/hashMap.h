#pragma once

#include "hashTable.h"

namespace Base
{

template<class t_Pair, class t_AllocatorWrapper = SystemAllocatorWrapper, uint t_nReservedCells = 16, uint t_nFillFactor = 3 >
class HashMap : public t_AllocatorWrapper
{
public:
	typedef t_Pair Pair_t;
	typedef t_AllocatorWrapper AllocatorWrapper_t;

public:
	static_assert(t_nFillFactor >= 2 && t_nFillFactor <= 16 && (t_nReservedCells == 4 || t_nReservedCells == 8 || t_nReservedCells == 16 || t_nReservedCells == 32 || t_nReservedCells == 64 || t_nReservedCells == 128 || t_nReservedCells == 256), "HashMap invalid template params" );

	struct Cell
	{
		Hash hash;
		t_Pair* pair;

		INLINE Cell() : hash(0), pair(0){}

		INLINE boolean isUsed()const
		{
			return pair != 0;
		}

		INLINE void clear()
		{
			delete pair; 
			pair = 0;
		}

		template<class t_Key>
		INLINE boolean isMatchKey(const t_Key& k)const
		{
			return *pair == k;
		}

		INLINE void assignMove(Cell& other)
		{
			delete pair;
			hash = other.hash;
			pair = other.pair;
			other.pair = 0;
		}
	};

	Cell* cells;
	uint cellsCount;
	uint minimalCellsCount;
	uint usedsCount;
	Cell bank[t_nReservedCells];


	INLINE void internal_resize(uint s)
	{
		Assert(s == saturate(s));
		maximize(s, minimalCellsCount);
		if(s == this->cellsCount)
			return;

		Cell* old_cells = this->cells;
		uint old_cellsCount = this->cellsCount;

		if(s > t_nReservedCells)
		{
			this->cells = new(this->allocator()) Cell[s];
			this->cellsCount = s;
		}
		else
		{
			if(old_cells == bank)
				return;
			this->cells = bank;
			this->cellsCount = t_nReservedCells;
		}

		for(uint i=0; i < old_cellsCount; ++i)
			if(old_cells[i].isUsed())
				HashTableOperations::reinsert(this->cells, this->cellsCount, old_cells[i]);

		if(old_cells != bank)
			delete[] old_cells;
	}

public:
	INLINE HashMap()
	{
		this->cells = this->bank;
		this->cellsCount = t_nReservedCells;
		this->minimalCellsCount = t_nReservedCells;
		this->usedsCount = 0;
	}

	INLINE HashMap(typename t_AllocatorWrapper::Data_t& ad) : t_AllocatorWrapper(ad)
	{
		this->cells = this->bank;
		this->cellsCount = t_nReservedCells;
		this->minimalCellsCount = t_nReservedCells;
		this->usedsCount = 0;
	}

	INLINE ~HashMap()
	{
		if(this->cells != bank)
			delete[] this->cells;
	}

	class ConstIterator;

	class Iterator
	{
	public:
		INLINE Iterator() : cell(0), begin(0), end(0){}
		INLINE Iterator(Cell* c, Cell* b, Cell* e) : cell(c), begin(b), end(e) {}
		INLINE Iterator(const Iterator& it) : cell(it.cell), begin(it.begin), end(it.end) {}
		INLINE Iterator operator = (const Iterator& it) { cell = it.cell; begin = it.begin; end = it.end; return *this; }
		INLINE Iterator operator ++ () 
		{
			for(++cell; cell != end && !cell->isUsed(); ++cell);
			return *this;
		}
		INLINE Iterator operator -- ()
		{
			for(--cell; cell != begin && !cell->isUsed(); --cell);
			return *this;
		}
		INLINE Iterator next()const
		{
			Cell* c = cell;
			for(++ c; c != end && !c->isUsed(); ++c);
			return Iterator(c, begin, end);
		}
		INLINE Iterator prev()const
		{
			Cell* c = cell;
			for(-- c; c != begin && !c->isUsed(); --c);
			return Iterator(c, begin, end);
		}
		friend INLINE boolean operator != (const Iterator& a, const Iterator& b) { return a.cell != b.cell; }
		friend INLINE boolean operator == (const Iterator& a, const Iterator& b) { return a.cell == b.cell; }
		INLINE t_Pair* operator -> ()const { return cell->pair; }
		INLINE t_Pair& operator * ()const { return *cell->pair; }
	private:
		Cell* cell;
		Cell* begin;
		Cell* end;

		friend class ConstIterator;
		friend class HashMap;
	};


	class ConstIterator
	{
	public:
		INLINE ConstIterator() : cell(0), begin(0), end(0){}
		INLINE ConstIterator(const Cell* c, const Cell* b, const Cell* e) : cell(c), begin(b), end(e) {}
		INLINE ConstIterator(const ConstIterator& it) : cell(it.cell), begin(it.begin), end(it.end) {}
		INLINE ConstIterator(const Iterator& it) : cell(it.cell), begin(it.begin), end(it.end) {}
		INLINE ConstIterator operator = (const ConstIterator& it) { cell = it.cell; begin = it.begin; end = it.end; return *this; }
		INLINE ConstIterator operator = (const Iterator& it) { cell = it.cell; begin = it.begin; end = it.end; return *this; }
		INLINE ConstIterator operator ++ () 
		{
			for(++cell; cell != end && !cell->isUsed(); ++cell);
			return *this;
		}
		INLINE ConstIterator operator -- ()
		{
			for(--cell; cell != begin && !cell->isUsed(); --cell);
			return *this;
		}
		INLINE ConstIterator next()const
		{
			Cell* c = cell;
			for(++ c; c != end && !c->isUsed(); ++c);
			return ConstIterator(c, begin, end);
		}
		INLINE ConstIterator prev()const
		{
			Cell* c = cell;
			for(-- c; c != begin && !c->isUsed(); --c);
			return ConstIterator(c, begin, end);
		}
		friend INLINE boolean operator != (const ConstIterator& a, const ConstIterator& b) { return a.cell != b.cell; }
		friend INLINE boolean operator == (const ConstIterator& a, const ConstIterator& b) { return a.cell == b.cell; }
		INLINE t_Pair* operator -> ()const { return cell->pair; }
		INLINE t_Pair& operator * ()const { return *cell->pair; }
	private:
		const Cell* cell;
		const Cell* begin;
		const Cell* end;
		friend class HashMap;
	};

	INLINE Iterator begin() { return Iterator(this->cells - 1, this->cells - 1, this->cells + this->cellsCount ); }
	INLINE Iterator first(){ return Iterator(this->cells - 1, this->cells - 1, this->cells + this->cellsCount ).next(); }
	INLINE Iterator last() { return Iterator(this->cells + this->cellsCount, this->cells - 1, this->cells + this->cellsCount ).prev(); }
	INLINE Iterator end() { return Iterator(this->cells + this->cellsCount, this->cells - 1, this->cells + this->cellsCount ); }

	INLINE ConstIterator begin()const { return ConstIterator(this->cells - 1, this->cells - 1, this->cells + this->cellsCount ); }
	INLINE ConstIterator first()const{ return ConstIterator(this->cells - 1, this->cells - 1, this->cells + this->cellsCount ).next(); }
	INLINE ConstIterator last()const { return ConstIterator(this->cells + this->cellsCount, this->cells - 1, this->cells + this->cellsCount ).prev(); }
	INLINE ConstIterator end()const { return ConstIterator(this->cells + this->cellsCount, this->cells - 1, this->cells + this->cellsCount ); }

	INLINE uint size()
	{
		return this->usedsCount;
	}

	INLINE uint reserve()
	{
		return this->minimalCellsCount / t_nFillFactor;
	}

	INLINE void reserve(uint s)
	{
		s *= t_nFillFactor;
		maximize(s, t_nReservedCells);
		this->minimalCellsCount = saturate(s);
		uint news;
		if(this->minimalCellsCount >= this->cellsCount)
			news = this->minimalCellsCount;
		else
			news = saturate(this->usedsCount * t_nFillFactor);
		internal_resize(news);
	}

	INLINE void clear()
	{
		for(Cell* c = this->cells, *c_e = this->cells + this->cellsCount; c != c_e; ++c)
			if(c->isUsed())
				c->clear();

		if(this->cells != bank)
			delete[] this->cells;

		this->cells = this->bank;
		this->cellsCount = t_nReservedCells;
		this->minimalCellsCount = t_nReservedCells;
		this->usedsCount = 0;
	}

	template<class t_Key>
	INLINE Iterator find(const t_Key& key)
	{
		int i = HashTableOperations::find(this->cells, this->cellsCount, DefaultHashGen<const t_Key>()(key), key);
		if(i >= 0)
			return Iterator(this->cells + i, this->cells - 1, this->cells + this->cellsCount );
		else
			return end();
	}

	template<class t_Key>
	INLINE ConstIterator find(const t_Key& key)const
	{
		int i = HashTableOperations::find(this->cells, this->cellsCount, DefaultHashGen<const t_Key>()(key), key);
		if(i >= 0)
			return ConstIterator(this->cells + i, this->cells - 1, this->cells + this->cellsCount );
		else
			return end();
	}

	template<class t_Key>
	INLINE Iterator insert(boolean01& isInserted, const t_Key& key)
	{
		if((this->usedsCount + 1)*t_nFillFactor > this->cellsCount)
			internal_resize(this->cellsCount << 1);

		uint i = HashTableOperations::insert(isInserted, this->cells, this->cellsCount, DefaultHashGen<const t_Key>()(key), key);
		if(isInserted){
			Assert(this->cells[i].pair == 0);
			this->cells[i].pair = new(this->allocator()) t_Pair();
			*this->cells[i].pair = key;
			this->usedsCount ++;
		}
		return Iterator(this->cells + i, this->cells - 1, this->cells + this->cellsCount );
	}

	INLINE void erase(const Iterator& it)
	{
		uint i = it.cell - this->cells;
		this->cells[i].clear();
		HashTableOperations::eraseShift(this->cells, this->cellsCount, i);
		this->usedsCount --;
		if(this->usedsCount*t_nFillFactor*t_nFillFactor < this->cellsCount)
			internal_resize(this->cellsCount >> 1);
	}

	template<class t_Key>
	INLINE boolean01 erase(const t_Key& key)
	{
		int i = HashTableOperations::find(this->cells, this->cellsCount, DefaultHashGen<const t_Key>()(key), key);
		if(i >= 0){
			this->cells[i].clear();
			HashTableOperations::eraseShift(this->cells, this->cellsCount, i);
			this->usedsCount --;
			if(this->usedsCount*t_nFillFactor*t_nFillFactor < this->cellsCount)
				internal_resize(this->cellsCount >> 1);
			return true;
		}
		return false;
	}

	INLINE Iterator erase_retNext(Iterator it)
	{
		Iterator ret = it;
		++ret;
		erase(it);
		return ret;
	}
	INLINE Iterator erase_retPrev(Iterator it)
	{
		Iterator ret = it;
		--ret;
		erase(it);
		return ret;
	}

	template<class t_Key>
	INLINE Iterator operator () (const t_Key& key) { boolean outIsInserted; return insert(outIsInserted, key); }

private:
	//TODO implement
	HashMap(const HashMap& other){}
	void operator = (const HashMap& other){}
};

}


