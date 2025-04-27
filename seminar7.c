#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crtdbg.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Heap {
	Masina* vector;
	int lungime;
	int nrElem;
};
typedef struct Heap Heap;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d | ", masina.id);
	printf("Nr. usi : %d | ", masina.nrUsi);
	printf("Pret: %.2f | ", masina.pret);
	printf("Model: %s | ", masina.model);
	printf("Nume sofer: %s | ", masina.numeSofer);
	printf("Serie: %c\n", masina.serie);
}

Heap initHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrElem = 0;
	heap.vector = (Masina*)malloc(sizeof(Masina) * lungime);
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int pozFiuSt = 2 * pozitieNod + 1;
	int pozFiuDr = 2 * pozitieNod + 2;

	int pozMax = pozitieNod;
	if (pozFiuSt < heap.nrElem && heap.vector[pozMax].pret < heap.vector[pozFiuSt].pret) {
		pozMax = pozFiuSt;
	}
	if (pozFiuDr < heap.nrElem && heap.vector[pozMax].pret < heap.vector[pozFiuDr].pret) {
		pozMax = pozFiuDr;
	}
	if (pozMax != pozitieNod) {
		Masina aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		if (pozMax <= (heap.nrElem - 2) / 2) {
			filtreazaHeap(heap, pozMax);
		}
	}
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	Heap h = initHeap(10);
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		h.vector[h.nrElem] = citireMasinaDinFisier(f);
		h.nrElem++;
	}
	fclose(f);
	for (int i = (h.nrElem - 2) / 2; i >= 0; i--) {
		filtreazaHeap(h, i);
	}
	return h;
}

void afisareHeap(Heap heap) {
	if (heap.vector) {
		for (int i = 0; i < heap.nrElem; i++) {
			afisareMasina(heap.vector[i]);
		}
	}
}

void afiseazaHeapAscuns(Heap heap) {
	if (heap.vector) {
		for (int i = heap.nrElem; i < heap.lungime; i++) {
			afisareMasina(heap.vector[i]);
		}
	}
}

Masina extrageMasina(Heap* heap) {
	if (heap->nrElem > 0) {
		Masina aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrElem - 1];
		heap->vector[heap->nrElem - 1] = aux;
		heap->nrElem--;
		for (int i = (heap->nrElem - 2) / 2; i >= 0; i--) {
			filtreazaHeap(*heap, i);
		}
		return aux; 
	}
}


void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++) {
		free(heap->vector[i].model);
		free(heap->vector[i].numeSofer);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrElem = 0;
}

typedef struct Nod Nod;
struct Nod {
	Masina info;
	Nod* next;
};

void inserareLaInceput(Nod** cap, Masina m) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = m;
	nou->next = *cap;
	*cap = nou;
}


void afisareLS(Nod* cap) {
	while (cap) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

Nod* salvareMasiniIDMare(Heap* heap, int idMin) {
	Nod* lista = NULL;
	while (heap->nrElem > 0 && heap->vector[0].id >= idMin) {
		Masina m = extrageMasina(heap);
		inserareLaInceput(&lista, m);
	}
	return lista;
}


void main() {
	Heap heap;
	heap = citireHeapDeMasiniDinFisier("masini.txt");
	printf("\n-------------------Afisare----------------------\n");
	afisareHeap(heap);


	Masina m = extrageMasina(&heap);
	printf("\n-------------------Prima masina extrasa----------------------\n");
	afisareMasina(m);

	printf("\n-------------------Afisare Elem Ascunse----------------------\n");
	//extrageMasina(&heap);
	//extrageMasina(&heap);
	//extrageMasina(&heap);
	//extrageMasina(&heap);
	afiseazaHeapAscuns(heap);

	printf("\n-------------------Salvare in LS----------------------\n");
	Nod* lista = salvareMasiniIDMare(&heap, 4);
	afisareLS(lista);

	printf("\n-------------------Dezalocare----------------------\n");
	dezalocareHeap(&heap);
	while (lista) {
		Nod* aux = lista;
		lista = aux->next;
		free(aux);
	}

	_CrtDumpMemoryLeaks();

}

