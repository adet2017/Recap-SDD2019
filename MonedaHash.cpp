#include <iostream>



struct Moneda {
	char cod[20];
	char* denumire;
	char* emitent;
	int sold;
};

struct Nod {
	Moneda info;
	Nod* next;
};

struct HashTable {
	Nod** vector;
	int dim;
};


Moneda creareMoneda(const char* cod, const char* denumire, const char* emitent, int sold) {
	Moneda m;
	strcpy(m.cod, cod);
	m.denumire = (char*)malloc(sizeof(char)*(strlen(denumire) + 1));
	strcpy(m.denumire, denumire);
	m.emitent = (char*)malloc(sizeof(char)*(strlen(emitent) + 1));
	strcpy(m.emitent, emitent);
	m.sold = sold;
	return m;
}


//fct de inserare inceput in lista simpla
Nod* inserareInceput(Nod* cap, Moneda m) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = m; //shalow copy
	nou->next = cap;
	return nou; //noul nod va fi returnat ca nou cap al listei
}

//fct de inserare la final in lista simpla
Nod* inserareSfarsit(Nod* cap, Moneda m) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = m;
	nou->next = NULL;
	if (cap) {
		Nod* p = cap;
		while (p->next) { //cat timp p->next diferit de NULL, trecem la urmatorul
			p = p->next;
		} //cand l-am gasit pe ultimul (p->next = NULL), a iesit din while. 
		p->next = nou; //fostul ultim nod(cel pe care l-am gasit in while) va pointa catre noul nod
	}
	else { //daca nu exista lista
		cap = nou; //noul nod este singurul nod si deci primul din lista
	}
	return cap;
}

//fct folosita in main pt initializarea tabelei
HashTable initializareHashTable(int dim) {
	HashTable h;
	h.dim = dim;
	h.vector = (Nod**)malloc(sizeof(Nod*)*dim);
	for (int i = 0; i < dim; i++) {
		h.vector[i] = NULL;
	}
	return h;
}


//fct ajutatoare pentru inserare tabela
int hashCode(/*const char* cod*/ int sold, HashTable h){
	return /*cod[0]*/sold % h.dim; //calculam codul ASCII
	//am pus sold in loc de cod ca sa mearga la cautareMoneda
}


int inserareHashTable(HashTable h, Moneda m) {
	if (h.vector) {
		int pozitie = hashCode(m.sold, h);
		if (h.vector[pozitie]) {
			h.vector[pozitie] = inserareInceput(h.vector[pozitie], m); //in loc de cap avem h.vector[pozitie] !
		}
		else {
			h.vector[pozitie] = inserareInceput(h.vector[pozitie], m); //else - exact acelasi lucru!!!
		}
		return pozitie; //trebuie sa returnam pozitia care e un int (tipul returnat al fct e un int)
	}
	else {
		return -1;
	}
}



//afisare Moneda
void afisareMoneda(Moneda m) {
	printf("Moneda %s, denumita %s, emisa de %s, are soldul valutar %d.\n", m.cod, m.denumire, m.emitent, m.sold);
}


//afisare tabela - similara cu afisarea la lista simpla
void afisareHashTable(HashTable h) {
	for (int i = 0; i < h.dim; i++) { //cat timp i < decat dimensiunea maxima a tabelei. Doar acest for e in plus fata de afisarea la lista
		Nod* p = h.vector[i]; //in loc de cap avem h.vector[i] (la fel ca la inserarea mai sus doar ca acolo in loc de i avem pozitie)
		while (p) {
			afisareMoneda(p->info);
			p = p->next;
	    }
	}
}


//cautare in tabela - afisare monede care au soldul valutar mai mare decat o valoare primita ca parametru
void afisareMonedeSoldPeste5(HashTable h, int sold = 5) {
	for (int i = 0; i < h.dim; i++) {
		Nod* p = h.vector[i];
		while (p) {
			if (p->info.sold > 5) {
				afisareMoneda(p->info);
			}
			p = p->next;
		}
	}
}

//functie ajutatoare pt cautare in tabela (mai intai cautam in lista, practic e ca functia de filtrare de la Carte):
Moneda* cautareMonedaInLista (Nod* cap, int soldMinim) { //tipul returnat ar trebui sa fie Moneda* ca sa returneze un vector de monede
	Nod* p = cap;
	int contor = 0;
	while (p) {
		if (p->info.sold > soldMinim) { //daca soldul din nodul curent este mai mare decat soldul minim
			contor++; //marim contorul
		}
		p = p->next; //trecem la urmatorul nod unde verificam acelasi lucru
	} //la iesire din while contorul isi pastreaza ultima valoare, deci retine cate monede indeplinesc conditia
	//mai jos folosim contorul ca sa alocam memorie fix pentru cate monede indeplinesc conditia:
	Moneda* vector = (Moneda*)malloc(sizeof(Moneda)*contor);
	p = cap; //revenim la inceputul listei (pt ca p nu mai era cap la iesire din while)
	int i = 0;
	while (p && i < contor) {
		if (p->info.sold > soldMinim) { //aceeasi conditie
			vector[i] = p->info; //daca se respecta conditia, copiem moneda in vector
			p = p->next; //si trecem la nodul urmator unde verificam acelasi lucru
			i++;
		}
		else { //daca nu se respecta conditia
			p = p->next; //doar trecem mai departe
		}
	}
	return vector;
}


