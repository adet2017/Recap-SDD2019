#include <iostream>
#include<stdio.h>
#include<string.h>
#include<malloc.h>

//HashTable - vector de liste

struct Curs {
	int id;
	char* denumire;
	int nrPrezenti;
};


//struct Nod {
//	Curs info;
//	Nod* next;
//};

struct HashTable {
	/*Nod** vector;*/ //cand facem inserare cu ajutorul listei simple
	Curs** vector; //cand facem inserare ca la seminar 2 ID 2019 - Muzeu
	int dimensiune;
};


Curs creareCurs(int id, const char* denumire, int nrPrezenti){
	Curs c;
	c.id = id;
	c.denumire = (char*)malloc(sizeof(char)*strlen(denumire) + 1);
	strcpy(c.denumire, denumire);
	c.nrPrezenti = nrPrezenti;
	return c;
	}


//Nod* inserareInceput(Nod* cap, Curs c) {
//	Nod* nou = (Nod*)malloc(sizeof(Nod));
//	nou->info = c;
//	nou->next = cap;
//	return nou;
//}

HashTable initializareHashTable(int dimensiune) {
	HashTable h;
	h.dimensiune = dimensiune;
	/*h.vector = (Nod**)malloc(sizeof(Nod*)*dimensiune);*/ //e tratat ca un float*
	h.vector = (Curs**)malloc(sizeof(Curs*)*dimensiune); //ca la Muzeu
	for (int i = 0; i < dimensiune; i++) {
		h.vector[i] = NULL;
	}
	return h;
}


//hashCode ca la Curs (gr.1044 s06) - se face inainte de inserareHashTable
int hashCode(int id, HashTable h) {
	return id % h.dimensiune;
}

//inserareHashTable ca la Curs (gr.1044 s06) - foloseste functia hashCode
//int inserareTabela(HashTable h, Curs c) { //tipul returnat int!!!
//	if (h.vector) {
//		int pozitie = hashCode(c.id, h); //c.id in loc de c (cum a facut la noi la seminar) pt ca functia hashCode primeste int si HashTable.
//		if (h.vector[pozitie]) {
//			h.vector[pozitie] = inserareInceput(h.vector[pozitie], c); //in loc de cap avem "h.vector[pozitie]"
//		}
//		else {
//			h.vector[pozitie] = inserareInceput(h.vector[pozitie], c);
//		}
//		return pozitie;
//	}
//	else {
//		return -1;
//	}
//}


//functie hasFunction ca la seminar 2 ID 2019
int hashFunction(Curs c, HashTable h) {
	return c.denumire[0] % h.dimensiune;
}


//inserare HashTable ca la seminar 2 - ID 2019 - foloseste hashFunction
int inserareHashTable(HashTable h, Curs c) {
	if (h.dimensiune > 0) { //daca dimensiunea tabelei nu e 0
		int pozitie = hashFunction(c, h);
		if (h.vector[pozitie]) {
			int index = (pozitie + 1) % h.dimensiune;
			while (h.vector[index] && index != pozitie) {
				index = (index + 1) % h.dimensiune;
			}
			if (pozitie == index) {
				return -1; //cod eroare pentru tabela full
			}
			else {
				h.vector[index] = (Curs*)malloc(sizeof(Curs)); //eroare compilare aici
				*(h.vector[index]) = creareCurs(c.id, c.denumire, c.nrPrezenti); //nu ma lasa
				return index;
			}
		}
		else { //acelasi lucru doar ca pozitie in loc de index:
			h.vector[pozitie] = (Curs*)malloc(sizeof(Curs)); //merge si h.vector[pozitie]=c in loc de astea doua randuri
			*(h.vector[pozitie]) = creareCurs(c.id, c.denumire, c.nrPrezenti);
			return pozitie;
		}
	}
	else {
		return -2;
		}
}



void afisareCurs(Curs c) {
	printf("%d. Cursul %s are %d studenti prezenti.\n", c.id, c.denumire, c.nrPrezenti);
}


//afisare HashTable ca la Curs - asemanatoare cu afisarea la lista
//void afisareHashTable(HashTable h) {
//	for (int i = 0; i < h.dimensiune; i++) { //doar acest for in plus fata de afisarea la lista simpla
//		Nod* p = h.vector[i];
//		while (p) {
//			afisareCurs(p->info);
//			p = p->next;
//		}
//	}
//}

