#include "../include.h"
#include "tests.h"

#include <map>
#include <set>
#include <algorithm>
#include <hash_map>

//#include "../sheeftz/hash_map.h"

/*
Base::uint rang(Base::uint x)
{
	x ^= x << 16;
	x ^= x >> 5;
	x ^= x << 1;
	return (x ^ 123456789);
}
*/

template<class t_Container, class t_KeyVal>
void testStdContainer(t_Container& container, Base::uint cycles, Base::uint megaCycles, t_KeyVal* randKeyArray, Base::uint* randValArray, double& insertTime, double& eraseTime, double& findTime, double& iterationTime, Base::uint& summ) 
{
	Base::Timer timer;

	insertTime = eraseTime = findTime = iterationTime = 0;

	for(Base::uint megaCycle = 0; megaCycle < megaCycles;  ++ megaCycle)
	{
		container.clear();

		timer.dt();
		for(Base::uint i=0; i < cycles; i++)
			container.insert(std::make_pair(randKeyArray[i], randValArray[i]));
		insertTime += timer.dt() / cycles;

		for(Base::uint i=0; i < cycles; i += 2)
			container.erase(randKeyArray[i]);
		eraseTime += timer.dt() / (cycles / 2);

		for(Base::uint i=1; i < cycles; i += 2)
		{
			typename t_Container::iterator it = container.find(randKeyArray[i]);
			if(it != container.end())
				summ += it->second;
		}
		findTime += timer.dt() / (cycles / 2);

		Base::uint iters = 0;
		timer.dt();
		for(typename t_Container::iterator it = container.begin(); it != container.end(); ++it, ++iters)
			summ += it->second;
		iterationTime += timer.dt() / iters;
	}

	insertTime /= megaCycles;
	eraseTime /= megaCycles;
	findTime /= megaCycles;
	iterationTime /= megaCycles;
}

template<class t_Container, class t_StdMapForVerify>
bool testContainerVecrify(t_Container& container, t_StdMapForVerify& verifyContainer) 
{
	//verify
	for(typename t_Container::Iterator it = container.first(); it != container.end(); ++it)
	{
		typename t_StdMapForVerify::iterator verit = verifyContainer.find(it->key);
		if(verit == verifyContainer.end())
			return false;
		if(verit->second != it->val)
			return false;
	}

	for(typename t_StdMapForVerify::iterator verit = verifyContainer.begin(); verit != verifyContainer.end(); ++verit)
	{
		typename t_Container::Iterator it = container.find(verit->first);
		if(it == container.end())
			return false;
		if(verit->second != it->val)
			return false;
	}
	return true;
}

template<class t_Container, class t_StdMapForVerify, class t_KeyVal>
bool testContainer(t_Container& container, Base::uint cycles, Base::uint megaCycles, t_KeyVal* randKeyArray, Base::uint* randValArray, t_StdMapForVerify& verifyContainer, double& insertTime, double& eraseTime, double& findTime, double& iterationTime, Base::uint& summ) 
{
	Base::Timer timer;

	insertTime = eraseTime = findTime = iterationTime = 0;
	
	for(Base::uint megaCycle = 0; megaCycle < megaCycles;  ++ megaCycle)
	{
		container.clear();

		timer.dt();
		for(Base::uint i=0; i < cycles; i++)
			container(randKeyArray[i])->val = randValArray[i];
		insertTime += timer.dt() / cycles;

		for(Base::uint i=0; i < cycles; i += 2)
			container.erase(randKeyArray[i]);
		eraseTime += timer.dt() / (cycles / 2);

		
		for(Base::uint i=1; i < cycles; i += 2){
			typename t_Container::Iterator it = container.find(randKeyArray[i]);
			if(it != container.end())
				summ += it->val;
		}
			
		findTime += timer.dt() / (cycles / 2);

		Base::uint iters = 0;
		timer.dt();
		for(typename t_Container::Iterator it = container.first(); it != container.end(); ++it, ++iters)
			summ += it->val;
		iterationTime += timer.dt() / iters;
	}

	insertTime /= megaCycles;
	eraseTime /= megaCycles;
	findTime /= megaCycles;
	iterationTime /= megaCycles;

	return testContainerVecrify(container, verifyContainer);
}

