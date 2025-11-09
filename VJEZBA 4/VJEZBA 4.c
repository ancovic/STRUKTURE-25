#define _CRT_SECURE_NO_WARNINGS
#define FILE_OPEN_ERROR -1
#define MEMORY_ALLOC_ERROR NULL
#define CREATE_ERROR -1
#define LIST_ERROR -1

#include <stdio.h>
#include <stdlib.h>

/*
4. Napisati program za zbrajanje i mnozenje polinoma. 
Koeficijenti i eksponenti se citaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nuzno sortirani.
*/

typedef struct _poly* position;
typedef struct _poly {

	int coeff;
	int power;
	position next;

} poly;

int polyCalculator(position Head, position firstPoly, position secondPoly);
int filePrintPoly();	//Koristi se da ispisemo polinome tocno onako kako su u file-u poredani 
int sortedInput(position firstPoly, position secondPoly);
position createElement(int coeff, int power); 
int insert(position fosPoly, int coeff, int power);
int polySum(position Head, position firstPoly, position secondPoly);
int polyProduct(position Head, position firstPoly, position secondPoly);
int deleteList(position poly);
int printPoly(position buffer);

int main() {

	poly Head = { .coeff = 0, .power = 0, .next = NULL };
	poly firstPoly = Head;
	poly secondPoly = Head; 

	filePrintPoly();

	polyCalculator(&Head, &firstPoly, &secondPoly);

	deleteList(&firstPoly);
	deleteList(&secondPoly);		//Nema potrebe zvati deleteList za Head jer se on brise svaki put na kraju polySum i polyProduct

	return EXIT_SUCCESS; 
}

int polyCalculator(position Head, position firstPoly, position secondPoly) {

	sortedInput(firstPoly, secondPoly);

	int choice = 0; 

	do {

		printf("\n\nOdaberite zeljenu opciju:\n1 - zbrajanje polinoma\n2 - mnozenje polinoma\n3 - Izlaz\nOdabir: ");
		scanf(" %d", &choice);
	 
		switch (choice) {

		case 1:
			polySum(Head, firstPoly, secondPoly);
			break;

		case 2:
			polyProduct(Head, firstPoly, secondPoly);
			break;
		
		case 3:
			printf("\nIzlaz...\n");
			break; 

		default:
			printf("\nOdabrana opcija ne postoji. Odaberite ponovno.\n");
			break;

		}
	} while (choice != 3);


	return EXIT_SUCCESS;
}

int filePrintPoly() {

	FILE* fp = fopen("POLINOMI.txt", "r");
	if (fp == NULL) {
		printf("\nERROR: Greska u otvaranju file-a (printPoly).\n");
		return FILE_OPEN_ERROR; 
	}

	int nmbrBuffer = 0;
	int counterX = 1;
	int check = 0;

	printf("\nPolinomi u file-u:\nPrvi polinom: ");

	//Petlja za prvi polinom
	while ((nmbrBuffer = fgetc(fp)) != '\n') {
		
		if (counterX % 2 == 0 && check == 0) {
			printf("x^");
			counterX++;
			check = 1;
			continue;
		}

		if (nmbrBuffer == ' ' && check == 1) {
			printf(" + ");
			check = 0; 
			counterX++;
			continue;
		}

		printf("%d", nmbrBuffer - '0');
		counterX++;
	}

	printf("\nDrugi polinom: ");

	counterX = 1;
	check = 0; 

	//Petlja za drugi polinom 
	while ((nmbrBuffer = fgetc(fp)) != EOF) {

		if (counterX % 2 == 0 && check == 0) {
			printf("x^");
			counterX++;
			check = 1;
			continue;
		}

		if (nmbrBuffer == ' ' && check == 1) {
			printf(" + ");
			check = 0;
			counterX++;
			continue;
		}

		printf("%d", nmbrBuffer - '0');
		counterX++;
	}

	fclose(fp);

	return EXIT_SUCCESS;
}

int sortedInput(position firstPoly, position secondPoly) {

	FILE* fp = fopen("POLINOMI.txt", "r");
	if (fp == NULL) {
		printf("\nERROR: Greska u otvaranju file-a (sortedInput).\n");
		return FILE_OPEN_ERROR;
	}

	int fgetcBuffer = 0;
	int counter = 0;
	int coeffBuffer = 0;
	int pwrBuffer = 0;

	//Petlja za firstPoly
	while ((fgetcBuffer = fgetc(fp)) != '\n') {		//Krece od pocetka file-a do kraja prvog polinoma tj. do '\n' 

		if (fgetcBuffer == ' ')
			continue;

		if (counter % 2 == 0)
			coeffBuffer = fgetcBuffer - '0';
		
		else {
			pwrBuffer = fgetcBuffer - '0';
			insert(firstPoly, coeffBuffer, pwrBuffer);
		}

		counter++; 
	}

	counter = 0; 

	//Petlja za secondPoly
	while ((fgetcBuffer = fgetc(fp)) != EOF) {		//Krece od pocetka drugog polinoma do EOF jer je fp u prethodnoj petlji
													//zavrsio na '\n' pa u ovoj pocinje od pocetka iduceg retka
		if (fgetcBuffer == ' ')
			continue;

		if (counter % 2 == 0)
			coeffBuffer = fgetcBuffer - '0';

		else {
			pwrBuffer = fgetcBuffer - '0';
			insert(secondPoly, coeffBuffer, pwrBuffer);
		}

		counter++;
	}

	printf("\n\nPolinomi nakon sortiranog upisa:\nPrvi polinom: ");
	printPoly(firstPoly);
	printf("\nDrugi polinom: ");
	printPoly(secondPoly);

	fclose(fp);

	return EXIT_SUCCESS;
}

