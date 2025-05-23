#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<crtdbg.h>

typedef struct Patiserie Patiserie;
typedef struct Nod Nod;
typedef struct LD Coada;

struct Patiserie {
	unsigned int id;
	char* nume;
	char* locatie;
	int nrProduse;
	char* specialitate;
	int nrAng;
	float profitLunar;
};

struct Nod {
	Patiserie info;
	Nod* next;
	Nod* prev;
};

struct LD {
	Nod* prim;
	Nod* ultim;
};

Patiserie initPatiserie(int id, const char* nume, const char* locatie, int nrProduse, const char* specialitate, int nrAng, float profitLunar) {
	Patiserie p;
	p.id = id;
	p.nume = malloc(strlen(nume) + 1);
	strcpy_s(p.nume, strlen(nume) + 1, nume);
	p.locatie = malloc(strlen(locatie) + 1);
	strcpy_s(p.locatie, strlen(locatie) + 1, locatie);
	p.nrProduse = nrProduse;
	p.specialitate = malloc(strlen(specialitate) + 1);
	strcpy_s(p.specialitate, strlen(specialitate) + 1, specialitate);
	p.nrAng = nrAng;
	p.profitLunar = profitLunar;
	return p;
}

void afisarePatiserie(Patiserie p) {
	printf("ID: %u | ", p.id);
	printf("Nume: %s | ", p.nume);
	printf("Locatie: %s | ", p.locatie);
	printf("Nr Prod: %d | ", p.nrProduse);
	printf("Specialitate: %s | ", p.specialitate);
	printf("Angajati: %d | ", p.nrAng);
	printf("Profit Lunar: %.2f\n", p.profitLunar);
}

//enqueue
void inserareCoadaFinal(Coada* coada, Patiserie p) {
	Nod* temp = (Nod*)malloc(sizeof(Nod));
	temp->info = p;
	temp->next = NULL;
	temp->prev = coada->ultim;
	if (coada->ultim) {
		coada->ultim->next = temp;
	}
	else {
		coada->prim = temp;
	}
	coada->ultim = temp;
}

Patiserie dequeue(Coada* coada) {
	Nod* aux = coada->prim;
	Patiserie p;
	p.id = -1;
	if (aux) {
		p = aux->info;
		coada->prim = aux->next;
		free(aux);
	}
	if (!coada->prim) {
		coada->ultim = NULL;
	}
	else {
		coada->prim->prev = NULL;
	}
	return p;
}

void citireDinFisier(Coada* coada, const char* fisier) {
	FILE* f = fopen(fisier, "r");
	if (!f) {
		return;
	}
	char buffer[100];
	while (fgets(buffer, sizeof(buffer), f)) {
		unsigned int id;
		char nume[50];
		char locatie[50];
		int nrProduse;
		char specialitate[50];
		int nrAng;
		float profitLunar;

		char* token = strtok(buffer, ",");
		id = atoi(token);

		token = strtok(NULL, ",");
		strcpy(nume, token);
		token = strtok(NULL, ",");
		strcpy(locatie, token);
		token = strtok(NULL, ",");
		nrProduse = atoi(token);
		token = strtok(NULL, ",");
		strcpy(specialitate, token);
		token = strtok(NULL, ",");
		nrAng = atoi(token);
		token = strtok(NULL, ",");
		profitLunar = atof(token);

		Patiserie p = initPatiserie(id, nume, locatie, nrProduse, specialitate, nrAng, profitLunar);
		inserareCoadaFinal(coada, p);
	}
	fclose(f);
}

void afisareCoada(Coada coada) {
	Nod* aux = coada.prim;
	while (aux) {
		afisarePatiserie(aux->info);
		aux = aux->next;
	}
}

void dezalocare(Coada* coada) {
	while (coada->prim) {
		Patiserie p = dequeue(coada);
		free(p.nume);
		free(p.locatie);
		free(p.specialitate);
	}
}

Patiserie getById(Coada* coada, unsigned int id) {
	Coada temp;
	temp.prim = NULL;
	temp.ultim = NULL;

	Patiserie patCautata;
	patCautata.id = -1;

	while (coada->prim) {
		Patiserie p = dequeue(coada);
		if (p.id == id && patCautata.id == -1) {
			patCautata = p;
		}
		inserareCoadaFinal(&temp, p);
	}
	while (temp.prim) {
		Patiserie p = dequeue(&temp);
		inserareCoadaFinal(coada, p);
	}
	return patCautata;
}

