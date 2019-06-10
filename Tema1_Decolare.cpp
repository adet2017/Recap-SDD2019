//Lista dublu inlantuita respecta principiul FIFO (principiul de lucru cu o coada). 
//Structura reprezinta coada formata de avioanele care urmeaza sa decoleze de pe pista.
//Articolul "Decolare" reprezinta informatia utila din elementele listei, cu ajutorul careia poate fi identificat un avion.

#include <iostream>

struct Decolare {
	int nrZbor;
	char* companie;
	bool arePrioritate;
	int poarta;
};

struct Nod {
	Decolare info;
	Nod* prev;
	Nod* next;
	
};

struct ListaDubla {
	Nod* first;
	Nod* last;
};

Decolare creareDecolare(int nrZbor, const char* companie, bool arePrioritate, int poarta) {
	Decolare d;
	d.nrZbor = nrZbor;
	d.companie = (char*)malloc(sizeof(char)*(strlen(companie)+1));
	strcpy(d.companie, companie);
	d.arePrioritate = arePrioritate;
	d.poarta = poarta;
	return d;
}

void afisareDecolare(Decolare d) { 
	if(d.arePrioritate){
	printf("Avionul cu numarul de zbor %d care apartine companiei aeriene %s va decola cu prioritate de la poarta %d\n", d.nrZbor, d.companie, d.poarta);
	}
	else {
		printf("Avionul cu numarul de zbor %d care apartine companiei aeriene %s va decola cu intarziere de la poarta %d\n", d.nrZbor, d.companie, d.poarta);
	}
}


Decolare copiereDecolare(Decolare d) {
	Decolare copie;
	copie.nrZbor = d.nrZbor;
	copie.companie = (char*)malloc(sizeof(char)*(strlen(d.companie)+1));
	strcpy(copie.companie, d.companie);
	copie.arePrioritate = d.arePrioritate;
	copie.poarta = d.poarta;
	return copie;
}

//inserare la sfarsit conform principiului FIFO
ListaDubla inserareSfarsit(ListaDubla ld, Decolare d) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = copiereDecolare(d);
	nou->prev = nou->next = NULL; 
	if (ld.last) {
		nou->prev = ld.last;
		ld.last->next = nou;
		ld.last = nou;
	}
	else {
		ld.first = ld.last = nou;
	}
	return ld;
}

//extragere de la inceput conform principiului FIFO
Decolare extragereDecolare(ListaDubla* ld) {
	Decolare d = copiereDecolare(ld->first->info);
	free(ld->first->info.companie);
	if (ld->first) {
		ld->first = ld->first->next;
		free(ld->first->prev);
	}
	else {
		printf("Nu exista noduri in lista");
	}
	return d;
}


void afisareLista(ListaDubla ld) {
	Nod* p = ld.first;
	while (p) {
		afisareDecolare(p->info);
		p = p->next;
	}
}


//fct ajutatoare pt filtrare!!
int nrDecolari(ListaDubla ld) {
	Nod* p = ld.first;
	int contor = 0;
	while (p) {
		if (p->info.arePrioritate) {
			contor++;
		}
		p = p->next;
	}
	return contor;
}

//functie care filtreaza lista si returneaza un vector care contine doar decolarile cu prioritate
Decolare* filtrare(ListaDubla ld) {
	Decolare* vector = (Decolare*)malloc(sizeof(Decolare) * nrDecolari(ld));
	Nod* p = ld.first;
	int i = 0; 
	while (p && i < nrDecolari(ld)) { //aici tot timpul e la fel in while
		if (p->info.arePrioritate) { //conditia o scriem de fapt in acest if
			vector[i] = copiereDecolare(p->info);
			p = p->next;
			i++;
		}
		else { //nu merge fara acest else!! (asta daca avem o conditie - un if)
			p = p->next;//situatia in care decolarea nu are prioritate...pt ca trebuie sa trecem peste acel nod
		}
	}
	return vector;
}

void stergereLista(ListaDubla ld) {
	while (ld.first) {
		free(ld.first->info.companie); 
		ld.first = ld.first->next; 
		if (ld.first) { 
		free(ld.first->prev); 
		}
		else { 
			free(ld.last); 
		}
	}
}


void main() {

	ListaDubla ld;
	ld.first = ld.last = NULL;

	Decolare d1 = creareDecolare(1, "TAROM", true, 12);
	Decolare d2 = creareDecolare(2, "BRITISH AIRWAYS", false, 8);

	ld = inserareSfarsit(ld, d1);
	ld = inserareSfarsit(ld, d2);
	ld = inserareSfarsit(ld, creareDecolare(3, "LUFTHANSA", true, 10));
	ld = inserareSfarsit(ld, creareDecolare(4, "ELAL", false, 3));
	ld = inserareSfarsit(ld, creareDecolare(5, "AIRFRANCE", true, 20));

	afisareLista(ld);
	
	printf("\nDecolarile cu prioritate:\n");
	Decolare* v = filtrare(ld);
	for (int i = 0; i < nrDecolari(ld); i++) {
		afisareDecolare(v[i]);
	}

	
	printf("\nDecolarea extrasa:\n");
	afisareDecolare(extragereDecolare(&ld));

	stergereLista(ld);
	ld.first = ld.last = NULL;
}

