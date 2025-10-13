#define _CRT_SECURE_NO_WARNINGS
#define MAX_BODOVI 50
#define MAX_CHAR 20
#define MAX_LINE 1024

#include <stdio.h>
#include <stdlib.h>

/* Napisati program koji prvo procita koliko redaka ima datoteka, tj. koliko ima studenata
 zapisanih u datoteci. Nakon toga potrebno je dinamicki alocirati prostor za niz struktura
 studenata (ime, prezime, bodovi) i ucitati iz datoteke sve zapise. Na ekran ispisati ime,
 prezime, apsolutni i relativni broj bodova.
 Napomena: Svaki redak datoteke sadrzi ime i prezime studenta, te broj bodova na kolokviju.
 relatvan_br_bodova = br_bodova/max_br_bodova*100 */

typedef struct Student {

	char ime[MAX_CHAR];
	char prezime[MAX_CHAR];
	int bodovi;

}student;

int brojacStudenata();
student* allocStud(int br);
int upisStud(int br);
float relBod(int bod);

int main() {

	int brStudent = 0;

	brStudent = brojacStudenata();

	printf("Broj studenata: %d\n", brStudent);

	upisStud(brStudent);

	return 0;
}

int brojacStudenata() {

	int brojac = 0;
	char imeBuffer[MAX_CHAR] = { "\0" };
	char prezimeBuffer[MAX_CHAR] = {"\0"};
	int bodoviBuffer = 0;
	FILE* fp;

	fp = fopen("STUDENTI.txt", "r");
	if (fp == NULL) {
		printf("ERROR: File cannot be opened.");
		return -1;
	}

	while (fscanf(fp, "%s %s %d", imeBuffer, prezimeBuffer, &bodoviBuffer) == 3) {
		brojac++;
	}
	
	fclose(fp);

	return brojac;
}

student* allocStud(int br) {

	student* polje = malloc(br * sizeof(student));

	if (polje == NULL) {
		printf("ERROR: greska u alociranju memorije.");
		return -1;
	}

	return polje;
}

int upisStud(int br) {

	student* popis = allocStud(br);
	FILE* fp;

	fp = fopen("STUDENTI.txt", "r");
	if (fp == NULL) {
		printf("ERROR: nije moguce otvaranje file-a (upisStud)");
		return -1;
	}

	printf("\nStudenti:\n\nIME: \tPREZIME: \tBODOVI: \tREL.BODOVI:\n\n");

	for (int i = 0; i < 4; i++) {
		fscanf(fp, "%s %s %d", popis[i].ime, popis[i].prezime, &popis[i].bodovi);
		printf("%s \t%s \t\t%d \t\t%f\n", popis[i].ime, popis[i].prezime, popis[i].bodovi, relBod(popis[i].bodovi));
	}

	return 0;
}

float relBod(int bod) {

	return (float)bod / MAX_BODOVI * 100;
}