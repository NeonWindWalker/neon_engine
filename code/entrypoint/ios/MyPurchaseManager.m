//
//  MyPurchaseManager.m
//  Rills Blossom
//
//  Created by Admin on 10/28/13.
//  Copyright (c) 2013 Admin. All rights reserved.
//

#import "MyPurchaseManager.h"


MyPurchaseManager *gMyPurchaseManager = 0;

@interface MyPurchaseManager () <SKProductsRequestDelegate, SKPaymentTransactionObserver>
@end

@implementation MyPurchaseManager

- (id)initWithProductIdentifiers:(NSSet *)productIdentifiers
{
    if ((self = [super init]))
    {
        _connected = false;
        _connectInProgress = false;
        
        _products = [[NSMutableDictionary alloc] init];
        
        // Store product identifiers
        _productIdentifiers = productIdentifiers;
        
        // Check for previously purchased products
        _purchasedProductIdentifiers = [NSMutableSet set];
        for (NSString * productIdentifier in _productIdentifiers) {
            BOOL productPurchased = [[NSUserDefaults standardUserDefaults] boolForKey:productIdentifier];
            if (productPurchased) {
                [_purchasedProductIdentifiers addObject:productIdentifier];
                NSLog(@"Previously purchased: %@", productIdentifier);
                char cstrId[1024];
                [productIdentifier getCString:cstrId maxLength:1024 encoding:NSASCIIStringEncoding];
                EntryPoint_setPurchased(cstrId);
            } else {
                NSLog(@"Not purchased: %@", productIdentifier);
            }
        }
        
        [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
        
        /*
        if(![[NSUserDefaults standardUserDefaults] boolForKey:@"CompletedTransactionsRestored"] && ![[NSUserDefaults standardUserDefaults] boolForKey:@"CompletedTransactionsRestoreCalled"])
        {
            [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
            [[NSUserDefaults standardUserDefaults] setBool:YES forKey:@"CompletedTransactionsRestoreCalled"];
            [[NSUserDefaults standardUserDefaults] synchronize];
        }
         */

    }
    return self;
}

- (bool)connect
{
    if(_connected)
        return true;
    
    if(_connectInProgress)
        return false;
    
    _connectInProgress = true;
    
    [self requestProductsWithCompletionHandler:^(BOOL success, NSArray *products)
     {
         if (success)
         {
             for(SKProduct* pr in products)
             {
                 [_products setObject:pr forKey:pr.productIdentifier];
                 [self setProductPrice:pr];
             }
             _connected = true;
         }
          _connectInProgress = false;
     }];
    
    return false;
}

- (void) restorePurchases
{
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}

- (void)requestProductsWithCompletionHandler:(RequestProductsCompletionHandler)completionHandler{
    
    // 1
    _completionHandler = [completionHandler copy];
    
    // 2
    _productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:_productIdentifiers];
    _productsRequest.delegate = self;
    [_productsRequest start];
}

- (void)setProductPrice:(SKProduct *)skProduct
{
    char cstrId[1024];
    char cstrPrice[1024];
    [skProduct.productIdentifier getCString:cstrId maxLength:1024 encoding:NSASCIIStringEncoding];
    
    NSNumberFormatter *numberFormatter = [[NSNumberFormatter alloc] init];
    [numberFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
    [numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
    [numberFormatter setLocale:skProduct.priceLocale];
    NSString *formattedString = [numberFormatter stringFromNumber:skProduct.price];
    
    [formattedString getCString:cstrPrice maxLength:1024 encoding:NSASCIIStringEncoding];
    EntryPoint_setPrice(cstrId, cstrPrice);
}

#pragma mark - SKProductsRequestDelegate

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    NSLog(@"Loaded list of products...");
    _productsRequest = nil;
    
    NSArray * skProducts = response.products;
    for (SKProduct * skProduct in skProducts)
    {
        NSLog(@"Found product: %@ %@ %0.2f",
              skProduct.productIdentifier,
              skProduct.localizedTitle,
              skProduct.price.floatValue);
        
        [self setProductPrice:skProduct];
        [_products setObject: skProduct forKey: skProduct.productIdentifier];
    }
    
    _completionHandler(YES, skProducts);
    _completionHandler = nil;
    
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error
{
    NSLog(@"Failed to load list of products.");
    _productsRequest = nil;
    
    _completionHandler(NO, nil);
    _completionHandler = nil;
}

- (BOOL)productPurchased:(NSString *)productIdentifier
{
    return [_purchasedProductIdentifiers containsObject:productIdentifier];
}

- (void)buyProduct:(SKProduct *)product
{
    NSLog(@"Buying %@...", product.productIdentifier);
    
    SKPayment * payment = [SKPayment paymentWithProduct:product];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

- (void)buyProductById:(NSString *)productIdentifier
{
    NSLog(@"Buying %@...", productIdentifier);
    SKProduct* product = [_products objectForKey:productIdentifier];
    if(product)
        [self buyProduct:product];
}


- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    bool anyRestore = false;
    for (SKPaymentTransaction * transaction in transactions)
    {
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased:
                [self completeTransaction:transaction];
                break;
            case SKPaymentTransactionStateFailed:
                [self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored:
                anyRestore = true;
                [self restoreTransaction:transaction];
            default:
                break;
        }
    };
    
    if(anyRestore)
    {
        [[NSUserDefaults standardUserDefaults] setBool:YES forKey:@"CompletedTransactionsRestored"];
        [[NSUserDefaults standardUserDefaults] synchronize];
    }
}

- (void)completeTransaction:(SKPaymentTransaction *)transaction {
    NSLog(@"completeTransaction...");
    
    char cstrId[1024];
    [transaction.payment.productIdentifier getCString:cstrId maxLength:1024 encoding:NSASCIIStringEncoding];
    EntryPoint_setPurchased(cstrId);
    [[NSUserDefaults standardUserDefaults] setBool:YES forKey:transaction.payment.productIdentifier];
    [[NSUserDefaults standardUserDefaults] synchronize];
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
}

- (void)restoreTransaction:(SKPaymentTransaction *)transaction {
    NSLog(@"restoreTransaction...");
    
    char cstrId[1024];
    [transaction.payment.productIdentifier getCString:cstrId maxLength:1024 encoding:NSASCIIStringEncoding];
    EntryPoint_setPurchased(cstrId);
    [[NSUserDefaults standardUserDefaults] setBool:YES forKey:transaction.payment.productIdentifier];
    [[NSUserDefaults standardUserDefaults] synchronize];
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
}

- (void)failedTransaction:(SKPaymentTransaction *)transaction {
    
    NSLog(@"failedTransaction...");
    if (transaction.error.code != SKErrorPaymentCancelled)
    {
        NSLog(@"Transaction error: %@", transaction.error.localizedDescription);
    }
    
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}


void MyObjCWrap_doPurchase(const char* str)
{
    if(gMyPurchaseManager)
    {
        NSString* productId = [NSString stringWithUTF8String:str];
        [gMyPurchaseManager buyProductById:productId];
    }
}

bool MyObjCWrap_connectToBillingServerRequest()
{
    if(gMyPurchaseManager)
        return [gMyPurchaseManager connect];
    return false;
}

void MyObjCWrap_restorePurchases()
{
    if(gMyPurchaseManager)
        return [gMyPurchaseManager restorePurchases];
}

@end
