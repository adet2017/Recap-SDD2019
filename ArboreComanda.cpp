#include <iostream>

struct Comanda {
	int id;
	char* numeClient;
	int nrProduse;
	char** produseComandate;
	char* dataLivrare;
};

struct Nod {
	Comanda info;
	Nod* st;
	Nod* dr;
};

Comanda creareComanda(int id, const char* numeClient, int nrProduse, const char** produseComandate, const char* dataLivrare) {
	Comanda c;
	c.id = id;
	c.numeClient = (char*)malloc(sizeof(char)*(strlen(numeClient) + 1));
	strcpy(c.numeClient, numeClient);
	c.nrProduse = nrProduse;

	c.produseComandate = (char**)malloc(sizeof(char*)*nrProduse); //1
	for (int i = 0; i < nrProduse; i++) { //2
		c.produseComandate[i] = (char*)malloc(sizeof(char)*(strlen(produseComandate[i]) + 1)); //3 - ca la un char obisnuit doar ca avem [i] peste tot
		strcpy(c.produseComandate[i], produseComandate[i]); //4
	}

	c.dataLivrare = (char*)malloc(sizeof(char)*(strlen(dataLivrare) + 1));
	strcpy(c.dataLivrare, dataLivrare);
	return c;
}

Comanda copiereComanda(Comanda c) {
	Comanda copie;
	copie.id = c.id;
	copie.numeClient = (char*)malloc(sizeof(char)*(strlen(c.numeClient) + 1));
	strcpy(copie.numeClient, c.numeClient);
	copie.nrProduse = c.nrProduse;

	copie.produseComandate = (char**)malloc(sizeof(char*)*c.nrProduse); //1
	for (int i = 0; i < c.nrProduse; i++) { //2 - in interiorul for e ca la un char obisnuit doar ca avem [i] peste tot
		copie.produseComandate[i] = (char*)malloc(sizeof(char)*(strlen(c.produseComandate[i]) + 1)); //3
		strcpy(copie.produseComandate[i], c.produseComandate[i]); //4
	}

	copie.dataLivrare = (char*)malloc(sizeof(char)*(strlen(c.dataLivrare) + 1));
	strcpy(copie.dataLivrare, c.dataLivrare);
	return copie;
}

Nod* creareNod(Comanda c, Nod* st, Nod* dr) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = copiereComanda(c);
	nou->st = st;
	nou->dr = dr;
	return nou;
}


Nod* inserareInArbore(Nod* rad, Comanda c) {
	if (rad) {
		if (c.id < rad->info.id) { //daca id-ul noii comenzi este mai mic decat id-ul din radacina
			rad->st = inserareInArbore(rad->st, c); //inseram in stanga radacinii
		}
		else { //daca e mai mare
			rad->dr = inserareInArbore(rad->dr, c); //inseram in dreapta
		}
	}
	else {
		rad = creareNod(c, NULL, NULL);
	}
	return rad;
}



void afisareComanda(Comanda c) {
	printf("Comanda cu id-ul %d, plasata de clientul %s, contine %d produse cu denumirile ", c.id, c.numeClient, c.nrProduse);
	for (int i = 0; i < c.nrProduse; i++) {
		printf("%s, ", c.produseComandate[i]);
	}
	printf("si are data de livrare %s.\n", c.dataLivrare);
}

//pct 2- afisare arbore in inordine
void afisareArbore(Nod* rad) {
	if (rad) {
		afisareArbore(rad->st);
		afisareComanda(rad->info);
		afisareArbore(rad->dr);
	}
}


// functie care returneaza un vector de id-uri dupa data de livrare
int* vectorId(Nod* rad, int* v, int i, const char* data) // v e vectorul, trebuie transmis ca parametru deja initializat in main
{
	if (rad == NULL)
		return v;

	if (strcmp(rad->info.dataLivrare, data) == 0) { // aici se face comparatia cu data de livrare
		v[i] = rad->info.id; // se copiaza id-ul in vector daca se confirma data
	}
	i++;  // se mareste contorul oricum si se continua recursiv
	if (rad->st != NULL)
		v = vectorId(rad->st, v, i, data);
	if (rad->dr != NULL)
		v = vectorId(rad->dr, v, i, data);

	return v;
}