Moneda* cautareMoneda(HashTable h, int soldMinim) { //trebuie si aici Moneda*
	if (h.vector) {
		int pozitie = hashCode(soldMinim, h); //aici nu mergea fiindca fct are cheia de cautare sold iar hashCode are cheia de cautare cod(char)
		return cautareMonedaInLista(h.vector[pozitie], soldMinim); 
	}
	else {
		Moneda m;
		strcpy(m.cod, "");
		m.denumire = NULL;
		m.emitent = NULL;
		m.sold = -1;
	}
}


//stergere in functie de un criteriu - mai ok facuta de Mihai:
void stergereBySold(HashTable * h, int soldCautat) { //HashTable* !
	for (int i = 0; i < h->dim; i++) {
		Nod* p = h->vector[i];
		while (p) {
			if (p->info.sold == soldCautat) {
				free(p->info.denumire);
				free(p->info.emitent);
				Nod* temp = p;
				p = p->next;
				free(temp);
			}
			else {
				p = p->next;
			}
		}
	}
}


//void stergereBySold(HashTable* h, int soldCautat) { //HashTable* !
//	for (int i = 0; i < h->dim; i++) {
//		Nod* p = h->vector[i];
//		while (p && p->info.sold == soldCautat) {
//			free(p->info.denumire);
//			free(p->info.emitent);
//			Nod* temp = p;
//			p = p->next;
//			free(temp);
//		}
//	}
//	free(h->vector);
//	h->vector = NULL;
//	h->dim = 0;
//}

//suma soldului
int soldCumulat(HashTable h) {
	int suma = 0; //int suma trebuie sa fie in afara for!!!
	for (int i = 0; i < h.dim; i++) {
		Nod* p = h.vector[i];
		while (p) {
			suma += p->info.sold;
			p = p->next;
		}
	}
	return suma;
}

//numarare monede care au soldul mai mare decat o val 
int nrMonede(int nr, HashTable h) {
	int contor = 0;
	for (int i = 0; i < h.dim; i++) {
		Nod* p = h.vector[i];
		while (p) {
			if (p->info.sold > nr) {
				contor++;
			}
			p = p->next;
		}
	}
	return contor;
}


void main() {

	HashTable tabela = initializareHashTable(5); //cerinta spune sa inseram cel putin 5 structuri de tip Moneda

	inserareHashTable(tabela, creareMoneda("RON", "lei", "Romania", 46));
	inserareHashTable(tabela, creareMoneda("EUR", "euro", "Uniunea Europeana", 1));
	inserareHashTable(tabela, creareMoneda("$", "dollar", "SUA", 44));

	afisareHashTable(tabela);


	//testare fct cautareMoneda - nu merge :(
	printf("\nVector monede cu soldul mai mare decat 5:\n");
	afisareMonedeSoldPeste5(tabela, 5);
	///*afisareMoneda(cautareMoneda(tabela, 5));*/
	//Nod* lista = NULL; //am initializat o lista pt ca nu era initializata
	//Moneda* v = cautareMoneda(tabela, 5); //cu lista nu merge fiindca nu este nimic in lista?
	////de aici in jos e acelasi cod ca in prima parte a functiei (pana la afisare!)
	//Nod* p = lista; //ii facem o copie (mai e necesar?)
	//int contor = 0;
	//while (p) {
	//	if (p->info.sold > 5) { //aici scriem direct cat vrem sa fie soldul minim
	//		contor++;
	//	}
	//	p = p->next;
	//}
	////afisam vectorul - aici trebuie contorul si de asta scriem codul de mai sus! 
	//sau putem sa facem o fct de numarare pe care o apelam in loc de contor(vezi la fct de conversie de la LSI Masina - pct 4)
	//for (int i = 0; i < contor; i++) { //i < contor la fel cum e si conditia din al 2-lea while din functie
	//	afisareMoneda(v[i]); //afisam pe rand monedele din vector
	//}
	

	printf("\nSoldul cumulat al monedelor este %d.\n", soldCumulat(tabela)); //afiseaza 0 

	printf("\nNumarul de monede care au soldul mai mare decat 10 este %d.\n", nrMonede(10, tabela)); //afiseaza 0



	stergereBySold(&tabela, 1);

	printf("\nAfisare dupa stergere:\n");
	afisareHashTable(tabela);

}
