#include "include.h"

#if defined(_WIN32)

namespace EntryPoint
{

Base::HashArray<Base::String> gPurchasedItems;
Mutex gPurchasedItemsMutex;

void doPurchase(Base::ConstString item)
{
	Base::LockGuard lg(gPurchasedItemsMutex);
	Base::boolean bNew;
	gPurchasedItems.insert(bNew, item);
}

Base::boolean isPurchased(Base::ConstString item)
{
	Base::LockGuard lg(gPurchasedItemsMutex);
	return gPurchasedItems.find(item) != gPurchasedItems.end();
}

Base::String getPrice(Base::ConstString item)
{
	return _CS("0");
}

Base::boolean connectToBillingServerRequest()
{
	static boolean connected = false;
	connected |= rand(0, 300) == 0;
	return connected;
}

void restorePurchases()
{

}

}

#endif

