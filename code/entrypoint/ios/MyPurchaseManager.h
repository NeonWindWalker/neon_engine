//
//  MyPurchaseManager.h
//  Rills Blossom
//
//  Created by Admin on 10/28/13.
//  Copyright (c) 2013 Admin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

typedef void (^RequestProductsCompletionHandler)(BOOL success, NSArray * products);

@interface MyPurchaseManager : NSObject
{
    SKProductsRequest * _productsRequest;

    RequestProductsCompletionHandler _completionHandler;
    NSSet * _productIdentifiers;
    NSMutableSet * _purchasedProductIdentifiers;
    NSMutableDictionary * _products;
    bool _connected;
    bool _connectInProgress;
}

- (id)initWithProductIdentifiers:(NSSet *)productIdentifiers;
- (void)requestProductsWithCompletionHandler:(RequestProductsCompletionHandler)completionHandler;
- (void)buyProduct:(SKProduct *)product;
- (BOOL)productPurchased:(NSString *)productIdentifier;
- (bool)connect;
- (void)setProductPrice:(SKProduct *)product;
- (void)restorePurchases;

@end

extern MyPurchaseManager *gMyPurchaseManager;

void MyObjCWrap_doPurchase(const char* str);
void EntryPoint_setPurchased(const char* str);
void EntryPoint_setPrice(const char* str, const char* price);
bool MyObjCWrap_connectToBillingServerRequest();
void MyObjCWrap_restorePurchases();
