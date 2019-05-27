#include<iostream>

struct Intrerupator {
	char* culoare;
	int numarButoane;
};

struct nodSimplu {
	Intrerupator info;
	nodSimplu* next;
};


struct nodDublu {
	Intrerupator info;
	nodDublu* next;
	nodDublu* prev;
};

struct LDI {
	nodDublu* prim;
	nodDublu* ult;
};



Intrerupator creareObiect(const char* culoare, int nrButoane) {
	Intrerupator intrerupator;
	intrerupator.culoare = (char*)malloc(sizeof(char)*(strlen(culoare) + 1));
	strcpy(intrerupator.culoare, culoare);
	intrerupator.numarButoane = nrButoane;
	return intrerupator;
}

nodDublu* creareNod(Intrerupator info, nodDublu* next, nodDublu* prev) {
	nodDublu* nodNou = (nodDublu*)malloc(sizeof(nodDublu));
	nodNou->info = creareObiect(info.culoare, info.numarButoane);
	nodNou->next = next;
	nodNou->prev = prev;
	return nodNou;
}

LDI inserareInceput(LDI lista, Intrerupator info) {
	nodDublu* nodNou = creareNod(info, lista.prim, NULL);
	if (lista.prim) {
		lista.prim->prev = nodNou;
		lista.prim = nodNou;
	}
	else {
		lista.prim = nodNou;
		lista.ult = nodNou;
	}
	return lista;
}

void afisareObiect(Intrerupator info) {
	printf("Intrerupatorul de culoare %s are %d butoane\n", info.culoare, info.numarButoane);
}

nodSimplu*inserareInceput(nodSimplu* cap, Intrerupator info) {
	nodSimplu* nodNou = (nodSimplu*)malloc(sizeof(nodSimplu));
	nodNou->info = creareObiect(info.culoare, info.numarButoane);
	nodNou->next = cap;
	return nodNou;

}
void afisareListaDubla(LDI lista, int flag) {
	if (flag == 0) { //n-a mai facut un p auxiliar ca sa nu altereze lista
		while (lista.prim) {
			afisareObiect(lista.prim->info);
			lista.prim = lista.prim->next;
		}
	}
	else { //el a pus flag 1 si atunci afiseaza doar invers si nu se vede ca e gresita inserarea sortata 
		while (lista.ult) {
			afisareObiect(lista.ult->info);
			lista.ult = lista.ult->prev;
		}
	}
}

void afisareListaCrescator(LDI ld) {
	nodDublu* p = ld.prim;
	while (p) {
		afisareObiect(p->info);
		p = p->next;
	}
}

LDI stergereLista(LDI lista) {
	while (lista.prim) {
		free(lista.prim->info.culoare);
		nodDublu* pNod = lista.prim;
		lista.prim = lista.prim->next;
		free(pNod);
	}
	lista.ult = NULL;
	return lista;
}

LDI inserareSortata(LDI lista, Intrerupator info) {
	if (lista.prim) {
		nodDublu* p = lista.prim;
		while (p && p->info.numarButoane < info.numarButoane) {
			p = p->next;
		}
		if (p) { 
			nodDublu* nodNou = creareNod(info, p, p->prev);
			p->prev = nodNou;
			if (nodNou->prev) {
				/*p->prev->next = nodNou;*/ //aici trebuie sa fie nodNou->prev->next = nodNou;
				nodNou->prev->next = nodNou;
			}
			else {
				lista.prim = nodNou;
			}
		}
		else {
			nodDublu* nodNou = creareNod(info, NULL, lista.ult);
			lista.ult->next = nodNou;
			lista.ult = nodNou;
		}
	}
	else {
		nodDublu* nodNou = creareNod(info, NULL, NULL);
		lista.prim = lista.ult = nodNou;
	}
	return lista;
}

