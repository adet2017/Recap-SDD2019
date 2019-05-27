#include <iostream>


struct Farmacie{
	unsigned int id;
	int nrMedicamente;
	float pretMedicament;
	char* numeFarmacie;
};

struct Nod{
	Farmacie info;
	Nod* prev;
	Nod* next;
};

struct ListaDubla {
	Nod* first;
	Nod* last;
};

Farmacie creareFarmacie(unsigned int id, int nrMedicamente, float pretMedicament, const char* numeFarmacie) {
	Farmacie f;
	f.id = id;
	f.nrMedicamente = nrMedicamente;
	f.pretMedicament = pretMedicament;
	f.numeFarmacie = (char*)malloc(sizeof(char)*(strlen(numeFarmacie)+1));
	strcpy(f.numeFarmacie, numeFarmacie);
	return f;
}

Farmacie copiereFarmacie(Farmacie f) {
	Farmacie copie;
	copie.id = f.id;
	copie.nrMedicamente = f.nrMedicamente;
	copie.pretMedicament = f.pretMedicament;
	copie.numeFarmacie= (char*)malloc(sizeof(char)*(strlen(f.numeFarmacie)+1));
	strcpy(copie.numeFarmacie, f.numeFarmacie);
	return copie;
}

//functie pe care o foloseste pe github la inserareSortata -  acelasi lucru cu ce am scris eu direct la inserare. Aici e nou->next = next si nou->prev =prev dar primeste ca parametri tot p si p->prev etc. 
//Nod* creareNod(Farmacie f, Nod* next, Nod* prev) {
//	Nod* nou = (Nod*)malloc(sizeof(Nod));
//	nou->info = creareFarmacie(f.id, f.nrMedicamente, f.pretMedicament, f.numeFarmacie);
//	nou->next = next; 
//	nou->prev = prev;
//	return nou;
//}

//inserare la final
ListaDubla inserareSfarsit(ListaDubla ld, Farmacie f) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = copiereFarmacie(f);
	nou->next = nou->prev = NULL;
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

//pct 2 - inserare cu cheie de cautare - id-ul ?
ListaDubla inserareSortata(ListaDubla ld, Farmacie f) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = copiereFarmacie(f);
	//aici de ce nu e nevoie de nou->next = nou->prev = NULL? -- trebuie dar o scriem la sf pe ultimul else. Sau o contine deja inserarea la sfarsit si atunci n-o mai scriem?. E acelasi lucru
	if (ld.first) { //la inserare pe pozitie eu am facut fara acest if, desi pe github e la fel cu if (ld.first) (vezi Avion)
		Nod* p = ld.first;
		while (p && p->info.id < f.id) { //cat timp exista lista si id-ul fiecarui nod pe rand este mai mic decat id-ul noului nod 
			p = p->next;//se trece la urm nod pana se ajunge la cel care e mai mare (sau egal?) si introducem inaintea lui (de ex daca id-ul nou e 3 ...parcurgem pana ajungem la 4 si acolo ne oprim si introducem inaintea lui 4?)
		} //in while trebuie sa se indeplineasca ambele conditii in acelasi timp, sa existe p si id-ul sa fie m mic
		if (p) { //daca exista lista (astea sunt doar niste vericari, dar dc e necesar sa verificam de atatea ori daca exista lista?
				 //de ce nu intra in if daca id-ul e m mare? pt ca conditia asta era doar in while. 
			/*Nod* nou = creareNod(f, p, p->prev);*/
			nou->next = p; //noul nod trebuie sa pointeze inainte catre p (acum p nu mai e ld.first, a ramas unde s-a oprit in while - la 4)
			nou->prev = p->prev;  //noul nod trebuie sa pointeze si inapoi catre nodul spre care pointa inainte p
			p->prev = nou; //nodul p pointeaza inainte catre noul nod - nu pointeaza inapoi? - de vazut pe desen
			if (nou->prev) { //daca exista un nod inainte de nou 
						   //pai cum adica daca exista? l-am pus pe nou->prev sa pointeze catre p->prev
				           //pe github la Aeroport - seminar04 gr1044 nu a mai pus un if aici, a scris direct p->prev->next = nou; 
				nou->prev->next = nou; //nodul dinaintea lui nou va pointa inainte catre nou 
			}
			else { //merge si fara acest else, asa a facut la seminar la ID in 2018
				ld.first = nou; //daca nu exista, noul nod va fi primul nod
			}
		}
		else { //in cazul in care lista nu exista sau id-ul nou este mai mare decat toate id-urile din lista, intra pe else (sa zicem daca introducem id-ul 3 iar in lista avem doar 1 si 2, ambele din lista sunt mai mici asa ca inseram dupa ele. 
			//-- de ce intra pe else daca id-ul e mai mare? ca mai sus avem doar if(p)
			//sau putem sa folosim inserareaSfarsit (cum am facut la inserare pe pozitie) si atunci nu mai trebuie sa scriem nimic din ce e mai jos pt ca e acelasi lucru. 
			ld = inserareSfarsit(ld, f); //folosim functia de inserare la final
			/*Nod* nou = creareNod(f, NULL, ld.last);*/
			/*nou->next = NULL; 
			nou->prev = ld.last;
			ld.last->next = nou;
			ld.last = nou;*/ //noul nod e ultimul din lista
		}
	}
	//else {//daca nu avem noduri in lista - si partea asta la fel, e inclusa in inserarea la sfarsit deci nu mai e nevoie de ea
	//	/*Nod* nou = creareNod(f, NULL, NULL);*/
	//	nou->next = nou->prev = NULL;
	//	ld.first = ld.last = nou;
	//}
	return ld;
}

