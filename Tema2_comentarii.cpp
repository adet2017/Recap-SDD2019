#include <iostream>

struct Persoana {
	int id;
	char* nume;
	int varsta;
	int inaltime;
};

struct Nod {
	Persoana info;
	Nod* st;
	Nod* dr;
};

struct NodLista {
	Persoana info;
	NodLista* next;
};


Persoana crearePersoana(int id, const char* nume, int varsta, int inaltime) {
	Persoana p;
	p.id = id;
	p.nume = (char*)malloc(sizeof(char)*(strlen(nume) + 1));
	strcpy(p.nume, nume);
	p.varsta = varsta;
	p.inaltime = inaltime;
	return p;
}

Persoana citirePersoanaDinFisier(FILE* f) {
	Persoana p;
	char buffer[30];
	fscanf(f, "%d", &p.id); //&p.id - e adresa variabilei in care citim

	//citire nume:
	fscanf(f, "%s", buffer); //citim buffer si astfel stim cat spatiu tre sa alocam pentru numele pers
	p.nume = (char*)malloc(sizeof(char)*(strlen(buffer) + 1)); //aici alocam spatiu pentru pers citita
	strcpy(p.nume, buffer); //copiem din buffer in nume

	fscanf(f, "%d", &p.varsta);
	fscanf(f, "%d", &p.inaltime);
	return p;
}

Persoana copierePersoana(Persoana p) {
	Persoana copie;
	copie.id = p.id;
	copie.nume = (char*)malloc(sizeof(char)*(strlen(p.nume) + 1));
	strcpy(copie.nume, p.nume);
	copie.varsta = p.varsta;
	copie.inaltime = p.inaltime;
	return p;
}

Nod* creareNod(Persoana p, Nod* st, Nod* dr) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = copierePersoana(p);
	nou->st = st;
	nou->dr = dr;
	return nou;
}


NodLista* inserareInceput(NodLista* cap, Persoana p) {
	NodLista* nou = (NodLista*)malloc(sizeof(NodLista));
	nou->info = copierePersoana(p);
	nou->next = cap;
	return nou;
}


void afisarePersoana(Persoana p) {
	printf("Persoana cu id-ul %d, numele %s are varsta de %d ani si inaltimea %d\n", p.id, p.nume, p.varsta, p.inaltime);
}


void afisareArbore(Nod* rad) {
	if (rad) {
		afisareArbore(rad->st); //aici apelam afisareArbore fiindca in stanga se afla chiar un alt arbore care trebuie parcurs(si pt care la fel afisam fiul din stanga-1, radacina - pers cu id-ul 2 si dr-3)
		afisarePersoana(rad->info);//daca punem radacina aici, persoanele se vor afisa in ordine in fct de id, daca punem rad la inceput (preordine) rad se va afisa la sfarsit. la fel pt postordine - la sf. 
		afisareArbore(rad->dr);
	}
}

void afisareLista(NodLista* cap) {
	NodLista* p = cap;
	while (p) {
		afisarePersoana(p->info);
		p = p->next;
	}
}


int max(int a, int b) {
	return a > b ? a : b;
}

int getInaltime(Nod* rad) {
	if (rad) {
		int s = getInaltime(rad->st);
		int d = getInaltime(rad->dr);
		return max(s, d) + 1;
	}
	else {
		return 0;
	}
}


int factorEchilibru(Nod* rad) {
	return getInaltime(rad->dr) - getInaltime(rad->st);
}


Nod* rotireStanga(Nod* rad) {
	Nod* p = rad->dr;
	rad->dr = p->st;
	p->st = rad;
	return p;
}

Nod* rotireDreapta(Nod* rad) {
	Nod* p = rad->st;
	rad->st = p->dr;
	p->dr = rad;
	return p;
}

