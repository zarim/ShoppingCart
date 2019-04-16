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

//OR ^^ IF WE WANT, we could make it to where the user can choose
//all of these values in the main.
//"Welcome to your store, how many customers do you want today?, "cashiers", etc. etc




//Thread function for Exchange21Customer
void* Exchange21Customer(void* tid) {
  //for(int i = 0; i < ORDERS_PER_CUSTOMER; i++) {

    int customer_id = (int)(long) tid;
    // randomly choose 1 of 3 options while in the forloop.

    //should we make another struct called order to hold all the clothes the customer may decide to buy? - it'll be another linked list.
    //that way when we check out, we can go through the orders of each customer which also
    //have the clothes they decided to keep (another linked list, the clothes one we already have)
    //just an idea... some of variable names are a bit messsy/confusing at the moment.
    //lemme know what you think bout this tho

    //that's just me brainstorming. the more i think bout this,
    //the more potential issues i see we'll need to fix later
    //let me know what you think, though. you can revise or start implementing.

    // }
  return NULL;
}




//Thread function for Exchange21Cashier
void* Exchange21Cashier(void* tid) {

  return NULL;
}


int main() {
    printf("Hello World\n");
    return 0;
}
