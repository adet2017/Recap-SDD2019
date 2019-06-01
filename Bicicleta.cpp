#include <iostream>


//pct 1 - cele 2 struct
struct Bicicleta {
	int id;
	float durataUtilizare;
	int nrStatie;
	int nrUtilizari;
	char* model;
};

struct Nod {
	Nod* st;
	Nod* dr;
	Bicicleta info;
};


//pct 2 - inserare
//fct ajutatoare pt apelarea fct de inserare din main
Bicicleta creareBicicleta(int id, float durataUtilizare, int nrStatie, int nrUtilizari, const char* model) {
	Bicicleta b;
	b.id = id;
	b.durataUtilizare = durataUtilizare;
	b.nrStatie = nrStatie;
	b.nrUtilizari = nrUtilizari;
	b.model = (char*)malloc(sizeof(char) * (strlen(model) + 1));
	strcpy(b.model, model);
	return b;
}

//fct ajutatoare pt fct creareNod care la randul ei e ajutatoare pt inserare
Bicicleta copiereBicicleta(Bicicleta b) {
	Bicicleta copie;
	copie.id = b.id;
	copie.durataUtilizare = b.durataUtilizare;
	copie.nrStatie = b.nrStatie;
	copie.nrUtilizari = b.nrUtilizari;
	copie.model = (char*)malloc(sizeof(char) * (strlen(b.model) + 1));
	strcpy(copie.model, b.model);
	return copie;
}

//fct ajutatoare pt inserare
Nod* creareNod(Bicicleta b, Nod* st, Nod* dr) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = copiereBicicleta(b);
	nou->st = st;
	nou->dr = dr;
	return nou;
}


//fct inserare
Nod* inserareNod(Nod* rad, Bicicleta b) { //fct recursiva, se autoapeleaza pt a pargurge in profunzime
	if (rad) {
		if (b.id < rad->info.id) { //daca id-ul este m mic decat id-ul din radacina
			rad->st = inserareNod(rad->st, b); //inseram bicicleta in stanga radacinii -- inserareNod returneaza Nod*, deci nu mergea asa simplu doar apelat
		}
		else { //daca id-ul este m mare
			rad->dr = inserareNod(rad->dr, b);//inseram bicicleta in dreapta radacinii -- la fel
		}
	}
	else {//daca nu exista rad inseamna ca nu avem niciun nod in arbore
		rad = creareNod(b, NULL, NULL); //inseram un nod care va fi singurul din arbore si practic acesta va fi radacina
	}                                   //NULL pt st si dr fiindca nu are nimic in dreapta si in stanga (e singurul nod)
	return rad;
}


//pct 3 - afisare
void afisareBicicleta(Bicicleta b) {
	printf("Bicicleta cu id-ul %d a fost utilizata astazi %.2f minute si a fost predata in statia de parcare nr. %d, avand numarul de utilizari/zi de %d, modelul bicicletei fiind %s \n", b.id, b.durataUtilizare, b.nrStatie, b.nrUtilizari, b.model);
}

void afisareArbore(Nod* rad) { //aici de ce nu e nevoie de un auxiliar pt a parcurge arborele fara a-l altera? -- nu e cazul, e prea simpla functia ca s-o alterezi
	if (rad) {
		afisareArbore(rad->st); //fct recursiva, se autoapeleaza pt a pargurge in profunzime. afisam tot ce e in stanga radacinii
		afisareBicicleta(rad->info);
		afisareArbore(rad->dr); //conteaza in ce ordine sunt aici? -- da
	}
}

//de la seminar:
//functie ajutatoare pt getInaltime
int max(int a, int b) {
	return (a < b)? b : a; //expresie ? true : false
	//operatorul ternar(?): daca expresia este adevarata, se returneaza ce e dupa semnul intrebarii, iar daca e falsa se returneaza ce e dupa :
}


//fct pt determinarea inaltimii arborelui(nr de noduri pe verticala) - returneaza inaltimea
int getInaltime(Nod* rad) {
	if (rad) { //daca avem noduri in arbore
		//in loc de astea 3 randuri se mai poate scrie ca la Vacanta gr 1044 s10:
		//return 1+ max(getInaltime(rad->st), getInaltime(rad->dr);
		int s = getInaltime(rad->st);
		int d = getInaltime(rad->dr);
		return max(s, d) + 1; //+ 1 pt ca nu se numara radacina, si trebuie adaugata. aici compara stanga cu dreapta?
	}                         //cum adica nu se numara? nu avem functie de numarare, functia int max doar compara nu?
	else { //daca nu exista radacina deci nu avem noduri in arbore
		return 0; //returnam inaltimea 0;
	}
}

