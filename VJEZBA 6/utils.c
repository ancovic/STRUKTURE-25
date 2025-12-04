#define _CRT_SECURE_NO_WARNINGS

#include "utils.h"

// Pomocne funkcije kao dio vecih

int fillList(position Head, char* filename) {

	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("\nERROR: Greska u otvaranju file-a (fillList).\n");
		return FILE_OPEN_ERROR;
	}

	int yearBuffer = 0, monthBuffer = 0, dayBuffer = 0;

	position tempHead = NULL;

	if (fscanf(fp, "%d-%d-%d", &yearBuffer, &monthBuffer, &dayBuffer) == 3) {

		if (checkDate(monthBuffer, dayBuffer) == CHECK_FAIL) {
			printf("\nDatum u file-u %s je neispravan.\n", filename);
			return EXIT_FAILURE;
		}

		tempHead = sortedReceiptInput(Head, yearBuffer, monthBuffer, dayBuffer);
	}

	fillReceipt(tempHead, filename);

	fclose(fp);

	return EXIT_SUCCESS;
}

position sortedReceiptInput(position Head, int yearBuffer, int monthBuffer, int dayBuffer) {	// Sortira racune silazno - prvi je najnoviji

	position newReceipt = createReceipt(yearBuffer, monthBuffer, dayBuffer);
	if (newReceipt == NULL) {
		printf("\nERROR: Greska u stvaranju novog racuna (sortedReceiptInput).\n");
		return CREATE_RECEIPT_ERROR;
	}

	if (Head->next == NULL) {
		Head->next = newReceipt;
		newReceipt->next = NULL;

		return newReceipt;
	}

	position temp = Head->next;
	position prev = Head;

	while (temp != NULL) {

		int cmpValue = compareDates(temp, newReceipt);

		if (cmpValue > 0) {
			newReceipt->next = temp;
			prev->next = newReceipt;
			break;
		}

		if (cmpValue == 0) {
			newReceipt->next = temp;
			prev->next = newReceipt;
			break;
		}

		prev = temp;
		temp = temp->next;
	}

	if (temp == NULL) {
		prev->next = newReceipt;
		newReceipt->next = NULL;
	}

	return newReceipt;
}

int fillCustomReceipt(position Head, position customReceipt) {

	positionProd tempProd = customReceipt->product;

	char prodNameBuffer[PRODUCT_NAME_MAX] = { '\0' };
	int quantBuffer = 0;
	double priceBuffer = 0;
	char choice = '\0';

	do {

		printf("\nUpisite naziv (jedna rijec) i kolicinu (NAZIV KOLICINA): ");
		scanf(" %s %d", prodNameBuffer, &quantBuffer);
		printf("\nUnesite cijenu (s tockom): ");
		priceBuffer = getPrice();

		if (equalProductChecker(Head, prodNameBuffer, priceBuffer) == CHECK_OK) {
			insertProduct(customReceipt, prodNameBuffer, quantBuffer, priceBuffer);
		}
		else {
			printf("\nUneseni proizvod vec postoji, a cijene se ne poklapaju. Molimo pokusajte ponovno.\n");
			return EXIT_FAILURE;
		}

		printf("\nZelite li nastaviti sa unosom proizvoda (y/n): ");
		scanf(" %c", &choice);
		choice = tolower(choice);		// U slucaju da korisnik unese Y ili N

		while (choice != 'y' && choice != 'n') {
			printf("\nNepoznata opcija: molimo odaberite ponovno: ");
			scanf(" %c", &choice);
			choice = tolower(choice);
		}

	} while (choice == 'y' && choice != 'n');

	return EXIT_SUCCESS;
}

int priceChanger(position Head, char* prodNameBuffer, double priceBuffer, double prevPrice) {

	position tempRec = Head->next;

	char tempCheck[PRODUCT_NAME_MAX] = { '\0' };

	while (tempRec != NULL) {

		positionProd tempProd = tempRec->product;

		while (tempProd != NULL) {

			strcpy(tempCheck, tempProd->name);
			lwrCase(tempCheck);

			if (strcmp(tempCheck, prodNameBuffer) == 0)
				tempProd->price = priceBuffer;

			tempProd = tempProd->next;
		}

		tempRec = tempRec->next;
	}

	printf("\nCijene proizvoda uspjesno promijenjene. Cijena proizvoda se promijenila za %.1lf%c\n", priceChangeCalc(prevPrice, priceBuffer), '%');

	return EXIT_SUCCESS;
}

