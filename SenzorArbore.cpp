#include <iostream>

//Arbore binar (teorie):
//- nu se foloseste while la functii
//- in schimb se foloseste recursivitatea! (asa se pargurge)
// - nivel maxim = inaltimea arborelui!!

struct Senzor {
	int idSenzor;
	char* producator;
	int nrPini;
};


struct Nod {
	Senzor info;
	Nod* dr;
	Nod* st;
};

Senzor creareSenzor(int idSenzor, const char* producator, int nrPini) {
	Senzor s;
	s.idSenzor = idSenzor;
	s.producator = (char*)malloc(sizeof(char)*(strlen(producator) + 1));
	strcpy(s.producator, producator);
	s.nrPini = nrPini;
	return s;
}

Senzor citireSenzorDinFisier(FILE* f) {
	Senzor s;
	fscanf(f, "%d", &s.idSenzor);

	char buffer[20]; //1
	fscanf(f, "%s", &s.producator); //2
	s.producator = (char*)malloc(sizeof(char)*(strlen(buffer) + 1)); //3
	strcpy(s.producator, buffer); //4

	fscanf(f, "%d", &s.nrPini);
	return s;
}

//fct ajutatoare pt creareNod (care e ajutatoare pt inserare)
Senzor copiereSenzor(Senzor s) {
	Senzor copie;
	copie.idSenzor = s.idSenzor;
	copie.producator = (char*)malloc(sizeof(char)*(strlen(s.producator) + 1));
	strcpy(copie.producator, s.producator);
	copie.nrPini = s.nrPini;
	return copie;
}

//fct ajutatoare pt inserareInArbore
Nod* creareNod(Senzor s, Nod* st, Nod* dr) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = copiereSenzor(s); //merge si cu shallow copy dar mai bine sa fac fct de copiere
	nou->st = st; //ca la orice fct de creare!!!
	nou->dr = dr;
	return nou;
}


//inserare in arbore
Nod* inserareInArbore(Nod* rad, Senzor s) {
	if (rad) {
		if (s.idSenzor < rad->info.idSenzor) { //daca id-ul noului senzor e mai mic decat id-ul radacinii
			rad->st = inserareInArbore(rad->st, s); //inseram in stanga
		}
		else { //daca id-ul este m mare
			rad->dr = inserareInArbore(rad->dr, s); //inseram in dreapta
		}
	}
	else {
		rad = creareNod(s, NULL, NULL); //sau putem sa facem ca in tutorial Zamfiroiu - arbore binar avion
	}
	return rad;
}

//afisare senzor
void afisareSenzor(Senzor s) {
	printf("%d. Producator: %s, Nr. pini: %d.\n", s.idSenzor, s.producator, s.nrPini);
}

//afisare arbore in inordine (SRD)
//preordine - RSD
//postordine - SDR
void afisareArbore(Nod* rad) {
	if (rad) {
		afisareArbore(rad->st);
		afisareSenzor(rad->info);
		afisareArbore(rad->dr);
	}
}

//afisare senzori care au nrPini mai mare decat o val
void afisareArboreDupaNrPini(Nod* rad, int nr) {
	if (rad) {
		if (rad->info.nrPini > nr) {
			afisareArboreDupaNrPini(rad->st, nr);
			afisareSenzor(rad->info);
			afisareArboreDupaNrPini(rad->dr, nr);
		}
	}
}


//fct de numarare elemente din arbore - recursivitate!!
int nrElemente(Nod* rad) { // - asemanatoare cu afisarea (contor in plus si nu are rad->info)
	static int contor = 0; //fara STATIC nu merge!!!! afiseaza 1 in loc de 4!
	if (rad) {
		nrElemente(rad->st);
		nrElemente(rad->dr);
		contor++;
	}
	return contor;
}


//calcul nivel maxim
//functie ajutatoare pt getInaltime:
int max(int a, int b) { //operatorul ternar (?) - ne punem o intrebare!
	return a < b ? b : a; //are forma: expresie? true:false
}                       //daca expresia este adevarata (a<b), il returnam pe b pt ca noua ne trebuie maximul si viceversa



int getInaltime(Nod* rad) {
	//functia returneaza partea care are inaltimea cea mai mare (dintre st sau dr) la care se adauga radacina:
	if (rad) { 
		return max(getInaltime(rad->st), getInaltime(rad->dr)) + 1; //+1 pt ca nu se numara radacina, si trebuie adaugata (la calculul inaltimii)
	}
	else { //daca nu avem arbore
		return 0; //returnam inaltimea 0
	}
}


