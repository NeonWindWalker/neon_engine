#include "../include.h"
#include "tests.h"
#include <algorithm>

void Base::testAlghoritms()
{
	Base::Timer timer;
	int cArrSize = 1000000;

#ifdef DEBUG
	cArrSize /= 100;
#endif

{
	typedef Base::Pair<int, int> ValType;
	uint summ = 0;
	ValType *arr = new ValType [cArrSize], *arrVals = new ValType[cArrSize];
	std::pair<int, int> *arrRef = new std::pair<int, int>[cArrSize], *arrRefVals = new std::pair<int, int>[cArrSize];
	for(int i=0; i < cArrSize; i++){
		arrVals[i].key = rand(-10*cArrSize, 10*cArrSize);
		arrVals[i].val = i;
		arrRefVals[i].first = arrVals[i].key;
		arrRefVals[i].second = i;
	}

	memoryCopy(arrRef, arrRefVals, sizeof(std::pair<int, int>) * cArrSize);
	timer.dt();
	std::sort(arrRef, arrRef + cArrSize);
	logInfoSkim <<"std::sort 1M array time = " << timer.dt() << "sec";

	memoryCopy(arr, arrVals, sizeof(ValType) * cArrSize);
	timer.dt();
	Base::sort(arr, cArrSize);
	logInfoSkim<<"sort 1M array time = " << timer.dt() << "sec";

	for(int i=0; i < cArrSize; i++)
		summ += arr[i].key;

	for(int i=0; i < cArrSize; i++)
		SeriosAssert(arr[i].key == arrRef[i].first);

	/*timer.dt();
	Base::sortclassic(arr1, arr1 + cArrSize, Base::DefaultComparator<int, int>());
	logInfoSkim("Base::sortclassic array %lf", timer.dt());

	for(int i=0; i < cArrSize - 1; i++)
		SeriosAssert(arr1[i] <= arr1[i + 1]);
	*/

	for(int i=0; i < cArrSize; i++){
		arrVals[i].key = rand(-cArrSize/8, cArrSize/8);
		arrVals[i].val = i;
		arrRefVals[i].first = arrVals[i].key;
		arrRefVals[i].second = i;
	}

	memoryCopy(arrRef, arrRefVals, sizeof(std::pair<int, int>) * cArrSize);
	timer.dt();
	std::sort(arrRef, arrRef + cArrSize);
	logInfoSkim <<"std::sort 1M array many eq time = " << timer.dt() << "sec";

	memoryCopy(arr, arrVals, sizeof(ValType) * cArrSize);
	timer.dt();
	Base::eqsort(arr, cArrSize);
	logInfoSkim<<"eqsort 1M array many eq time = " << timer.dt() << "sec";

	for(int i=0; i < cArrSize; i++)
		summ += arr[i].key;

	for(int i=0; i < cArrSize; i++)
		SeriosAssert(arr[i].key == arrRef[i].first);

	//for(int i=0; i < cArrSize - 1; i++)
	//	if(arr[i].key == arr[i + 1].key)
	//		SeriosAssert(arr[i].val < arr[i + 1].val);


	logInfoSkim<<"summ " << summ;

	delete[] arr;
	delete[] arrRef;
	delete[] arrVals;
}

{
	float summ = 0;
	float *flarr = new float [cArrSize];
	for(int i=0; i < cArrSize; i++)
		flarr[i] = rand(-10*cArrSize, 10*cArrSize);

	Base::Timer timer;
	Base::sort(flarr, cArrSize);
	logInfoSkim<<"sort flt 1M array time = " << timer.dt() << "sec";

	for(int i=0; i < cArrSize; i++)
		summ += flarr[i];
	logInfoSkim<<"summ " << summ;

	for(int i=0; i < cArrSize - 1; i++)
		SeriosAssert(flarr[i] <= flarr[i + 1]);

	delete[] flarr;
}

}
