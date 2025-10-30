#define _CRT_SECURE_NO_WARNINGS
#define NAME_MAX 20
#define MEMORY_ALLOC_ERR NULL
#define CREATE_ERR -1
#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
3. Prethodnom zadatku dodati funkcije :
   A.dinamicki dodaje novi element iza odredjenog elementa,  
   B.dinamicki dodaje novi element ispred odredjenog elementa,
   C.sortira listu po prezimenima osoba,
   D.upisuje listu u datoteku,
   E.cita listu iz datoteke.
*/

typedef struct Person* position;

typedef struct Person {

	char name[NAME_MAX];
	char surname[NAME_MAX];
	int birthyear;
	position next;

}person;

//VJEZBA 2

position createPerson(char* name, char* surname, int birthYear);
int addToBegining(position Head);
int printList(position Head);
int addToEnd(position Head);
int findSurname(position Head);
int deletePerson(position Head);
int deleteList(position Head);
int userInterface(position Head);

//VJEZBA 3

int addAfter(position Head);
int addBefore(position Head);
int sortList(position Head);	//sortira listu po prezimenu
int lwrCase(char* string);		//pomocna funkcija za sortList
int listToFile(position Head);
int fileToFileList();

int main() {

	person Head = { {0}, {0}, 0, NULL };

	userInterface(&Head);

	deleteList(&Head);

	return 0;
}

position createPerson(char* name, char* surname, int birthYear) {

	position newPerson = (position)malloc(sizeof(person));
	if (newPerson == NULL) {
		printf("ERROR: Greska u alociranju memorije.\n");
		return MEMORY_ALLOC_ERR;
	}

	strcpy(newPerson->name, name);
	strcpy(newPerson->surname, surname);
	newPerson->birthyear = birthYear;
	newPerson->next = NULL;

	return newPerson;
}

int addToBegining(position Head) {

	char nameBuffer[NAME_MAX] = { '\0' };
	char surnBuffer[NAME_MAX] = { '\0' };
	int byBuffer = 0;

	printf("\nUpisite ime, prezime i godinu rodjenja osobe:\n");
	scanf("%s %s %d", nameBuffer, surnBuffer, &byBuffer);

	position newPerson = createPerson(nameBuffer, surnBuffer, byBuffer);
	if (newPerson == NULL) {
		printf("ERROR: Greska u stvaranju osobe (addToBegining).");
		return CREATE_ERR;
	}

	newPerson->next = Head->next;
	Head->next = newPerson;

	return 0;
}

int printList(position Head) {

	position buffer = Head->next;
	int nmbr = 1;

	printf("\n");

	if (buffer == NULL)
		printf("\nLista je prazna.\n");

	while (buffer != NULL) {
		printf("Osoba %d: %s %s %d.\n", nmbr, buffer->name, buffer->surname, buffer->birthyear);
		nmbr++;
		buffer = buffer->next;
	}

	return 0;
}

int addToEnd(position Head) {

	char nameBuffer[NAME_MAX] = { '\0' };
	char surnBuffer[NAME_MAX] = { '\0' };
	int byBuffer = 0;

	printf("\nUpisite ime, prezime i godinu rodjenja osobe:\n");
	scanf("%s %s %d", nameBuffer, surnBuffer, &byBuffer);

	position newPerson = createPerson(nameBuffer, surnBuffer, byBuffer);
	if (newPerson == NULL) {
		printf("ERROR: Greska u stvaranju osobe (addToBegining).");
		return CREATE_ERR;
	}

	position temp = Head->next;

	while (temp->next != NULL)
		temp = temp->next;

	temp->next = newPerson;
	newPerson->next = NULL;

	return 0;
}

int findSurname(position Head) {

	char surnBuffer[NAME_MAX] = { '\0' };

	printf("\nUpisite prezime: ");
	scanf(" %s", surnBuffer);

	position temp = Head->next;
	int found = 0;

	while (temp != NULL) {
		if (strcmp(temp->surname, surnBuffer) == 0) {
			printf("\nTrazena osoba: %s %s %d.\n", temp->name, temp->surname, temp->birthyear);
			found = 1;
			return 0;
		}
		else
			temp = temp->next;
	}

	if (found == 0)
		printf("\nTrazena osoba ne postoji.\n");

	return 0;
}