void Base::testContainers()
{
	logInfoSkim<<"<<<<< CONTAINERS TESTS >>>>>";

	Base::uint cyclesCount = 10000;
	Base::uint megaCyclesCount = 10;

#ifdef DEBUG
	cyclesCount /= 10;
	megaCyclesCount = 4;
#endif


	Base::uint* randArr = new Base::uint[cyclesCount];
	for(Base::uint i=0; i < cyclesCount; i++)
		randArr[i] = Base::rand(0,100000000);

	std::map<Base::uint, Base::uint> stdmap;
#ifdef _MSC_VER
	std::hash_map<Base::uint, Base::uint> stdhashmap;
#endif
	Base::RedBlackTreeMap< Base::Pair<Base::uint, Base::uint> > rbtmap;
	Base::HashTable< Base::Pair<Base::uint, Base::uint> > hashTable;
	Base::HashMap< Base::Pair<Base::uint, Base::uint> > hashMap;
	Base::HashArray< Base::Pair<Base::uint, Base::uint> > hashArray;
	Base::SortedArray< Base::Pair<Base::uint, Base::uint> > sortedArray;
	Base::RedBlackTreeMap< Base::Pair<Base::uint, Base::uint>, AllocatorWrapper<TCellDefragmentAllocator< Base::RedBlackTreeMapNodeT<Pair<Base::uint, Base::uint> >::Type > > > rbtmapCellAlloc;

	double insertTime;
	double eraseTime;
	double findTime;
	double iterationTime;
	Base::uint summ = 0;

	testStdContainer(stdmap, cyclesCount, megaCyclesCount, randArr, randArr, insertTime, eraseTime, findTime, iterationTime, summ);
	logInfoSkim<<"std::map insert ns = " << insertTime * 1e9;
	logInfoSkim<<"std::map erase ns = " << eraseTime * 1e9;
	logInfoSkim<<"std::map find ns = " << findTime * 1e9;
	logInfoSkim<<"std::map iteration ns = " << iterationTime * 1e9;

#ifdef _MSC_VER
	testStdContainer(stdhashmap, cyclesCount, megaCyclesCount, randArr, randArr, insertTime, eraseTime, findTime, iterationTime, summ);
	logInfoSkim<<"std::hash_map insert ns = " << insertTime * 1e9;
	logInfoSkim<<"std::hash_map erase ns = " << eraseTime * 1e9;
	logInfoSkim<<"std::hash_map find ns = " << findTime * 1e9;
	logInfoSkim<<"std::hash_map iteration ns = " << iterationTime * 1e9;
#endif

	if(testContainer(rbtmap, cyclesCount, megaCyclesCount, randArr, randArr, stdmap, insertTime, eraseTime, findTime, iterationTime, summ))
	{
		logInfoSkim<<"RedBlackTreeMap insert ns = " << insertTime * 1e9;
		logInfoSkim<<"RedBlackTreeMap erase ns = " << eraseTime * 1e9;
		logInfoSkim<<"RedBlackTreeMap find ns = " << findTime * 1e9;
		logInfoSkim<<"RedBlackTreeMap iteration ns = " << iterationTime * 1e9;
	}
	else
		logInfoSkim<<"! RedBlackTreeMap FAIL";

	if(testContainer(hashTable, cyclesCount, megaCyclesCount, randArr, randArr, stdmap, insertTime, eraseTime, findTime, iterationTime, summ))
	{
		logInfoSkim<<"HashTable insert ns = " << insertTime * 1e9;
		logInfoSkim<<"HashTable erase ns = " << eraseTime * 1e9;
		logInfoSkim<<"HashTable find ns = " << findTime * 1e9;
		logInfoSkim<<"HashTable iteration ns = " << iterationTime * 1e9;
	}
	else
		logInfoSkim<<"! HashTable FAIL";

	if(testContainer(hashMap, cyclesCount, megaCyclesCount, randArr, randArr, stdmap, insertTime, eraseTime, findTime, iterationTime, summ))
	{
		logInfoSkim<<"HashMap insert ns = " << insertTime * 1e9;
		logInfoSkim<<"HashMap erase ns = " << eraseTime * 1e9;
		logInfoSkim<<"HashMap find ns = " << findTime * 1e9;
		logInfoSkim<<"HashMap iteration ns = " << iterationTime * 1e9;
	}
	else
		logInfoSkim<<"! HashMap FAIL";
	
	if(testContainer(hashArray, cyclesCount, megaCyclesCount, randArr, randArr, stdmap, insertTime, eraseTime, findTime, iterationTime, summ))
	{
		logInfoSkim<<"HashArray insert ns = " << insertTime * 1e9;
		logInfoSkim<<"HashArray erase ns = " << eraseTime * 1e9;
		logInfoSkim<<"HashArray find ns = " << findTime * 1e9;
		logInfoSkim<<"HashArray iteration ns = " << iterationTime * 1e9;
	}
	else
		logInfoSkim<<"! HashArray FAIL";

	if(testContainer(sortedArray, cyclesCount, megaCyclesCount, randArr, randArr, stdmap, insertTime, eraseTime, findTime, iterationTime, summ))
	{
		logInfoSkim<<"SortedArray insert ns = " << insertTime * 1e9;
		logInfoSkim<<"SortedArray erase ns = " << eraseTime * 1e9;
		logInfoSkim<<"SortedArray find ns = " << findTime * 1e9;
		logInfoSkim<<"SortedArray iteration ns = " << iterationTime * 1e9;
	}
	else
		logInfoSkim<<"! SortedArray FAIL";

	if(testContainer(rbtmapCellAlloc, cyclesCount, megaCyclesCount, randArr, randArr, stdmap, insertTime, eraseTime, findTime, iterationTime, summ))
	{
		logInfoSkim<<"RedBlackTreeMap<uint, CellAllocator> time ns = " << insertTime * 1e9;
		logInfoSkim<<"RedBlackTreeMap<uint, CellAllocator> erase ns = " << eraseTime * 1e9;
		logInfoSkim<<"RedBlackTreeMap<uint, CellAllocator> find ns = " << findTime * 1e9;
		logInfoSkim<<"RedBlackTreeMap<uint, CellAllocator> iteration ns = " << iterationTime * 1e9;
	}
	else
		logInfoSkim<<"! RedBlackTreeMap<uint, CellAllocator> FAIL";

	rbtmapCellAlloc.allocator().defragment();
	if(testContainerVecrify(rbtmapCellAlloc, stdmap))
		logInfoSkim<<"RedBlackTreeMap<uint, CellAllocator>::defragemnt OK ";
	else
		logInfoSkim<<"! RedBlackTreeMap<uint, CellAllocator>::defragemnt FAIL";
	//rbtmapCellAlloc.clear();

	

	std::map<Base::String, Base::uint> stdmapS;
	Base::RedBlackTreeMap< Base::Pair<Base::String, Base::uint> > rbtmapS;
	Base::HashTable< Base::Pair<Base::String, Base::uint> > hashTableS;
	Base::HashMap< Base::Pair<Base::String, Base::uint> > hashMapS;
	Base::HashArray< Base::Pair<Base::String, Base::uint> > hashArrayS;
	Base::RedBlackTreeMap< Base::Pair<Base::String, Base::uint>, AllocatorWrapper<TCellDefragmentAllocator< Base::RedBlackTreeMapNodeT<Pair<Base::String, Base::uint>>::Type > > > rbtmapCellAllocS;

	Base::String* randStrArr = new Base::String[cyclesCount];
	for(Base::uint i=0; i < cyclesCount; i++)
		randStrArr[i] << Base::rand(0,100000000);

	testStdContainer(stdmapS, cyclesCount, megaCyclesCount, randStrArr, randArr, insertTime, eraseTime, findTime, iterationTime, summ);
	logInfoSkim<<"std::map<String> time ns = " << insertTime * 1e9;
	logInfoSkim<<"std::map<String> erase ns = " << eraseTime * 1e9;
	logInfoSkim<<"std::map<String> find ns = " << findTime * 1e9;
	logInfoSkim<<"std::map<String> iteration ns = " << iterationTime * 1e9;

	if(testContainer(rbtmapS, cyclesCount, megaCyclesCount, randStrArr, randArr, stdmapS, insertTime, eraseTime, findTime, iterationTime, summ))
	{
		logInfoSkim<<"RedBlackTreeMap<String> time ns = " << insertTime * 1e9;
		logInfoSkim<<"RedBlackTreeMap<String> erase ns = " << eraseTime * 1e9;
		logInfoSkim<<"RedBlackTreeMap<String> find ns = " << findTime * 1e9;
		logInfoSkim<<"RedBlackTreeMap<String> iteration ns = " << iterationTime * 1e9;
	}
	else
		logInfoSkim<<"! RedBlackTreeMap<String> FAIL";

	if(testContainer(rbtmapCellAllocS, cyclesCount, megaCyclesCount, randStrArr, randArr, stdmapS, insertTime, eraseTime, findTime, iterationTime, summ))
	{
		logInfoSkim<<"RedBlackTreeMap<String, CellAllocator> time ns = " << insertTime * 1e9;
		logInfoSkim<<"RedBlackTreeMap<String, CellAllocator> erase ns = " << eraseTime * 1e9;
		logInfoSkim<<"RedBlackTreeMap<String, CellAllocator> find ns = " << findTime * 1e9;
		logInfoSkim<<"RedBlackTreeMap<String, CellAllocator> iteration ns = " << iterationTime * 1e9;
	}
	else
		logInfoSkim<<"! RedBlackTreeMap<String, CellAllocator> FAIL";

	rbtmapCellAllocS.allocator().defragment();
	if(testContainerVecrify(rbtmapCellAllocS, stdmapS))
		logInfoSkim<<"RedBlackTreeMap<String, CellAllocator>::defragemnt OK ";
	else
		logInfoSkim<<"! RedBlackTreeMap<String, CellAllocator>::defragemnt FAIL";
	//rbtmapCellAllocS.clear();

	if(testContainer(hashMapS, cyclesCount, megaCyclesCount, randStrArr, randArr, stdmapS, insertTime, eraseTime, findTime, iterationTime, summ))
	{
		logInfoSkim<<"HashMap<String> time ns = " << insertTime * 1e9;
		logInfoSkim<<"HashMap<String> erase ns = " << eraseTime * 1e9;
		logInfoSkim<<"HashMap<String> find ns = " << findTime * 1e9;
		logInfoSkim<<"HashMap<String> iteration ns = " << iterationTime * 1e9;
	}
	else
		logInfoSkim<<"! HashMap<String> FAIL";
	
	
	if(testContainer(hashArrayS, cyclesCount, megaCyclesCount, randStrArr, randArr, stdmapS, insertTime, eraseTime, findTime, iterationTime, summ))
	{
		logInfoSkim<<"HashArray<String> time ns = " << insertTime * 1e9;
		logInfoSkim<<"HashArray<String> erase ns = " << eraseTime * 1e9;
		logInfoSkim<<"HashArray<String> find ns = " << findTime * 1e9;
		logInfoSkim<<"HashArray<String> iteration ns = " << iterationTime * 1e9;
	}
	else
		logInfoSkim<<"! HashArray<String> FAIL";

	

	delete[] randArr;

	/*
	timer.dt();
	Base::BiMap<float, float> crsp;
	for(Base::uint i=1; i < Count; i++){
		float f = float(rang[i]);
		crsp.insert(isInserted, f, f);
	}
	logInfoSkim<<"BiMap insert time ns = %lf", timer.dt() * 1e9 / (Count-1));

	timer.dt();
	for(Base::uint i=1; i < Count; i+=2)
		crsp.erase1((float)rang[i]);
	logInfoSkim<<"BiMap erase time ns = %lf", timer.dt() * 1e9 / (Count/2));

	timer.dt();
	for(Base::uint i=1; i < Count; i++){
		Base::BiMap<float, float>::Iterator n = crsp.find0(rang[i]);
		if(n != crsp.end())
			Assert(n.key0() == n.key1());
	}
	logInfoSkim<<"BiMap find time ns = %lf", timer.dt() * 1e9 / (Count-1));
	logInfoSkim<<"for all summ %d" << " " << summ;

	*/
	logInfoSkim<<"summ " << summ;
	logInfoSkim<<".";
}