//extragere cu returnarea intrerupatoarelor intr-o lista simpla
//extragere cu prag maxim - cu exemplu de intrerupator care indeplineste conditia - Rosu, 1 (am mers cu debugger)
nodSimplu* extragereIntrerupatoare(LDI *lista, int pragButoane) {
	nodDublu* p = lista->prim; //nu e neaparat necesar nici p? vezi la Test si la Comanda. Dar e mai clar cu p si mai usor de desenat
	nodSimplu* cap = NULL; //in plus fata de extragere cu tip returnat Muzeu. Partea asta e pt inserare in lista simpla
	while (p) {
		if (p->info.numarButoane < pragButoane) { //de ce nu a pus conditia asta in while ca la Muzeu? La prima iteratie descoperim ca primul nod din lista (Rosu) indeplineste conditia din if
			cap = inserareInceput(cap, p->info); //il inseram pe Rosu in lista simpla
			nodDublu*q = p; //in plus fata de extragere la Muzeu. q devine p adica Rosu,1
		    p = p->next; //p devine Galben (q a ramas Rosu)
			if (q->prev) { //q->prev este NULL la prima iteratie (nu e nimic inainte de Rosu) deci se executa else, nu se executa if
				if (p) { //nu puteam mai sus sa punem if(p&&...) ca sa nu mai punem inca un if aici?
					p->prev = q->prev; //
				}
				else {
					lista->ult = q->prev; //aici e situatia daca nu exista noduri in lista?
				}
				q->prev->next = p; //ca la Muzeu doar ca e p in loc de p->next
			}
			else { //se executa else pt ca nu exista noduri inaintea lui q (Rosu)
				if (p) {
					p->prev = NULL; //p, adica Galben, pointeaza in stanga catre NULL pt ca inaintea lui Galben nu va mai fi nimic, pe Rosu il extragem si il stergem
				}
				else {
					lista->ult = NULL;
				}
				lista->prim = p; //p, adica Galben, devine primul nod din lista
			}
			free(q->info.culoare); //am mutat stergerea mai jos aici la fel ca la Muzeu (e acelasi lucru). La Test nu este stergere, de ce??
			free(q); //il stergem pe Rosu
		}
		else {
			p = p->next;
		}
	} //p a ramas Galben, p->next e Albastru
	return cap;
}

//extragere cu prag minim - cu exemplu de intrerupator care indeplineste conditia - Gri, 5 (cu debugger)
nodSimplu* extragereIntrerupatoarePragMinim(LDI *lista, int pragButoane) {
	nodDublu* p = lista->prim;
	nodSimplu* cap = NULL;
	while (p) {
		if (p->info.numarButoane > pragButoane) { //la prima iteratie Rosu nu indeplineste conditia deci sare pe ramura de else (la fel si pt urmatoarele - Galben, Albastru, Alb)
			                                     //ajungem la Gri care respecta conditia (5 > decat pragul de 4)
			cap = inserareInceput(cap, p->info); //il inseram pe Gri in lista simpla
			nodDublu*q = p; //q devine p, adica Gri
			p = p->next; //p devine Negru (q a ramas Gri)
			free(q->info.culoare);
			if (q->prev) { //se executa acest if pt ca la iteratia aceasta exista q->prev (avem un nod inaintea lui q (a lui Gri)
				if (p) { //dc p->next apare ca fiind Null aici cand mergem cu debugger? 
					p->prev = q->prev; //p (Negru) pointeaza inapoi spre nodul dinaintea lui q (adica nodul din stanga lui Gri). Practic il sare pe Gri pt ca acesta va fi sters
				}
				else {
					lista->ult = q->prev;
				}
				q->prev->next = p; //nodul din stanga lui q va pointa in fata (dreapta) catre p(Negru). Sare peste Gri
			}
			else {
				if (p) {
					p->prev = NULL;
				}
				else {
					lista->ult = NULL;
				}
				lista->prim = p;
			}
			free(q); //il stergem pe Gri
		}
		else {
			p = p->next; //le sarim pe Rosu, Galben, Albastru, Alb pt ca nu indeplinesc conditia din if si trecem la urmatorul pana ajungem la Gri
		}
	} //p a ramas Negru. p->next e NULL fiindca nu mai avem alte noduri dupa 

	return cap;
}


void main() {
	LDI lista;
	lista.prim = NULL;
	lista.ult = NULL;
	lista = inserareInceput(lista, creareObiect("Alb", 2)); //2
	lista = inserareInceput(lista, creareObiect("Albastru", 4)); //4,2
	lista = inserareInceput(lista, creareObiect("Galben", 3));//3,4,2
	lista = inserareInceput(lista, creareObiect("Rosu", 1));//1,3,4,2
	/*lista = inserareSortata(lista, creareObiect("Gri", 4));*/ //asta se insereaza pe ramura if
	lista = inserareSortata(lista, creareObiect("Negru", 6)); //am inserat pe ramura else
	lista = inserareSortata(lista, creareObiect("Gri", 5)); //inseram pe ramura if

	/*afisareListaDubla(lista, 1);*/ //aici afiseaza descrescator si de asta apare 5
	afisareListaCrescator(lista); //aici nu mai afiseaza intrerupatorul cu 5 butoane din cauza ca e gresita inserarea

	nodSimplu* cap = extragereIntrerupatoare(&lista, 4);
	printf("\nLista extrasa:\n");
	while (cap) {
		afisareObiect(cap->info); //nici aici nu-l afiseaza pe 5
		free(cap->info.culoare);
		nodSimplu* q = cap;
		cap = cap->next; //la fel il sare, daca nu e facuta legatura (e gresita fct de inserareSortata)
		free(q);
	}
	printf("\nAfisare lista dupa extragere:\n");
	/*afisareListaDubla(lista, 1);*/ //aici nu afiseaza fiindca e o problema la extragere, daca o comentam merge
	afisareListaCrescator(lista);

	lista = stergereLista(lista);
}