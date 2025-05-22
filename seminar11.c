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

typedef struct NodPrincipal NodP;
typedef struct NodSecundar NodS;

struct NodPrincipal {
	Masina info;
	NodP* next;
	NodS* vecini;
};

struct NodSecundar {
	NodS* next;
	NodP* nodInfo;
};

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
	printf("Serie: %c.\n", masina.serie);
}

void inserareLP(NodP** cap, Masina m) {
	NodP* nod = (NodP*)malloc(sizeof(NodP));
	nod->next = NULL;
	nod->info = m;
	nod->vecini = NULL;
	if (*cap) {
		NodP* temp = *cap;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nod;
	}
	else {
		*cap = nod;
	}
}


void inserareLS(NodS** cap, NodP* info) {
	NodS* nod = (NodS*)malloc(sizeof(NodS));
	nod->next = *cap;
	nod->nodInfo = info;
	*cap = nod;
}

NodP* cautaNodDupaID(NodP* cap, int id) {
	while (cap && cap->info.id != id) {
		cap = cap->next;
	}
	return cap;
}

void inserareMuchie(NodP* cap, int idStart, int idStop) {
	NodP* nodStart = cautaNodDupaID(cap, idStart);
	NodP* nodStop = cautaNodDupaID(cap, idStop);
	if (nodStart && nodStop) {
		inserareLS(&(nodStart->vecini), nodStop);
		inserareLS(&(nodStop->vecini), nodStart);
	}
}


NodP* citireNoduriMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodP* graf = NULL;
	if (f) {
		while (!feof(f)) {
			inserareLP(&graf, citireMasinaDinFisier(f));
		}
	}
	fclose(f);
	return graf;
}

void citireMuchiiDinFisier(NodP* graf, const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			int idStart;
			int idStop;
			fscanf(f, "%d %d", &idStart, &idStop);
			inserareMuchie(graf, idStart, idStop);
		}
	}
	fclose(f);
}

void stergeLS(NodS** cap) {
	while (*cap) {
		NodS* aux = (*cap)->next;
		free(*cap);
		*cap = aux;
	}
}

void dezalocareNoduriGraf(NodP** cap) {
	while (*cap) {
		NodP* aux = (*cap)->next;
		stergeLS(&(*cap)->vecini);
		if ((*cap)->info.model) {
			free((*cap)->info.model);
		}
		if ((*cap)->info.numeSofer) {
			free((*cap)->info.numeSofer);
		}
		free(*cap);
		*cap = aux;
	}
}

void afisareGraf(NodP* graf) {
	while (graf) {
		printf("\n\nIn nodul %d avem masina: \n", graf->info.id);
		afisareMasina(graf->info);
		printf("Aceasta are ca vecini nodurile urmatoare: ");
		NodS* temp = graf->vecini;
		while (temp) {
			printf("%d, ", temp->nodInfo->info.id);
			temp = temp->next;
		}
		graf = graf->next;
	}
}

NodP* citireGrafDinFisier(const char* fisierNoduri, const char* fisierMuchii) {
	NodP* graf = citireNoduriMasiniDinFisier(fisierNoduri);
	citireMuchiiDinFisier(graf, fisierMuchii);
	return graf;
}

int main() {

	NodP* graf = citireGrafDinFisier("masini.txt", "muchii.txt");


	printf("\n-----------Afisare------------\n");
	afisareGraf(graf);


	printf("\n-----------Dezalocare------------\n");
	dezalocareNoduriGraf(&graf);

	_CrtDumpMemoryLeaks();
	return 0;
}