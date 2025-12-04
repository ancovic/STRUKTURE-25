#define _CRT_SECURE_NO_WARNINGS

#include "functions.h"

// Funkcije (opcije) za racune

int findMostExpensiveReceipt(position Head) {

	position tempRec = Head->next;
	positionProd tempProd = tempRec->product;
	position MERec = tempRec;

	double totalPrice = 0;
	double MEBuffer = 0;

	while (tempRec != NULL) {

		if ((totalPrice = receiptTotal(tempRec)) > MEBuffer) {
			MERec = tempRec;
			MEBuffer = totalPrice;
		}

		tempRec = tempRec->next;
	}

	printf("\nNajskuplji racun:\n");
	printReceipt(MERec, 0);
	printf("\nUkupni iznos racuna: %.2lf eura\n", MEBuffer);

	return EXIT_SUCCESS;
}

int findLeastExpensiveReceipt(position Head) {

	position tempRec = Head->next;
	positionProd tempProd = tempRec->product;
	position LERec = tempRec;

	double totalPrice = 0;
	double LEBuffer = receiptTotal(tempRec);

	while (tempRec != NULL) {

		if ((totalPrice = receiptTotal(tempRec)) < LEBuffer) {
			LERec = tempRec;
			LEBuffer = totalPrice;
		}

		tempRec = tempRec->next;
	}

	printf("\nNajjeftiniji racun:\n");
	printReceipt(LERec, 0);
	printf("\nUkupni iznos racuna: %.2lf eura\n", LEBuffer);

	return EXIT_SUCCESS;

}

