#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;


void afisareMasina(Masina m) {
	printf("ID: %d | ", m.id);
	printf("Nr usi: %d | ", m.nrUsi);
	printf("Pret: %.2f | ", m.pret);
	printf("Model: %s | ", m.model);
	printf("Sofer: %s | ", m.numeSofer);
	printf("Serie: %c \n", m.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	if (masini != NULL && nrMasini > 0) {
		for (int i = 0; i < nrMasini; i++) {
			afisareMasina(masini[i]);
		}
	}
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	Masina* aux;
	aux = (Masina*)malloc(sizeof(Masina) * ((*nrMasini) + 1));
	for (int i = 0; i < (*nrMasini); i++) {
		aux[i] = (*masini)[i]; 
	}
	aux[(*nrMasini)] = masinaNoua;
	if ((*masini) != NULL) {
		free(*masini);
	}
	(*masini) = aux;
	(*nrMasini)++;
}

Masina citireMasinaFisier(FILE* file) {
	Masina m;
	char buffer[100]; 
	fgets(buffer, 100, file);
	char delimitator[3] = ",\n"; 
	char* token;
	token = strtok(buffer, delimitator);
	m.id = atoi(token);
	m.nrUsi = atoi(strtok(NULL, delimitator));
	m.pret = atof(strtok(NULL, delimitator));
	token = strtok(NULL, delimitator);
	m.model = malloc(strlen(token) + 1);
	strcpy_s(m.model, (strlen(token) + 1), token);

	token = strtok(NULL, delimitator);
	m.numeSofer = malloc(strlen(token) + 1);
	strcpy_s(m.numeSofer, (strlen(token) + 1), token);

	token = strtok(NULL, delimitator);
	m.serie = token[0];
	return m;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	FILE* f = fopen(numeFisier, "r");
	Masina* masini = NULL;
	while (!feof(f)) { //file end of file
		Masina m = citireMasinaFisier(f);
		adaugaMasinaInVector(&masini, nrMasiniCitite, m);
	}
	fclose(f);
	return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	for (int i = 0; i < (*nrMasini); i++) {
		free((*vector)[i].model);
		free((*vector)[i].numeSofer);
	}
	free(*vector);
	(*vector) = NULL;
	(*nrMasini) = 0;
}

int main() {
	Masina* vectorMasini = NULL;
	int nrMasini = 0;
	vectorMasini = citireVectorMasiniFisier("masini.txt", &nrMasini);

	afisareVectorMasini(vectorMasini, nrMasini);

	dezalocareVectorMasini(&vectorMasini, &nrMasini);

	return 0;
}