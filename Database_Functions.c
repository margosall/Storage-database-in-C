#include <string.h>

typedef struct product
{
  long productCode;
  char productName[MAX_STRING_LENGTH];
} product;

typedef struct inventory
{
  long productCode;
  long inventoryCount;
} inventory;

typedef struct shopInventory
{
  long productCode;
  long inventoryCount;
  long averageDaySales;
} shopInventory;

typedef struct shop
{
  long shopID;
  shopInventory inventory[MAX_PRODUCTS];
  long productCount;
} shop;

typedef struct shopName
{
  long shopID;
  char shopName[MAX_STRING_LENGTH];
} shopName;

int ReadProducts(char *fileName,product *productStruct);
int ReadInventories(char *fileName,inventory *inventoryStruct);

int FindMinIndex(product *productStruct, int count, int sortedIndex);
void SortProducts(product *productStruct, int count);

long FindProductInventoryAll(inventory *inventoryStruct,shop *shopStruct,long productCode, int storageProductCount, int shopCount);
char *FindProductName(product *productStruct,long productCode,int productCount);
void NecessaryToOrder(product *productStruct,inventory *inventoryStruct,shop *shopStruct, int productCount,int inventoryCount,int shopCount);
void NecessaryToOrderShop(product *productStruct,shop *shopStruct, shopName *shopNameStruct,long productCount, long shopCount, long shopNameCount);
long SumAllDaily(shop *shopStruct,long shopCount,long productCode);

int ReadShops(char *fileName, shop *shopStruct);
int ReadShopNames(char * fileName,shopName *shopNamesStruct);

int FindShopIDIndex(shop *shopStruct,int count,long id);

char* FindShopName(shopName *shopNamesStruct,int id,int shopNamesCount);

int ReadProducts(char *fileName,product *productStruct)
{
  int reading = 0;
  FILE *inputFile = fopen(fileName,"r");
  if(inputFile == NULL)
  {
    printf("%s file not found.\n",fileName);
  }
  else
  {
    printf("%s file opened.\n",fileName);
    while(fscanf(inputFile,"%ld%s",
				&(productStruct+reading)->productCode,
				(productStruct+reading)->productName
			) == 2)
      {
        reading++;
      }
    fclose(inputFile);
  }
  return reading;
}

int ReadInventories(char *fileName,inventory *inventoryStruct)
{
  int reading = 0;
  FILE *inputFile = fopen(fileName,"r");
  if(inputFile == NULL)
  {
    printf("%s file not found.\n",fileName);
  }
  else
  {
    printf("%s file opened.\n",fileName);
    while(fscanf(inputFile,"%ld%ld",
				&(inventoryStruct+reading)->productCode,
				&(inventoryStruct+reading)->inventoryCount
			) == 2)
      {
        reading++;
      }
    fclose(inputFile);
  }
  return reading;
}

int ReadShops(char *fileName, shop *shopStruct)
{
  int reading = 0;
  int shopIndex = 0;
  long shopIDTemp;
  long invIndex;
  int shopsRead = 0;
  FILE *inputFile = fopen(fileName,"r");
  if(inputFile == NULL)
  {
    printf("%s file not found.\n",fileName);
  }
  else
  {
    printf("%s file opened.\n",fileName);
    while(fscanf(inputFile,"%ld",&shopIDTemp) != EOF && 0 < MAX_PRODUCTS)
    {
      shopIndex = FindShopIDIndex(shopStruct,shopsRead,shopIDTemp);
      if(shopIndex == -1)
      {
        shopIndex = shopsRead;
        shopsRead += 1;
        (shopStruct + shopIndex)->shopID = shopIDTemp;
      }
        invIndex = (shopStruct + shopIndex)->productCount;

        if(fscanf(inputFile,"%ld%ld%ld",
        &(shopStruct + shopIndex)->inventory[invIndex].productCode,
        &(shopStruct + shopIndex)->inventory[invIndex].inventoryCount,
        &(shopStruct + shopIndex)->inventory[invIndex].averageDaySales
        ) == 3)
        {
          (shopStruct + shopIndex)->productCount += 1;
        }
      reading++;
    }
    fclose(inputFile);
  }
  return shopsRead;
}

int ReadShopNames(char * fileName,shopName *shopNamesStruct)
{
  int reading = 0;
  FILE *inputFile = fopen(fileName,"r");
  if(inputFile == NULL)
  {
    printf("%s file not found.\n",fileName);
  }
  else
  {
    printf("%s file opened.\n",fileName);
    while(fscanf(inputFile,"%ld %s",&(shopNamesStruct + reading)->shopID,(shopNamesStruct + reading)->shopName) == 2)
    {
      reading++;
    }
  }
  return reading;

}

int FindMinIndex(product *productStruct, int count, int sortedIndex)
{
  product min;
  int index, minIndex;
  min = *(productStruct+sortedIndex);
  minIndex = sortedIndex;
  sortedIndex++;
  for(index = sortedIndex; index < count;index++)
  {
      if(strcmp(min.productName,(productStruct+index) -> productName) > 0)
      {
        //not sorted
        min = *(productStruct+index);
        minIndex = index;
      }
  }
  return minIndex;
}

