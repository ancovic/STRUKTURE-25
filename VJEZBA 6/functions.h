#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "utils.h"

// Funkcije (opcije) za racune

int findMostExpensiveReceipt(position Head);
int findLeastExpensiveReceipt(position Head);
int insertCustomReceipt(position Head);
int deleteWantedReceipt(position Head);

// Funkcije (opcije) za proizvode

int changeProdPrice(position Head);
int timespanTotal(position Head);
int mostBoughtProduct(position Head);
int findMostExpensiveProduct(position Head);
int findLeastExpensiveProduct(position Head);

// Opcenite funkcije

int printAll(position Head);
int printWanted(position Head);

#endif