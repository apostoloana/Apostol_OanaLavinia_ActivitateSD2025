#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<crtdbg.h>

typedef struct Telefon Telefon;

struct Telefon {
	int id;
	int RAM;
	char* producator;
	float pret;
	char serie;
};
struct Telefon initializare(int id, int RAM, const char* producator, float pret, char serie) {
	struct Telefon t;
	t.id = id;
	t.RAM = RAM;
	t.producator = (char*)malloc(strlen(producator) + 1);
	strcpy(t.producator, producator);
	t.pret = pret;
	t.serie = serie;
	return t;
}

void afisareTel(Telefon t) {
	printf("Id: %d | ", t.id);
	printf("RAM: %d | ", t.RAM);
	printf("Producator: %s | ", t.producator);
	printf("Pret: %.2f | ", t.pret);
	printf("Serie: %c\n", t.serie);
}

void afisareVector(struct Telefon* vector, int nrTel) {
	for (int i = 0; i < nrTel; i++) {
		afisareTel(vector[i]);
	}
}

struct Telefon* copiazaPrimeleNElemente(struct Telefon* vector, int nrElemente, int nrElementeCopiate) {
	Telefon *vectorNou=NULL;
	vectorNou = (Telefon*)malloc(sizeof(Telefon) * nrElementeCopiate);
	for (int i = 0; i < nrElementeCopiate; i++) { 
		vectorNou[i] = vector[i];
		vectorNou[i].producator = (char*)malloc(strlen(vector[i].producator) + 1);
		strcpy(vectorNou[i].producator, vector[i].producator);
	}
	return vectorNou;
}

void dezalocare(struct Telefon** vector, int* nrElemente) {
	for (int i = 0; i < (*nrElemente); i++) {
		if ((*vector)[i].producator) {
			free((*vector)[i].producator);
		}
	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

void copyTelScumpe(struct Telefon* vector, char nrElemente, float pretMin, struct Telefon** vectorNou, int* dim) {
	*dim = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].pret >= pretMin) {
			(*dim)++; 
		}
	}
	if (*vectorNou) {
		free(*vectorNou);
	}
	*vectorNou = (Telefon*)malloc(sizeof(Telefon) * (*dim)); 

	int k = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].pret >= pretMin) {
			//(*vectorNou)[k] = vector[i];
			/*(*vectorNou)[k].producator = (char*)malloc(strlen(vector[i].producator) + 1);
			strcpy((*vectorNou)[k].producator, vector[i].producator);
			k++;*/
			(*vectorNou)[k++] = initializare(vector[i].id, vector[i].RAM, vector[i].producator, vector[i].pret, vector[i].serie);
		}
	}

}

Telefon getPrimulTelDupaProd(Telefon* vector, int nrElemente, const char* producator) {
	Telefon t;
	t.producator = NULL;
	for (int i = 0; i < nrElemente; i++) {
		if (strcmp(vector[i].producator, producator) == 0) {
			t = vector[i];
			vector[i].producator = (char*)malloc(strlen(t.producator) + 1);
			strcpy(vector[i].producator, t.producator);
			return t;
		}
	}

	return t;
}
	


int main() {
	Telefon* vector = NULL;
	int nrTel = 3;
	vector = (Telefon*)malloc(sizeof(Telefon) * nrTel);
	vector[0] = initializare(1, 256, "Samsung", 2000, 'A');
	vector[1] = initializare(2, 256, "Apple", 5000, 'X');
	vector[2] = initializare(3, 256, "Huawei", 3000, 'S');

	printf("\n-----------------------AFISARE-----------------------\n");
	afisareVector(vector, nrTel);

	printf("\n-----------------------Copiere primele 2 elem-----------------------\n");
	Telefon* vectCopy = NULL;
	int nrTelCop = 2;
	vectCopy = copiazaPrimeleNElemente(vector, nrTel, nrTelCop);
	afisareVector(vectCopy, nrTelCop);

	printf("\n-----------------------Copy Telefoane Scumpe-----------------------\n");
	Telefon* telScumpe = NULL;
	int nrTelScumpe = 0;
	copyTelScumpe(vector, nrTel, 2500, &telScumpe, &nrTelScumpe);
	afisareVector(telScumpe, nrTelScumpe);

	printf("\n-----------------------Get Primul Tel By Prod-----------------------\n");
	Telefon tel = getPrimulTelDupaProd(vector, nrTel, "Apple");
	afisareTel(tel);
	if (tel.producator) {
		free(tel.producator);
	}
	tel.producator = NULL;

	printf("\n-----------------------DEZALOCARE-----------------------\n");
	dezalocare(&vectCopy, &nrTelCop);
	afisareVector(vectCopy, nrTelCop);

	dezalocare(&telScumpe, &nrTelScumpe);
	afisareVector(&telScumpe, &nrTelScumpe);

	dezalocare(&vector, &nrTel);
	afisareVector(vector, nrTel);


	_CrtDumpMemoryLeaks();

	return 0;
}