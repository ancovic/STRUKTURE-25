#define _CRT_SECURE_NO_WARNINGS

#include "productreceipt.h"

int insertProduct(position tempHead, char* productBuffer, int quantBuffer, double priceBuffer) {

	positionProd newProduct = createProduct(productBuffer, quantBuffer, priceBuffer);
	if (newProduct == NULL) {
		printf("\nERROR: Greska u stvaranju proizvoda (insertProduct).\n");
		return CREATE_PRODUCT_ERROR;
	}

	if (tempHead->product == NULL) {
		tempHead->product = newProduct;
		newProduct->next = NULL;
		return EXIT_SUCCESS;
	}

	positionProd checker = tempHead->product;
	positionProd prev = NULL;

	while (checker != NULL) {
		if (strcmp(checker->name, newProduct->name) == 0) {
			checker->quantity += newProduct->quantity;

			newProduct->next = NULL;
			free(newProduct);

			return EXIT_SUCCESS;
		}

		prev = checker;
		checker = checker->next;
	}

	prev->next = newProduct;
	newProduct->next = NULL;

	return EXIT_SUCCESS;
}

positionProd createProduct(char* productName, int quantity, double price) {

	positionProd newProduct = (positionProd)malloc(sizeof(product));
	if (newProduct == NULL) {
		printf("\nERROR: Greska u alociranju memorije (createProduct).\n");
		return MEMORY_ALLOC_ERROR;
	}

	strcpy(newProduct->name, productName);
	newProduct->quantity = quantity;
	newProduct->price = price;
	newProduct->next = NULL;

	return newProduct;
}

int deleteProducts(position tempRec) {

	positionProd tempProd = tempRec->product;
	positionProd currentProd = NULL;

	while (tempProd != NULL) {

		currentProd = tempProd;
		tempProd = tempProd->next;

		currentProd->next = NULL;
		free(currentProd);

	}

	tempRec->product = NULL;

	return EXIT_SUCCESS;
}

int generateList(position Head) {

	FILE* fp = fopen("RACUNI.txt", "r");
	if (fp == NULL) {
		printf("\nERROR: Greska u otvaranju file-a (RACUNI.txt / generateList).\n");
		return FILE_OPEN_ERROR;
	}

	char filenameBuffer[FILE_MAX] = { '\0' };

	while (fscanf(fp, " %s", filenameBuffer) == 1)
		fillList(Head, filenameBuffer);

	fclose(fp);

	return EXIT_SUCCESS;
}

position createReceipt(int yearBuffer, int monthBuffer, int dayBuffer) {

	position newReceipt = (position)malloc(sizeof(receipt));
	if (newReceipt == NULL) {
		printf("\nERROR: Greska u stvaranju racuna (createReceipt).\n");
		return MEMORY_ALLOC_ERROR;
	}

	newReceipt->year = yearBuffer;
	newReceipt->month = monthBuffer;
	newReceipt->day = dayBuffer;
	newReceipt->next = NULL;
	newReceipt->product = NULL;

	return newReceipt;
}

int fillReceipt(position tempHead, char* filename) {	// Funkcija radi na pretpostavci da su svi nazivi jedna rijec

	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("\nERROR: Greska u otvaranju file-a (fillReceipt).\n");
		return FILE_OPEN_ERROR;
	}

	char firstLineBuffer[100] = { '\0' };

	fgets(firstLineBuffer, 100, fp);		// Dovedemo fp na kraj prvog retka, tj. pocetak drugog - preskocimo prvi red

	char productBuffer[PRODUCT_NAME_MAX] = { '0' };
	int quantBuffer = 0;
	double priceBuffer = 0;

	while (fscanf(fp, "%s %d %lf", productBuffer, &quantBuffer, &priceBuffer) == 3) {
		insertProduct(tempHead, productBuffer, quantBuffer, priceBuffer);
	}

	fclose(fp);

	return EXIT_SUCCESS;
}

int printReceipt(position tempRec, int counter) {

	if (counter != 0)
		printf("\nRacun %d:\n\t%d-%d-%d\n\n", counter, tempRec->year, tempRec->month, tempRec->day);
	else
		printf("\nRacun:\n\t%d-%d-%d\n\n", tempRec->year, tempRec->month, tempRec->day);

	positionProd tempProd = tempRec->product;

	while (tempProd != NULL) {
		printf("\t%s %d %.2lf\n", tempProd->name, tempProd->quantity, tempProd->price);

		tempProd = tempProd->next;
	}

	return EXIT_SUCCESS;
}