//fct de afisare nivel de la seminar
void afisareNivel(Nod* rad, int nivelDorit, int nivelCurent) { //nivelCurent trebuie sa fie mereu 1 cand apelam!!! 1 e cel mai de jos? 
	if (rad) { //e exact ca afisarea arborelui doar ca se afiseaza un anumit nivel deci avem acest if/else in plus:
		if (nivelDorit = nivelCurent) { //daca suntem la nivelul dorit
			afisareSenzor(rad->info); //afisam senzorul 
		}
		else {
			afisareNivel(rad->st, nivelDorit, nivelCurent + 1); //daca nu se identifica nivelul dorit, se cauta in stanga
			afisareNivel(rad->dr, nivelDorit, nivelCurent + 1); //si in dreapta nodului curent
		} //deci suntem pe un nivel (mai intai pe nivelul 1-cel mai de jos), daca nu e cel dorit, ne uitam ce are in stanga si ce are in dreapta (urcam un nivel mai sus cu +1?) si tot asa pt fiecare nod
	}     //se afiseaza toate nodurile de pe nivelul dorit!! nu doar unul
}


//fct de cautare dupa un criteriu
Senzor cautareSenzor(Nod* rad, int id) {
	if (rad) {
		if (id == rad->info.idSenzor) {
			return rad->info;
		}
		else if (id < rad->info.idSenzor) { //daca i-dul cautat este mai mic decat id-ul nodului curent
			return cautareSenzor(rad->st, id); //cautam in stanga radacinii
		}
		else { //daca este mai mare
			return cautareSenzor(rad->dr, id); //cautam in dreapta
		}
	}
}


//stergere arbore obisnuita
Nod* stergereArbore(Nod* rad) { //tipul returnat Nod*! si returnam NULL
	if (rad) {
		stergereArbore(rad->st);
		stergereArbore(rad->dr);
		free(rad->info.producator);
		free(rad);
		return NULL;
	}
}




//stergere dupa un criteriu
//nivel maxim al arborelui - fct folosita doar la extragere dupa un criteriu si stergere dupa un criteriu
Nod* cautareMaxim(Nod* rad) {
	if (rad) {
		if (rad->dr) {
			return cautareMaxim(rad->dr); //cautam doar in dreapta pt ca doar acolo poate sa fie maximul
		}
		else {
			return rad;
		}
	}
	else { //daca nu avem arbore
		return NULL;
	}
}


Nod* stergereNodDupaId(Nod* rad, int id) {
	if (rad) { //3 if-uri!!!!
		if (id == rad->info.idSenzor) { //aceeasi conditie ca la cautare in arbore
			if (rad->st == NULL) { //luam mai intai pe stanga, daca e NULL atunci luam un temp care e = rad->dr!
				Nod* temp = rad->dr; //in plus fata de stergerea obisnuita avem acest temp pe care il returnam (in loc sa returnam NULL)
				/*free(rad->info.producator);*/ //astea 2 randuri ca la stergerea obisnuita
				free(rad);
				return temp;
			}
			else if (rad->dr == NULL) { //apoi dreapta
				Nod* temp = rad->st; //la fel ca mai sus!
				/*free(rad->info.producator);*/ //merge si fara randul asta (ca la extragere)
				free(rad);
				return temp;
			}
			else { //acest else in plus la extragere, se pare ca merge si cu si fara 
				Nod* temp = cautareMaxim(rad->st); //asta e al 3-lea temp si e cel cu cautareMaxim
				//de retinut astea 3 randuri(rad->info si temp->info merg si inversate):
				Senzor s = temp->info; //1 
				temp->info = rad->info;//2
				rad->info = s;//3
				rad->st = stergereNodDupaId(rad->st, id);
			}
		} //daca nu este egal:
		else if (id < rad->info.idSenzor) { //daca id-ul cautat este m mic decat id-ul nodului curent (la fel ca la cautare)
			rad->st = stergereNodDupaId(rad->st, id); //ca la cautare doar ca e stergere
		}
		else { //la fel ca la cautare 
			rad->dr = stergereNodDupaId(rad->dr, id);
		}
	} //aici nu da eroare de exec in situatia in care nu avem arbore deci nu e nevoie de acel else de la extragere dupa un criteriu
}


//extragere dupa un criteriu
Senzor extragereSenzorDupaId(Nod** rad, int id) {
	if (rad) {
		if (id == (*rad)->info.idSenzor) {
			Senzor rezultat = (*rad)->info; //in plus fata de stergere dupa un criteriu!!!
			if ((*rad)->st == NULL) {
				Nod* temp = (*rad)->dr;
				/*free((*rad)->info.producator);*/
				free((*rad));
				*rad = temp; //in loc de return cum era la stergere avem *rad =
			}
			else if ((*rad)->dr == NULL) {
				Nod* temp = (*rad)->st;
				/*free((*rad)->info.producator);*/ //randul asta nu e necesar?
				free((*rad));
				*rad = temp; //rad = in loc de return..iar mai jos avem return in loc de rad->info= (fata de stergere)
			}
			else {
				Nod* temp = cautareMaxim((*rad)->st); //al 3-lea temp
				Senzor s = temp->info;
				temp->info = (*rad)->info;
				(*rad)->info = s;
			}
			return rezultat;
		} //daca nu este egal:
		else if(id < (*rad)->info.idSenzor) { //daca id-ul cautat este mai mic decat id-ul nodului curent
			return extragereSenzorDupaId(&(*rad)->st, id); //cand avem eroarea Nod* is incomplatible with Nod** - sa incerc sa pun & 
		}
		else { //daca este mai mare
			return extragereSenzorDupaId(&(*rad)->dr, id);
		}
	} //daca nu exista noduri in arbore
	else {
		return creareSenzor(-1, "", -1); 
	}
}

