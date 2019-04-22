#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "shopping-cart.h"


#define EXCHANGE21_SIZE 100
#define STOCK 35
#define NUM_CUSTOMERS 10
#define NUM_CASHIERS 1
#define ENTRIES_PER_CUSTOMER 5
#define POTENTIAL_ORDERS NUM_CUSTOMERS * ENTRIES_PER_CUSTOMER

Exchange21 * exch21;

//Thread function for Exchange21Customer
void* Exchange21Customer(void* tid) {

  Receipt * myReceipt = malloc(sizeof(Receipt));

  //each customer has X attemps to find items they like in the store
  for(int i = 0; i < ENTRIES_PER_CUSTOMER; i++) {

      int customer_id = (int)(long) tid;
      myReceipt->customer_id = customer_id;
      int decision = ShoppingDecision();

      // force the customer to checkout if it's their last entry in the store
      if(i == ENTRIES_PER_CUSTOMER-1)
        decision = 1;

      switch (decision) {
        case 0: //add to cart
            myReceipt->clothes++;
            exch21->closingStock--;
            printf("Customer #[%d] found something they liked - they now have [%d] item(s) in their cart.\n", customer_id, myReceipt->clothes);
          break;

        case 1: //see cashier
          if(myReceipt->clothes == 0) {
            exch21->customers_handled++;
            goToCheckout(exch21, myReceipt);
            return NULL;
          } else {
            exch21->customers_handled++;
            goToCheckout(exch21, myReceipt);
            return NULL;
          }
          break;
      }
  }
  return NULL;
}

//Thread function for Exchange21Cashier
void* Exchange21Cashier(void* tid) {
  Receipt * receipt = checkOutCustomer(exch21);

  while(receipt != NULL) {
      if(receipt->clothes > 0)
        printf("Checking out Customer #[%d] with their [%d] items.\n", receipt->customer_id, receipt->clothes);
      else
        printf("Customer #[%d] only needed assistance, they're gone now.\n", receipt->customer_id);
      free(receipt);
      receipt = checkOutCustomer(exch21);
    }

  return NULL;
}

int main() {
    srand(time(0));
    pthread_t customers[NUM_CUSTOMERS]; //customer threads
    pthread_t cashiers[NUM_CASHIERS]; //cashier threads
    exch21 = OpenStore(STOCK, POTENTIAL_ORDERS, NUM_CUSTOMERS, EXCHANGE21_SIZE);

    //Create customer and cashier threads
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_create(&customers[i], NULL, Exchange21Customer, (void*)(long)i);
    }
    for (int i = 0; i < NUM_CASHIERS; i++) {
        pthread_create(&cashiers[i], NULL, Exchange21Cashier, (void*)(long)i);
    }

    //Wait for customers and cashiers
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(customers[i], NULL);
    }
    for (int i = 0; i < NUM_CASHIERS; i++) {
        pthread_join(cashiers[i], NULL);
    }

    CloseStore(exch21);
    return 0;
}
