#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<iostream>

struct Echipa {
	int codEchipa;
	char* capitan;
	int domeniu;
	int nrStudenti;
	float puncte;
};

Echipa creareEchipa(int cod, const char* capitan, int domeniu, int nrstudenti, float puncte) {
	Echipa e;
	e.codEchipa = cod;
	e.capitan = (char*)malloc(sizeof(char)*(strlen(capitan) + 1));
	strcpy(e.capitan, capitan);
	e.domeniu = domeniu;
	e.nrStudenti = nrstudenti;
	e.puncte = puncte;
	return e;
}


Echipa copiereEchipa(Echipa e) {
	Echipa copie;
	copie.codEchipa = e.codEchipa;
	copie.capitan = (char*)malloc(sizeof(char)*(strlen(e.capitan) + 1));
	strcpy(copie.capitan, e.capitan);
	copie.domeniu = e.domeniu;
	copie.nrStudenti = e.nrStudenti;
	copie.puncte = e.puncte;
	return copie;
}

void afisareEchipa(Echipa e) {
	printf("Cod echipa: %d, capitan: %s, domeniu %d, numar studenti %d, puncte %5.2f\n", e.codEchipa, e.capitan, e.domeniu, e.nrStudenti, e.puncte);
}

struct nod {
	Echipa info;
	nod* next;
};


//subiectul 1 - crearea unei structuri liniare  simplu inlantuite gestionata pe principiul LIFO
nod* inserareInceput(nod* cap, Echipa e) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creareEchipa(e.codEchipa, e.capitan, e.domeniu, e.nrStudenti, e.puncte);
	nou->next = cap;
	return nou;
}

nod* pushStack(nod* stiva, Echipa e) {
	stiva = inserareInceput(stiva, e);
	return stiva;
}


Echipa popStack(nod** stiva) {
	if (*stiva) {
		Echipa e = creareEchipa((*stiva)->info.codEchipa, (*stiva)->info.capitan, (*stiva)->info.domeniu, (*stiva)->info.nrStudenti, (*stiva)->info.puncte);
		nod* aux = *stiva;
		*stiva = (*stiva)->next;
		free(aux->info.capitan);
		free(aux);
		return e;
	}
	else {
		return creareEchipa(-1, "", -1, -1, -1);
	}
}


//afisare simpla
void afisareStiva(nod* stiva) {
	if (stiva) {
		nod* p = stiva;
		while (p) {
			afisareEchipa(p->info);
			p = p->next;
		}
	}
}

//afisare cu ajutorul metodei popStack
void afisareStiva2(nod* stiva) {
	nod* p = stiva;
	while (p) {
		Echipa e = popStack(&p);
		afisareEchipa(e);
	}
}

//subiectul 2 - cauta si afiseaza echipa care are un anumit capitan
void cautareEchipaDupaCapitan(nod* stiva, const char* capitanCautat) {
	while (stiva) {
		if (strcmp(stiva->info.capitan, capitanCautat) == 0) {
			afisareEchipa(stiva->info);
		}
		stiva = stiva->next;
	}

}


//subiectul 3 - conversia structurii intr-un masiv unidimensional
Echipa* conversie(nod* stiva, int* dim) {
	int nr = 0;
	nod* temp = stiva;
	while (temp) {
		nr++;
		temp = temp->next;
	}
	Echipa* vectorEchipe = (Echipa*)malloc(sizeof(Echipa)*nr);
	for (int i = 0; i < nr; i++) {
		vectorEchipe[i] = popStack(&stiva); //pt ca subiectul cere sa extragem elementele
	}
	*dim = nr;
	return vectorEchipe;
}

//subiectul 4 - numarul de echipe care au un punctaj mai mare decat media calculata
int filtrareEchipePunctajPesteMedie(Echipa* vector, int* dim) {
	int nrEchipe = *dim;
	float suma = 0;
	for (int i = 0; i < nrEchipe; i++) {
		suma += vector[i].puncte;
	}
	float media = suma / nrEchipe;
	int nrEchipeCuPunctajPesteMedie = 0;
	for (int i = 0; i < nrEchipe; i++) {
		if (vector[i].puncte > media) {
			nrEchipeCuPunctajPesteMedie++;
		}
	}
	return nrEchipeCuPunctajPesteMedie;
}

//subiectul 5



void main() {
	nod* stiva = NULL;
	stiva = pushStack(stiva, creareEchipa(1, "CapitanEchipa1", 1, 1, 1));
	stiva = pushStack(stiva, creareEchipa(2, "CapitanEchipa2", 2, 2, 2));
	stiva = pushStack(stiva, creareEchipa(3, "CapitanEchipa3", 3, 3, 3));
	stiva = pushStack(stiva, creareEchipa(4, "CapitanEchipa4", 4, 4, 4));
	stiva = pushStack(stiva, creareEchipa(5, "CapitanEchipa5", 5, 5, 5));
	//afisareStiva(stiva);
	afisareStiva2(stiva);

	printf("\nCautare echipa dupa capitan:\n");
	cautareEchipaDupaCapitan(stiva, "CapitanEchipa1");

	printf("\nAfisare masiv unidimensional:\n");
	int nrEchipe = 0;
	Echipa* vectorEchipe = conversie(stiva, &nrEchipe);
	for (int i = 0; i < nrEchipe; i++) {
		afisareEchipa(vectorEchipe[i]);
	}

	int nrEchipeCuPunctajPesteMedie = filtrareEchipePunctajPesteMedie(vectorEchipe, &nrEchipe);
	printf("\nNumar echipe cu punctaj peste medie: %d\n", nrEchipeCuPunctajPesteMedie);



}
