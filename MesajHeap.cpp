#include <iostream>;

//heap se foloseste la coada de prioritati


struct Mesaj {
	char* text;
	int prioritate;
};

struct Heap {
	Mesaj* vector;
	int dimensiune;
};



Mesaj creareMesaj(const char* text, int prioritate) {
	Mesaj m;
	m.text = (char*)malloc(sizeof(char)*(strlen(text) + 1));
	strcpy(m.text, text);
	m.prioritate = prioritate;
	return m;
}



void afisareMesaj(Mesaj m) {
	printf("Mesajul cu textul %s are prioritatea %d.\n", m.text, m.prioritate);
}



void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.dimensiune; i++) {
		afisareMesaj(heap.vector[i]);
	}
}




void filtrare(Heap heap, int pozitie) {
	int fiuS = 2 * pozitie + 1;
	int fiuD = 2 * pozitie + 2;
	int max = pozitie;
	//daca e mai mic decat dimensiunea heap (ca sa incapa) si e mai mare decat actualul max (max se calculeaza in functie de prioritatea cea mai mare)
	if (fiuS < heap.dimensiune && heap.vector[fiuS].prioritate > heap.vector[max].prioritate) { //cand avem min heap aici doar schimbam semnele (< in loc de >)
		max = fiuS; //atunci fiul din stanga devine max
	}
	if (fiuD < heap.dimensiune && heap.vector[fiuD].prioritate > heap.vector[max].prioritate) {
		max = fiuD;
	}
	if (max != pozitie) { //daca s-a modificat intre timp
		Mesaj aux = heap.vector[max];
		heap.vector[max] = heap.vector[pozitie];
		heap.vector[pozitie] = aux;
		if (max < (heap.dimensiune - 1) /2 ){ //calculam fiul din stanga
			filtrare(heap,max); //recursivitate!
			}
	}
}


void inserareHeap(Heap &heap, Mesaj m) {
	Mesaj* temp = (Mesaj*)malloc(sizeof(Mesaj)*(heap.dimensiune + 1));
	for (int i = 0; i < heap.dimensiune; i++) {
		temp[i] = heap.vector[i];
	}
	temp[heap.dimensiune] = m;
	free(heap.vector);
	heap.dimensiune++;
	heap.vector = temp;
	for (int i = (heap.dimensiune - 1) / 2; i >= 0; i--) {
		filtrare(heap, i);
	}
}


//extragere de pe prima pozitie
Mesaj extragereMesaj(Heap &heap) {
	if (heap.dimensiune > 0) {
		Mesaj rezultat = heap.vector[0];
		Mesaj* temp = (Mesaj*)malloc(sizeof(Mesaj)*(heap.dimensiune - 1)); //-1 pt ca pe unul il extragem
		for (int i = 1; i < heap.dimensiune; i++) { //ori asa ori: int i = 0; i < heap.dimensiune-1 si mai jos
			                             //temp[i] = heap.vector[i+1]; Dar nu combinat! Da eroare de exec
			temp[i-1] = heap.vector[i]; //nu mai stergem textul pt ca aici am facut shallow copy
		}
		free(heap.vector);
		heap.vector = temp; //temp pointeaza catre vector? nu putem sa stergem vect pt ca atunci am pierde heap-ul
		heap.dimensiune--;
		for (int i = (heap.dimensiune - 1) / 2; i >= 0; i--) {
			filtrare(heap, i);
		}
		return rezultat;
	}
	else {
		Mesaj m;
		m.text = NULL;
		m.prioritate = -1;
		return m;
	}
}





void main() {

	Heap heap;

	heap.dimensiune = 6; //si la scrierea in fisier nrMesaje era tot 6, reprezenta primul numar din fisier (din capul listei) - vezi la seminar 3 2019 ID

	heap.vector = (Mesaj*)malloc(sizeof(Mesaj)*heap.dimensiune); //alocam memorie pentru heap

	heap.vector[0] = creareMesaj("Buna", 8);
	heap.vector[1] = creareMesaj("Ce faci?", 6);
	heap.vector[2] = creareMesaj("Bine", 3);
	heap.vector[3] = creareMesaj("Dar tu?", 9);
	heap.vector[4] = creareMesaj("Foarte bine", 4);
	heap.vector[5] = creareMesaj("Pa", 2);

	printf("\n\n");
	afisareHeap(heap);


	//aici se filtreaza iar cel mai mare ajunge sus!
	//ca sa apelam doar pt parintii din arbore, apelam de jos in sus, ca sa ramana cel mai mare sus (primul)
	//pe prima pozitie il vom avea pe cel cu prioritatea cea mai mare, restul nu se sorteaza, raman asa cum erau - ar trebui, dar nu raman?? de fiec data se afiseaza in alta ordine
	for (int i = (heap.dimensiune - 1) / 2; i >= 0; i--) {
		filtrare(heap, i);
	}

	printf("\n\n");
	afisareHeap(heap); //dupa filtrare, mesajul cu prioritatea 9 a devenit primul de sus


	inserareHeap(heap, creareMesaj("Hey!", 11));

	printf("\n\n");     //de fiecare data mesajele se afiseaza in alta ordine - de ce?
	afisareHeap(heap); //cand afiseaza aici, apare 11 ca fiind primul (cu prioritatea cea mai mare)
	                  //dar dc? pt ca nu s-a facut filtrarea. se insereaza la inceput cum ar veni?

	printf("\nMesajul extras:\n");
	afisareMesaj(extragereMesaj(heap)); //il extrage pe cel cu prioritatea 11, pt ca extrage de la inceput

	printf("\n\n");
	afisareHeap(heap);
}