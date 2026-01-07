#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* 9. Zadan je niz brojeva 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 koji su spremljeni u cvorove binarnog stabla.
   a) Napisati funkciju insert koja dodaje element u stablo tako da se pozivima te funkcije za
   sve element zadanog niza brojeva stvori stablo kao na slici Slika 1. Funkcija vraca
   pokazivac na korijen stabla.
   b) Napisati funkciju replace koja ce svaki element stabla zamijeniti sumom elemenata u
   njegovom lijevom i desnom podstablu (tj. sumom svih potomaka prije zamjene
   vrijednosti u tim potomcima). Npr. stablo sa slike Slika 1 transformirat ce se u stablo na
   slici Slika 2.
   c) Prepraviti program na nacin da umjesto predefiniranog cjelobrojnog polja koristenjem
   funkcije rand() generira slucajne brojeve u rasponu <10, 90>. Takodjer, potrebno je
   upisati u datoteku sve brojeve u inorder prolasku nakon koristenja funkcije iz a), zatim b)
   dijela zadatka.*/

typedef struct _Node* nodePointer;
typedef struct _Node {
	
	int value;
	nodePointer left;
	nodePointer right;

} Node;

nodePointer createNode(int value);
nodePointer insert(nodePointer root, int value);
int replace(nodePointer root);
int InOrder(nodePointer root, FILE* file);

int main() {

	int stablo[] = { 2,5,7,8,11,1,4,2,3,7 };
	int i = 0;
	int n = sizeof(stablo) / sizeof(stablo[0]);

	FILE* fp;

	nodePointer root = NULL;
	for (i = 0; i < n; i++)
		root = insert(root, stablo[i]);

	// a)
	fp = fopen("InOrder_a.txt", "w");
	if (fp == NULL) {
		printf("\nERROR: Greska u otvaranju file-a (a).\n");
		return 1;
	}
	InOrder(root, fp);
	fclose(fp);

	// b)
	replace(root);
	fp = fopen("InOrder_b.txt", "w");
	if (fp == NULL) {
		printf("\nERROR: Greska u otvaranju file-a (b).\n");
		return 1;
	}
	InOrder(root, fp);
	fclose(fp);

	// c)
	srand(time(NULL));
	nodePointer randomRoot = NULL;
	for (i = 0; i < n; i++) {
		int randomVal = 10 + rand() % 81;
		randomRoot = insert(randomRoot, randomVal);
	}

	fp = fopen("InOrder_c.txt", "w");
	if (fp == NULL) {
		printf("\nERROR: Greska u otvaranju file-a (c).\n");
		return 1;
	}
	InOrder(randomRoot, fp);
	fclose(fp);

	return 0;
}

nodePointer createNode(int value) {
	nodePointer newNode = (nodePointer)malloc(sizeof(Node));
	newNode->value = value;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

nodePointer insert(nodePointer root, int value) {
	if (root == NULL)
		return createNode(value);

	if (value <= root->value)
		root->left = insert(root->left, value);
	else
		root->right = insert(root->right, value);

	return root;
}

int replace(nodePointer root) {
	if (root == NULL)
		return 0;

	int LSum = replace(root->left);
	int RSum = replace(root->right);
	int oldValue = root->value;

	root->value = LSum + RSum;
	return oldValue + root->value;
}

int InOrder(nodePointer root, FILE* file) {
	if (root == NULL)
		return 0;

	InOrder(root->left, file);
	fprintf(file, "%d ", root->value);
	InOrder(root->right, file);
}