int deletePerson(position Head) {				//pod pretpostavkom da vise osoba u listi nema isto ime, prezime i god rodjenja
	//ako ima onda jedan prolaz funkcije brise prvu takvu osobu po redu
	char nameBuffer[NAME_MAX] = { '\0' };
	char surnBuffer[NAME_MAX] = { '\0' };
	int byBuffer = 0;

	printf("\nUpisite ime, prezime i god. rodjenja osobe koju zelite izbrisati: ");
	scanf(" %s %s %d", nameBuffer, surnBuffer, &byBuffer);

	position temp = Head->next;
	position prev = Head;
	int found = 0;

	while (temp != NULL) {
		if (strcmp(temp->name, nameBuffer) == 0 && strcmp(temp->surname, surnBuffer) == 0 && temp->birthyear == byBuffer) {
			prev->next = temp->next;
			temp->next = NULL;
			free(temp);
			found = 1;
			return 0;
		}

		prev = temp;
		temp = temp->next;
	}

	if (found == 0)
		printf("\nTrazena osoba ne postoji.\n");

	return 0;
}

int deleteList(position Head) {

	position temp = Head->next;
	position current = NULL;

	while (temp != NULL) {

		current = temp;
		temp = temp->next;
		free(current);

	}

	Head->next = NULL;

	return EXIT_SUCCESS;
}

