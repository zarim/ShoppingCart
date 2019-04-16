#include <shopping-cart.h>

#include <assert.h>
#include <stdlib.h>
#include <time.h>
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