//afisare HashTable ca la seminar 2 2019 ID
void afisareTabela(HashTable h) {
	for (int i = 0; i < h.dimensiune; i++) {
		if (h.vector[i]) {
			afisareCurs(*(h.vector[i]));
		}
	}
}


//fct de cautare - de la seminar 2 ID 2019 / am modificat in loc de denumire sa fie cu int ca la ZA
int cautaCurs(Curs c, HashTable h) {
	if (h.dimensiune > 0) {
		int pozitie = hashFunction(c, h);
		if (/*h.vector[pozitie]->id==c.id*/ h.vector[pozitie] && strcmp(c.denumire, h.vector[pozitie]->denumire)==0) { 
		return pozitie;
		}
		else {
			int index = (pozitie + 1) % h.dimensiune;
			while (index != pozitie) {
				if (h.vector[index] && strcmp(c.denumire, h.vector[index]->denumire) == 0) { 
					return index;
				}
			}
			index = (index + 1) % h.dimensiune;
		}
		return -1;
	}
	else {
		return -2;
	}
}


//cautare in lista- functie ajutatoare pt cautare in tabela
//Curs cautareCursInLIsta(Nod* cap, int id) {
//	while (cap && cap->info.id != id) {
//		cap = cap->next;
//	}
//	if (cap) {
//		return cap->info;
//	}
//	else {
//		Curs c;
//		c.id = -1;
//		c.denumire = NULL;
//		return c;
//	}
//}


//fct cautare - de la Curs
//Curs cautareCursDupaId(HashTable h, int id) {
//	if (h.vector) {
//		int pozitie = hashCode(id, h);
//		return cautareCursInLIsta(h.vector[pozitie], id);
//	}
//	else {
//		Curs c;
//		c.id = -1;
//		c.denumire = NULL;
//		return c;
//	}
//}


//extragere - de la Za seminar 6
//Curs extragereCursDinTabela(HashTable h, int pozitie) {
//	if (h.dimensiune > 0 && pozitie < h.dimensiune && pozitie >= 0) {
//		if (h.vector[pozitie]) {
//			Curs* pointer = h.vector[pozitie]; //nu merge
//			h.vector[pozitie] = NULL;
//			return pointer;
//		}
//	}
//	return NULL;
//}


//stergere tabela - de la Curs
//void stergere(HashTable* h) {
//	for (int i = 0; i < h->dimensiune; i++) {
//		Nod* p = h->vector[i];
//		while (p) {
//			free(p->info.denumire);
//			Nod* temp = p;
//			p = p->next;
//			free(temp);
//		}
//	}
//	free(h->vector);
//	h->vector = NULL;
//	h->dimensiune = 0;
//}


//stergere de la seminar 2 2019 - ID
HashTable stergereTabela(HashTable h) {
	for (int i = 0; i < h.dimensiune; i++) {
		if (h.vector[i]) {
			free(h.vector[i]->denumire); //stergem denumirile alocate dinamic
			free(h.vector[i]); //stergem memoria alocata pt fiecare element in parte din vector
		}
	}
	free(h.vector); //stergem vectorul
	h.vector = NULL;
	return h;
}






void main() {
	HashTable tabela = initializareHashTable(5); //5 este dimensiunea tabelei

	Curs c1 = creareCurs(1, "SDD", 35);
	inserareHashTable (tabela, c1); //fara "=" !!!!!!!!!!!!!
	inserareHashTable(tabela, creareCurs(2, "PAW", 30));
	inserareHashTable (tabela, creareCurs(3, "PEAG", 15));
	/*inserareTabela (tabela, creareCurs(6, "Java", 20));*/
	/*inserareTabela (tabela, creareCurs(7, "PSI", 17));*/
	/*inserareTabela (tabela, creareCurs(8, "DAM", 21));*/


	/*afisareHashTable(tabela);*/
	afisareTabela(tabela);


	/*printf("\nCautare curs dupa id:\n");*/
	/*afisareCurs(cautareCursDupaId(tabela, 3));*/

	printf("\nTestare cautaCurs:\n");
	int indexSDD = cautaCurs(c1, tabela);
	afisareCurs(*(tabela.vector[indexSDD]));


	/*stergere(&tabela);*/
	tabela = stergereTabela(tabela); 


}