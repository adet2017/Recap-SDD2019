#include <iostream>


struct CotaIntretinere {
	char* adresa;
	int nrApartament;
	int nrLocuitori;
	int an;
	int luna;
	float valIntretinere;
};


struct Nod {
	CotaIntretinere info;
	Nod* next;
};

struct Hashtable {
	Nod** vector;
	int dim;
};


CotaIntretinere creareCotaIntretinere(const char* adresa, int nrApartament, int nrLocuitori, int an, int luna, float valIntretinere) {
	CotaIntretinere ci;
	ci.adresa = (char*)malloc(sizeof(char)*(strlen(adresa) + 1));
	strcpy(ci.adresa, adresa);
	ci.nrApartament = nrApartament;
	ci.nrLocuitori = nrLocuitori;
	ci.an = an;
	ci.luna = luna;
	ci.valIntretinere = valIntretinere;
	return ci;
}


//nu mai facem fct de copiere



//pct 1 - inserare in tabela
//inserare la inceput lista simpla
Nod* inserareInceput(Nod* cap, CotaIntretinere ci) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = ci; //facem shallow copy
	nou->next = cap;
	return nou;
}


//fct folosita in main pt initializarea tabelei
Hashtable initializareHashtable(int dim) {
	Hashtable h;
	h.dim = dim;
	h.vector = (Nod**)malloc(sizeof(Nod*)*dim); //ca la vectorul de char** doar ca cu Nod**
	for (int i = 0; i < dim; i++) {
		h.vector[i] = NULL;
	}
	return h;
}

//fct ajutatoare pt inserare in tabela - returneaza int!! si inserarea la fel
int hashCode(Hashtable h, int nrApartament) {
	return nrApartament % h.dim; //calculam codul ASCII, in fct de cheia de cautare
}

int inserareTabela(Hashtable h, CotaIntretinere ci) {
	if (h.vector) {
		int pozitie = hashCode(h, ci.nrApartament);
		if (h.vector[pozitie]) { //in loc de cap
			h.vector[pozitie] = inserareInceput(h.vector[pozitie], ci);
		}
		else { //else-acelasi lucru
			h.vector[pozitie] = inserareInceput(h.vector[pozitie], ci);
		}
		return pozitie; //trebuie sa returnam pozitia care e un int (tipul returnat al functiei e un int)
	}
	else {
		return -1;
	}
}

//pct 2 - afisare tabela
void afisareCotaIntretinere(CotaIntretinere ci) {
	printf("La adresa %s, nr apartament %d, nr locuitori/apartament %d, in anul %d, luna %d, valoarea intretinerii este de %.2f\n", ci.adresa, ci.nrApartament, ci.nrLocuitori, ci.an, ci.luna, ci.valIntretinere);
}


void afisareTabela(Hashtable h) {
	for (int i = 0; i < h.dim; i++) { //doar for-ul asta in plus fata de afisarea la lista simpla
		Nod* p = h.vector[i]; // in loc de cap avem h.vector[i]
		while (p) {
			afisareCotaIntretinere(p->info);
			p = p->next;
		}
		
	}
}

//pct 3 - cu afisare din functie
float valTotala(Hashtable h, int nrApartament, int an, const char* adresa) {
	float suma = 0; //int suma trebuie sa fie in afara for!!!
	for (int i = 0; i < h.dim; i++) {
		Nod* p = h.vector[i];
		while (p) {
			if (p->info.nrApartament == nrApartament) {
				suma += p->info.valIntretinere;
			}
			p = p->next;
		}
	}
	//parametrii practic ii primeste aici, si in loc de apelul fct valTotala primeste direct suma:
	printf("\nVal totala intretinere pt apartamentul cu nr %d de la adresa %s pentru anul %d este %.2f\n", nrApartament, adresa, an, suma); 
	return suma;
}

//float valTotala(Hashtable h, int nrApartament, int an, const char* adresa) {
//	float suma = 0; //int suma trebuie sa fie in afara for!!!
//	for (int i = 0; i < h.dim; i++) {
//		Nod* p = h.vector[i];
//		while (p) {
//			if (p->info.nrApartament == nrApartament && p->info.an == an && strcmp(p->info.adresa, adresa)==0) {
//				suma += p->info.valIntretinere;
//			}
//			p = p->next;
//		}
//	}
//	return suma;
//}

//pct 4
int nrCote(Hashtable h, float val) {
	int contor = 0; //contorul trebuie sa fie in afara for!!
	for (int i = 0; i < h.dim; i++) {
		Nod* p = h.vector[i];
		while (p) {
			if (p->info.valIntretinere > val) {
				contor++;
			}
			p = p->next;
		}
	}
	return contor;
}


//pct 5
void stergereDupaAdresa(Hashtable* h, const char* adresa) {

}

void main() {
	Hashtable tabela = initializareHashtable(5); //5 este dimensiunea tabelei

	inserareTabela(tabela, creareCotaIntretinere("Mircea Voda", 33, 4, 2018, 06, 400.55));
	inserareTabela(tabela, creareCotaIntretinere("Mircea Voda", 33, 4, 2018, 06, 600));
	inserareTabela(tabela, creareCotaIntretinere("Bd Libertatii, nr 14", 55, 4, 2018, 06, 400.55));
	inserareTabela(tabela, creareCotaIntretinere("Bd Unirii, nr 45", 45, 4, 2018, 06, 700.50));

	afisareTabela(tabela);

	printf("\nNumarul de cote intretinere a caror valoare depaseste 500 este %d\n", nrCote(tabela, 500));

	/*printf("\nVal totala intretinere pt apartamentul cu nr 33 este %.2f\n", valTotala(tabela, 33, 2018, "Mircea Voda"));*/
	valTotala(tabela, 33, 2018, "Mircea Voda");

}