void SortProducts(product *productStruct, int count)
{
  int sortedIndex=0, minIndex;
  product temp;
  for(sortedIndex = 0;sortedIndex <= count;sortedIndex++)
  {
    minIndex = FindMinIndex(productStruct,count,sortedIndex);
    /*Swap*/
    temp = *(productStruct + sortedIndex);
		*(productStruct + sortedIndex) = *(productStruct + minIndex);
    *(productStruct + minIndex) = temp;
  }
}

long FindProductInventoryAll(inventory *inventoryStruct,shop *shopStruct,long productCode, int storageProductCount, int shopCount)
{
  int shopIndex, productIndex;
  long inventory = 0;

  /*Product inventory for main warehouse*/
  for(productIndex = 0;productIndex < storageProductCount;productIndex++)
  {
    inventory += ((inventoryStruct + productIndex)->productCode == productCode) ? (inventoryStruct + productIndex)->inventoryCount : 0;
  }
  /*Product inventory summing all shops*/
  for(shopIndex = 0;shopIndex < shopCount;shopIndex++)
  {
    for(productIndex = 0;productIndex < (shopStruct + shopIndex)->productCount;productIndex++)
    {
      inventory += ((shopStruct + shopIndex)->inventory[productIndex].productCode == productCode) ? (shopStruct+shopIndex)->inventory[productIndex].inventoryCount : 0;
    }
  }
  return inventory;
}

char* FindProductName(product *productStruct,long productCode,int productCount)
{
  int index;
  static char output[MAX_STRING_LENGTH];
  for(index = 0;index < productCount;index++)
  {
    if((productStruct + index)->productCode == productCode)
    {
      strcpy(output,(productStruct + index)->productName);
    }
  }
  return output;
}

void NecessaryToOrder(product *productStruct,inventory *inventoryStruct,shop *shopStruct, int productCount,int inventoryCount,int shopCount)
{
  int index;
  long tempInventory;
  for(index = 0;index < productCount;index++)
  {
    tempInventory = FindProductInventoryAll(inventoryStruct,shopStruct,(productStruct+index)->productCode,inventoryCount,shopCount);
    printf("You have %ld %ss in your inventory and shops.\n",tempInventory,FindProductName(productStruct,(productStruct+index)->productCode,productCount));
    if(SumAllDaily(shopStruct,shopCount,(productStruct+index)->productCode) > tempInventory)
    {
      printf("Need to order %ld %ss to main storage!\n",SumAllDaily(shopStruct,shopCount,(productStruct+index)->productCode)*3-tempInventory,(productStruct+index)->productName);
    }
  }
}

void NecessaryToOrderShop(product *productStruct,shop *shopStruct, shopName *shopNameStruct,long productCount, long shopCount, long shopNameCount)
{
  int shopIndex, productIndex;
  /*Product inventory summing all shops*/
  for(shopIndex = 0;shopIndex < shopCount;shopIndex++)
  {
    printf("\nSend to %s\n",FindShopName(shopNameStruct,(shopStruct + shopIndex)->shopID,shopNameCount));
    for(productIndex = 0;productIndex < (shopStruct + shopIndex)->productCount;productIndex++)
    {
        if((shopStruct + shopIndex)->inventory[productIndex].averageDaySales * 3 > (shopStruct + shopIndex)->inventory[productIndex].inventoryCount)
        {
          printf("\t%ld %ss\n",(shopStruct + shopIndex)->inventory[productIndex].averageDaySales * 3 - (shopStruct + shopIndex)->inventory[productIndex].inventoryCount,FindProductName(productStruct,(productStruct+productIndex)->productCode,productCount));
        }

    }
  }
}

long SumAllDaily(shop *shopStruct,long shopCount,long productCode)
{
  long output=0;
  int shopIndex, productIndex;
  for(shopIndex = 0;shopIndex < shopCount;shopIndex++)
  {
    for(productIndex = 0;productIndex < (shopStruct + shopIndex)->productCount;productIndex++)
    {
      if((shopStruct + shopIndex)->inventory[productIndex].productCode == productCode)
      {
        output +=  (shopStruct + shopIndex)->inventory[productIndex].averageDaySales;
      }
    }
  }
  return output;
}

char* FindShopName(shopName *shopNamesStruct,int id,int shopNamesCount)
{
  int index;
  static char output[MAX_STRING_LENGTH];
  for(index = 0;index < shopNamesCount;index++)
  {
    if((shopNamesStruct+index)->shopID == id)
    {
      strcpy(output, (shopNamesStruct+index)->shopName);
    }
  }
  return output;
}

int FindShopIDIndex(shop *shopStruct,int count,long id)
{
  int index, found = 0,output;
  for(index = 0;index < count;index++)
  {
    if((shopStruct+index)->shopID == id)
    {

      found = 1;
      output = index;
    }
  }
  if(found == 0)
  {
    output = -1;
  }
  return output;
}