//pct 4 - fct care returneaza nr de noduri frunza (criteriu de cautare: comenzi plasate de un anumit client)
int nrNoduriFrunza(Nod* rad, const char* client)
{
	int contor = 1;
	if (rad == NULL)
		return 0;
	else if (strcmp(rad->info.numeClient, client) == 0) // aici nu e corect, probabil trebuie gasita o functie mai simpla de numarare a elementelor din arbore
	// si pusa conditia asta -- face ceva, dar nu corect
	{
		contor += nrNoduriFrunza(rad->st, client);
		contor += nrNoduriFrunza(rad->dr, client);
		return contor;
	}
}


//pct 5 (de retinut, are 3 puncte!!) - la fel ca marireNrPini de la Senzor
//fct care modifica nrProduse si produseComandate pentru o anumita comanda (criteriu primit ca param: id-ul comenzii)
void modificareComanda(Nod * rad, int id, int nrProduse, const char** produseComandate) { //nu e necesar sa returneze Nod*
	if (rad) {
		modificareComanda(rad->st, id, nrProduse, produseComandate); //cautam comanda 
		modificareComanda(rad->dr, id, nrProduse, produseComandate);
		if (id == rad->info.id) {
			rad->info.nrProduse = nrProduse; // aici returnai o comanda care n-are nicio treaba(vezi mai jos), trebuie modificat rad

			rad->info.produseComandate = (char**)malloc(sizeof(char*) * nrProduse); //1
			for (int i = 0; i < nrProduse; i++) { //2
				rad->info.produseComandate[i] = (char*)malloc(sizeof(char) * (strlen(produseComandate[i]) + 1)); //3 - ca la un char obisnuit doar ca avem [i] peste tot
				strcpy(rad->info.produseComandate[i], produseComandate[i]); //4
			}
		}
	}
}


//gresita:
//Comanda modificareComanda(Nod* rad, int id, int nrProduse, const char** produseComandate) {
//	if (rad) {
//		modificareComanda(rad->st, id, nrProduse, produseComandate); //cautam comanda 
//		modificareComanda(rad->dr, id, nrProduse, produseComandate);
//		if (id == rad->info.id) {
//			Comanda c; //NU
//			c.nrProduse = nrProduse;
//
//			c.produseComandate = (char**)malloc(sizeof(char*)*nrProduse); //1
//			for (int i = 0; i < nrProduse; i++) { //2
//				c.produseComandate[i] = (char*)malloc(sizeof(char)*(strlen(produseComandate[i]) + 1)); //3 - ca la un char obisnuit doar ca avem [i] peste tot
//				strcpy(c.produseComandate[i], produseComandate[i]); //4
//			}
//			return c;
//		}
//	}
//}






void main() {
	Nod* arbore = NULL;



	const char* v1[2] = { "lentile", "picaturi" };
	const char* v2[1] = { "covor" };
	const char* v3[1] = { "malm" };
	const char* v4[3] = { "comoda", "televizor", "boxa" };
	arbore = inserareInArbore(arbore, creareComanda(3, "Client1", 2, v1, "05.06.2019"));
	arbore = inserareInArbore(arbore, creareComanda(1, "Client1", 1, v2, "05.06.2019"));
	arbore = inserareInArbore(arbore, creareComanda(5, "Client1", 1, v3, "05.06.2019"));
	arbore = inserareInArbore(arbore, creareComanda(4, "Client1", 3, v4, "05.06.2019"));

	//pct 2
	afisareArbore(arbore);

	//pct 4
	printf("\nNr de noduri frunza care contin comenzi plasate de client1 este %d\n", nrNoduriFrunza(arbore, "Client1"));

	//pct 5
	const char* v5[2] = { "veioza", "plafoniera" };
	modificareComanda(arbore, 1, 2, v5); 

	printf("\nAfisare arbore dupa modificare comanda cu id-ul 1:\n");
	afisareArbore(arbore);

}