//fct afisareNivel
//nivel max este = cu inaltimea? - da
void afisareNivel(Nod* rad, int nivelDorit, int nivelCurent) { 
	if (rad) {
		if (nivelDorit == nivelCurent) {
			afisareBicicleta(rad->info);
		}
		else {
			afisareNivel(rad->st, nivelDorit, nivelCurent + 1); //ce se int aici? -- ca si la afisare, se foloseste recursivitatea, daca nu se identifica nivelul dorit, se cauta in stanga
			afisareNivel(rad->dr, nivelDorit, nivelCurent + 1); // si in dreapta nodului curent
		}
	}
}

//cautare maxim - de la Vacanta gr 1044 s10
Nod* cautareMaxim(Nod* rad) {
	if (rad) {
		if (rad->dr) {
			return cautareMaxim(rad->dr);
		}
		else {
			return rad;
		}
	}
	else {
		return NULL;
	}
}

Nod* stergereArbore(Nod* rad) {
	if (rad) {
		stergereArbore(rad->st);
		stergereArbore(rad->dr);
		free(rad->info.model); //nu trebuia sa stergem informatiile astea inainte sa stergem arborele? -- se sterg, dar pare ca nu din cauza recursivitatii
		free(rad);
		return NULL; //pointerul la nod este acum NULL fiindca s-a sters arborele
	}
}


//extra (pe langa subiect):


//fct de cautare Bicicleta dupa nr statiei (de la Angajat):
Bicicleta cautareBicicleta(Nod* rad, int nrCautat) { //fct recursiva
	if (rad) {
		if (rad->info.nrStatie == nrCautat) { //daca nr statiei este egal cu nr cautat
			return rad->info; //returnam info din nod (bicicleta)
		}
		else if (nrCautat < rad->info.nrStatie) { //daca nr cautat este m mic decat nr statiei
			return cautareBicicleta(rad->st, nrCautat); //cautam in stanga radacinii - aici lipsea return si de asta afisa doar radacina!
		}
		else { //daca este m mare
			return cautareBicicleta(rad->dr, nrCautat); //cautam in dreapta radacinii
		}
	}
}

//fct de cautare Bicicleta dupa nr statiei - de la Vacanta 1044 s10
Bicicleta cautareBicicletaDupaStatie(Nod* rad, int nrCautat) {
	if (rad) {
		if (rad->info.nrStatie > nrCautat) {
			return cautareBicicletaDupaStatie(rad->st, nrCautat);
		}
		else if (rad->info.nrStatie < nrCautat) {
			return cautareBicicletaDupaStatie(rad->dr, nrCautat);
		}
		else {
			Bicicleta rezultat = creareBicicleta(rad->info.id, rad->info.durataUtilizare, rad->info.nrStatie, rad->info.nrUtilizari, rad->info.model);
			return rezultat;
		}
	}
	else {
		Bicicleta rezultat = creareBicicleta(-1, -1, -1, -1, "");
	}

}


//fct de numarare a elementelor(de la Senzor):
int nrElemente(Nod* rad) { //numaram bicicletele
	static int contor = 0; //de ce e static?
	if (rad) {
		nrElemente(rad->st);
		nrElemente(rad->dr);
		contor++;
	}
	return contor;
}



//extragere nod dupa un criteriu - ca la Vacanta gr. 1044 s10
Bicicleta extragereBicicleta(Nod** rad, int statieCautata) { //de ce doua **?
	if (*rad) {
		if (statieCautata > (*rad)->info.nrStatie) {
			return extragereBicicleta(&(*rad)->dr, statieCautata);
		}
		else if (statieCautata < (*rad)->info.nrStatie) {
			return extragereBicicleta(&(*rad)->st, statieCautata);
		}
		else {
			if ((*rad)->st == NULL) {
				Bicicleta rezultat = creareBicicleta((*rad)->info.id, (*rad)->info.durataUtilizare, (*rad)->info.nrStatie, (*rad)->info.nrUtilizari, (*rad)->info.model);
				Nod* temp = (*rad)->dr;
				free(*rad);
				*rad = temp;
				return rezultat;
			}
			else if((*rad)->dr ==NULL) {
				Bicicleta rezultat = creareBicicleta((*rad)->info.id, (*rad)->info.durataUtilizare, (*rad)->info.nrStatie, (*rad)->info.nrUtilizari, (*rad)->info.model);
				Nod* temp = (*rad)->dr;
				free(*rad);
				*rad = temp;
				return rezultat;
			}
			else {
				Nod* temp = cautareMaxim((*rad)->st);
				Bicicleta aux = (*rad)->info;
				(*rad)->info = temp->info;
				temp->info = aux;
				return extragereBicicleta(&(*rad)->st, statieCautata);
			}
		}
	}

}