//inserare in arbore in functie de id
Nod* inserareAVL(Nod* rad, Persoana p) {
	if (rad) {
		if (p.id < rad->info.id) {
			rad->st = inserareAVL(rad->st, p);
		}
		else {
			rad->dr = inserareAVL(rad->dr, p);
		}
		int FE = factorEchilibru(rad);
		if (FE == 2) {
			if (factorEchilibru(rad->dr) == 1) {
				rad = rotireStanga(rad);
			}
			else {
				rad->dr = rotireDreapta(rad->dr);
				rad = rotireStanga(rad);
			}
		} 
		if (FE == -2) {
			if (factorEchilibru(rad->st) == -1) {
				rad = rotireDreapta(rad);
			}
			else {
				rad->st = rotireStanga(rad->st);
				rad = rotireDreapta(rad);
			}
		}
		return rad;
	}
	else { 
		rad = creareNod(p, NULL, NULL); 
	} 
}


//la fel ca copiereInLista de la 1044 s11
void copiereInLista(Nod* rad, NodLista** cap) {
	if (rad) {
		copiereInLista(rad->st, cap);
		*cap = inserareInceput(*cap, rad->info);
		copiereInLista(rad->dr, cap);
	}
}

//la fel ca si copiereDroneImpare la Drona - 1047 s10, sau copiereVacanteInLista la 1044 s10
//NodLista* listaPersoane(Nod* rad) {
//	NodLista*cap = NULL;
//	if (rad) {
//		copiereInLista(rad, &cap);
//
//	}
//	return cap;
//}

Nod* stergereArbore(Nod* rad) {
	if (rad) {
		stergereArbore(rad->st);
		stergereArbore(rad->dr);
		free(rad->info.nume);
		free(rad);
		return NULL;
	}
}

void stergereLista(NodLista* cap) {
	while (cap) {
		free(cap->info.nume);
		NodLista* p = cap;
		cap = p->next;
		free(p);
	}
}







void main() {
	Nod* rad = NULL; 

	//aici doar testam functia de afisare, fara inserare in arbore:
	//mai intai scriem in main - deschidere fisier si citirea din fisier
	//FILE* f = fopen("ID_Daramus_Alexa_tema2.txt", "r"); //primul parametru este numele fisierului iar al doilea modul - r sau w de la read sau write
	//int nrPersoane = 0; //declaram o variabila cu ajutorul careia vom citi nr de persoane (de pe prima linie)
	//if (f) { //verificam daca fisierul a fost deschis. Daca nu a fost deschis nu avem de unde sa citim
	//	fscanf(f, "%d", &nrPersoane); //primul parametru e f- ii spunem de unde sa citeasca, %d- formatul in care sa citeasca(int) si adresa variabilei in care citim - nrAvioane
	//	printf("Numarul persoanelor este: %d\n", nrPersoane); //afisam nr de persoane de pe prima linie
	//	for (int i = 0; i < nrPersoane; i++) { //for ca sa citim mai multe persoane (cate nrPersoane sunt)
	//		Persoana p = citirePersoanaDinFisier(f); //aici apelam functia de citire din fisier de mai sus
	//		afisarePersoana(p); //afisam pe ecran pe rand persoanele citite
	//	}
	//}
	//fclose(f); //inchidem fisierul

	FILE* f = fopen("ID_Daramus_Alexa_tema2.txt", "r");
	if (f) {
		int nrPersoane = 0;
		fscanf(f, "%d", &nrPersoane);
		printf("Numarul persoanelor este: %d\n", nrPersoane);
		for (int i = 0; i < nrPersoane; i++) {
			rad = inserareAVL(rad, citirePersoanaDinFisier(f)); //in loc de crearePersoana folosim citirea din fisier
		}
	}
	fclose(f);

	afisareArbore(rad); 

	//testare copiereInLista ca la Vacanta - seminar 11 gr. 1044 (fara functia a 2-a - listaPersoane):
	printf("Afisare lista simplu inlantuita:\n");
	NodLista* cap = NULL;
	copiereInLista(rad, &cap);
	while (cap) {
		afisarePersoana(cap->info);
		NodLista* temp = cap;
		cap = cap->next;
		free(temp);
	}


	//testare copiere in lista ca la Drona - seminar 10 gr. 1047
	//folosim functia listaPersoane (vezi la Drona cum se apeleaza):
	//NodLista* cap = listaPersoane(rad);
	//while (cap) {
	//	afisarePersoana(cap->info);
	//	cap = cap->next;
	//}

	
	stergereLista(cap);
	cap = NULL;
	rad = stergereArbore(rad);



}