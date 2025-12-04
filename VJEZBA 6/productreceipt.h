#ifndef PRODUCTRECEIPT_H
#define	PRODUCTRECEIPT_H

#define PRODUCT_NAME_MAX 20
#define CREATE_PRODUCT_ERROR -1
#define MEMORY_ALLOC_ERROR NULL
#define FILE_MAX 15
#define FILE_OPEN_ERROR -1
#define CREATE_RECEIPT_ERROR NULL
#define LIST_ERROR NULL
#define CREATE_ERROR -1
#define CHECK_OK 0
#define CHECK_FAIL 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct _product {

	char name[PRODUCT_NAME_MAX];
	int quantity;						// Int zbog uvjeta da se moze kupiti samo cjelina nekog proizvoda, npr. 1 kruh
	double price;

	struct _product* next;

}product;
typedef struct _product* positionProd;


typedef struct _receipt {

	int year;
	int month;
	int day;

	struct _receipt* next;
	positionProd product;

}receipt;
typedef struct _receipt* position;

int insertProduct(position tempHead, char* productBuffer, int quantBuffer, double priceBuffer);
positionProd createProduct(char* productName, int quantity, double price);
int deleteProducts(position tempRec);
int generateList(position Head);
position createReceipt(int yearBuffer, int monthBuffer, int dayBuffer);
int fillReceipt(position tempHead, char* filename);
int printReceipt(position tempRec, int counter);

#endif