Patiserie getByName(Coada* coada, const char* nume) {
	Coada temp;
	temp.prim = NULL;
	temp.ultim = NULL;

	Patiserie patCautata;
	patCautata.id = -1;

	while (coada->prim) {
		Patiserie p = dequeue(coada);
		if (strcmp(p.nume, nume) == 0 && patCautata.id == -1) {
			patCautata = p;
		}
		inserareCoadaFinal(&temp, p);
	}
	while (temp.prim) {
		Patiserie p = dequeue(&temp);
		inserareCoadaFinal(coada, p);
	}
	return patCautata;
}

float profitTotal(Coada* coada) {
	Coada temp;
	temp.prim = NULL;
	temp.ultim = NULL;

	float sum = 0;
	while (coada->prim) {
		Patiserie p = dequeue(coada);
		sum += p.profitLunar;
		inserareCoadaFinal(&temp, p);
	}
	while (temp.prim) {
		Patiserie p = dequeue(&temp);
		inserareCoadaFinal(coada, p);
	}

	return sum;
}

Coada filtrareDupaLocatie(Coada* coada, const char* locatie) {
	Coada filtrata;
	filtrata.prim = NULL;
	filtrata.ultim = NULL;

	Coada temp;
	temp.prim = NULL;
	temp.ultim = NULL;

	/*Patiserie pat;
	pat.id = -1;*/
	while (coada->prim) {
		Patiserie p = dequeue(coada);
		if (strcmp(p.locatie, locatie) == 0) {
			inserareCoadaFinal(&filtrata, p);
		}
		inserareCoadaFinal(&temp, p);
	}
	while (temp.prim) {
		Patiserie p = dequeue(&temp);
		inserareCoadaFinal(coada, p);
	}
	return filtrata;
}

Coada filtrareDupaProfit(Coada* coada, float pragProfit) {
	Coada filtrata;
	filtrata.prim = NULL;
	filtrata.ultim = NULL;

	Coada temp;
	temp.prim = NULL;
	temp.ultim = NULL;
	while (coada->prim) {
		Patiserie p = dequeue(coada);
		if (p.profitLunar > pragProfit) {
			inserareCoadaFinal(&filtrata, p);
		}
		inserareCoadaFinal(&temp, p);
	}while (temp.prim) {
		Patiserie p = dequeue(&temp);
		inserareCoadaFinal(coada, p);
	}

	return filtrata;
}

void stergePatiseriiDupaLocatie(Coada* coada, const char* locatie) {
	Coada temp;
	temp.prim = NULL;
	temp.ultim = NULL;

	while (coada->prim) {
		Patiserie p = dequeue(coada);
		if (strcmp(p.locatie, locatie) != 0) {
			inserareCoadaFinal(&temp, p);
		}
		else {

		}
	}
	while (temp.prim) {
		Patiserie p = dequeue(&temp);
		inserareCoadaFinal(coada, p);
	}
}

int main() {
	Coada coada;
	coada.prim = NULL;
	coada.ultim = NULL;

	printf("\n--  Coada  --\n");
	citireDinFisier(&coada, "patiserie.txt");

	printf("\n-----------------------Get By Id----------------------\n");
	Patiserie patID = getById(&coada, 4);
	afisarePatiserie(patID);

	printf("\n-----------------------Get By Nume----------------------\n");
	Patiserie patNume = getByName(&coada, "Elif");
	afisarePatiserie(patNume);

	printf("\n--------------------------------------------\n");
	printf("Profit total: %.2f.", profitTotal(&coada));
	
	printf("\n-------------Coada Filtrata Dupa Locatie---------------\n");
	Coada filtrata = filtrareDupaLocatie(&coada, "Ploiesti");
	afisareCoada(filtrata);

	printf("\n-------------Coada Filtrata Prag Profit---------------\n");
	Coada filtrareProfit = filtrareDupaProfit(&coada, 3000);
	afisareCoada(filtrareProfit);

	printf("\n-------------Sterge Dupa Locatie---------------\n");
	printf("Coada inainte de stergere: \n");
	afisareCoada(coada);
	stergePatiseriiDupaLocatie(&coada, "Romana");
	printf("Coada dupa de stergere: \n");
	afisareCoada(coada);
	printf("\n--------------------Dezalocare--------------------\n");
	dezalocare(&coada);


	_CrtDumpMemoryLeaks();
	return 0;
}