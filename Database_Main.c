#include <stdio.h>

#define MAX_STRING_LENGTH 100
#define MAX_PRODUCTS 500
#define MAX_SHOPS 100

#include "Database_Functions.c"

#define PRODUCT_FILE "products.txt"
#define STORAGE_FILE "storage.txt"
#define SHOP_FILE "shops.txt"
#define SHOPNAME_FILE "shopnames.txt"
int main(void)
{
  product products[MAX_PRODUCTS];
  inventory inventories[MAX_PRODUCTS];
  shop shops[MAX_SHOPS];
  shopName shopNames[MAX_SHOPS];
  int readProductsCount, inventoryProductsCount, shopsCount,shopNamesCount;

  readProductsCount = ReadProducts(PRODUCT_FILE, products);
  inventoryProductsCount = ReadInventories(STORAGE_FILE, inventories);
  shopNamesCount = ReadShopNames(SHOPNAME_FILE,shopNames);
  shopsCount = ReadShops(SHOP_FILE,shops);

  //printf("%ld\n",(shops+0)->inventory[0].productCode);
  //SortProducts(products,readProductsCount);
  printf("Shopcount %d\n",shopsCount);
  NecessaryToOrder(products,inventories,shops,readProductsCount,inventoryProductsCount,shopsCount);
  NecessaryToOrderShop(products,shops,shopNames,readProductsCount,shopsCount,shopNamesCount);
  //printf("%s %ld\n",(products + 0)->productName,FindProductInventory(inventories,(products + 0)->productCode,readProductsCount));
  return 0;
}