int addAfter(position Head) {

	char nameBuffer[NAME_MAX] = { '\0' };
	char surnBuffer[NAME_MAX] = { '\0' };
	int byBuffer = 0;

	printf("\nUpisite ime, prezime i godinu rodjenja osobe:\n");
	scanf("%s %s %d", nameBuffer, surnBuffer, &byBuffer);

	position newPerson = createPerson(nameBuffer, surnBuffer, byBuffer);
	if (newPerson == NULL) {
		printf("ERROR: Greska u stvaranju osobe (addAfter).");
		return CREATE_ERR;
	}

	printf("\nUpisite prezime osobe nakon koje zelite ubaciti novu osobu: ");
	scanf(" %s", surnBuffer);

	position temp = Head->next;
	int found = 0;

	while (temp != NULL) {

		if (strcmp(temp->surname, surnBuffer) == 0) {
			newPerson->next = temp->next;
			temp->next = newPerson;
			found++;

			return EXIT_SUCCESS;
		}
		
		temp = temp->next;
	}

	if (found == 0) {
		printf("\nTrazena osoba ne postoji. Pokusajte ponovno.\n");
		free(newPerson);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int addBefore(position Head) {

	char nameBuffer[NAME_MAX] = { '\0' };
	char surnBuffer[NAME_MAX] = { '\0' };
	int byBuffer = 0;

	printf("\nUpisite ime, prezime i godinu rodjenja osobe:\n");
	scanf("%s %s %d", nameBuffer, surnBuffer, &byBuffer);

	position newPerson = createPerson(nameBuffer, surnBuffer, byBuffer);
	if (newPerson == NULL) {
		printf("ERROR: Greska u stvaranju osobe (addBefore).");
		return CREATE_ERR;
	}

	printf("\nUpisite prezime osobe prije koje zelite ubaciti novu osobu: ");
	scanf(" %s", surnBuffer);

	position temp = Head->next;
	position prev = Head;
	int found = 0;

	while (temp != NULL) {

		if (strcmp(temp->surname, surnBuffer) == 0) {
			newPerson->next = temp;
			prev->next = newPerson;
			found++;

			return EXIT_SUCCESS;			
		}

		prev = temp;
		temp = temp->next;
	}

	if (found == 0) {
		printf("\nTrazena osoba nije pronadjena. Pokusajte ponovno.\n");
		free(newPerson);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int sortList(position Head) {

	int swapped = 0;
	position temp = Head;
	position lastCorr = NULL;

	if (Head->next == NULL) {
		printf("\nLista je prazna.\n");
		return EXIT_FAILURE;
	}

	char firstBuffer[NAME_MAX] = { '\0' };
	char secondBuffer[NAME_MAX] = { '\0' };

	do {
		swapped = 0;
		temp = Head;

		while (temp->next->next != lastCorr) {

			position first = temp->next;
			position second = first->next;

			strcpy(firstBuffer, first->surname);			// Potrebno za izbjegavanje pogreske uzrokovane 
			strcpy(secondBuffer, second->surname);			// malim i velikim slovima 
			lwrCase(firstBuffer);							// buduci kompariramo 
			lwrCase(secondBuffer);							// ascii kodove

			if (strcmp(firstBuffer, secondBuffer) > 0) {

				temp->next = second;
				first->next = second->next;
				second->next = first;
				swapped = 1;
			}

			temp = temp->next;
		}

		lastCorr = temp->next;

	} while (swapped != 0);

	printf("\nSortirana lista:\n");
	printList(Head);

	return EXIT_SUCCESS;
}

int lwrCase(char* string) {		
	
	for (int i = 0; i < strlen(string); i++)
		string[i] = tolower(string[i]);

	return 0;
}

int listToFile(position Head) {
	
	FILE* fp = fopen("LISTA.txt", "w");
	if (fp == NULL) {
		printf("\nERROR: Greska u otvaranju file-a (listToFile).\n");
		return FILE_OPEN_ERROR;
	}

	fprintf(fp, "Lista osoba:\n");

	position temp = Head->next;

	while (temp != NULL) {
		fprintf(fp, "%s %s %d\n", temp->name, temp->surname, temp->birthyear);
		temp = temp->next;
	}

	printf("\nLista ispisana u datoteku.\n");
	
	fclose(fp);

	return EXIT_SUCCESS;
}

int fileToFileList() {								//Pod pretpostavkom da datoteka sadrzi samo listu osoba
													//u formatu ime prezime god., bez linija teksta prije
	FILE* fp1 = fopen("fileToList.txt", "r");
	if (fp1 == NULL) {
		printf("\nERROR: Greska u otvaranju file-a (fileToList).\n");
		return FILE_OPEN_ERROR;
	}

	FILE* fp2 = fopen("copiedList.txt", "w");
	if (fp2 == NULL) {
		printf("\nERROR: Greska u otvaranju file-a (fileToList 2).\n");
		return FILE_OPEN_ERROR;
	}

	char nameBuffer[NAME_MAX] = { '\0' };
	char surnBuffer[NAME_MAX] = { '\0' };
	int byBuffer = 0;

	while (fscanf(fp1, " %s %s %d", nameBuffer, surnBuffer, &byBuffer) == 3) {
		fprintf(fp2, "%s %s %d\n", nameBuffer, surnBuffer, byBuffer);
	}

	printf("\nPrepis liste izvrsen.\n");

	fclose(fp1);
	fclose(fp2);

	return EXIT_SUCCESS;
}

int userInterface(position Head) {

	char choice = '\0';

	do {		//Ne mogu iz nekog razloga slomiti printf pa je ovako sve u jednom lineu :(
		printf("\nOdaberite zeljenu opciju (utipkajte tocno slovo):\n\nA: Dodavanje novog elementa na pocetak liste\nB: Ispis liste\nC: Dodavanje novog elementa na kraj liste\nD: Pronalazak elementa po prezimenu\nE: Brisanje odredjenog elementa\nG: Dodavanje novog elementa nakon\nH: Dodavanje novog elementa prije\nI: Sortiraj listu\nJ: Prepis liste u datoteku\nK: Prepis iz datoteke u datoteku\nF: Izlaz\n\n");
		scanf(" %c", &choice);

		switch (choice) {

		case 'A':
		case 'a':
			addToBegining(Head);
			break;

		case 'B':
		case 'b':
			printList(Head);
			break;

		case 'C':
		case 'c':
			addToEnd(Head);
			break;

		case 'D':
		case 'd':
			findSurname(Head);
			break;

		case 'E':
		case 'e':
			deletePerson(Head);
			break;

		case 'F':
		case 'f':
			printf("\nIzlaz...\n");
			break;

		case 'G':
		case 'g':
			addAfter(Head);
			break;

		case 'H':
		case 'h':
			addBefore(Head);
			break;

		case 'I':
		case 'i':
			sortList(Head);
			break;

		case 'J':
		case 'j':
			listToFile(Head);
			break;

		case 'K':
		case 'k':
			fileToFileList();
			break;

		default:
			printf("\nNepoznata opcija.");
			break;
		}

	} while (choice != 'F' && choice != 'f');

	return 0;
}