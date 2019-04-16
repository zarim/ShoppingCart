#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef char* ClothingItem;
typedef char* ClothingSize;


typedef struct ClothingStruct {
  ClothingItem clothing_item;
  ClothingSize clothing_size;
  int clothing_id;
  int quantity;
  struct ClothingStruct *next;
} Clothing;

typedef struct StoreStruct {
  Clothing* clothes;
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

//Open Store
Exchange21* OpenStore(int max_size, int expected_customer_orders);

//Close a Store
void CloseStore(Exchange21* exch21);

//Adds Clothing Item
int AddClothingItem(Exchange21* exch21, Clothing* clothes);

//Gets Clothing Item
Clothing *GetClothingItem(Exchange21* exch21);
