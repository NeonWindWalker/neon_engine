#pragma once

#include "scalar.h"
#include "comparator.h"

#define foreach(it, cont) for(auto it = (cont).first(); it != (cont).end(); ++it)
#define rforeach(it, cont) for(auto it = (cont).last(); it != (cont).begin(); --it)
#define iforeach(n, cont) for(uint n = 0; n != (cont).size(); ++n)
#define irforeach(n, cont) for(int n = (cont).size() - 1; n != -1; --n)


namespace Base
{

template <typename t_Val>
static INLINE void swap(t_Val& a, t_Val& b)
{
	t_Val c(a);
	a = b;
	b = c;
}

template <typename t_Val>
static INLINE void move(t_Val& newObj, t_Val& oldObj)
{
	newObj = oldObj;
}

template <typename t_Val>
static INLINE void construct_destroy(t_Val* newmem, t_Val& objToMoveAndDestroy)
{
	new (newmem) t_Val(objToMoveAndDestroy);
	objToMoveAndDestroy.~t_Val();
}

template <class t_Element>
static INLINE t_Element * findNullElementPtr(t_Element* str){ while(*str)++str; return str; }

template<typename t_Element>
INLINE void reverse(t_Element* first, t_Element* last)
{
	for(t_Element *b = first, *e = last; b < e; ++b, --e)
		swap(*b,*e);
}

template<typename t_Element, typename t_ValElement, typename t_EqualComparator>
static INLINE t_Element* find(t_Element* first, t_Element* end, const t_ValElement& val, t_EqualComparator cmp)
{
	for(t_Element* i = first; i != end; ++i)
		if(cmp(*i, val) == 0)
			return i;
	return end;
}

template<typename t_Element, typename t_ValElement>
static INLINE t_Element* find(t_Element* first, t_Element* end, const t_ValElement& val)
{
	for(t_Element* i = first; i != end; ++i)
		if(*i == val)
			return i;
	return end;
}

template<typename t_Element, typename t_ValElement, typename t_EqualComparator>
static INLINE t_Element* rfind(t_Element* last, t_Element* begin, const t_ValElement& val, t_EqualComparator cmp)
{
	for(t_Element* i = last; i != begin; --i)
		if(cmp(*i, val) == 0)
			return i;
	return begin;
}

template<typename t_Element, typename t_ValElement>
static INLINE t_Element* rfind(t_Element* last, t_Element* begin, t_ValElement& val)
{
	for(t_Element* i = last; i != begin; --i)
		if(*i == val)
			return i;
	return begin;
}

template<typename t_Element, typename t_ValElement, typename t_EqualComparator>
static INLINE t_Element* find(t_Element* first, t_Element* end, const t_ValElement* valfirst, const t_ValElement* valend, t_EqualComparator cmp)
{
	const t_ValElement* v = valfirst;
	for(t_Element* i = first; i != end; ++i)
	{
		if(cmp(*i,*v)){
			if(end - i <= valend - valfirst)
				return end;
			v = valfirst;
			continue;
		}

		++ v;
		if(v == valend)
			return i - (valend - valfirst - 1);
	}
	return end;
}

template<typename t_Element, typename t_ValElement>
static INLINE t_Element* find(t_Element* first, t_Element* end, const t_ValElement* valfirst, const t_ValElement* valend)
{
	DefaultComparator<const t_Element, const t_Element> cmp;
	return find(first, end, valfirst, valend, cmp);
}

template<typename t_Element, typename t_ValElement, typename t_EqualComparator>
static INLINE t_Element* rfind(t_Element* last, t_Element* begin, const t_ValElement* vallast, const t_ValElement* valbegin, t_EqualComparator cmp)
{
	t_Element* v = vallast;
	for(t_Element* i = last; i != begin; --i)
	{
		if(cmp(*i,*v)){
			if(i - begin <= vallast - valbegin)
				return begin;
			v = vallast;
			continue;
		}

		-- v;
		if(v == valbegin)
			return i;
	}
	return begin;
}

template<typename t_Element, typename t_ValElement>
static INLINE t_Element* rfind(t_Element* last, t_Element* begin, const t_ValElement* vallast, const t_ValElement* valbegin)
{
	DefaultComparator<const t_Element, const t_Element> cmp;
	return rfind(last, begin, vallast, valbegin, cmp);
}

template<typename t_Element, typename t_ValElement, class t_Comparator>
static INLINE int findLesserOrEqualInSortedArray(const t_Element* arr, uint size, const t_ValElement& val, const t_Comparator& cmp)
{
	uint low = 0;
	uint high = size;
	while(low != high)
	{
		uint mid = low + (high - low) / 2;
		if(cmp(arr[mid], val) <= 0)
			low = mid + 1;
		else
			high = mid;
	}
	return (int)high - 1;
}

template<typename t_Element, typename t_ValElement>
static INLINE int findLesserOrEqualInSortedArray(const t_Element* arr, uint size, const t_ValElement& val)
{
	uint low = 0;
	uint high = size;
	while(low != high)
	{
		uint mid = low + (high - low) / 2;
		if(arr[mid] <= val)
			low = mid + 1;
		else
			high = mid;	
	}
	return (int)high - 1;
}

template<typename t_Element, typename t_ValElement, class t_Comparator>
static INLINE uint findGreaterOrEqualInSortedArray(const t_Element* arr, uint size, const t_ValElement& val, const t_Comparator& cmp)
{
	uint low = 0;
	uint high = size;
	while(low != high)
	{
		uint mid = low + (high - low) / 2;
		if(cmp(arr[mid], val) < 0)
			low = mid + 1;
		else
			high = mid;
	}
	return low;
}

template<typename t_Element, typename t_ValElement>
static INLINE uint findGreaterOrEqualInSortedArray(const t_Element* arr, uint size, const t_ValElement& val)
{
	uint low = 0;
	uint high = size;
	while(low != high)
	{
		uint mid = low + (high - low) / 2;
		if(arr[mid] < val)
			low = mid + 1;
		else
			high = mid;
	}
	return low;
}

template<typename t_Element, typename t_ValElement, class t_Comparator>
static INLINE uint findInsertPositionSortedArray(boolean& bIns, const t_Element* arr, uint size, t_ValElement& val, const t_Comparator& cmp)
{
	uint low = findGreaterOrEqualInSortedArray(arr, size, val, cmp);
	bIns = low == size || cmp(arr[low], val) != 0;
	return low;
}

template<typename t_Element, typename t_ValElement>
static INLINE uint findInsertPositionSortedArray(boolean& bIns, const t_Element* arr, uint size, const t_ValElement& val)
{
	uint low = findGreaterOrEqualInSortedArray(arr, size, val);
	bIns = low == size || arr[low] != val;
	return low;
}

template<typename t_Element, typename t_ValElement, class t_Comparator>
static INLINE uint findInSortedArray(const t_Element* arr, uint size, const t_ValElement& val, const t_Comparator& cmp)
{
	uint low = findGreaterOrEqualInSortedArray(arr, size, val, cmp);
	if (low < size && cmp(arr[low], val) == 0)
		return low;
	else
		return size;
}

template<typename t_Element, typename t_ValElement>
static INLINE uint findInSortedArray(const t_Element* arr, uint size, const t_ValElement& val)
{
	uint low = findGreaterOrEqualInSortedArray(arr, size, val);
	if (low < size && arr[low] == val)
		return low;
	else
		return size;
}

template<typename t_Element, typename t_ValElement, class t_Comparator>
static INLINE t_Element* findEqualRangeInSortedArray(t_Element*& outEnd, t_Element* arr, uint size, const t_ValElement& val)
{
	t_Element* end = arr + size;
	t_Element* fnd = arr + findInSortedArray(arr, size, val);
	if(fnd == end){
		outEnd = end;
		return end;
	}
	t_Element* outFirst = fnd;
	while(outFirst - 1 >= arr && *(outFirst - 1) == val)
		--outFirst;

	outEnd = fnd;
	while(outEnd != end && *outEnd == val)
		++outEnd;

	//verify
	for(t_Element* p = outFirst; p != outEnd; ++p)
		Assert(cmp(*(const t_Element*)p, val) == 0);

	return outFirst;
}

template<typename t_Element, typename t_ValElement, class t_Comparator>
static INLINE t_Element* findEqualRangeInSortedArray(t_Element*& outEnd, t_Element* arr, uint size, const t_ValElement& val, const t_Comparator& cmp)
{
	t_Element* end = arr + size;
	t_Element* fnd = arr + findInSortedArray(arr, size, val, cmp);
	if(fnd == end){
		outEnd = end;
		return end;
	}
	t_Element* outFirst = fnd;
	while(outFirst - 1 >= arr && cmp(*(const t_Element*)(outFirst - 1), val) == 0)
		--outFirst;

	outEnd = fnd;
	while(outEnd != end && cmp(*(const t_Element*)outEnd, val) == 0)
		++outEnd;

	//verify
	for(t_Element* p = outFirst; p != outEnd; ++p)
		Assert(cmp(*(const t_Element*)p, val) == 0);

	return outFirst;
}


template<class t_Element, class t_Comparator>
static INLINE void isort(t_Element* a, uint size, const t_Comparator& cmp)
{
	for (int p = 1; p < (int)size; p++){
		t_Element tmp = a[p];
		int j;
		for (j = p; j > 0 && cmp(tmp, a[j - 1]) < 0; j--)
			a[j] = a[j - 1];
		a[j] = tmp;
	}
}

template<class t_Element>
static INLINE void isort(t_Element* a, uint size)
{
	for (int p = 1; p < (int)size; p++){
		t_Element tmp = a[p];
		int j;
		for (j = p; j > 0 && tmp < a[j - 1]; j--)
			a[j] = a[j - 1];
		a[j] = tmp;
	}
}

template<class t_Element, class t_Comparator>
static void eqsort(t_Element* first, t_Element* end, const t_Comparator& cmp)
{
	uint s = end - first;

	/*
	if(s <= 6)
	{
		if(s <= 3)
		{
			if(s == 2)
			{
				if(cmp(first[0], first[1]) > 0)
					swap(first[0], first[1]);
				return;
			}
			else if(s == 3)
			{
				if(cmp(first[0], first[1]) > 0)
					swap(first[0], first[1]);

				if(cmp(first[1], first[2]) > 0){
					swap(first[1], first[2]);

					if(cmp(first[0], first[1]) > 0)
						swap(first[0], first[1]);
				}
				return;
			}
			return;
		}
		else
		{
			if(s == 4)
			{
				if(cmp(first[0], first[1]) > 0)
					swap(first[0], first[1]);

				if(cmp(first[2], first[3]) > 0)
					swap(first[2], first[3]);

				if(cmp(first[0], first[2]) > 0)
					swap(first[0], first[2]);

				if(cmp(first[1], first[3]) > 0)
					swap(first[1], first[3]);

				if(cmp(first[1], first[2]) > 0)
					swap(first[1],first[2]);
				return;
			}
			else if(s == 5)
			{
				if(cmp(first[0], first[1]) > 0)
					swap(first[0], first[1]);

				if(cmp(first[1], first[2]) > 0){
					swap(first[1], first[2]);

					if(cmp(first[0], first[1]) > 0)
						swap(first[0], first[1]);
				}

				if(cmp(first[3], first[4]) > 0)
					swap(first[3], first[4]);


				if(cmp(first[0], first[3]) > 0)
					swap(first[0], first[3]);


				if(cmp(first[2], first[4]) > 0){
					swap(first[2], first[4]);

					if(cmp(first[1], first[2]) > 0)
						swap(first[1], first[2]);
				}

				if(cmp(first[2], first[3]) > 0){
					swap(first[2], first[3]);

					if(cmp(first[1], first[2]) > 0)
						swap(first[1], first[2]);
				}

				return;
			}
			else
			{
				if(cmp(first[0], first[1]) > 0)
					swap(first[0], first[1]);

				if(cmp(first[1], first[2]) > 0){
					swap(first[1], first[2]);

					if(cmp(first[0], first[1]) > 0)
						swap(first[0], first[1]);
				}

				if(cmp(first[3], first[4]) > 0)
					swap(first[3], first[4]);

				if(cmp(first[4], first[5]) > 0){
					swap(first[4], first[5]);

					if(cmp(first[3], first[4]) > 0)
						swap(first[3], first[4]);
				}


				if(cmp(first[0], first[3]) > 0){
					swap(first[0], first[3]);

					if(cmp(first[3], first[4]) > 0)
						swap(first[3], first[4]);
				}

				if(cmp(first[2], first[5]) > 0){
					swap(first[2], first[5]);

					if(cmp(first[1], first[2]) > 0)
						swap(first[1], first[2]);
				}

				if(cmp(first[1], first[3]) > 0)
					swap(first[1], first[3]);

				if(cmp(first[2], first[4]) > 0)
					swap(first[2], first[4]);

				if(cmp(first[2], first[3]) > 0)
					swap(first[2], first[3]);

				return;
			}
		}
	}
	*/

	if(s <= 7)
	{
		isort(first, s, cmp);
		return;
	}

	/*
	if(s <= 6)
	{
		switch(s)
		{
		case 2:
			if(cmp(first[0], first[1]) > 0)
				swap(first[0], first[1]);
			return;
		case 3:
			if(cmp(first[0], first[1]) > 0)
				swap(first[0], first[1]);

			if(cmp(first[1], first[2]) > 0){
				swap(first[1], first[2]);

				if(cmp(first[0], first[1]) > 0)
					swap(first[0], first[1]);
			}
			return;
		case 4:
			if(cmp(first[0], first[1]) > 0)
				swap(first[0], first[1]);

			if(cmp(first[2], first[3]) > 0)
				swap(first[2], first[3]);

			if(cmp(first[0], first[2]) > 0)
				swap(first[0], first[2]);

			if(cmp(first[1], first[3]) > 0)
				swap(first[1], first[3]);

			if(cmp(first[1], first[2]) > 0)
				swap(first[1],first[2]);
			return;

		case 5:
			if(cmp(first[0], first[1]) > 0)
				swap(first[0], first[1]);

			if(cmp(first[1], first[2]) > 0){
				swap(first[1], first[2]);

				if(cmp(first[0], first[1]) > 0)
					swap(first[0], first[1]);
			}

			if(cmp(first[3], first[4]) > 0)
				swap(first[3], first[4]);


			if(cmp(first[0], first[3]) > 0)
				swap(first[0], first[3]);


			if(cmp(first[2], first[4]) > 0){
				swap(first[2], first[4]);

				if(cmp(first[1], first[2]) > 0)
					swap(first[1], first[2]);
			}

			if(cmp(first[2], first[3]) > 0){
				swap(first[2], first[3]);

				if(cmp(first[1], first[2]) > 0)
					swap(first[1], first[2]);
			}

			return;

		case 6:
			if(cmp(first[0], first[1]) > 0)
				swap(first[0], first[1]);

			if(cmp(first[1], first[2]) > 0){
				swap(first[1], first[2]);

				if(cmp(first[0], first[1]) > 0)
					swap(first[0], first[1]);
			}

			if(cmp(first[3], first[4]) > 0)
				swap(first[3], first[4]);

			if(cmp(first[4], first[5]) > 0){
				swap(first[4], first[5]);

				if(cmp(first[3], first[4]) > 0)
					swap(first[3], first[4]);
			}


			if(cmp(first[0], first[3]) > 0){
				swap(first[0], first[3]);

				if(cmp(first[3], first[4]) > 0)
					swap(first[3], first[4]);
			}

			if(cmp(first[2], first[5]) > 0){
				swap(first[2], first[5]);

				if(cmp(first[1], first[2]) > 0)
					swap(first[1], first[2]);
			}

			if(cmp(first[1], first[3]) > 0)
				swap(first[1], first[3]);

			if(cmp(first[2], first[4]) > 0)
				swap(first[2], first[4]);

			if(cmp(first[2], first[3]) > 0)
				swap(first[2], first[3]);

			return;
		}
		return;
	}
	*/

	t_Element* midvalptr = first + (s >> 1);

	if(cmp(*first, *midvalptr) > 0)
			swap(*first, *midvalptr);

	if(cmp(*midvalptr, *(end - 1)) > 0)
	{
		swap(*midvalptr, *(end - 1));

		if(cmp(*first, *midvalptr) > 0)
			swap(*first, *midvalptr);
	}

	t_Element midval = *midvalptr;
	t_Element* mfirst = midvalptr;
	t_Element* mlast = midvalptr;

	t_Element* l = first;
	t_Element* r = end - 1;
	while(true)
	{
		while(true)
		{
			Assert(r >= mlast);
			Assert(l <= mfirst);
			Assert(mfirst <= mlast);

			int c = cmp(*l, midval);

			if(c < 0){
				++l;
				continue;
			}
			else if(c > 0){
				break;
			}
			else
			{
				do{
					if(l == mfirst)
					{
						for(; r != mlast; )
						{
							int c;
							c = cmp(midval, *r);
SecondStageRightNoCompare:
							if(c < 0){
								--r;
								continue;
							}
							else if(c > 0){
								t_Element tmp = *mfirst;
								*mfirst = *r;
								++ mfirst;
								++ mlast;
								*r = *mlast;
								*mlast = tmp;
							}
							else{
								do{
									if(r == mlast)
										goto SubStep;
								}while((c = cmp(midval, *(++mlast))) == 0);

								swap(*mlast, *r);
								goto SecondStageRightNoCompare;
							}
						}
						goto SubStep;
					}
						
				}while((c = cmp(*(--mfirst), midval)) == 0);
				
				swap(*mfirst, *l);

				if(c < 0){
					++l;
					continue;
				}
				else
					break;
			}
		}

		while(true)
		{
			Assert(r >= mlast);
			Assert(l <= mfirst);
			Assert(mfirst <= mlast);

			int c = cmp(midval, *r);

			if(c < 0){
				--r;
				continue;
			}
			else if(c > 0){
				break;
			}
			else
			{
				do{
					if(r == mlast)
					{
						for(; l != mfirst; ){
							int c;
							c = cmp(*l, midval);
SecondStageLeftNoCompare:
							if(c < 0){
								++l;
								continue;
							}
							else if(c > 0){
								t_Element tmp = *mlast;
								*mlast = *l;
								-- mlast;
								-- mfirst;
								*l = *mfirst;
								*mfirst = tmp;
							}
							else{
								do{
									if(l == mfirst)
										goto SubStep;
								}while((c = cmp(*(--mfirst), midval)) == 0);

								swap(*mfirst, *l);
								goto SecondStageLeftNoCompare;
							}
						}
						goto SubStep;
					}
						
				}while((c = cmp(midval, *(++mlast))) == 0);

				swap(*mlast, *r);
				
				if(c < 0){
					--r;
					continue;
				}
				else
					break;
			}
		}

		swap(*l, *r);
		++ l;
		-- r;

		Assert(r >= mlast);
		Assert(l <= mfirst);
		Assert(mfirst <= mlast);
	}

SubStep:

	eqsort(first, mfirst, cmp);
	eqsort(mlast + 1, end, cmp);
}



template<class t_Element, class t_Comparator>
static void qsort(t_Element* first, t_Element* end, const t_Comparator& cmp)
{
	uint s = end - first;

	if(s <= 7){
		isort(first, s, cmp);
		return;
	}

	t_Element* midvalptr = first + (s >> 1);

	if(cmp(*first, *midvalptr) > 0)
		swap(*first, *midvalptr);

	if(cmp(*midvalptr, *(end - 1)) > 0)
	{
		swap(*midvalptr, *(end - 1));

		if(cmp(*first, *midvalptr) > 0)
			swap(*first, *midvalptr);
	}

	t_Element* l = first;
	t_Element* r = end - 1;
	do
	{
		while(cmp(*l, *midvalptr) < 0){ ++ l; Assert(l < end); }
		while(cmp(*r, *midvalptr) > 0){ -- r; Assert(r >= first); }
		
		if(l < r)
		{
			swap(*l, *r);
			Assert(l >= first);
			Assert(l < end);
			Assert(r >= first);
			Assert(r < end);
				
			if(midvalptr == l)
				midvalptr = r;
			else if(midvalptr == r)
				midvalptr = l;

			++ l;
			-- r;
		}
		else if(l == r){
			++ l;
			-- r;
			break;
		}
	}
	while(l <= r);

	if(r > first)
		qsort(first, r + 1, cmp);
	if(l + 1 < end)
		qsort(l, end, cmp);
}



template<class t_Element>
static INLINE void eqsort(t_Element* first, t_Element* end)
{
	DefaultComparator<const t_Element, const t_Element> cmp;
	eqsort(first, end, cmp);
}

template<class t_Element, class t_Comparator>
static INLINE void eqsort(t_Element* first, uint size, const t_Comparator& cmp)
{
	eqsort(first, first + size, cmp);
}

template<class t_Element>
static INLINE void eqsort(t_Element* first, uint size)
{
	DefaultComparator<const t_Element, const t_Element> cmp;
	eqsort(first, first + size, cmp);
}

template<class t_Element>
static INLINE void qsort(t_Element* first, t_Element* end)
{
	DefaultComparator<const t_Element, const t_Element> cmp;
	qsort(first, end, cmp);
}

template<class t_Element, class t_Comparator>
static INLINE void qsort(t_Element* first, uint size, const t_Comparator& cmp)
{
	qsort(first, first + size, cmp);
}

template<class t_Element>
static INLINE void qsort(t_Element* first, uint size)
{
	DefaultComparator<const t_Element, const t_Element> cmp;
	qsort(first, first + size, cmp);
}

template<class t_Element>
static INLINE void sort(t_Element* first, t_Element* end)
{
	DefaultComparator<const t_Element, const t_Element> cmp;
	qsort(first, end, cmp);
}

template<class t_Element, class t_Comparator>
static INLINE void sort(t_Element* first, uint size, const t_Comparator& cmp)
{
	qsort(first, first + size, cmp);
}

template<class t_Element, class t_Comparator>
static INLINE void sort(t_Element* first, t_Element* end, const t_Comparator& cmp)
{
	qsort(first, end, cmp);
}

template<class t_Element>
static INLINE void sort(t_Element* first, uint size)
{
	DefaultComparator<const t_Element, const t_Element> cmp;
	qsort(first, first + size, cmp);
}



template<typename t_ElementA, typename t_ElementB, class t_Comparator>
static INLINE int compare(const t_ElementA* firstA, uinteger sizeA, const t_ElementB* firstB, uinteger sizeB, const t_Comparator& cmp)
{
	uinteger len = Base::min(sizeA, sizeB);

	const t_ElementB* b = firstB;
	for(const t_ElementA* a = firstA, *a_e = a + len; a != a_e; ++a, ++b){
		int c = cmp(*a, *b);
		if(c == 0)
			continue;
		return c;
	}
	return int(sizeA - sizeB);
}

template<typename t_ElementA, typename t_ElementB>
static INLINE int compare(const t_ElementA* firstA, uinteger sizeA, const t_ElementB* firstB, uinteger sizeB)
{
	DefaultComparator<const t_ElementA, const t_ElementB> cmp;
	return compare(firstA, sizeA, firstB, sizeB, cmp);
}

template<typename t_ElementA, typename t_ElementB, class t_Comparator>
static INLINE int compare(const t_ElementA* firstA, const t_ElementA* endA, const t_ElementB* firstB, const t_ElementB* endB, const t_Comparator& cmp)
{
	return compare(firstA, endA - firstA, firstB, endB - firstB, cmp);
}

template<typename t_ElementA, typename t_ElementB>
static INLINE int compare(const t_ElementA* firstA, const t_ElementA* endA, const t_ElementB* firstB, const t_ElementB* endB)
{
	DefaultComparator<const t_ElementA, const t_ElementB> cmp;
	return compare(firstA, endA - firstA, firstB, endB - firstB, cmp);
}

template<typename t_ElementA, typename t_ElementB, class t_Comparator>
static INLINE int rcompare(const t_ElementA* lastA, uinteger sizeA,  const t_ElementB* lastB, uinteger sizeB, const t_Comparator& cmp)
{
	uinteger len = Base::min(sizeA, sizeB);

	const t_ElementB* b = lastB;
	for(const t_ElementA* a = lastA, *a_e = a - len; a != a_e; --a, --b){
		int c = cmp(*a, *b);
		if(c == 0)
			continue;
		return c;
	}
	return int(sizeA - sizeB);
}

template<typename t_ElementA, typename t_ElementB>
static INLINE int rcompare(const t_ElementA* lastA, uinteger sizeA, const t_ElementB* lastB, uinteger sizeB)
{
	DefaultComparator<const t_ElementA, const t_ElementB> cmp;
	return rcompare(lastA, sizeA, lastB, sizeB, cmp);
}

template<typename t_ElementA, typename t_ElementB, class t_Comparator>
static INLINE int rcompare(const t_ElementA* lastA, const t_ElementA* beginA, const t_ElementB* lastB, const t_ElementB* beginB, const t_Comparator& cmp)
{
	return rcompare(lastA, lastA - beginA, lastB, lastB - beginB, cmp);
}

template<typename t_ElementA, typename t_ElementB>
static INLINE int rcompare(const t_ElementA* lastA, const t_ElementA* beginA, const t_ElementB* lastB, const t_ElementB* beginB)
{
	DefaultComparator<const t_ElementA, const t_ElementB> cmp;
	return rcompare(lastA, lastA - beginA, lastB, lastB - beginB, cmp);
}


template<typename t_Val0, typename t_Val1>
class BinDataCompare32bitAlligned
{
public:
	INLINE int operator () (const t_Val0& a, const t_Val1& b)
	{
		return Base::compare(reinterpret_cast<const uint32*>(&a), sizeof(t_Val0) / sizeof(uint32), reinterpret_cast<const uint32*>(&b), sizeof(t_Val1) / sizeof(uint32));
	}
};


template<class t_DstContainer, class t_SrcContainer, class t_Value, class t_EqualComparator>
static INLINE void split(t_DstContainer& dest, const t_SrcContainer& src, const t_Value& val, t_EqualComparator ecmp)
{
	const typename t_SrcContainer::Value_t* r_first = src.first();
	const typename t_SrcContainer::Value_t* r_end = r_first;
	for(const typename t_SrcContainer::Value_t* p = r_first; p != src.end(); ++p)
	{
		if(ecmp(*p, val)){
			r_end = p + 1;
			continue;
		}

		if(r_first != r_end)
			dest.push(typename t_DstContainer::Value_t(r_first, r_end));
		r_first = r_end = p + 1;
	}
	if(r_first != r_end)
		dest.push(typename t_DstContainer::Value_t(r_first, r_end));
}

template<class t_DstContainer, class t_SrcContainer, class t_Value>
static INLINE void split(t_DstContainer& dest, const t_SrcContainer& src, const t_Value& val)
{
	split(dest, src, val, DefaultComparator<const typename t_SrcContainer::Value_t, const t_Value>());
}

}
