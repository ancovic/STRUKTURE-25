#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 256
#define HASH_SIZE 11

/*11. Prepraviti zadatak 10 na nacin da se formira hash tablica drzava. Tablica ima 11 mjesta, a
funkcija za preslikavanje kljuc racuna da se zbraja ASCII vrijednost prvih pet slova drzave zatim
racuna ostatak cjelobrojnog dijeljenja te vrijednosti s velicinom tablice. Drzave s istim kljucem se
pohranjuju u vezanu listu sortiranu po nazivu drzave. Svaki cvor vezane liste sadrzi stablo
gradova sortirano po broju stanovnika, zatim po nazivu grada.*/

typedef struct _city* cityPntr;
typedef struct _city {
    char name[MAX_CHAR];
    int population;
    cityPntr left;
    cityPntr right;
} city;

typedef struct _country* countryPntr;
typedef struct _country {
    char name[MAX_CHAR];
    cityPntr cities;
    countryPntr next;
} country;

cityPntr createCity(char* name, int population);
cityPntr insertCity(cityPntr root, char* name, int population);
int printCities(cityPntr root);
int searchCities(cityPntr root, int minPop);
cityPntr loadCities(const char* fileName);
int hashFunction(char* name);
countryPntr createCountry(char* name);
countryPntr insertCountrySorted(countryPntr head, countryPntr new);
int loadCountries(countryPntr hashTable[], const char* fileName);
countryPntr findCountry(countryPntr hashTable[], char* name);
int printHashTable(countryPntr hashTable[]);

int main() {
    countryPntr hashTable[HASH_SIZE] = { NULL };
    char countryName[MAX_CHAR];
    int minPop;

    loadCountries(hashTable, "drzave.txt");
    printHashTable(hashTable);

    printf("\nUnesite drzavu: ");
    scanf("%s", countryName);
    printf("Unesite minimalan broj stanovnika: ");
    scanf("%d", &minPop);

    countryPntr found = findCountry(hashTable, countryName);
    if (!found)
        printf("Drzava nije pronadena!\n");
    else {
        printf("Gradovi u drzavi %s s vise od %d stanovnika:\n",
            countryName, minPop);
        searchCities(found->cities, minPop);
    }

    return 0;
}

cityPntr createCity(char* name, int population) {
    cityPntr new = (cityPntr)malloc(sizeof(city));
    strcpy(new->name, name);
    new->population = population;
    new->left = new->right = NULL;
    return new;
}

cityPntr insertCity(cityPntr root, char* name, int population) {
    if (!root)
        return createCity(name, population);

    if (population < root->population ||
        (population == root->population && strcmp(name, root->name) < 0))
        root->left = insertCity(root->left, name, population);
    else
        root->right = insertCity(root->right, name, population);

    return root;
}

int printCities(cityPntr root) {
    if (!root)
        return EXIT_FAILURE;
    printCities(root->left);
    printf("\t%s, %d\n", root->name, root->population);
    printCities(root->right);

    return EXIT_SUCCESS;
}

int searchCities(cityPntr root, int minPop) {
    if (!root)
        return EXIT_FAILURE;
    searchCities(root->left, minPop);
    if (root->population > minPop)
        printf("\t%s, %d\n", root->name, root->population);
    searchCities(root->right, minPop);

    return EXIT_SUCCESS;
}

cityPntr loadCities(const char* fileName) {
    FILE* fp = fopen(fileName, "r");
    if (!fp) {
        printf("Greska pri otvaranju grada!\n");
        return NULL;
    }

    cityPntr root = NULL;
    char name[MAX_CHAR];
    int pop;

    while (fscanf(fp, "%[^,],%d\n", name, &pop) == 2)
        root = insertCity(root, name, pop);

    fclose(fp);
    return root;
}

int hashFunction(char* name) {
    int sum = 0;
    for (int i = 0; i < 5 && name[i] != '\0'; i++)
        sum += name[i];
    return sum % HASH_SIZE;
}

countryPntr createCountry(char* name) {
    countryPntr new = (countryPntr)malloc(sizeof(country));
    strcpy(new->name, name);
    new->cities = NULL;
    new->next = NULL;
    return new;
}

countryPntr insertCountrySorted(countryPntr head, countryPntr new) {
    if (!head || strcmp(new->name, head->name) < 0) {
        new->next = head;
        return new;
    }

    countryPntr current = head;
    while (current->next && strcmp(new->name, current->next->name) > 0)
        current = current->next;

    new->next = current->next;
    current->next = new;
    return head;
}

int loadCountries(countryPntr hashTable[], const char* fileName) {
    FILE* fp = fopen(fileName, "r");
    if (!fp) {
        printf("Greska pri otvaranju drzava!\n");
        return EXIT_FAILURE;
    }

    char name[MAX_CHAR], cityFile[MAX_CHAR];

    while (fscanf(fp, "%s %s\n", name, cityFile) == 2) {
        int index = hashFunction(name);

        countryPntr new = createCountry(name);
        new->cities = loadCities(cityFile);

        hashTable[index] = insertCountrySorted(hashTable[index], new);
    }

    fclose(fp);
    return EXIT_SUCCESS;
}

countryPntr findCountry(countryPntr hashTable[], char* name) {
    int index = hashFunction(name);
    countryPntr current = hashTable[index];

    while (current) {
        if (strcmp(current->name, name) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

int printHashTable(countryPntr hashTable[]) {
    for (int i = 0; i < HASH_SIZE; i++) {
        printf("\nHash %d:\n", i);
        countryPntr current = hashTable[i];
        while (current) {
            printf("%s:\n", current->name);
            printCities(current->cities);
            current = current->next;
        }
    }

    return EXIT_SUCCESS;
}
