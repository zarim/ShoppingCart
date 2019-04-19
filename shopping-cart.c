#include <shopping-cart.h>

#include <assert.h>
#include <stdlib.h>
#include <time.h>

/* helper function prototypes */
bool isEmpty(Exchange21 * exch21);
void printList(Exchange21 * exch21);
bool isFull(Exchange21 * exch21);
void AddReceiptToBack(Receipt** receipts, Receipt* receipt);
int lengthList(Exchange21 * exch21);

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
  int decision = rand() % 2;
  return decision;
}

Exchange21* OpenStore(int max_size, int expected_customer_orders) {
  Exchange21 * myStore = malloc(sizeof(Exchange21));
  myStore->receipts = NULL;
  myStore->current_size = 0;
  myStore->max_size = max_size;
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
  if(exch21->customers_handled < exch21->expected_customer_orders) {
  }

  pthread_mutex_destroy(&exch21->mutex);
  pthread_cond_destroy(&exch21->can_check_out);
  pthread_cond_destroy(&exch21->ready_to_checkout);
  free(exch21);
  printf("Exchange21 has been closed for the day!\n");
}

void goToCheckout(Exchange21* exch21, Receipt* receipt) {
  pthread_mutex_lock(&exch21->mutex);

  // printf("current: %d max: %d\n", exch21->current_size, exch21->max_size);
  while(isFull(exch21)) {
    pthread_cond_wait(&exch21->can_check_out, &exch21->mutex);
  }

  AddReceiptToBack(&exch21->receipts, receipt);
  exch21->current_size++;
  pthread_cond_signal(&exch21->ready_to_checkout); //signal to cashier that there is someone in line
  pthread_mutex_unlock(&exch21->mutex);
}

Receipt * checkOutCustomer(Exchange21 * exch21) {
  pthread_mutex_lock(&exch21->mutex);

  while(isEmpty(exch21)) {

    if(exch21->customers_handled <= exch21->expected_customer_orders) {
      printf("customers_handled: %d expected_customers: %d\n", exch21->customers_handled, exch21->expected_customer_orders);
      pthread_cond_broadcast(&exch21->can_check_out);
      pthread_mutex_unlock(&exch21->mutex);
      return NULL;
    }
    pthread_cond_wait(&exch21->can_check_out, &exch21->mutex);
  }

  Receipt * receipt = exch21->receipts;
  exch21->receipts = receipt->next;

  //if customer has clothes in their cart consider them handled.
  if(receipt->clothes > 0)
  exch21->customers_handled++;

  exch21->current_size--;

  // printList(exch21);
  pthread_cond_signal(&exch21->can_check_out); //signal that you can add order
  pthread_mutex_unlock(&exch21->mutex);
  return receipt;
}

/* OPTIONAL HELPER FUNCTIONS */

bool isEmpty(Exchange21 * exch21) {
  return(exch21->current_size == 0);
}

bool isFull(Exchange21 * exch21) {
  return(exch21->current_size == exch21->max_size);
}

void AddReceiptToBack(Receipt** receipts, Receipt* receipt) {
  if(*receipts == NULL) {
    receipt->next = NULL;
    *receipts = receipt;
  } else {
    Receipt* current = *receipts;
    while(current->next != NULL) {
      current = current->next;
    }
    current->next = receipt;
    current->next->next = NULL;
  }
}

void printList(Exchange21 * exch21) {
    Receipt* current = exch21->receipts;
    if(current == NULL) {
        printf("Empty list!\n");
        return;
    }
    printf("Receipt:\n");
    printf("start--\n");
    while(current != NULL) {
        printf("[%d] - [%d]\n", current->receipt_number, current->clothes);
        current = current->next;
    }
    printf("--finish\n");
    int length = lengthList(exch21);
    printf("Length of list: [%d] ** orders complete:[%d] out of [%d] total\n", length, exch21->customers_handled, exch21->expected_customer_orders);
}

int lengthList(Exchange21* exch21) {
	int count = 0;
    Receipt* current = exch21->receipts;
    if(current == NULL) { //if list is empty
        return 0;
    }
    while(current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}
