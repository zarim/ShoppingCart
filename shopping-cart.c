#include <shopping-cart.h>

#include <assert.h>
#include <stdlib.h>
#include <time.h>

/* helper function prototypes (if necessary) */
//isEmpty
//isFull
//printList
//AddOrderToback
//LengthList

//Items
 ClothingItem Exchange21Selection[] = {
   "Maxi Dress",
   "Body Suit",
   "Graphic Tee",
   "Crop-Top",
   "High-Rise Leggings",
   "Biker Shorts",
   "Mini-Skirt",
   "Denim Shorts",
   "Romper",
   "Jump-Suit",
   "Culotte"
 };

 //Sizes
ClothingSize Exchange21Sizes[] = {
  "XS",
  "S",
  "M",
  "L",
  "XL",
};

int ExchangeSelectionLength = 11;

ClothingItem PickRandomClothingItem() {
  int item = rand() % ExchangeSelectionLength;
  return Exchange21Selection[item];
}

Exchange21* OpenStore(int max_size, int expected_customer_orders) {
  Exchange21 * myStore = malloc(sizeof(Exchange21));

  myStore->clothes = NULL; // <----- this needs to different. we need to
  //randomly generate the clothes along with the sizes.

  myStore->current_size = 0;
  myStore->max_size = 0;
  myStore->next_customer_number = 0;
  myStore->customers_handled = 0;

  //should we add the #ofCustomers here too? we can use it to compare
  //customers handled to #ofCustomers. If we do this, it'll need to be
  //the third parameter in this function.
  //if we add this, be sure to add it to the StoreStruct.

  myStore->expected_customer_orders = expected_customer_orders;

  /* Synchronization objects */
  if(pthread_mutex_init(&myStore->mutex, NULL) != 0) {
    printf("\n mutex init has failed!\n");
    return NULL;
  }
  if(pthread_cond_init(&myStore->ready_to_checkout, NULL) != 0) {
    printf("\n ready_to_checkout cond_t init has failed!\n");
    return NULL;
  }
  if(pthread_cond_init(&myStore->can_check_out, NULL) != 0) {
    printf("\n can_check_out cond_t init has failed!\n");
    return NULL;
  }

  printf("Exchange21 is open for business!\n");
  return myStore;
}

void CloseStore(Exchange21* exch21) {
  //as i mentioned in the OpenStore function, we should have it to where if
  //the expected_customers != customers_handled then we can't close,
  //otherwise, if it does == then we destroy the Synchronization objects,
  //print everything first - what the customers bought, what's left in the Store
  //THEN free(exch21).
  printf("Exchange21 has been closed for the day!\n");
}

//using McGoogles as our template, the next function would be addOrder,
//but we need to consider how ours is gonna work cause the customer
//has 3 random options:
//1. Shop - [try on and keep] VS. [try on and leave]
//2. Checkout (this should only be possible if they have at least one item in their cart (they kept one item...))
//3. Upon checkingOut, the customer sends the signal "ready_to_checkout" to the cashier
// after receiving this signal, the cashier updates the stock/clothing quantity that the customer bought and when finished
//sends the "can_check_out" singal to the next customer...
//i'm not sure if this'll ^^^ work exactly like that, but we'll see. sounds pretty dope.
//3. Leave - walked in and walked out.


////using McGoogles as our template, the next function would be getOrder,
//i already explained some of this above, but this will be the cashier's function
//we can call it "checkOutCustomer" or something.


/* OPTIONAL HELPER FUNCTIONS */
