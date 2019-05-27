#include <iostream>

struct Restaurant {
	unsigned int id;
	char* adresaRestaurant;
	unsigned int nrLocuri;
	unsigned int locuriRezervate;
};


struct Nod {
	Restaurant info;
	Nod* next;
};


Restaurant creareRestaurant(unsigned int id, const char* adresaRestaurant, unsigned int nrLocuri, unsigned int locuriRezervate) {
	Restaurant r;
	r.id = id;
	r.adresaRestaurant = (char*)malloc(sizeof(char) * (strlen(adresaRestaurant) + 1));
	strcpy(r.adresaRestaurant, adresaRestaurant);
	r.nrLocuri = nrLocuri;
	r.locuriRezervate = locuriRezervate;
	return r;
}

Restaurant copiereRestaurant(Restaurant r) {
	Restaurant copie;
	copie.id = r.id;
	copie.adresaRestaurant = (char*)malloc(sizeof(char) * (strlen(r.adresaRestaurant) + 1));
	strcpy(copie.adresaRestaurant, r.adresaRestaurant);
	copie.nrLocuri = r.nrLocuri;
	copie.locuriRezervate = r.locuriRezervate;
	return copie;
}

//inserare la inceput - lista simpla circulara
Nod * inserareInceput(Nod * cap, Restaurant r) { //Nod steluta!!!
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = copiereRestaurant(r);
	if (cap) {
		nou->next = cap; //pana aici e la fel ca la lista simpla
		Nod* p = cap; //aici de ce am mai facut un nod? -- pentru ca si la inserarea la inceput trebuie parcursa lista, deoarece ultimul nod (pe care nu-l putem afla decat daca parcurgem lista) trebuie sa pointeze catre noul nod inserat,
		while (p->next != cap) { //cat timp p e diferit de cap //aici de ce nu mai testam daca exista p? -- pentru ca testam deja if(cap) si atunci nu mai e nevoie
			p = p->next; //se trece la urmatorul nod
		}
		p->next = nou; //aici se rupe legatura veche si se creaza cea noua -- practic, p e fostul ultim nod, deci facem ca acest fost ultim nod sa pointeze catre noul nod, care pointeaza deja catre cap sa pointeze catre cap
	}
	else { //daca nu avem noduri in lista
		nou->next = nou; //?? daca nu avem noduri facem ca nodul introdus sa pointeze catre el insusi si returnam mai jos acest nod
						// nodul cap nu mai conteaza, nu se foloseste nicaieri pt era pur si simplu o adresa care pointa catre NULL si deci nu ne mai interseaza
						// intr-un eventual al doilea apel de inserareInceput, acest nod va fi cap, se va intra in if si va conta
	}
	return nou; //la fel ca la lista simpla si aici
}


//inserare la final - lista simpla circulara
Nod* inserareSfarsit(Nod * cap, Restaurant r) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = copiereRestaurant(r);
	nou->next = cap; //in loc de NULL cum era la lista simpla. aici e singura diferenta
	if (cap) {
		Nod* p = cap; //se creaza un nou pointer catre capul listei pentru a parcurge lista fara a o altera
		while (p->next != cap) { // la fel ca la inserareInceput
			p = p->next;//se trece la urmatorul nod (daca acesta exista). Daca nu exista (adica daca p->next=NULL?) se iese din while, iar p ramane sa pointeze catre ultimul nod.
		}
		p->next = nou; //fostul ultim nod(cel pe care l-am gasit in while) va pointa catre noul nod. 
						// p isi pastreaza valoarea pe care o are dupa ce "trece prin" while - asa se comporta variabilele in c
	}
	else {
		nou->next = nou; // mai trebuie adaugat si aici ca nodul introdus sa pointeze catre el insusi
		cap = nou;//noul nod este singurul nod si deci primul din lista. de ce la inserarea la inceput e nou->next = nou;? - vezi la inserareInceput
	}
	return cap;
}


//afisare Restaurant
void afisareRestaurant(Restaurant r) {
	printf("Restaurantul cu id-ul %d, de la adresa %s are in total %d locuri, din care %d sunt rezervate\n", r.id, r.adresaRestaurant, r.nrLocuri, r.locuriRezervate);
}

//afisare lista
void afisareLista(Nod * cap) {
	Nod* p = cap;
	while (p && p->next != cap) { //aici zamfi zice ca trebuie sa verificam si daca exista p 
		afisareRestaurant(p->info);
		p = p->next;
	}
	afisareRestaurant(p->info); //afisam si ultimul restaurant. Altfel nu se afiseaza din cauza conditiei din while(la ultimul element p->next e cap si atunci nu se respecta conditia)
}

