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

typedef struct Nod Nod;
struct Nod {
	Masina info;
	Nod* fiuStang;
	Nod* fiuDrept;
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
	m1.pret= atof(strtok(NULL, sep));
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


void adaugaMasinaInArbore(Nod** rad, Masina masinaNoua) {
	if (*rad) {
		if ((*rad)->info.id > masinaNoua.id) {
			adaugaMasinaInArbore(&((*rad)->fiuStang), masinaNoua);
		}
		else {
			adaugaMasinaInArbore(&((*rad)->fiuDrept), masinaNoua);
		}
	}
	else {
		Nod* nou = malloc(sizeof(Nod));
		nou->info = masinaNoua;
		nou->fiuDrept = NULL;
		nou->fiuStang = NULL;
		*rad = nou;
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* arbore = NULL;
	while (!feof(f))
	{
		Masina m = citireMasinaDinFisier(f);
		adaugaMasinaInArbore(&arbore, m);
	}
	fclose(f);
	return arbore;
}

//SRD
void afisareIO(Nod* rad) {
	if (rad) {
		afisareIO(rad->fiuStang);
		afisareMasina(rad->info);
		afisareIO(rad->fiuDrept);
	}
}

//SDR
void afisarePreO(Nod* rad) {
	if (rad) {
		afisareMasina(rad->info);
		afisarePreO(rad->fiuStang);
		afisarePreO(rad->fiuDrept);
	}
}

//RSD
void afisarePostO(Nod* rad) {
	if (rad) {
		afisarePostO(rad->fiuStang);
		afisarePostO(rad->fiuDrept);
		afisareMasina(rad->info);
	}
}

void dezalocareArboreDeMasini(Nod** rad) {
	if (*rad) {
		dezalocareArboreDeMasini(&(*rad)->fiuStang);
		dezalocareArboreDeMasini(&(*rad)->fiuDrept);
		free((*rad)->info.model);
		free((*rad)->info.numeSofer);
		free(*rad);
	}
}

Masina getMasinaByID(Nod* rad, int id) {
	Masina m;
	m.id = -1;
	if (rad) {
		if (rad->info.id < id) {
			getMasinaByID(rad->fiuStang, id);
		}
		else if(rad->info.id > id) {
			getMasinaByID(rad->fiuDrept, id);
		}
		else {
			return rad->info;
		}
	}
	return m;
}

int determinaNumarNoduri(Nod* rad) {
	//calculeaza numarul total de noduri din arborele binar de cautare
	if (rad) {
		int stanga = determinaNumarNoduri(rad->fiuStang);
		int dreapta = determinaNumarNoduri(rad->fiuDrept);
		return 1 + stanga + dreapta;
	}
	else {
		return 0;
	}
}

int calculeazaInaltimeArbore(Nod* rad) {
	if (rad) {
		int inaltimeST = calculeazaInaltimeArbore(rad->fiuStang);
		int inaltimeDR = calculeazaInaltimeArbore(rad->fiuDrept);
		return 1 + (inaltimeST > inaltimeDR ? inaltimeST : inaltimeDR);
	}
	else {
		return 0;
	}
}

float calculeazaPretTotal(Nod* rad) {
	if (rad == NULL) {
		return 0;
	}
	float totalST = calculeazaPretTotal(rad->fiuStang);
	float totalDR = calculeazaPretTotal(rad->fiuDrept);
	
	return rad->info.pret + totalST + totalDR;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* rad, const char* numeSofer) {
	int sum = 0;
	if (rad) {
		if (strcmp(rad->info.numeSofer, numeSofer) == 0) {
			sum += rad->info.pret;
		}
		sum += calculeazaPretulMasinilorUnuiSofer(rad->fiuStang, numeSofer);
		sum += calculeazaPretulMasinilorUnuiSofer(rad->fiuDrept, numeSofer);
	}
	return sum;
}

int main() {
	Nod* arbore = NULL;
	arbore = citireArboreDeMasiniDinFisier("masini.txt");
	afisareIO(arbore);

	printf("\n----------------------Get By ID------------------\n");
	Masina m = getMasinaByID(arbore, 5);
	afisareMasina(m);

	printf("\n----------------------Pret Total------------------\n");
	float suma = calculeazaPretTotal(arbore);
	printf("Pret total: %.2f.", suma);

	printf("\n----------------------Pret Masini Sofer------------------\n");
	float pretMasiniSofer = calculeazaPretulMasinilorUnuiSofer(arbore, "Ionescu");
	printf("Pret total masini pt Ionescu: %.2f.", pretMasiniSofer);

	printf("\n----------------------Nr total noduri------------------\n");
	int nrNoduri = determinaNumarNoduri(arbore);
	printf("Nr total noduri: %d.", nrNoduri);

	printf("\n----------------------Inaltime arbore------------------\n");
	int inaltime = calculeazaInaltimeArbore(arbore);
	printf("Inaltimea este: %d.", inaltime);

	printf("\n----------------------Dezalocare------------------\n");
	dezalocareArboreDeMasini(&arbore);

	_CrtDumpMemoryLeaks();
	return 0;
}