//pct 3
void afisareFarmacie(Farmacie f) {
	printf("Farmacia %s cu id-ul %d, are un numar de %d medicamente, pretul unui medicament fiind %.2f.\n", f.numeFarmacie, f.id, f.nrMedicamente, f.pretMedicament);
}



void afisareListaCrescator(ListaDubla ld) {
	Nod* p = ld.first;
	while (p) {
		afisareFarmacie(p->info);
		p = p->next;
	}
}

void afisareListaDescrescator(ListaDubla ld) {
	Nod* aux = ld.last;
	while (aux) {
		afisareFarmacie(aux->info);
		aux = aux->prev;
	}
}

//pct 4
float pretCumulat(ListaDubla ld) {
	Nod* p = ld.first;
	float suma = 0;
	while (p) {
		suma += p->info.pretMedicament;
		p = p->next;
	}
	return suma;
}


//pct 5 - rezultatul este returnat prin numele functiei? (vezi cerinta)
int nrFarmacii(int nr, ListaDubla ld) {
	Nod* p = ld.first;
	int contor = 0;
	while (p) {
		if (p->info.nrMedicamente > nr) {
			contor++;
		}
		p = p->next;
	}
	return contor;
}

//stergere
void stergereLista(ListaDubla ld) {
	while (ld.first) {
		free(ld.first->info.numeFarmacie);
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

	//pct 2
	//ld = inserareSfarsit(ld, creareFarmacie(1, 150, 30.55, "Sensiblu"));
	//ld = inserareSfarsit(ld, creareFarmacie(2, 200, 40.2, "HelpNet"));
	//ld = inserareSfarsit(ld, creareFarmacie(3, 100, 20, "Doina"));
	//ld = inserareSfarsit(ld, creareFarmacie(4, 300, 10.2, "Catena"));
	//ld = inserareSfarsit(ld, creareFarmacie(5, 170, 150, "Tei"));
	ld = inserareSortata(ld, creareFarmacie(7, 170, 50.1, "2na")); //daca comentam randurile cu inserare la sfarsit si facem doar inserare sortata nu merge (nu se insereaza)
	ld = inserareSortata(ld, creareFarmacie(6, 170, 50.1, "2na")); //desi ar fi trebuit sa intre pe else si sa insereze la sfarsit!?
	ld = inserareSortata(ld, creareFarmacie(8, 170, 50.1, "2na"));

	//pct 3
	printf("\nAfisarea listei crescator:\n ");
	afisareListaCrescator(ld);

	printf("\nAfisarea listei descrescator:\n ");
	afisareListaDescrescator(ld);

	//pct 4
	printf("Pretul cumulat al medicamentelor este de %.2f.\n", pretCumulat(ld));

	//pct 5
	printf("Numarul de farmacii care au stocul de medicamente mai mare decat 120 este %d.\n", nrFarmacii(120, ld));

	stergereLista(ld);

}