int insertCustomReceipt(position Head) {

	int yearBuffer = 0, monthBuffer = 0, dayBuffer = 0;
	position customReceipt = NULL;

	printf("\nUnesite datum novog racuna (YYYY-MM-DD): ");
	scanf(" %d-%d-%d", &yearBuffer, &monthBuffer, &dayBuffer);

	if (checkDate(monthBuffer, dayBuffer) == CHECK_OK) {

		customReceipt = sortedReceiptInput(Head, yearBuffer, monthBuffer, dayBuffer);
		if (customReceipt == NULL) {
			printf("\nERROR: Greska u stvaranju racuna (insertCustomReceipt).\n");
			return CREATE_ERROR;
		}
	}
	else {
		printf("\nUneseni datum je neispravan. Pokusajte ponovno.\n");
		return EXIT_FAILURE;
	}

	if (fillCustomReceipt(Head, customReceipt) == EXIT_FAILURE) {
		free(customReceipt);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int deleteWantedReceipt(position Head) {

	int yearBuffer = 0, monthBuffer = 0, dayBuffer = 0;

	printf("\nUpisite datum racuna kojeg zelite izbrisati (YYYY-MM-DD): ");
	scanf(" %d-%d-%d", &yearBuffer, &monthBuffer, &dayBuffer);

	if (checkDate(monthBuffer, dayBuffer) == CHECK_FAIL) {
		printf("\nUneseni datum je neispravan. Pokusajte ponovno.\n");
		return EXIT_FAILURE;
	}

	position cmpBuffer = createReceipt(yearBuffer, monthBuffer, dayBuffer);
	if (cmpBuffer == NULL) {
		printf("\nERROR: Greska u stvaranju racuna (deleteWantedReceipt).\n");
		return CREATE_ERROR;
	}

	if (Head->next == NULL) {
		printf("\nLista je prazna.\n");
		return EXIT_FAILURE;
	}

	position temp = Head->next;
	position prev = Head;

	while (temp != NULL) {

		if (compareDates(temp, cmpBuffer) == 0) {

			deleteProducts(temp);

			prev->next = temp->next;
			temp->next = NULL;

			free(temp);
			free(cmpBuffer);

			printf("\nRacun je uspjesno izbrisan.\n");
			return EXIT_SUCCESS;
		}

		prev = temp;
		temp = temp->next;
	}


	printf("\nRacun koji zelite izbrisati ne postoji.\n");
	free(cmpBuffer);

	return EXIT_FAILURE;
}

// Funkcije (opcije) za proizvode

int changeProdPrice(position Head) {

	char prodNameBuffer[PRODUCT_NAME_MAX] = { '\0' };
	char tempCheck[PRODUCT_NAME_MAX] = { '\0' };
	double priceBuffer = 0;
	double prevPrice = 0;

	printf("\nUpisite ime proizvoda kojem zelite promijeniti cijenu: ");
	scanf(" %s", prodNameBuffer);
	lwrCase(prodNameBuffer);

	if (Head->next == NULL) {
		printf("\nLista je prazna.\n");
		return EXIT_FAILURE;
	}

	position tempRec = Head->next;

	while (tempRec != NULL) {		// Petlja potrebna zbog provjere postoji li proizvod uopce, a ako postoji onda 
		// samo da prikazemo cijenu
		positionProd tempProd = tempRec->product;

		while (tempProd != NULL) {

			strcpy(tempCheck, tempProd->name);
			lwrCase(tempCheck);

			if (strcmp(tempCheck, prodNameBuffer) == 0) {
				printf("\nTrenutna cijena ovog proizvoda: %.2lf eura\n\nUnesite novu cijenu (s tockom): ", tempProd->price);
				priceBuffer = getPrice();

				prevPrice = tempProd->price;

				priceChanger(Head, prodNameBuffer, priceBuffer, prevPrice);

				printf("\n\n%.2lf eura - cijena nakon promjene\n\n\n", tempProd->price);

				return EXIT_SUCCESS;
			}

			tempProd = tempProd->next;
		}

		tempRec = tempRec->next;
	}

	printf("\nTrazeni proizvod ne postoji.\n");

	return EXIT_FAILURE;
}

int timespanTotal(position Head) {

	int startYearBuffer = 0, endYearBuffer = 0;
	int startMonthBuffer = 0, endMonthBuffer = 0;
	int startDayBuffer = 0, endDayBuffer = 0;
	char prodNameBuffer[PRODUCT_NAME_MAX] = { '\0' };

	if (Head->next == NULL) {
		printf("\nLista je prazna.\n");
		return EXIT_FAILURE;
	}

	printf("\nMolimo unosite datume po kronoloskom redu u formatu YYYY-MM-DD.");

	getStartDate(&startYearBuffer, &startMonthBuffer, &startDayBuffer);
	getEndDate(&endYearBuffer, &endMonthBuffer, &endDayBuffer);

	printf("\nUnesite proizvod koji zelite provjeriti: ");
	scanf(" %s", prodNameBuffer);
	lwrCase(prodNameBuffer);

	position startTemp = createReceipt(startYearBuffer, startMonthBuffer, startDayBuffer);
	position endTemp = createReceipt(endYearBuffer, endMonthBuffer, endDayBuffer);
	if (startTemp == NULL || endTemp == NULL) {
		printf("\nERROR: Greska u stvaranju racuna (timespanTotal).\n");
		return CREATE_ERROR;
	}

	if (compareDates(startTemp, endTemp) < 0) {
		printf("\nDatumi nisu kronoloski poredani. Pocetni datum ne moze biti noviji od zavrsnog. Pokusajte ponovno.\n");
		free(startTemp);
		free(endTemp);

		return EXIT_FAILURE;
	}

	position dateSetter = Head->next;
	position start = NULL;
	position end = NULL;

	while (dateSetter != NULL && compareDates(dateSetter, endTemp) < 0)
		dateSetter = dateSetter->next;

	if (dateSetter == NULL) {
		printf("\nNijedan racun nije unutar vremenskog perioda (nema racuna starijih ili jednakog datuma kao zavrsni).\n");
		free(startTemp);
		free(endTemp);

		return EXIT_FAILURE;
	}
	end = dateSetter;

	dateSetter = Head->next;

	do {
		dateSetter = dateSetter->next;
	} while (dateSetter != NULL && compareDates(dateSetter, startTemp) < 0);

	if (dateSetter == NULL) {
		printf("\nNijedan racun nije unutar vremenskog perioda (nema racuna starijih ili jednakog datuma kao pocetni).\n");
		free(startTemp);
		free(endTemp);

		return EXIT_FAILURE;
	}
	start = dateSetter;

	free(startTemp);
	free(endTemp);

	int quantCounter = 0;
	double totalPrice = 0;

	if (totalPriceAndQuant(start, end, prodNameBuffer, &quantCounter, &totalPrice) == 0) {
		printf("\nTrazeni proizvod nije pronadjen u ovom vremenskom periodu.\n");
		return EXIT_FAILURE;
	}

	printf("\nNa proizvod %s je potroseno ukupno %.2lf eura i kupljeno je %d komada.\n", prodNameBuffer, totalPrice, quantCounter);

	return EXIT_SUCCESS;
}

int mostBoughtProduct(position Head) {

	receipt tempHead = { .year = 0, .month = 0, .day = 0, .next = NULL, .product = NULL };

	if (Head->next == NULL) {
		printf("\nLista je prazna.\n");
		return EXIT_SUCCESS;
	}
	position tempRec = Head->next;

	while (tempRec != NULL) {

		positionProd tempProd = tempRec->product;

		while (tempProd != NULL) {
			addNewOrUpdateProd(&tempHead, tempProd->name, tempProd->quantity);
			tempProd = tempProd->next;
		}

		tempRec = tempRec->next;
	}

	findMostBought(&tempHead);

	deleteProducts(&tempHead);

	/*
	if (tempHead.product == NULL)
		printf("\nBrisanje temp liste je uspjesno.\n");		// Provjera za uspjesno brisanje temp liste proizvoda
	*/

	return EXIT_SUCCESS;
}

int findMostExpensiveProduct(position Head) {

	position tempRec = Head->next;

	double MEPbuffer = 0;
	char MEPname[PRODUCT_NAME_MAX] = { '0' };

	while (tempRec != NULL) {

		positionProd tempProd = tempRec->product;

		while (tempProd != NULL) {

			if (tempProd->price > MEPbuffer) {
				MEPbuffer = tempProd->price;
				strcpy(MEPname, tempProd->name);
			}

			tempProd = tempProd->next;
		}

		tempRec = tempRec->next;
	}

	printf("\nNajskuplji proizvod: %s, %.2lf eura\n", MEPname, MEPbuffer);

	return EXIT_SUCCESS;
}

int findLeastExpensiveProduct(position Head) {

	position tempRec = Head->next;

	double LEPbuffer = tempRec->product->price;
	char LEPname[PRODUCT_NAME_MAX] = { '\0' };

	while (tempRec != NULL) {

		positionProd tempProd = tempRec->product;

		while (tempProd != NULL) {

			if (tempProd->price < LEPbuffer) {
				LEPbuffer = tempProd->price;
				strcpy(LEPname, tempProd->name);
			}

			tempProd = tempProd->next;
		}

		tempRec = tempRec->next;
	}

	printf("\nNajskuplji proizvod: %s, %.2lf eura\n", LEPname, LEPbuffer);

	return EXIT_SUCCESS;

}

// Opcenite funkcije

int printAll(position Head) {

	position tempRec = Head->next;

	if (tempRec == NULL) {
		printf("\nERROR: Lista racuna nije upisana (printReceipts)");
		return EXIT_FAILURE;
	}

	int counter = 1;

	while (tempRec != NULL) {

		printReceipt(tempRec, counter);

		tempRec = tempRec->next;
		counter++;
	}

	return EXIT_SUCCESS;

}

int printWanted(position Head) {

	position temp = Head->next;
	int yearBuffer = 0, monthBuffer = 0, dayBuffer = 0;

	printf("\nUnesite datum racuna kojeg zelite ispisati (YYYY-MM-DD): ");
	scanf("%d-%d-%d", &yearBuffer, &monthBuffer, &dayBuffer);

	if (checkDate(monthBuffer, dayBuffer) == CHECK_FAIL) {
		printf("\nUneseni datum je neispravan. Pokusajte ponovno.\n");
		return EXIT_FAILURE;
	}

	while (temp != NULL) {

		if (temp->year == yearBuffer && temp->month == monthBuffer && temp->day == dayBuffer) {
			printReceipt(temp, 0);
			return EXIT_SUCCESS;
		}

		temp = temp->next;
	}

	if (temp == NULL)
		printf("\nTrazeni racun ne postoji. Provjerite uneseni datum.\n");

	return EXIT_SUCCESS;
}