int addNewOrUpdateProd(position tempHead, char* prodName, int prodQuant) {

	positionProd newEl = createProduct(prodName, prodQuant, 0);
	if (newEl == NULL) {
		printf("\nERROR: Greska u stvaranju proizvoda (addNewOrUpdateProd).\n");
		return EXIT_FAILURE;
	}

	if (tempHead->product == NULL) {
		tempHead->product = newEl;
		newEl->next = NULL;

		return EXIT_SUCCESS;
	}

	positionProd tempProd = tempHead->product;
	positionProd prevProd = NULL;

	while (tempProd != NULL) {

		if (strcmp(tempProd->name, newEl->name) == 0) {
			tempProd->quantity += newEl->quantity;
			free(newEl);

			return EXIT_SUCCESS;
		}

		prevProd = tempProd;
		tempProd = tempProd->next;
	}

	if (tempProd == NULL) {
		prevProd->next = newEl;
		newEl->next = NULL;
	}

	return EXIT_SUCCESS;
}

int findMostBought(position tempHead) {

	positionProd tempProd = tempHead->product;
	positionProd largestTemp = tempProd;

	if (tempHead->product == NULL) {
		printf("\nLista proizvoda je prazna.\n");
		return EXIT_FAILURE;
	}

	while (tempProd != NULL) {
		if (tempProd->quantity > largestTemp->quantity)
			largestTemp = tempProd;

		tempProd = tempProd->next;
	}

	printf("\nProizvod koji je kupljen najvise puta je %s i kupljeno je %d komada.\n", largestTemp->name, largestTemp->quantity);

	return EXIT_SUCCESS;
}

int totalPriceAndQuant(position start, position end, char* prodNameBuffer, int* quantCounter, double* totalPrice) {

	char tempProdBuffer[PRODUCT_NAME_MAX] = { '\0' };
	position current = end;
	int check = 0;

	while (current != start->next) {

		positionProd tempProd = current->product;

		while (tempProd != NULL) {

			strcpy(tempProdBuffer, tempProd->name);
			lwrCase(tempProdBuffer);

			if (strcmp(tempProdBuffer, prodNameBuffer) == 0) {

				*totalPrice += tempProd->price * tempProd->quantity;
				*quantCounter += tempProd->quantity;
				check = 1;
			}

			tempProd = tempProd->next;
		}

		current = current->next;
	}

	return check;
}

int equalProductChecker(position Head, char* prodNameBuffer, double priceBuffer) {		// Provjerava postoji li vec uneseni proizvod
	// Ako postoji provjerava je li unesena tocna cijena
	position tempRec = Head->next;
	if (tempRec == NULL) {
		printf("\nLista je prazna.");
		return EXIT_FAILURE;
	}

	char prodCheck[PRODUCT_NAME_MAX] = { '\0' };
	char tempCheck[PRODUCT_NAME_MAX] = { '\0' };

	strcpy(prodCheck, prodNameBuffer);
	lwrCase(prodCheck);

	while (tempRec != NULL) {

		positionProd tempProd = tempRec->product;

		while (tempProd != NULL) {

			strcpy(tempCheck, tempProd->name);
			lwrCase(tempCheck);

			if ((strcmp(prodCheck, tempCheck) == 0) && priceBuffer == tempProd->price)
				return CHECK_OK;
			else if ((strcmp(prodCheck, tempCheck) == 0) && priceBuffer != tempProd->price)
				return CHECK_FAIL;
			else
				tempProd = tempProd->next;
		}

		tempRec = tempRec->next;
	}

	return CHECK_OK;
}

// Opcenite pomocne funkcije

int deleteList(position Head) {

	position tempRec = Head->next;
	position currentRec = Head;

	while (tempRec != NULL) {

		deleteProducts(tempRec);

		currentRec = tempRec;
		tempRec = tempRec->next;

		currentRec->next = NULL;
		free(currentRec);
	}

	Head->next = NULL;

	return EXIT_SUCCESS;
}

int lwrCase(char* string) {

	for (int i = 0; i < strlen(string); i++)
		string[i] = tolower(string[i]);

	return 0;
}

