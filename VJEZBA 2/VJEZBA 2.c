#define _CRT_SECURE_NO_WARNINGS
#define NAME_MAX 20
#define MEMORY_ALLOC_ERR NULL
#define CREATE_ERR -1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
2. Definirati strukturu osoba(ime, prezime, godina rodjenja) i napisati program koji :
A.dinamicki dodaje novi element na pocetak liste,
B.ispisuje listu,
C.dinamicki dodaje novi element na kraj liste,
D.pronalazi element u listi (po prezimenu),
E.brise odredjeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable. 
*/

typedef struct Person* position;

typedef struct Person {

	char name[NAME_MAX];
	char surname[NAME_MAX];
	int birthyear;
	position next;

}person;

position createPerson(char* name, char* surname, int birthYear);
int addToBegining(position Head);
int printList(position Head);
int addToEnd(position Head);
int findSurname(position Head);
int userInterface(position Head);

int main() {

	person Head = { {0}, {0}, 0, NULL };
	
	userInterface(&Head);

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

int userInterface(position Head) {

	char choice = '\0';

	do {
		printf("\nOdaberite zeljenu opciju (utipkajte tocno slovo):\n\nA: Dodavanje novog elementa na pocetak liste\nB: Ispis liste\nC: Dodavanje novog elementa na kraj liste\nD: Pronalazak elementa po prezimenu\nE: Brisanje odredjenog elementa\nF: Izlaz\n\n");
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
			printf("\nIzlaz...");
			break;

		default:
			printf("\nNepoznata opcija.");
			break;
		}

	} while (choice != 'F' && choice != 'f');

	return 0;
} 