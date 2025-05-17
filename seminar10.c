#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crtdbg.h>

typedef struct {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
} Masina;

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

typedef struct Nod {
	Masina info;
	struct Nod* next;
} Nod;

void pushStack(Nod** stiva, Masina masina) {
	Nod* nod = malloc(sizeof(Nod));
	nod->info = masina;
	nod->next = *stiva;
	*stiva = nod;
}

Masina popStack(Nod** stiva) {
	Masina m;
	m.id = -1;
	if (*stiva) {
		Nod* aux = *stiva;
		m = aux->info;
		*stiva = aux->next;
		free(aux);
	}
	return m;
}

unsigned char emptyStack(Nod* stiva) {
	return stiva == NULL;
}

void* citireStackMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* stiva = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		pushStack(&stiva, m);
	}
	fclose(f);
	return stiva;
}

void dezalocareStivaDeMasini(Nod** stiva) {
	while (*stiva) {
		Masina m = popStack(stiva);
		free(m.model);
		free(m.numeSofer);
	}
}

int size(Nod* stiva) {
	int count = 0;
	while (stiva) {
		count++;
		stiva = stiva->next;
	}
	return count;
}

typedef struct NodLD {
	Masina info;
	struct NodLD* next;
	struct NodLD* prev;
} NodLD;

typedef struct {
	NodLD* prim;
	NodLD* ultim;
} Coada;

void enqueue(Coada* coada, Masina masina) {
	NodLD* nod = malloc(sizeof(NodLD));
	nod->info = masina;
	nod->prev = coada->ultim;
	nod->next = NULL;
	if (coada->ultim) {
		coada->ultim->next = nod;
	}
	else {
		coada->prim = nod;
	}
	coada->ultim = nod;
}

Masina dequeue(Coada* coada) {
	Masina m;
	m.id = -1;
	if (coada->prim) {
		m = coada->prim->info;
		NodLD* temp = coada->prim;
		coada->prim = coada->prim->next;
		free(temp);
		if (!coada->prim) {
			coada->ultim = NULL;
		}
		else {
			coada->prim->prev = NULL;
		}
	}
	return m;
}

Coada citireCoadaDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Coada coada;
	coada.prim = NULL;
	coada.ultim = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		enqueue(&coada, m);
	}
	fclose(f);
	return coada;
}

void dezalocareCoadaDeMasini(Coada* coada) {
	while (coada->prim) {
		Masina m = dequeue(coada);
		free(m.model);
		free(m.numeSofer);
	}
}

Masina getMasinaByIDStiva(Nod** stiva, int id) {
	Nod* temp = NULL;
	Masina masinaGasita;
	masinaGasita.id = -1;
	while (*stiva) {
		Masina m = popStack(stiva);
		if (m.id == id && masinaGasita.id == -1) {
			masinaGasita = m;
		}
		pushStack(&temp, m);
	}
	while (temp) {
		Masina m = popStack(&temp);
		pushStack(stiva, m);
	}
	return masinaGasita;
}

Masina getMasinaByIDCoada(Coada* coada, int id) {
	Coada temp;
	temp.prim = NULL;
	temp.ultim = NULL;
	Masina masinaCautata;
	masinaCautata.id = -1;
	while (coada->prim) {
		Masina m = dequeue(coada);
		if (m.id == id) {
			masinaCautata = m;
		}
		enqueue(&temp, m);
	}
	while (temp.prim) {
		Masina m = dequeue(&temp);
		enqueue(coada, m);
	}
	return masinaCautata;
}

float calculeazaPretTotalStiva(Nod** stiva) {
	float sum = 0;
	Nod* aux = NULL;
	while (*stiva) {
		Masina m = popStack(stiva);
		sum += m.pret;
		pushStack(&aux, m);
	}
	while (aux) {
		Masina m = popStack(&aux);
		pushStack(stiva, m);
	}
	return sum;
}

float calculeazaPretTotalCoada(Coada* coada) {
	float sum = 0;
	Coada aux;
	aux.prim = NULL;
	aux.ultim = NULL;
	while (coada->prim) {
		Masina m = dequeue(coada);
		sum += m.pret;
		enqueue(&aux, m);
	}
	while (aux.prim) {
		Masina m = dequeue(&aux);
		enqueue(coada, m);
	}
	return sum;
}

int main() {
	printf("\n-----------------------Stiva----------------------\n");

	Nod* stiva = NULL;
	stiva = citireStackMasiniDinFisier("masini.txt");
	Masina m = popStack(&stiva);
	afisareMasina(m);

	printf("\n----Pret Total----\n");
	float suma = calculeazaPretTotalStiva(&stiva);
	printf("Pretul total este: %.2f\n", suma);

	printf("\n----Get By ID----\n");
	Masina masinaID = getMasinaByIDStiva(&stiva, 5);
	afisareMasina(masinaID);

	printf("\n-----------------------Coada----------------------\n");
	Coada coada;
	coada = citireCoadaDeMasiniDinFisier("masini.txt");
	Masina masina = dequeue(&coada);
	afisareMasina(masina);

	printf("\n----Pret Total----\n");
	float sum = calculeazaPretTotalCoada(&coada);
	printf("Pretul total este: %.2f\n", sum);

	printf("\n----Get By ID----\n");
	Masina masinaIDCoada = getMasinaByIDCoada(&coada, 5);
	afisareMasina(masinaIDCoada);

	printf("\n-----------------------Dezalocare Stiva----------------------\n");
	dezalocareStivaDeMasini(&stiva);

	printf("\n-----------------------Dezalocare Coada----------------------\n");
	dezalocareCoadaDeMasini(&coada);

	_CrtDumpMemoryLeaks();
	return 0;
}