void Base::testOctree()
{

#ifdef DEBUG
	Base::uint Count = 10000;
#else
	Base::uint Count = 500000;
#endif

	uint summ = 0;
	Base::Timer timer;

	int a = 5;
	typedef TOctree<
		Base::AllocatorWrapper< Base::TCellDefragmentAllocator< Base::TOctreeFlat<int> > >,
		int> Octree;
	Octree::Element_t* elems[1024];
	for(int i=0; i < 1024; ++i)
		elems[i] = new Octree::Element_t(&a);

	Octree spaceSD(100000.0f, 64);
	//Base::Octree<
	//	Base::SystemAllocator&,
	//	int, float> spaceSD(*Base::getGlobalAllocator(), 100000.0f, 64);

	timer.dt();
	for(Base::uint j=0; j < Count; j++)
	{
		Base::uint n = Base::rand(0, 1023);
		int ver = Base::rand(0, 100);
		if(ver > 50)
		{
			if(!elems[n]->included())
				spaceSD.include(*elems[n],
				Base::Vector3f(Base::rand(-10000.0f, 10000.0f), Base::rand(-10000.0f, 10000.0f), Base::rand(-10000.0f, 10000.0f)),
				Base::Vector3f(Base::rand(1.0f, 10000.0f), Base::rand(1.0f, 10000.0f), Base::rand(1.0f, 10000.0f)));
		}
		else
		{
			if(elems[n]->included())
				spaceSD.exclude(*elems[n]);
		}

	}
	logInfoSkim<<"Octree include/exclude operation time ns"<< timer.dt() * 1e9 / Count;

	for(int i=0; i < 1024; ++i){
		if(elems[i]->included()){
			summ ++;
			spaceSD.exclude(*elems[i]);
		}
		delete elems[i];
	}
	logInfoSkim<<"summ " << summ;
}

