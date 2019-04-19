#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "shopping-cart.h"

//we need to define whatever values we're going to put in the store...
//the size,
//customers
//cashiers
//orders_per_customer
//expected_customer_orders = customers * orders per customer
#define EXCHANGE21_SIZE 100
#define NUM_CUSTOMERS 10
#define NUM_CASHIERS 5
#define ENTRIES_PER_CUSTOMER 10
#define EXPECTED_NUM_ORDERS NUM_CUSTOMERS * ENTRIES_PER_CUSTOMER

//OR ^^ IF WE WANT, we could make it to where the user can choose
//all of these values in the main.
//"Welcome to your store, how many customers do you want today?, "cashiers", etc. etc

Exchange21 * exch21;

//Thread function for Exchange21Customer
void* Exchange21Customer(void* tid) {
  Receipt * myReceipt = malloc(sizeof(Receipt));
  for(int i = 0; i < ENTRIES_PER_CUSTOMER; i++) {
      int customer_id = (int)(long) tid;
      myReceipt->customer_id = customer_id;
      int decision = ShoppingDecision();
      // printf("decision [%d] - customer[%d]\n", decision, customer_id);

      // force the customer to checkout if it's their last entry in the store
      if(i == ENTRIES_PER_CUSTOMER-1)
        decision = 2;

      switch (decision) {
        case 0: //add to cart/receipt
            myReceipt->clothes++;
            printf("Customer[%d] found something they liked - they now have [%d] item(s) in their cart.\n", customer_id, myReceipt->clothes);
          break;

        case 1: //leave
          if(myReceipt->clothes == 0) {
            printf("Customer[%d] entered the store and left without buying anything.\n", customer_id);
            free(myReceipt);
            return NULL;
          }
          else {
            printf("Customer[%d] has items in their cart, they are not allowed to leave in your cart!\n", customer_id);
            return NULL;
          }
          break;
        case 2:
          goToCheckout(exch21, myReceipt);
          printf("Customer[%d] is in line to check out.\n", customer_id);
          return NULL;
          break;
      }
      // int order_number = AddOrder(mcg, myOrder);
  }
  return NULL;
}



//Thread function for Exchange21Cashier
void* Exchange21Cashier(void* tid) {

  return NULL;
}


int main() {
    srand(time(0));
    pthread_t customers[NUM_CUSTOMERS]; //customer threads
    pthread_t cashiers[NUM_CASHIERS]; //cashier threads
    exch21 = OpenStore(EXCHANGE21_SIZE, EXPECTED_NUM_ORDERS);

    //Create customer and cashier threads
    for (int i = 1; i <= NUM_CUSTOMERS; i++) {
        pthread_create(&customers[i], NULL, Exchange21Customer, (void*)(long)i);
    }
    for (int i = 1; i <= NUM_CASHIERS; i++) {
        pthread_create(&cashiers[i], NULL, Exchange21Cashier, (void*)(long)i);
    }

    //Wait for customers and cashiers
    for (int i = 1; i <= NUM_CUSTOMERS; i++) {
        pthread_join(customers[i], NULL);
    }
    for (int i = 1; i <= NUM_CASHIERS; i++) {
        pthread_join(cashiers[i], NULL);

    CloseStore(exch21);

    return 0;
}
}
