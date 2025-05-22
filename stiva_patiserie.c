#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<crtdbg.h>

typedef struct Patiserie Patiserie;
typedef struct Nod Nod;

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
	printf("Angajati: %d | ", p.nrAng);
	printf("Profit Lunar: %.2f.\n", p.profitLunar);
}

void pushPatiserie(Nod** stiva, Patiserie p) {
	Nod* aux = (Nod*)malloc(sizeof(Nod));
	aux->info = p;
	aux->next = *stiva;
	*stiva = aux;
}

Patiserie popPatiserie(Nod** stiva) {
	Patiserie p;
	p.id = -1;

	if (*stiva) {
		Nod* temp = *stiva;
		p = temp->info;
		*stiva = temp->next;

		free(temp);
	}
	return p;
}

int isEmpty(Nod* stiva) {
	return stiva == NULL;
}

void citireDinFisier(Nod** stiva, const char* fisier) {
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
		pushPatiserie(stiva, p);
	}
	fclose(f);
}

void afisareStiva(Nod* stiva) {
	while (stiva) {
		afisarePatiserie(stiva->info);
		stiva = stiva->next;
	}
}

void dezalocare(Nod** stiva) {
	while (*stiva) {
		Patiserie p = popPatiserie(stiva);
		free(p.nume);
		free(p.locatie);
		free(p.specialitate);
	}
}

Patiserie getPatiserieProfitLunarMax(Nod** stiva) {
	Patiserie patCautata;
	patCautata.id = -1;

	Nod* temp = NULL;
	patCautata = popPatiserie(stiva);
	float maxProfit = patCautata.profitLunar;
	pushPatiserie(&temp, patCautata);

	while (*stiva) {
		Patiserie p = popPatiserie(stiva);
		if (p.profitLunar > maxProfit) {
			maxProfit = p.profitLunar;
			patCautata = p;
		}
		pushPatiserie(&temp, p);
	}
	while (temp) {
		Patiserie p = popPatiserie(&temp);
		pushPatiserie(stiva, p);
	}

	return patCautata;
}

Patiserie getPatiserieByName(Nod** stiva, const char* nume) {
	Patiserie patCautata;
	patCautata.id = -1;
	Nod* temp = NULL;
	while (*stiva) {
		Patiserie patiserie = popPatiserie(stiva);
		if (strcmp(patiserie.nume, nume) == 0 && patCautata.id == -1) {
			patCautata = patiserie;
		}
		pushPatiserie(&temp, patiserie);
	}
	while (temp) {
		Patiserie patiserie = popPatiserie(&temp);
		pushPatiserie(stiva, patiserie);
	}
	return patCautata;
}

Patiserie modificaNrProduse(Nod** stiva, int id, int nrNouProd) {
	Patiserie patCautata;
	patCautata.id = -1;
	Nod* temp = NULL;
	while (*stiva) {
		Patiserie p = popPatiserie(stiva);
		if (p.id == id && patCautata.id == -1) {
			p.nrProduse = nrNouProd;
			patCautata = p;
		}
		pushPatiserie(&temp, p);
	}
	while (temp) {
		Patiserie p = popPatiserie(&temp);
		pushPatiserie(stiva, p);
	}
	return patCautata;
}

int nrPatiseriiPragAngajati(Nod** stiva, int prag) {
	Nod* temp = NULL;
	int count = 0;
	while (*stiva) {
		Patiserie p = popPatiserie(stiva);
		if (p.nrAng > prag) {
			count++;
		}
		pushPatiserie(&temp, p);
	}
	while (temp) {
		Patiserie p = popPatiserie(&temp);
		pushPatiserie(stiva, p);
	}
	return count;
}

float profitMediuLunarTotal(Nod** stiva) {
	Nod* temp = NULL;
	float sum = 0;
	while (*stiva) {
		Patiserie p = popPatiserie(stiva);
		sum += p.profitLunar;
		pushPatiserie(&temp, p);
	}
	while (temp) {
		Patiserie p = popPatiserie(&temp);
		pushPatiserie(stiva, p);
	}
	return sum / 2;
}

int main() {
	Nod* stiva = NULL;
	citireDinFisier(&stiva, "patiserie.txt");

	printf("\n----------------Patiseria cu Profitul Maxim--------------------\n");
	Patiserie profitMax = getPatiserieProfitLunarMax(&stiva);
	afisarePatiserie(profitMax);

	printf("\n----------------Get By Name--------------------\n");
	Patiserie nume = getPatiserieByName(&stiva, "Mihai");
	afisarePatiserie(nume);

	printf("\n----------------Modifica nr Produse--------------------\n");
	Patiserie modificaProd = modificaNrProduse(&stiva, 2, 100);
	afisarePatiserie(modificaProd);

	printf("\n----------------------------------------------\n");
	printf("Exista %d patiserii cu nr de angajati mai mare de 10.", nrPatiseriiPragAngajati(&stiva, 4));

	printf("\n----------------------------------------------\n");
	printf("Profitul mediu lunar total: %.2f.", profitMediuLunarTotal(&stiva));

	printf("\n----------------Afisare--------------------\n");
	afisareStiva(stiva);

	printf("\n----------------Dezalocare--------------------\n");

	_CrtDumpMemoryLeaks();
	return 0;
}