int checkDate(int month, int day) {		// Provjerava je li datum koji se zeli unijeti ispravan

	if (month < 1 || month > 12)
		return CHECK_FAIL;
	if (day < 1 || day > 31)
		return CHECK_FAIL;

	return CHECK_OK;
}

double priceChangeCalc(double oldPrice, double newPrice) {
	return ((newPrice - oldPrice) / oldPrice) * 100;
}

double receiptTotal(position tempRec) {

	positionProd tempProd = tempRec->product;

	double totalBuffer = 0;

	while (tempProd != NULL) {
		totalBuffer += (tempProd->quantity * tempProd->price);
		tempProd = tempProd->next;
	}

	return totalBuffer;
}

int compareDates(position temp, position newEl) {		// Usporedjuje datume za sortirani unos - vraca >0 ako je newEl noviji od temp-a,
	// <0 ako je stariji i 0 ako su datumi jednaki

	if (newEl->year != temp->year)
		return (newEl->year - temp->year);

	else if (newEl->month != temp->month)
		return (newEl->month - temp->month);

	else
		return (newEl->day - temp->day);

}

int getStartDate(int* startYearBuffer, int* startMonthBuffer, int* startDayBuffer) {

	int checker = 0;

	do {
		printf("\nUnesite pocetni datum: ");
		scanf(" %d-%d-%d", startYearBuffer, startMonthBuffer, startDayBuffer);

		checker = checkDate(*startMonthBuffer, *startDayBuffer);
		if (checker == CHECK_FAIL)
			printf("\nDatum nije ispravno unesen. Pokusajte ponovno.\n");

	} while (checker != CHECK_OK);

	return EXIT_SUCCESS;
}

int getEndDate(int* endYearBuffer, int* endMonthBuffer, int* endDayBuffer) {

	int checker = 0;

	do {
		printf("\nUnesite zavrsni datum: ");
		scanf(" %d-%d-%d", endYearBuffer, endMonthBuffer, endDayBuffer);

		checker = checkDate(*endMonthBuffer, *endDayBuffer);
		if (checker == CHECK_FAIL)
			printf("\nDatum nije ispravno unesen. Pokusajte ponovno.\n");

	} while (checker != CHECK_OK);

	return EXIT_SUCCESS;
}

double getPrice() {

	char buffer[50];
	double value = 0;

	scanf(" %49s", buffer);

	for (int i = 0; buffer[i] != '\0'; i++) {

		if (buffer[i] == ',')
			buffer[i] = '.';
	}

	value = atof(buffer);

	return value;
}

// Glavna funkcija za UI

int userInterface(position Head) {

	int choice = 0;

	do {
		printf("\nOdaberite zeljenu opciju (utipkajte tocan broj):\n\t1 - Ispis trazenog racuna\n\t2 - Ispis svih racuna"
			"\n\t3 - Pronadi najskuplji racun\n\t4 - Pronadi najjeftiniji racun\n\t5 - Pronadi najskuplji proizvod"
			"\n\t6 - Pronadi najjeftiniji proizvod\n\t7 - Unesi custom racun\n\t8 - Izbrisi jedan racun"
			"\n\t9 - Promijeni cijenu nekog proizvoda\n\t10 - Ukupno potroseno u periodu"
			"\n\t11 - Pronadji najprodavaniji proizvod\n\n\t0 - Izlaz\n");

		printf("\nOdabir: ");
		scanf(" %d", &choice);

		switch (choice) {

		case 1:
			printWanted(Head);
			break;

		case 2:
			printAll(Head);
			break;

		case 3:
			findMostExpensiveReceipt(Head);
			break;

		case 4:
			findLeastExpensiveReceipt(Head);
			break;

		case 5:
			findMostExpensiveProduct(Head);
			break;

		case 6:
			findLeastExpensiveProduct(Head);
			break;

		case 7:
			insertCustomReceipt(Head);
			break;

		case 8:
			deleteWantedReceipt(Head);
			break;

		case 9:
			changeProdPrice(Head);
			break;

		case 10:
			timespanTotal(Head);
			break;

		case 11:
			mostBoughtProduct(Head);
			break;

		case 0:
			printf("\nIzlaz...\n");
			break;

		default:
			printf("\nOpcija ne postoji. Molimo pokusajte ponovno.\n");
			break;
		}
	} while (choice != 0);

	return EXIT_SUCCESS;
}