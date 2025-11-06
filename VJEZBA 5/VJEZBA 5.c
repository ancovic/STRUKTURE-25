#define _CRT_SECURE_NO_WARNINGS
#define FILE_OPEN_ERROR -1
#define POSTFIX_MAX 30
#define MEMORY_ALLOC_ERROR NULL
#define CREATE_ERROR NULL

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/*
5. Napisati program koji iz datoteke cita postfiks izraz i zatim koristenjem stoga racuna
 rezultat. Stog je potrebno realizirati preko vezane liste.
*/

typedef struct _postfix* position;
typedef struct _postfix {

	double number;
	position next;

}postfix;

int postfixCalc(position Head);
int printPostfix();
position push(position Head, double nmbr);
position createEl(double nmbr);
position pop(position Head, position first, position second, int operation);
double calc(double firstNum, double secNum, int operation);
int deleteList(position Head);

// DRUGE (lakse) OPCIJE (bez pracenja first i second): 
// int push2(position Head, double nmbr);
// int pop2(position Head, int operation);

int main() {

	postfix Head = { 0, NULL };

	postfixCalc(&Head);

	deleteList(&Head); 

	return EXIT_SUCCESS;
}

int postfixCalc(position Head) {

	FILE* fp = fopen("POSTFIX.txt", "r");
	if (fp == NULL) {
		printf("\nERROR: Greska u otvaranju file-a (postfixCalc).\n");
		return FILE_OPEN_ERROR;
	}

	printPostfix();

	position first = NULL;
	position second = NULL;
	int pfBuffer = 0;
	int op = 0; 

	while ((pfBuffer = fgetc(fp)) != EOF) {

		if (pfBuffer == ' ')
			continue;

		if (pfBuffer >= 42 && pfBuffer <= 47) {

			first = Head->next;
			second = first->next;

			first = pop(Head, first, second, pfBuffer);

			//pop2(Head, pfBuffer);
			continue;
		}

		first = push(Head, pfBuffer - '0');
		second = first->next;

		//push2(Head, pfBuffer - '0');
	}

	printf("\nRezultat postfix izraza: %lf\n", Head->next->number);

	fclose(fp);

	return EXIT_SUCCESS;
}

int printPostfix() {

	FILE* fp = fopen("POSTFIX.txt", "r");
	if (fp == NULL) {
		printf("\nERROR: Greska u otvaranju file-a (printPostfix).\n");
		return FILE_OPEN_ERROR;
	}

	char printBuffer[POSTFIX_MAX] = { '\0' };

	printf("\nPostfix izraz: ");

	fgets(printBuffer, POSTFIX_MAX, fp);

	printf("%s", printBuffer);

	fclose(fp);

	return EXIT_SUCCESS;
}

position push(position Head, double nmbr) {

	position newfirst = createEl(nmbr);
	if (newfirst == NULL) {
		printf("\nERROR: Greska u stvaranju elementa (push)\n.");
		return CREATE_ERROR;
	}

	newfirst->next = Head->next;
	Head->next = newfirst;

	return newfirst;
}

position createEl(double nmbr) {

	position newEl = (position)malloc(sizeof(postfix));
	if (newEl == NULL) {
		printf("\nERROR: Greska u alociranju memorije (createEl).\n");
		return MEMORY_ALLOC_ERROR;
	}

	newEl->number = nmbr;
	newEl->next = NULL;

	return newEl;
}

position pop(position Head, position first, position second, int operation) {

	double numA = second->number;
	double numB = first->number;
	double result = calc(numA, numB, operation);

	Head->next = second->next; 
	first->next = NULL;
	second->next = NULL;

	free(first);
	free(second);

	position temp = push(Head, result);

	return temp;
}

double calc(double numA, double numB, int operation) {

	double result = 0;

	switch (operation) { 

	case '+':
		result = numA + numB;
		break;

	case '-':
		result = numA - numB;
		break;

	case '*':
		result = numA * numB;
		break;

	case '/':
		result = numA / numB;
		break;

	default:
		printf("\nOperacija ne postoji.\n");
		break;
	}

	return result;
}

int deleteList(position Head) {		//mozemo i samo free(temp) buduci da nakon postfixa ostaje 
									//ostaje samo jedan element u listi pa nema potrebe za petljom 
	position temp = Head->next;
	position current = NULL;

	if (Head->next == NULL) {
		printf("\nLista je prazna.\n");
		return EXIT_FAILURE;
	}

	while (temp != NULL) {

		current = temp;
		temp = temp->next;
		current->next = NULL;
		free(current);

	}

	Head->next = NULL;

	return EXIT_SUCCESS;
}


//OPCIONALNE: 

// int push2(position Head, double nmbr) {
/*
	position temp = createEl(nmbr);
	if (temp == NULL) {
		printf("\nERROR: Greska u stvaranju elementa (push)\n.");
		return NULL;
	}

	temp->next = Head->next;
	Head->next = temp;

	return EXIT_SUCCESS;
}
*/

// int pop2(position Head, int operation) {
/*
	position first = Head->next;
	position second = first->next; 

	double firstNum = first->number;
	double secnum = second->number;
	double result = calc(firstNum, secnum, operation);

	Head->next = second->next; 

	first->next = NULL;
	second->next = NULL;

	free(first);
	free(second);

	push(Head, result);

	return EXIT_SUCCESS;
}
*/


