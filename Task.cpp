#include <iostream>


struct Task {
	int idTask;
	char dataCreare[10]; //in subiect nu specifica ce tip sa fie data
	char dataAsignare[10];//aici mi-a spus proful sa fac ori asa, ori alta structura care contine int luna, int an etc..
	char* inginerAsignat;
	int prioritate;
	char* stareTask;
};


struct Nod {
	Task info;
	Nod* next;
};


Task creareTask(int idTask, const char* dataCreare, const char* dataAsignare, const char* inginerAsignat, int prioritate, const char* stareTask) {
	Task t;
	t.idTask = idTask;
	strcpy(t.dataCreare, dataCreare);
	strcpy(t.dataAsignare, dataAsignare);
	t.inginerAsignat = (char*)malloc(sizeof(char)*(strlen(inginerAsignat) + 1));
	strcpy(t.inginerAsignat, inginerAsignat);
	t.prioritate = prioritate;
	t.stareTask = (char*)malloc(sizeof(char)*(strlen(stareTask) + 1));
	strcpy(t.stareTask, stareTask);
	return t;
}


Task copiereTask(Task t) {
	Task copie;
	copie.idTask = t.idTask;
	strcpy(copie.dataCreare, t.dataCreare);
	strcpy(copie.dataAsignare, t.dataAsignare);
	copie.inginerAsignat = (char*)malloc(sizeof(char)*(strlen(t.inginerAsignat) + 1));
	strcpy(copie.inginerAsignat, t.inginerAsignat);
	copie.prioritate = t.prioritate;
	copie.stareTask = (char*)malloc(sizeof(char)*(strlen(t.stareTask) + 1));
	strcpy(copie.stareTask, t.stareTask);
	return copie;
}


Nod* inserareSfarsit(Nod* cap, Task t) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = copiereTask(t);
	nou->next = NULL;
	if (cap) {
		Nod* p = cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		cap = nou;
	}
	return cap;
}


void afisareTask(Task t) {
	printf("Task-ul cu id-ul %d, data creare %s, data asignare %s, a fost asignat inginerului %s, are prioritatea %d si starea actuala este %s\n", t.idTask, t.dataCreare, t.dataAsignare, t.inginerAsignat, t.prioritate, t.stareTask);
}

void afisareLista(Nod* cap) {
	Nod* p = cap;
	while (p) {
		afisareTask(p->info);
		p = p->next;
	}
}



//pct 2
int nrTaskuri(Nod* cap, const char* stareNou, const char* stareDeschis) {
	Nod* p = cap;
	int contor = 0;
	while (p) {
		if (strcmp(p->info.stareTask, stareNou) == 0) {
			contor++;
		}
		else if(strcmp(p->info.stareTask, stareDeschis) == 0){
			contor++;
		}
		p = p->next;
	}
	return contor;
}

//pct 3 
int nrIngineri(Nod* cap, const char* data) {
	Nod* p = cap;
	int contor = 0;
	while (p) {
		if (strcmp(p->info.dataAsignare, data) == 0) {
			contor++;
		}
		p = p->next;
	}
	return contor;
}


char** vectorIngineri(Nod* cap, const char* data) {
	Nod* p = cap;
	char** vector = (char**)malloc(sizeof(char*)*nrIngineri(cap, data));
	for (int i = 0; i < nrIngineri(cap, data); i++) {
		if (strcmp(p->info.dataAsignare, data) == 0) {
			vector[i] = (char*)malloc(sizeof(char)*(strlen(p->info.inginerAsignat) + 1));
			strcpy(vector[i], p->info.inginerAsignat);
		}
	}
	return vector;
}


//pct 4
void asignareTask(Nod* cap, int id, const char* inginer, const char* stare) {
	Nod* p = cap;
	while (p) {
		if (p->info.idTask == id) {
			p->info.inginerAsignat = (char*)malloc(sizeof(char)*(strlen(inginer) + 1));
			strcpy(p->info.inginerAsignat, inginer);

			if (strcmp(p->info.stareTask, "nou") == 0) {
				p->info.stareTask = (char*)malloc(sizeof(char)*(strlen(stare) + 1));
				strcpy(p->info.stareTask, stare);
			}
		}
		p = p->next;
	}
}


//pct 5 
Task extragereTask(Nod* &cap) {
	if (cap) {
		Task t = copiereTask(cap->info);
		Nod* p = cap;
		cap = p->next;
		free(p);
		return t;
	}
	else {
		return creareTask(-1, "", "", "", -1, "");
	}
}

void main() {
	Nod* lista = NULL;

	lista = inserareSfarsit(lista, creareTask(1, "10/06/2019", "15.06.2019", "Inginer1", 1, "nou"));
	lista = inserareSfarsit(lista, creareTask(2, "10/06/2018", "15.06.2018", "Inginer2", 1, "deschis"));
	lista = inserareSfarsit(lista, creareTask(3, "10/06/2016", "15.06.2016", "Inginer3", 1, "in lucru"));
	lista = inserareSfarsit(lista, creareTask(4, "10/06/2017", "15.06.2017", "Inginer4", 1, "nou"));
	lista = inserareSfarsit(lista, creareTask(5, "10/06/2019", "15.06.2019", "Inginer5", 1, "rezolvat"));
	lista = inserareSfarsit(lista, creareTask(6, "10/06/2019", "15.06.2019", "Inginer6", 1, "inchis"));

	afisareLista(lista);

	//pct 2
	printf("\nNr de task-uri la care nu s-a inceput lucrul este %d.\n", nrTaskuri(lista, "nou", "deschis"));


	//pct 3
	char** v = vectorIngineri(lista, "10/06/2019");
	for (int i = 0; i < nrIngineri(lista, "10/06/2019"); i++) {
		//aici nu putem afisa cu fct afisareTask pt ca aici afisam doar inginerii, nu un task intreg!
		printf("\nIngineri asignati pe task-uri la data de 10/06/2019: %s\n", v[i]); 
	}


	//pct 4
	asignareTask(lista, 1, "Inginer5", "deschis");
	printf("\nAfisare lista dupa asignarea task-ului cu id-ul 1 catre Inginer5:\n");
	afisareLista(lista);


	//pct 5
	printf("\nNodul extras:\n");
	afisareTask(extragereTask(lista));

	printf("\nNodul extras:\n"); //extragem  de 3 ori pt ca asa cere pct 6
	afisareTask(extragereTask(lista));

	printf("\nNodul extras:\n");
	afisareTask(extragereTask(lista));

	printf("\nAfisare lista dupa extragere:\n"); //aici a vrut sa vada ca s-au sters cele 3 noduri extrase din lista
	afisareLista(lista);




}