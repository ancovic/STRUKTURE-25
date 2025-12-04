#include "functions.h"

/*6. Napisati program koji cita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji
 predstavljaju pojedini racun. Na pocetku svake datoteke je zapisan datum u kojem vremenu je
 racun izdat u formatu YYYY-MM-DD. Svaki sljedeci red u datoteci predstavlja artikl u formatu
 naziv, kolicina, cijena. Potrebno je formirati vezanu listu racuna sortiranu po datumu. Svaki cvor
 vezane liste sadrzava vezanu listu artikala sortiranu po nazivu artikla. Nakon toga potrebno je
 omoguciti upit kojim ce korisnik saznati koliko je novaca sveukupno potroseno na specificni
 artikl u odredjenom vremenskom razdoblju i u kojoj je kolicini isti kupljen.*/

int main() {

	receipt Head = { .year = 0, .month = 0, .day = 0, .next = NULL, .product = NULL };

	generateList(&Head);

	userInterface(&Head);

	deleteList(&Head);

	return EXIT_SUCCESS;
}