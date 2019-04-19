#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef char* ClothingItem;

typedef struct ClothingStruct {
  ClothingItem clothing_item;
  int quantity;
  struct ClothingStruct *next;
} Clothing;

typedef struct ReceiptStruct {
    int clothes;
    int customer_id;
    int receipt_number;
    struct ReceiptStruct *next;
} Receipt;

typedef struct StoreStruct {
  Receipt* receipts;
  int current_size;
  int max_size;
  int next_customer_number;
  int customers_handled;
  int expected_customer_orders;
  pthread_mutex_t mutex;
  pthread_cond_t ready_to_checkout, can_check_out;
} Exchange21;

//Picks random clothing itme
ClothingItem PickRandomClothingItem();

int ShoppingDecision();

//Open Store
Exchange21* OpenStore(int max_size, int expected_customer_orders);

//Close a Store
void CloseStore(Exchange21* exch21);

//Adds Clothing Item
void goToCheckout(Exchange21* exch21, Receipt* receipt);

//Gets Clothing Item
Receipt *checkOutCustomer(Exchange21* exch21);
