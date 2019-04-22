#include <shopping-cart.h>

#include <assert.h>
#include <stdlib.h>
#include <time.h>

/* helper function prototypes */
bool IsEmpty(Exchange21 * exch21);
void printList(Exchange21 * exch21);
bool IsFull(Exchange21 * exch21);
void AddReceiptToBack(Receipt** receipts, Receipt* receipt);
int lengthList(Exchange21 * exch21);
void kickOut(Exchange21 * exch21);

int ShoppingDecision() {
  int decision = rand() % 2;
  return decision;
}

Exchange21* OpenStore(int currentStock, int potential_orders, int num_customers, int max) {
  Exchange21 * myStore = malloc(sizeof(Exchange21));
  myStore->receipts = NULL;
  myStore->openingStock = currentStock;
  myStore->closingStock = currentStock;
  myStore->customers_handled = 0;
  myStore->customers = num_customers;
  myStore->potential_orders = potential_orders;
  myStore->current_size = 0;
  myStore->max_size = max;

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

  printf("Exchange21 has opened with a stock of [%d] items!\n", myStore->openingStock);
  return myStore;
}

void CloseStore(Exchange21* exch21) {
  if(exch21->receipts != NULL && exch21->customers_handled == exch21->customers) {
    printList(exch21);
    printf("You can't close the store with customers in line!\n");
    return;
  }
  printf("Exchange21 opened with a stock of [%d] and is closing with a stock of [%d]\n", exch21->openingStock, exch21->closingStock);
  pthread_mutex_destroy(&exch21->mutex);
  pthread_cond_destroy(&exch21->can_check_out);
  pthread_cond_destroy(&exch21->ready_to_checkout);
  free(exch21);
  return;
}

void goToCheckout(Exchange21* exch21, Receipt* receipt) {
  pthread_mutex_lock(&exch21->mutex);


  while(IsFull(exch21)) {
    pthread_cond_wait(&exch21->ready_to_checkout, &exch21->mutex);
  }

  //if customer has clothes in their cart consider them handled.
  if(receipt->clothes > 0)
    printf("Customer #[%d] is in line with [%d] item(s).\n", receipt->customer_id, receipt->clothes);

  // printList(exch21);

  AddReceiptToBack(&exch21->receipts, receipt);
  exch21->current_size++;
  pthread_cond_signal(&exch21->can_check_out); //signal to cashier that there is someone in line
  pthread_mutex_unlock(&exch21->mutex);
}

Receipt * checkOutCustomer(Exchange21 * exch21) {
  pthread_mutex_lock(&exch21->mutex);

  while(IsEmpty(exch21)) {
    if(exch21->customers_handled == exch21->customers) {
      pthread_cond_broadcast(&exch21->can_check_out);
      pthread_mutex_unlock(&exch21->mutex);
      return NULL;
    }
      pthread_cond_wait(&exch21->can_check_out, &exch21->mutex);
  }
  
  // assert(!IsEmpty(exch21));
  // printList(exch21);

  Receipt* receipt = exch21->receipts;
  exch21->receipts = receipt->next;
  exch21->current_size--;
  pthread_cond_signal(&exch21->ready_to_checkout); //signal that you can add order
  pthread_mutex_unlock(&exch21->mutex);
  return receipt;
}

bool IsEmpty(Exchange21* exch21) {
    return (exch21->current_size == 0);
}

bool IsFull(Exchange21* exch21) {
    return (exch21->current_size == exch21->max_size);
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
    // printf("Receipt:\n");
    printf("\nstart--\n");
    while(current != NULL) {
        printf("Customer #[%d] - items: [%d]\n", current->customer_id, current->clothes);
        current = current->next;
    }
    printf("--finish\n");
    int length = lengthList(exch21);
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