//extragere nod dupa un criteriu - ca la Drona gr. 1047 s10
Bicicleta extragereBicicletaDinArbore(Nod** root, int nrCautat) {
	if (*root) {
		if ((*root)->info.nrStatie == nrCautat) {
			Bicicleta rezultat = (*root)->info;
			if ((*root)->dr == NULL) {
				Nod* temp = (*root)->st;
				free(*root);
				*root = temp;
			}
			else if ((*root)->st == NULL) {
				Nod* temp = (*root)->dr;
				free(*root);
				*root = temp;
			}
			else {
				Nod* temp = cautareMaxim((*root)->st); //de ce folosim fct cautare maxim? si de ce doar pe stanga - pt ca fct de cautareMaxim e doar pe dreapta
				Bicicleta aux = temp->info;
				temp->info = (*root)->info;
				(*root)->info = aux;
				extragereBicicletaDinArbore(&(*root)->st, nrCautat); //de ce doar stanga?
			}

			return rezultat;
		}
		else if ((*root)->info.nrStatie > nrCautat) {
			return extragereBicicletaDinArbore(&(*root)->st, nrCautat);
		}
		else {
			return extragereBicicletaDinArbore(&(*root)->dr, nrCautat);
		}
	}
	else {
		return creareBicicleta(-1, -1, -1, -1, "");
	}

}

//stergere nod dupa un criteriu - asemanatoare cu extragerea dupa un criteriu, doar ca nodul sters nu se mai salveaza in "rezultat"
Nod* stergereNod(Nod* root, int idCautat) {
	if (root) {
		if (root->info.id == idCautat) {
			if (root->st == NULL) {
				Nod* temp = root->dr;
				free(root->info.model);
				free(root);
				return temp;
			}
			if (root->dr == NULL) {
				Nod* temp = root->st;
				free(root->info.model);
				free(root);
				return temp;
			}

			Nod* temp = cautareMaxim(root->st); //de ce folosim cautare maxim?
			Bicicleta aux = root->info;
			root->info = temp->info;
			temp->info = aux;
			root->st = stergereNod(root->st, idCautat); //de ce e stanga aici? (la fel si la extragere)
			return root;

		}
		else if (root->info.id > idCautat) {
			root->st = stergereNod(root->st, idCautat); //la fel ca la extragere doar ca in loc de return aici avem root->st =
		}
		else {
			root->dr = stergereNod(root->dr, idCautat);
		}
	}
	return root;
}




void main() {

	//pct 1 - aici nu e nevoie sa initializam un arbore
	Bicicleta b1 = creareBicicleta(3, 20.1, 5, 1, "Scirocco"); //e o idee buna sa nu dam aici id-ul 1 ca sa putem insera in stanga
	

	//pct 2
	Nod* arbore = creareNod(b1, NULL, NULL); //b1 este acum radacina -- aici se creeaza arborele
	arbore = inserareNod(arbore, creareBicicleta(2, 30, 15, 1, "Brompton"));
	arbore = inserareNod(arbore, creareBicicleta(8, 25, 12, 1, "Firstbike"));
	arbore = inserareNod(arbore, creareBicicleta(5, 40, 22, 1, "Electra"));

	//pct3
	afisareArbore(arbore);


	//testare stergere nod cu id-ul 5
	/*stergereNod(arbore, 5);

	printf("\nAfisare dupa stergere:\n");
	afisareArbore(arbore);*/

	//testare fct numarare elemente din arbore
	printf("\nNr. elemente: %d\n", nrElemente(arbore));

	//testare getInaltime
	printf("\nInaltime arbore: %d\n", getInaltime(arbore));

	//testare afisareNivel
	printf("\nAfisare nivel:\n");
	afisareNivel(arbore, 2, 1); //nivelCurent trebuie sa fie mereu 1! (adica porneste de la rad?)

	//testare extragere
	printf("\nNodul extras:\n");
	/*afisareBicicleta(extragereBicicleta(&arbore, 12));*/ //aici daca pun 15 sau 22 da eroare de exec (adica nu merge la cele din stanga?). 5 si 12 merg
	/*afisareBicicleta(extragereBicicletaDinArbore(&arbore, 12));*/ //nu merge daca pun 15 sau 22 - afiseaza pe ramura else cu -1
	                                                                //cred ca nu extrage din stanga 

	printf("\nAfisare arbore dupa extragere:\n");
	afisareArbore(arbore);


	//testare fct de cautare Bicicleta - da eroare de exec
	printf("\nTestare cautareBicicleta dupa nr statiei:\n");
	afisareBicicleta(cautareBicicleta(arbore, 15)); //nu merg 15 si 22 - nu da eroare de exec dar afiseaza toate valorile cu 0 sau null
	/*afisareBicicleta(cautareBicicletaDupaStatie(arbore, 5));*/ //la fel - nu merg 15 si 22 (eroare de exec)


	//testare fct numarare elemente din arbore
	/*printf("\nNr. elemente: %d\n", nrElemente(arbore));*/ //aici nu merge - afiseaza un nr gresit, merge doar mai sus

	//testare stergere
	arbore = stergereArbore(arbore); //"arbore =" -pt ca functia de stergere returneaza Nod*

}