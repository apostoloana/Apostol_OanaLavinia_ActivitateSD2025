#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
typedef struct HashTable HT;

struct Nod {
	Masina info;
	Nod* next;
};

struct HashTable {
	Nod** vector; 
	int dim;
};

Masina initMasina(int id,
	int nrUsi,
	float pret,
	const char* model,
	const char* numeSofer,
	unsigned char serie) {
	Masina m;
	m.id = id;
	m.nrUsi = nrUsi;
	m.pret = pret;
	m.model = (char*)malloc(strlen(model) + 1);
	strcpy_s(m.model, strlen(model) + 1, model);
	m.numeSofer = (char*)malloc(strlen(numeSofer) + 1);
	strcpy_s(m.numeSofer, strlen(numeSofer) + 1, numeSofer);
	m.serie = serie;
	return m;
}

void afisareMasina(Masina masina) {
	printf("Id: %d | ", masina.id);
	printf("Nr. usi : %d | ", masina.nrUsi);
	printf("Pret: %.2f | ", masina.pret);
	printf("Model: %s | ", masina.model);
	printf("Nume sofer: %s | ", masina.numeSofer);
	printf("Serie: %c\n", masina.serie);
}

void afisareListaMasini(Nod* cap) {
	while (cap) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaMasinaInListaFinal(Nod* cap, Masina masinaNoua) { 
	Nod* nod = cap;
	while (nod->next) {
		nod = nod->next;
	}
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;//shallow copy
	nou->next = NULL;
	nod->next = nou;
}


HT initializareHashTable(int dimensiune) {
	HT hash;
	hash.dim = dimensiune;
	hash.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune); 
	for (int i = 0; i < dimensiune; i++) {
		hash.vector[i] = NULL; 
	}
	return hash;
}

int calculeazaHash(const char* nume, int dimensiune) {
	int sum = 0;
	for (int i = 0; i < strlen(nume); i++) {
		sum += nume[i];
	}
	return sum % dimensiune;
}

void inserareMasinaInTabela(HT* hash, Masina m) {
	int poz = calculeazaHash(m.numeSofer, hash->dim);
	if (hash->vector[poz] == NULL) {
		hash->vector[poz] = (Nod*)malloc(sizeof(Nod));
		hash->vector[poz]->info = m;
		hash->vector[poz]->next = NULL;
	}
	else {
		adaugaMasinaInListaFinal(hash->vector[poz], m);
	}
}

void afisareTabelaDeMasini(HT hash) {
	for (int i = 0; i < hash.dim; i++) {
		if (hash.vector[i] != NULL) {
			printf("Clusterul: %d \n", i);
			afisareListaMasini(hash.vector[i]);
		}
		else {
			printf("Pe poz %d nu avem masini. \n", i);
		}
	}
}

void citireDinFisier(HT* hash, const char* numeFisier, int dim) {
	*hash = initializareHashTable(dim);
	FILE* f = fopen(numeFisier, "r");
	if (!f) {
		return;
	}
	char buffer[100];
	while (fgets(buffer, sizeof(buffer), f)) {
		int id;
		int nrUsi;
		float pret;
		char model[50];
		char numeSofer[50];
		unsigned char serie;

		char* token = strtok(buffer, ",");
		id = atoi(token);

		token = strtok(NULL, ",");
		nrUsi = atoi(token);
		token = strtok(NULL, ",");
		pret = atof(token);
		token = strtok(NULL, ",");
		strcpy(model, token);
		token = strtok(NULL, ",");
		strcpy(numeSofer, token);
		token = strtok(NULL, ",");
		serie = token[0];

		Masina m = initMasina(id, nrUsi, pret, model, numeSofer, serie);
		inserareMasinaInTabela(hash, m);
	}
	fclose(f);
}


void dezalocareLista(Nod** cap) {
	Nod* temp = (*cap);
	while (temp) {
		Nod* aux = temp;
		temp = temp->next;
		free(aux->info.model);
		free(aux->info.numeSofer);
		free(aux);
	}
	*cap = NULL;
}


void dezalocareTabelaDeMasini(HT* hash) {
	for (int i = 0; i < hash->dim; i++) {
		dezalocareLista(&(hash->vector[i]));
	}
	free(hash->vector);
	hash->vector = NULL;
	hash->dim = 0;

}

float pretMediu(Nod* cap) {
	float suma = 0;
	int count = 0;
	while (cap) {
		suma += cap->info.pret;
		count++;
		cap = cap->next;
	}
	if (count > 0) {
		return suma / count;
	}
	else {
		return 0;
	}
}

float* calculeazaPreturiMediiPerClustere(HT hash, int* nrClustere) {
	float* vectorPreturi = NULL;
	*nrClustere = 0;
	for (int i = 0; i < hash.dim; i++) {
		if (hash.vector[i]) {
			(*nrClustere)++;
		}
	}
	vectorPreturi = (float*)malloc(sizeof(float) * (*nrClustere));
	int contor = 0;
	for (int i = 0; i < hash.dim; i++) {
		if (hash.vector[i]) {
			vectorPreturi[contor] = pretMediu(hash.vector[i]);
			contor++; 
		}
	}
	return vectorPreturi;
}

Masina getMasinaDupaSoferLista(Nod* cap, const char* sofer) {
	Masina m;
	m.id = -1;
	while (cap && (strcmp(cap->info.numeSofer, sofer) != 0)) { 
		cap = cap->next;
	}
	if (cap) {
		m = initMasina(cap->info.id, cap->info.nrUsi, cap->info.pret, cap->info.model, cap->info.numeSofer, cap->info.serie);
	}
	return m;
}

Masina getMasinaDupaSofer(HT hash, const char* numeCautat) {
	Masina m;
	m.id = -1; 
	int poz = calculeazaHash(numeCautat, hash.dim);
	if (poz >= 0 && poz < hash.dim) {
		return getMasinaDupaSoferLista(hash.vector[poz], numeCautat);
	}
	return m;
}

int main() {
	HT hash;
	printf("\n-----------------------CITIRE DIN FISIER----------------------\n");
	citireDinFisier(&hash, "masini.txt", 7);
	afisareTabelaDeMasini(hash);
	printf("\n-----------------------PRET MEDIU / CLUSTER----------------------\n");
	int nrClustere = 0;
	float* vector = calculeazaPreturiMediiPerClustere(hash, &nrClustere);
	for (int i = 0; i < nrClustere; i++) {
		printf("Pretul mediu: %.2f.\n", vector[i]);
	}
	free(vector);
	printf("\n-----------------------CAUTA MASINA DUPA SOFER----------------------\n");
	Masina masina = getMasinaDupaSofer(hash, "Ionescu Bia");
	if (masina.id != -1) {
		afisareMasina(masina);
		if (masina.model != NULL) {
			free(masina.model);
		}
		if (masina.numeSofer != NULL) {
			free(masina.numeSofer);
		}
	}
	else {
		printf("Masina nu a fost gasita! Soferul nu are masini.\n");
	}
	printf("\n-----------------------DEZALOCARE----------------------\n");
	dezalocareTabelaDeMasini(&hash);

	_CrtDumpMemoryLeaks();

	return 0;
}