void Base::testQuadtree()
{

#ifdef DEBUG
	Base::uint Count = 10000;
#else
	Base::uint Count = 500000;
#endif

	uint summ = 0;
	Base::Timer timer;

	int a = 5;
	typedef Base::QuadtreeWithCellAllocatorT<int>::Type Quadtree;
	Quadtree::Element_t* elems[1024];
	for(int i=0; i < 1024; ++i)
		elems[i] = new Quadtree::Element_t(&a);

	Quadtree spaceSD(100000.0f, 64);
	//Base::Octree<
	//	Base::SystemAllocator&,
	//	int, float> spaceSD(*Base::getGlobalAllocator(), 100000.0f, 64);

	timer.dt();
	for(Base::uint j=0; j < Count; j++)
	{
		Base::uint n = Base::rand(0, 1023);
		int ver = Base::rand(0, 100);
		if(ver > 50)
		{
			if(!elems[n]->included())
				spaceSD.include(*elems[n],
				Base::Vector2f(Base::rand(-10000.0f, 10000.0f), Base::rand(-10000.0f, 10000.0f)),
				Base::Vector2f(Base::rand(1.0f, 10000.0f), Base::rand(1.0f, 10000.0f)));
		}
		else
		{
			if(elems[n]->included())
				spaceSD.exclude(*elems[n]);
		}

	}
	logInfoSkim<<"Quadtree include/exclude operation time ns"<< timer.dt() * 1e9 / Count;

	for(int i=0; i < 1024; ++i){
		if(elems[i]->included()){
			summ ++;
			spaceSD.exclude(*elems[i]);
		}
		delete elems[i];
	}
	logInfoSkim<<"summ " << summ;
}
