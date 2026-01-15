#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 256

/*10. Napisati program koji cita datoteku drzave.txt u kojoj su zapisani nazivi pojedinih drzava. Uz
ime drzave u datoteci se nalazi i ime dodatne datoteke u kojoj se nalaze gradovi pojedine
drzave. Svaka datoteka koja predstavlja drzavu sadrzi popis gradova u formatu naziv_grada,
broj_stanovnika.
a) Potrebno je formirati sortiranu vezanu listu drzava po nazivu. Svaki cvor vezane liste
sadrzi stablo gradova sortirano po broju stanovnika, zatim po nazivu grada.
b) Potrebno je formirati stablo drzava sortirano po nazivu. Svaki cvor stabla sadrzi vezanu
listu gradova sortiranu po broju stanovnika, zatim po nazivu grada.
Nakon formiranja podataka potrebno je ispisati drzave i gradove te omoguciti korisniku putem
tastature pretragu gradova odredjene drzave koji imaju broj stanovnika veci od unosa na
tastaturi.*/

//pod a)

typedef struct _city* cityPntr;
typedef struct _city {
	char name[MAX_CHAR];
	int population;
	cityPntr left;
	cityPntr right;
}city;

typedef struct _country* countryPntr;
typedef struct _country {
	char name[MAX_CHAR];
	cityPntr cities;
	countryPntr next;
}country;

cityPntr createCityNode(char* name, int population);
cityPntr InsertCity(cityPntr root, char* name, int population);
int printCities(cityPntr root);
int searchCities(cityPntr root, int minPop);
countryPntr createCountry(char* name);
countryPntr InsertCountry(countryPntr head, char* name);
int printCountries(countryPntr head);
countryPntr findCountry(countryPntr head, char* name);
cityPntr loadCitiesFromFile(const char* fileName);
countryPntr loadCountriesFromFile(const char* fileName);

int main() {

	char countryName[MAX_CHAR];
	int minPop;
	char countryFile[] = "drzave.txt";
	countryPntr countries = loadCountriesFromFile(countryFile);

	if (countries == NULL) {
		printf("Greska 3");
		return 1;
	}

	printCountries(countries);

	printf("\nUnesite ime drzave za pretragu: ");
	scanf("%s", countryName);
	printf("\nUnesite minimalan broj stanovnika: ");
	scanf("%d", &minPop);

	countryPntr selected = findCountry(countries, countryName);
	if (selected == NULL)
		printf("Drzava nije pronadena.");
	else {
		printf("Gradovi u drzavi %s sa vise od %d stanovnika:\n", countryName, minPop);
		searchCities(selected->cities, minPop);
	}

	return 0;
}

cityPntr createCityNode(char* name, int population) {
	cityPntr new = (cityPntr)malloc(sizeof(city));
	strcpy(new->name, name);
	new->population = population;
	new->left = NULL;
	new->right = NULL;
	return new;
}

cityPntr InsertCity(cityPntr root, char* name, int population) {
	if (root == NULL)
		return createCityNode(name, population);

	if (population < root->population || (population == root->population && strcmp(name, root->name) < 0))
		root->left = InsertCity(root->left, name, population);
	else
		root->right = InsertCity(root->right, name, population);

	return root;
}

int printCities(cityPntr root) {
	if (root == NULL)
		return 0;
	printCities(root->left);
	printf("\t%s, %d\n", root->name, root->population);
	printCities(root->right);

	return EXIT_SUCCESS;
}

int searchCities(cityPntr root, int minPop) {
	if (root == NULL)
		return 0;
	searchCities(root->left, minPop);
	if (root->population > minPop)
		printf("\t%s, %d\n", root->name, root->population);
	searchCities(root->right, minPop);

	return EXIT_SUCCESS;
}

countryPntr createCountry(char* name) {
	countryPntr new = (countryPntr)malloc(sizeof(country));
	strcpy(new->name, name);
	new->cities = NULL;
	new->next = NULL;
	return new;
}

countryPntr InsertCountry(countryPntr head, char* name) {
	countryPntr new = createCountry(name);
	if (head == NULL || strcmp(name, head->cities) < 0) {
		new->next = head;
		return new;
	}

	countryPntr current = head;
	while (current->next != NULL && strcmp(name, current->next->name) > 0)
		current = current->next;

	new->next = current->next;
	current->next = new;
	return head;
}

int printCountries(countryPntr head) {
	while (head != NULL) {
		printf("%s:\n", head->name);
		printCities(head->cities);
		head = head->next;
	}

	return EXIT_SUCCESS;
}

countryPntr findCountry(countryPntr head, char* name) {
	while (head != NULL) {
		if (strcmp(head->name, name) == 0)
			return head;
		head = head->next;
	}
	return NULL;
}

cityPntr loadCitiesFromFile(const char* fileName) {
	FILE* fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("Greska 1");
		return NULL;
	}

	cityPntr root = NULL;
	char name[MAX_CHAR];
	int population;
	while (fscanf(fp, "%[^,],%d\n", name, &population) == 2)
		root = InsertCity(root, name, population);

	fclose(fp);
	return root;
}

