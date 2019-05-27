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
	fscanf(f, "%d", &p.id);
	fscanf(f, "%s", buffer);
	p.nume = (char*)malloc(sizeof(char)*(strlen(buffer) + 1));
	strcpy(p.nume, buffer);
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

void afisarePersoana(Persoana p) {
	printf("Persoana cu id-ul %d, numele %s, are varsta de %d ani si inaltimea %d.\n", p.id, p.nume, p.varsta, p.inaltime);
}

void afisareLista(NodLista* cap) {
	NodLista* p = cap;
	while (p) {
		afisarePersoana(p->info);
		p = p->next;
	}
}

void afisareArbore(Nod* rad) {
	if (rad) {
		afisareArbore(rad->st); 
		afisarePersoana(rad->info);
		afisareArbore(rad->dr);
	}
}

NodLista* inserareInceput(NodLista* cap, Persoana p) {
	NodLista* nou = (NodLista*)malloc(sizeof(NodLista));
	nou->info = copierePersoana(p);
	nou->next = cap;
	return nou;
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

void copiereInLista(Nod* rad, NodLista** cap) {
	if (rad) {
		copiereInLista(rad->st, cap);
		*cap = inserareInceput(*cap, rad->info);
		copiereInLista(rad->dr, cap);
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

Nod* stergereArbore(Nod* rad) {
	if (rad) {
		stergereArbore(rad->st);
		stergereArbore(rad->dr);
		free(rad->info.nume);
		free(rad);
		return NULL;
	}
}




void main() {
	Nod* rad = NULL; 
	FILE* f = fopen("ID_Daramus_Alexa_tema2.txt", "r");
	if (f) {
		int nrPersoane = 0;
		fscanf(f, "%d", &nrPersoane);
		printf("\nNumarul persoanelor este: %d\n", nrPersoane);
		for (int i = 0; i < nrPersoane; i++) {
			rad = inserareAVL(rad, citirePersoanaDinFisier(f));
		}
	}
	fclose(f);

	afisareArbore(rad); 

	printf("\nAfisare lista simplu inlantuita:\n");
	NodLista* cap = NULL;
	copiereInLista(rad, &cap);
	while (cap) {
		afisarePersoana(cap->info);
		NodLista* temp = cap;
		cap = cap->next;
		free(temp);
	}
	
	stergereLista(cap);
	cap = NULL;
	rad = stergereArbore(rad);
}