//functie care mareste nrPini cu 1 - la toti senzorii din arbore:
void marireNrPini(Nod* rad) { //similara cu o functie de afisare
	if (rad) {
		marireNrPini(rad->st);
		marireNrPini(rad->dr);
		rad->info.nrPini++; //marit cu 1
		/*rad->info.nrPini += 10;*/ //marit cu 10
	}
}

//functie care mareste nrPini cu 1 - la toti senzorii produsi de un anumit producator: 
void marireNrPiniDupaProducator(Nod* rad, const char* producator) { //primeste param producator in plus
	if (rad) {
		marireNrPiniDupaProducator(rad->st, producator);
		marireNrPiniDupaProducator(rad->dr, producator);
		if (strcmp(rad->info.producator, producator) == 0) { //doar randul asta in plus daca marim in fct de un producator
			rad->info.nrPini++;
		}
	}
}


void main() {

	Nod* arbore = NULL;

	//citire din fisier - de testat (de facut un fisier si de adaugat senzori in el cu nr de zensori sus si fiecare articol pe un rand)
	//FILE* f = fopen("fisier", "r"); //daca scriem aici w va scrie in fisier? cum se face scrierea in fisier?
	//if (f) { //verificam daca fisierul a fost deschis
	//	int nrSenzori = 0; //declaram o variabila cu ajutorul careia citim numarul de senzori din fisier
	//	fscanf(f, "%d", &nrSenzori);
	//	printf("\nNumarul senzorilor este: %d.\n", nrSenzori); //afisam pe ecran numarul persoanelor in capul listei (arborelui)
	//	for (int i = 0; i < nrSenzori; i++) { //for ca sa putem citi mai multi senzori
	//		arbore = inserareInArbore(arbore, citireSenzorDinFisier(f));
	//	}
	//}
	//fclose(f); //inchidem fisierul


	arbore = inserareInArbore(arbore, creareSenzor(3, "IBM", 256));
	arbore = inserareInArbore(arbore, creareSenzor(1, "Siemens", 120));
	arbore = inserareInArbore(arbore, creareSenzor(5, "Intel", 50));
	arbore = inserareInArbore(arbore, creareSenzor(4, "Samsung", 3));

	afisareArbore(arbore);

	printf("\nAfisare senzori cu nr de pini mai mare decat 100:\n");
	afisareArboreDupaNrPini(arbore, 100);

	//atentie nrElemente e bine sa fie apelat chiar aici dupa inserare in arbore, altfel e posibil sa nu numere bine, daca mai apelam alte functii inainte
	printf("\nNumarul de senzori din arbore este: %d.\n", nrElemente(arbore)); 

	//testare functie marire nrPini:
	printf("\nnrPini marit cu 1:\n");
	marireNrPini(arbore);
	/*marireNrPiniDupaProducator(arbore, "Siemens");*/
	afisareArbore(arbore);

	//testare getInaltime arbore:
	printf("\nNivelul maxim al arborelui: %d.\n", getInaltime(arbore));

	//testare afisare nivel (se apeleaza identic cu afisarea arborelui):
	printf("\nAfisare nivel:\n");
	afisareNivel(arbore, 3, 1); //nivelCurent trebuie sa fie mereu 1!
	                            //orice pun la nivelDorit, afiseaza acelasi lucru - 3.IBM (practic afiseaza nivelul 3 mereu?)



	//testare cautare in arbore dupa id
	printf("\nCautare in arbore:\n");
	afisareSenzor(cautareSenzor(arbore, 5)); //afiseaza pt oricare id (si din stanga si din dreapta, spre deosebire de problema de la bicicleta!)

	//nrElemente eronat:
	/*printf("\nNumarul de senzori din arbore este: %d.\n", nrElemente(arbore));*/ //de ex aici afiseaza in mod eronat 8 senzori - la fel facea si la Bicicleta

	//testare extragere dupa un criteriu:
	printf("\nNodul extras:\n");
	afisareSenzor(extragereSenzorDupaId(&arbore, 4));
	printf("\nAfisare arbore dupa extragerea senzorului cu id-ul 4:\n");
	afisareArbore(arbore);


	//testare stergere dupa id:
	stergereNodDupaId(arbore, 1); //daca il stergem pe cel cu id-ul 4, il sterge si pe 5? in rest merge cu oricare
	printf("\nAfisare arbore dupa stergerea senzorului cu id-ul 1:\n");
	afisareArbore(arbore);

	stergereArbore(arbore);

}

