countryPntr loadCountriesFromFile(const char* fileName) {
	FILE* fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("Greska 2");
		return NULL;
	}

	countryPntr head = NULL;
	char name[MAX_CHAR], File[MAX_CHAR];
	while (fscanf(fp, "%s %s\n", name, File) == 2) {
		head = InsertCountry(head, name);
		countryPntr country = findCountry(head, name);
		if (country != NULL)
			country->cities = loadCitiesFromFile(File);
	}
	fclose(fp);
	return head;
}



//pod b)
/*
typedef struct _city* cityPntr;
typedef struct _city {

	char name[MAX_CHAR];
	int population;
	cityPntr next;

}city;

typedef struct _country* countryPntr;
typedef struct _country {

	char name[MAX_CHAR];
	cityPntr cities;
	countryPntr left;
	countryPntr right;

}country;

cityPntr createCity(char* name, int population);
cityPntr insertCity(cityPntr head, cityPntr new);
countryPntr createCountry(char* name);
countryPntr insertCountry(countryPntr root, countryPntr new);
cityPntr loadCities(const char* fileName);
int printCities(cityPntr head);
int printCountries(countryPntr root);
countryPntr loadCountries(const char* fileName);
int findCities(countryPntr root, char* nameCountry, int minPop);

int main() {

	char wanted[MAX_CHAR];
	int minPop;

	countryPntr root = loadCountries("drzave.txt");
	if (!root)
		return 1;

	printCountries(root);

	printf("\nUnesite naziv drzave: "); 
	scanf("%s", wanted);
	printf("Unesite minimalan broj stanovnika: "); 
	scanf("%d", &minPop);

	findCities(root, wanted, minPop);

	return 0;
}

cityPntr createCity(char* name, int population) {

	cityPntr new = (cityPntr)malloc(sizeof(city));
	strcpy(new->name, name);
	new->population = population;
	new->next = NULL;
	return new;
}

cityPntr insertCity(cityPntr head, cityPntr new) {

	if (!head || new->population > head->population || (new->population == head->population && strcmp(new->name, head->name) < 0)) {
		new->next = head;
		return new;
	}

	cityPntr current = head;
	while (current->next && (current->next->population > new->population || (current->next->population == new->population && strcmp(current->next->name, new->name) < 0)))
		current = current->next;

	new->next = current->next;
	current->next = new;
	return head;
}

countryPntr createCountry(char* name) {

	countryPntr new = (countryPntr)malloc(sizeof(country));
	strcpy(new->name, name);
	new->cities = NULL;
	new->left = NULL;
	new->right = NULL;

	return new;
}

countryPntr insertCountry(countryPntr root, countryPntr new) {

	if (!root)
		return new;

	if (strcmp(new->name, root->name) < 0)
		root->left = insertCountry(root->left, new);
	else if (strcmp(new->name, root->name) > 0)
		root->right = insertCountry(root->right, new);
	return root;
}

cityPntr loadCities(const char* fileName) {

	FILE* fp = fopen(fileName, "r");
	if (!fp) {
		printf("Greska 1");
		return NULL;
	}

	cityPntr head = NULL;
	char name[MAX_CHAR];
	int population;

	while (fscanf(fp, "%[^,],%d\n", name, &population) == 2) {
		cityPntr new = createCity(name, population);
		head = insertCity(head, new);
	}

	fclose(fp);
	return head;
}

int printCities(cityPntr head) {

	while (head) {
		printf("\t%s, %d\n", head->name, head->population);
		head = head->next;
	}

	return EXIT_SUCCESS;
}

int printCountries(countryPntr root) {

	if (!root)
		return EXIT_FAILURE;

	printCountries(root->left);
	printf("%s:\n", root->name);
	printCities(root->cities);
	printCountries(root->right);

	return EXIT_SUCCESS;
}

countryPntr loadCountries(const char* fileName) {

	FILE* fp = fopen(fileName, "r");
	if (!fp) {
		printf("Greska 2");
		return NULL;
	}

	countryPntr root = NULL;
	char name[MAX_CHAR], fileCity[MAX_CHAR];

	while (fscanf(fp, "%s %s\n", name, fileCity) == 2) {
		countryPntr new = createCountry(name);
		new->cities = loadCities(fileCity);
		root = insertCountry(root, new);
	}

	fclose(fp);
	return root;
}

int findCities(countryPntr root, char* nameCountry, int minPop) {

	if (!root)
		return EXIT_FAILURE;

	if (strcmp(nameCountry, root->name) < 0)
		findCities(root->left, nameCountry, minPop);
	else if (strcmp(nameCountry, root->name) > 0)
		findCities(root->right, nameCountry, minPop);
	else {
		printf("Gradovi u drzavi %s s vise od %d stanovnika:\n", nameCountry, minPop);
		cityPntr current = root->cities;
		while (current) {
			if (current->population > minPop)
				printf("\t%s, %d\n", current->name, current->population);
			current = current->next;
		}
	}

	return EXIT_SUCCESS;
}
*/