//stergere
void stergereLista(Nod * cap) {
	Nod* p = cap;
	while (p && p->next != cap) { //doar conditia e diferita fata de lista simpla
		free(p->info.adresaRestaurant); //dezalocam ce e alocat dinamic
		Nod* aux = p;//se salveaza adresa nodului curent in p. 
		p = p->next; //trecem la urmatorul nod
		free(aux);
	}
	free(p);
}

// Functia de filtrare returneaza o noua lista - aici m-am folosit de functia inserareSfarsit, cu care am creat lista filtrata tot circulara
// El cere lista simplu inlantuita, deci o lista simplu inlantuita circulara se incadreaza
// Alternativa ar fi sa faci functii de inserare, afisare samd pt o lista simplu inlantuita normala si sa le folosesti aici, dar e ft mult de scris in plus
Nod* filtrare(Nod* cap, float gradOcupareAcceptat) {
	Nod* listaFiltrata = NULL; // trebuie sa initializam si aici o noua lista ca s-o returnam
	Nod* p = cap;
	while (p && p->next != cap) { // aceeasi conditie ca la afisare
		if ((p->info.nrLocuri / p->info.locuriRezervate) > gradOcupareAcceptat) { // gradul de ocupare reprezinta procentul de locuri rezervate fata de cele ale restaurantului
																				// deci verificam pentru fiecare nod daca gradul de ocupare real (p->info.nrLocuri / p->info.locuriRezervate) este mai mare decat gradul de ocupare transmis ca parametru
			//aici nu puteam sa scriem pur si simplu conditia p->info.locuriRezervate > gradOcupareAcceptat? unde scrie ca trebuie procentual sau dc trebuie neaparat sa facem impartire? nu reiese de nicaieri care a fost rezultatul impartirii si de ce anumite restaurante au trecut testul... -- scrie grad si asta inseamna procent. e ok
			listaFiltrata = inserareSfarsit(listaFiltrata, p->info); // daca este mai mare, folosim functia de inserareSfarsit si adaugam nodul in noua lista
		}                                                           //nu trebuia copiereRestaurant(p->info)? cum faceam la fct care returneaza vector - Nu, asa a facut si Zamfiroiu pe github in 3 cazuri(
		p = p->next;
	}
	// verificam si ultimul restaurant
	if ((p->info.nrLocuri / p->info.locuriRezervate) > gradOcupareAcceptat) {
		listaFiltrata = inserareSfarsit(listaFiltrata, p->info);
	}
	// returnam lista filtrata
	return listaFiltrata;
}

int numarLocuriRezervate(Nod* cap) {
	int nr = 0; // initializam un int unde vom calcula numarul de locuri total de locuri rezervate
	Nod* p = cap;
	while (p && p->next != cap) { // aceeasi conditie ca la afisare
		nr += p->info.locuriRezervate; // pentru fiecare nod adunam (prin acumulare cu operatorul +=) numarul de locuri rezervate din nod
		p = p->next;
	}
	nr += p->info.locuriRezervate; // adunam si numarul de locuri rezervate din ultimul nod
	return nr; // returnam nr total de locuri rezervate
}


void main() {
	Nod* lista = NULL;

	lista = inserareInceput(lista, creareRestaurant(1, "Mircea Voda", 100, 20));
	lista = inserareInceput(lista, creareRestaurant(2, "Vintila Voda", 70, 10)); // este bine sa testezi de 2 ori inserareInceput, ca sa stii ca merge si cu noduri deja in lista si fara
	lista = inserareSfarsit(lista, creareRestaurant(3, "Lipscani", 120, 15)); // si de 2 ori inserareSfarsit fara inserare inceput (sa comentezi randurile de mai sus de exemplu)
	lista = inserareSfarsit(lista, creareRestaurant(4, "Calea Victoriei", 70, 10));
	lista = inserareSfarsit(lista, creareRestaurant(5, "Bd Unirii", 50, 12));

	afisareLista(lista);


	Nod* listaF = NULL; // se initializeaza o noua lista
	listaF = filtrare(lista, 5); // se filtreaza lista cu un grad de ocupare de 5%
	printf("\nLista filtrata:\n");
	afisareLista(listaF); // se afiseaza lista filtrata


	printf("Numarul total de locuri rezervate este: %d", numarLocuriRezervate(lista));
	

	stergereLista(lista);
	lista = NULL;//zamfiroiu a facut stergerea de tip Nod* si aici in main nu a mai scris lista=NULL. de ce? -- nu inteleg ce rost avea lista = NULL asta de fapt oricum
				// faptul ca a facut stergerea de tip Nod* nu-l inteleg si nu stiu la ce ar ajuta, sa returnezi ceva cand stergi lista

}