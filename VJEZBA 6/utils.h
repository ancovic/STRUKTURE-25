#ifndef UTILS_H
#define UTILS_H

#include "productreceipt.h"

// Pomocne funkcije kao dio vecih

int fillList(position Head, char* filename);
position sortedReceiptInput(position Head, int yearBuffer, int monthBuffer, int dayBuffer);
int fillCustomReceipt(position Head, position customReceipt);
int priceChanger(position Head, char* prodNameBuffer, double priceBuffer, double prevPrice);
int addNewOrUpdateProd(position tempHead, char* prodName, int prodQuant);
int findMostBought(position tempHead);
int totalPriceAndQuant(position start, position end, char* prodNameBuffer, int* quantCounter, double* totalPrice);
int equalProductChecker(position Head, char* prodNameBuffer, double priceBuffer);

// Opcenite pomocne funkcije

int deleteList(position Head);
int lwrCase(char* string);
int checkDate(int month, int day);
double priceChangeCalc(double oldPrice, double newPrice);
double receiptTotal(position tempRec);
int compareDates(position temp, position newEl);
int getStartDate(int* startYearBuffer, int* startMonthBuffer, int* startDayBuffer);
int getEndDate(int* endYearBuffer, int* endMonthBuffer, int* endDayBuffer);
double getPrice();

// Glavna funkcija za UI

int userInterface(position Head);

#endif