position createElement(int coeff, int power) {

	position newEl = (position)malloc(sizeof(poly));
	if (newEl == NULL) {
		printf("\nERROR: Greska u alociranju memorije (createElement).\n");
		return MEMORY_ALLOC_ERROR;
	}

	newEl->coeff = coeff;
	newEl->power = power;
	newEl->next = NULL;

	return newEl;
}

int insert(position fosPoly, int coeff, int power) {	//Sortira elemente silazno po eksponentu

	position newEl = createElement(coeff, power);
	if (newEl == NULL) {
		printf("\nERROR: Greska u stvaranju novog elementa (addBefore).\n");
		return CREATE_ERROR;
	}

	if (fosPoly->next == NULL) {
		fosPoly->next = newEl;
		newEl->next = NULL;

		return EXIT_SUCCESS; 
	}

	position temp = fosPoly->next;
	position prev = fosPoly; 
	int found = 0; 

	while (temp != NULL) {

		if (temp->power < newEl->power) {
			newEl->next = temp; 
			prev->next = newEl;
			found = 1; 

			return EXIT_SUCCESS;
		}

		if (temp->power == newEl->power) {
			temp->coeff += newEl->coeff;
			free(newEl);
			found = 1;

			return EXIT_SUCCESS; 
		}

		prev = temp; 
		temp = temp->next;
	}

	//U slucaju da je petlja dosla na kraj liste, tj. da je newEl eksponent najmanji
	prev->next = newEl;
	newEl->next = NULL;
	found = 1; 

	if (found == 0) {
		printf("\nERROR: Greska u listi (insert).\n");
		return LIST_ERROR;
	}

	return EXIT_SUCCESS;
}

int polySum(position Head, position firstPoly, position secondPoly) {		//Koristimo head kao nekakav buffer da ne ostane neiskoristen

	position firstTemp = firstPoly->next;
	position secondTemp = secondPoly->next;

	while (firstTemp != NULL && secondTemp != NULL) {

		if (firstTemp->power == secondTemp->power) {
			insert(Head, firstTemp->coeff + secondTemp->coeff, firstTemp->power);
			firstTemp = firstTemp->next;
			secondTemp = secondTemp->next;
		}
		else if (firstTemp->power > secondTemp->power) {
			insert(Head, firstTemp->coeff, firstTemp->power);
			firstTemp = firstTemp->next;
		}
		else {
			insert(Head, secondTemp->coeff, secondTemp->power);
			secondTemp = secondTemp->next;
		}
	}

	//Dvije petlje za slucaj ako su polinomi razlicitih duzina da ubaci preostale clanove
	while (firstTemp != NULL) {
		insert(Head, firstTemp->coeff, firstTemp->power);
		firstTemp = firstTemp->next;
	}

	while (secondTemp != NULL) {
		insert(Head, secondTemp->coeff, secondTemp->power);
		secondTemp = secondTemp->next;
	}

	printf("\nZbroj polinoma:\n");

	printPoly(Head); 

	deleteList(Head); 

	return EXIT_SUCCESS;
}

int polyProduct(position Head, position firstPoly, position secondPoly) {

	position firstTemp = firstPoly->next;
	position secondTemp = secondPoly->next;

	int coeffBuffer = 0;
	int pwrBuffer = 0; 

	while (firstTemp != NULL) {

		while (secondTemp != NULL) {

			coeffBuffer = firstTemp->coeff * secondTemp->coeff;
			pwrBuffer = firstTemp->power + secondTemp->power;
			
			insert(Head, coeffBuffer, pwrBuffer);

			secondTemp = secondTemp->next; 
		}

		secondTemp = secondPoly->next;
		firstTemp = firstTemp->next; 
	}

	printf("\nSuma polinoma :\n");

	printPoly(Head);

	deleteList(Head);

	return EXIT_SUCCESS; 
}

int deleteList(position poly) {

	position temp = poly->next;
	position current = poly;

	while (temp != NULL) {

		current = temp;
		temp = temp->next;
		free(current);

	}

	poly->next = NULL;

	return EXIT_SUCCESS;
}

int printPoly(position poly) {

	position temp = poly->next;

	if (temp == NULL) {
		printf("\nPolinom nije upisan.\n");
		return EXIT_SUCCESS; 
	}

	while (temp != NULL) {

		printf("%dx^%d", temp->coeff, temp->power);

		if (temp->next != NULL)
			printf(" + ");

		temp = temp->next;
	}

	return EXIT_SUCCESS; 
}
