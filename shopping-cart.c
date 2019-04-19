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
 ClothingItem Exchange21Clothes[] = {
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

int ExchangeSelectionLength = 11;

ClothingItem PickRandomClothingItem() {
  int item = rand() % ExchangeSelectionLength;
  return Exchange21Clothes[item];
}

int ShoppingDecision() {
  int decision = rand() % 3;
  return decision;
}

Exchange21* OpenStore(int max_size, int expected_customer_orders) {
  Exchange21 * myStore = malloc(sizeof(Exchange21));
  myStore->receipts = NULL;
  myStore->current_size = 0;
  myStore->max_size = 0;
  myStore->next_customer_number = 0;
  myStore->customers_handled = 0;
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

void goToCheckout(Exchange21* exch21, Receipt* receipt) {
  
}

/* OPTIONAL HELPER FUNCTIONS */
