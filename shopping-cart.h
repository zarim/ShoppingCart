#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct ReceiptStruct {
    int clothes;
    int customer_id;
    struct ReceiptStruct *next;
} Receipt;

typedef struct StoreStruct {
  Receipt* receipts;
  int openingStock;
  int closingStock;
  int customers_handled;
  int customers;
  int potential_orders;
  int current_size;
  int max_size;
  pthread_mutex_t mutex;
  pthread_cond_t ready_to_checkout, can_check_out;
} Exchange21;

//Poicks random ShoppingDecision
int ShoppingDecision();

//Open Store
Exchange21* OpenStore(int storeStock, int num_customers, int max);

//Close a Store
void CloseStore(Exchange21* exch21);

//Adds Clothing Item
void goToCheckout(Exchange21* exch21, Receipt* receipt);

//Gets Clothing Item
Receipt *checkOutCustomer(Exchange21* exch21);
