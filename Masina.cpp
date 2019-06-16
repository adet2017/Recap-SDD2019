#include <iostream>

struct Masina { 
	char* inmatriculare;
	int an;
	int nrLocuri;
	float pret;
};

struct Nod { //elemente de tip pointer la Masina: Masina* info 
	Masina info;
	Nod* next;
};

Masina creareMasina(const char* inmatriculare, int an, int nrLocuri, float pret) {
	Masina m;
	m.inmatriculare = (char*)malloc(sizeof(char)*(strlen(inmatriculare) + 1));
	strcpy(m.inmatriculare, inmatriculare);
	m.an = an;
	m.nrLocuri = nrLocuri;
	m.pret = pret;
	return m;
}

Masina copiereMasina(Masina m) {
	Masina copie;
	copie.inmatriculare = (char*)malloc(sizeof(char)*(strlen(m.inmatriculare) + 1));
	strcpy(copie.inmatriculare, m.inmatriculare);
	copie.an = m.an;
	copie.nrLocuri = m.nrLocuri;
	copie.pret = m.pret;
	return copie;
}


//pct 1 - LSI gestionata pe principiul FIFO
Nod* inserareSfarsit(Nod* cap, Masina m) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = copiereMasina(m);
	nou->next = NULL;
	if (cap) {
		Nod* p = cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		cap = nou;
	}
	return cap;
}


Nod* pushQueue(Nod* coada, Masina m) {
	coada = inserareSfarsit(coada, m);
	return coada;
}


Masina popQueue(Nod* &coada) {
	if (coada) {
		Masina m = copiereMasina(coada->info);
		Nod* p = coada; //facem o copie ca apoi sa il stergem pe p (ca la stergere LSI-vezi comentarii la Carte)
		coada = p->next; //aici "coada=" nu "p="
		//(functia trebuie sa extraga din fata un element, deci coada(primul element) devine p->next(fostul al doilea)
		free(p);
		return m;
	}
	else {
		return creareMasina("", -1, -1, -1);
	}
}




void afisareMasina(Masina m) {
	printf("Masina cu nr de inmatriculare %s, fabricata in anul %d, are %d locuri, iar pretul este de %5.2f EUR.\n", m.inmatriculare, m.an, m.nrLocuri, m.pret);
}


void afisareLista(Nod* cap) {
	Nod* p = cap;
	while (p) {
		afisareMasina(p->info);
		p = p->next;
	}
}


//pct 2 - afisare lista folosind doar operatiile specifice structurii de tip FIFO (conform cerintei):
void afisareCoada(Nod* &coada) {
	while (coada) { 
		Masina m = popQueue(coada);
		afisareMasina(m);
	}
}


//pct 3 - extragerea masinilor care au atributul nrLocuri intr-un interval 
//(extragerea este implementata folosind doar operatiile de tip FIFO?)
Nod* extragereMasini(Nod* cap, int pragMin, int pragMax) {
	Nod* coada = NULL;
	Nod* p = cap;
	while (p) {
		if (p->info.nrLocuri>pragMin && p->info.nrLocuri<pragMax) {
			coada = pushQueue(coada, p->info);
			/*free(p);*/ //eroare de exec
		}
		p = p->next;
	}
	return coada;
}


//pct 4 - conversia structurii intr-un masiv unidimensional prin extragerea tuturor elementelor din lista
int nrMasini(Nod* cap) {
	Nod* p = cap;
	int contor = 0;
	while (p) {
		contor++;
		p = p->next; 
	}
	return contor;
}

Masina* conversie (Nod* cap) {
	Masina* vector = (Masina*)malloc(sizeof(Masina)*nrMasini(cap));
	Nod* p = cap;
	int nr = nrMasini(cap); //de scris in plus fata de fct filtrare
	for (int i = 0; i < nr; i++) { //si aici il folosim pe nr
		vector[i] = popQueue(cap); //deoarece din cauza extragerii, la fiecare iteratie nrMasini(cap) se micsoreaza si il deruteaza
	}                                                           
	return vector;
}


//pct 5 - fct care utilizeaza masivul pentru a afisa valoarea totala a masinilor pe fiecare an
//ar trebui sa faci:
//1. sa nr cati ani diferiti ai in lista
//2. sa faci un vector de ani alocat dinamic
//3. sa parcurgi lista si sa afisezi cand anul din struct e acelasi cu primul an
//apoi cu al doilea si tot asa


int isEmptyQueue(Nod* cap) {
	return cap == NULL;
}



void main() {
	Nod* coada = NULL;

	coada = pushQueue(coada, creareMasina("B394820", 2010, 4, 5000.55));
	coada = pushQueue(coada, creareMasina("B993799", 2016, 5, 8000));
	coada = pushQueue(coada, creareMasina("B45DIY", 2016, 2, 9000));
	coada = pushQueue(coada, creareMasina("B55PKI", 2010, 20, 4400));

	afisareLista(coada);

	//pct 2 - afisare conform cerintei - sterge lista
	/*afisareCoada(coada);*/


	//pct 3
	printf("\nLista masini care au nr de locuri in intervalul 1-10:\n");
	afisareLista(extragereMasini(coada, 1, 10));

	//pct 4
	printf("\nAfisare vector masini:\n");
	Masina* v = conversie(coada);
	int nr = nrMasini(coada);
	for (int i = 0; i < nr; i++) {
		afisareMasina(v[i]);
	}


	//pct 5


}