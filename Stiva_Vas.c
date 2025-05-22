#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<crtdbg.h>

typedef struct Vas Vas;
typedef struct Nod Nod;

struct Vas {
	int id;
	char* nume;
	int diametru;
	int capacitate;
};

struct Nod {
	Vas info;
	Nod* next;
};

Vas initVas(int id, const char* nume, int diametru, int capacitate) {
	Vas v;
	v.id = id;
	v.nume = malloc(strlen(nume) + 1);
	strcpy_s(v.nume, strlen(nume) + 1, nume);
	v.diametru = diametru;
	v.capacitate = capacitate;
	return v;
}

void afisareVas(Vas v) {
	printf("Id: %d | ", v.id);
	printf("Nume: %s | ", v.nume);
	printf("Diametru: %d | ", v.diametru);
	printf("Capacitate: %d.\n", v.capacitate);
}

void push(Nod** stiva, Vas v) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = v;
	nod->next = *stiva;
	*stiva = nod;
}

Vas pop(Nod** stiva) {
	Vas v;
	v.id = -1;
	if(*stiva) {
		Nod* aux = *stiva;
		v = aux->info;
		*stiva = aux->next;
		free(aux);
	}
	return v;
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
		int id;
		char nume[50];
		int diametru;
		int capacitate;

		char* token = strtok(buffer, ",");
		id = atoi(token);

		token = strtok(NULL, ",");
		strcpy(nume, token);

		token = strtok(NULL, ",");
		diametru = atoi(token);

		token = strtok(NULL, ",");
		capacitate = atoi(token);

		Vas v = initVas(id, nume, diametru, capacitate);
		push(stiva, v);
	}
	fclose(f);
}

void afisareStiva(Nod* stiva) {
	printf("Stiva contine:\n");
	while (stiva) {
		afisareVas(stiva->info);
		stiva = stiva->next;
	}
}

void dezalocare(Nod** stiva) {
	while (*stiva) {
		Vas v = pop(stiva);
		free(v.nume);
	}
}

Vas getById(Nod** stiva, int id) {
	Nod* temp = NULL;
	Vas vasCautat;
	vasCautat.id = -1;
	while (*stiva) {
		Vas vas = pop(stiva);
		if (vas.id == id && vasCautat.id == -1) {
			vasCautat = vas;
		}
		push(&temp, vas);
	}
	while (temp) {
		Vas vas = pop(&temp);
		push(stiva, vas);
	}
	return vasCautat;
}

Vas getByName(Nod** stiva, const char* nume) {
	Nod* temp = NULL;
	Vas vasCautat;
	vasCautat.id = -1;

	while (*stiva) {
		Vas vas = pop(stiva);
		if (strcmp(vas.nume, nume) == 0 && vasCautat.id == -1) {
			vasCautat = vas;
		}
		push(&temp, vas);
	}

	while (temp) {
		Vas vas = pop(&temp);
		push(stiva, vas);
	}
	return vasCautat;
}

int nrTotalVase(Nod* stiva) {
	int sum = 0;
	while (stiva) {
		sum++;
		stiva = stiva->next;
	}
	return sum;
}

Vas modificaCapacitate(Nod** stiva, int id, int capacitateNoua) {
	Vas vasCautat;
	vasCautat.id = -1;
	Nod* temp = NULL;
	while (*stiva) {
		Vas vas = pop(stiva);
		if (vas.id == id && vasCautat.id == -1) {
			vas.capacitate = capacitateNoua;
			vasCautat = vas;
		}
		push(&temp, vas);
	}
	while (temp) {
		Vas vas = pop(&temp);
		push(stiva, vas);
	}

	return vasCautat;
}

Vas getVasMinDiametru(Nod** stiva) {
	Vas vasCautat;
	vasCautat.id = -1;

	Nod* temp = NULL;
	vasCautat = pop(stiva);
	int minDiam = vasCautat.diametru;
	push(&temp, vasCautat);

	while (*stiva) {
		Vas vas = pop(stiva);
		if (vas.diametru < minDiam) {
			minDiam = vas.diametru;
			vasCautat = vas;
		}
		push(&temp, vas);
	}
	while (temp) {
		Vas vas = pop(&temp);
		push(stiva, vas);
	}
	return vasCautat;
}

int main() {
	Nod* stiva = NULL;
	citireDinFisier(&stiva, "vas.txt");
	Vas v1 = pop(&stiva);
	afisareVas(v1);
	push(&stiva, v1);
	//free(v1.nume);

	printf("\n--------------Get By Id---------------\n");
	Vas vasId = getById(&stiva, 1);
	afisareVas(vasId);

	printf("\n--------------Get By Nume---------------\n");
	Vas vasNume = getByName(&stiva, "Tigaie");
	afisareVas(vasNume);

	printf("\n--------------Nr total vase---------------\n");
	printf("Nr total vase: %d", nrTotalVase(stiva));

	printf("\n--------------Modifica capacitate---------------\n");
	Vas capacitate = modificaCapacitate(&stiva, 3, 10);
	afisareVas(capacitate);

	printf("\n--------------Get Vas cu Diametrul Minim---------------\n");
	Vas minDiam = getVasMinDiametru(&stiva);
	afisareVas(minDiam);

	printf("\n--------------Afisare Stiva---------------\n");
	afisareStiva(stiva);

	printf("\n--------------Dezalocare---------------\n");
	dezalocare(&stiva);

	_CrtDumpMemoryLeaks();
	return 0;
}