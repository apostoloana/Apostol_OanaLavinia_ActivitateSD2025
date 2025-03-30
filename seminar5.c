#define _CRT_SECURE_NO_WARNINGS
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
typedef struct ListaDubla LD;

struct Nod {
	Masina info;
	Nod* prev;
	Nod* next;
};

struct ListaDubla {
	Nod* prim;
	Nod* ultim;
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
	printf("Serie: %c \n", masina.serie);
}

void afisareListaMasiniInceput(LD* lista) {
	Nod* aux = lista->prim;
	while (aux != NULL) {
		afisareMasina(aux->info);
		aux = aux->next;
	}
}

void afisareListaMasiniFinal(LD* lista) {
	Nod* aux = lista->ultim;
	while (aux != NULL) {
		afisareMasina(aux->info);
		aux = aux->prev;
	}
}

void addListaSfarsit(LD* lista, Masina masinaNoua) {
	Nod* nod = malloc(sizeof(Nod));
	nod->info = masinaNoua;
	nod->next = NULL;
	nod->prev = lista->ultim;
	if (lista->ultim != NULL) {
		lista->ultim->next = nod;
	}
	else {
		lista->prim = nod;
	}
	lista->ultim = nod;

}

void addListaInceput(LD* lista, Masina masinaNoua) {
	Nod* nod = malloc(sizeof(Nod));
	nod->info = masinaNoua;
	nod->prev = NULL;
	nod->next = lista->prim;
	if (lista->prim != NULL) {
		lista->prim->prev = nod;
	}
	else {
		lista->ultim = nod;
	}
	lista->prim = nod;
}

LD citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	LD lista;
	lista.prim = NULL;
	lista.ultim = NULL;
	while (!feof(f)) {
		Masina m;
		m = citireMasinaDinFisier(f);
		//addListaSfarsit(&lista, m);
		addListaInceput(&lista, m);
	}
	fclose(f);
	return lista;
}

void dezalocareLDMasini(LD* lista) {
	Nod* aux = lista->prim;
	while (aux) {
		free(aux->info.model);
		free(aux->info.numeSofer);
		Nod* temp = aux->next;
		free(aux);
		aux = temp;
	}
	lista->ultim = NULL;
}

float calculeazaPretMediu(LD lista) {
	int suma = 0;
	int contor = 0;
	Nod* nod = lista.prim;
	while (nod != NULL) {
		suma += nod->info.pret;
		contor++;
		nod = nod->next;
	}
	return suma / 2;
}

void stergeMasinaDupaID(LD* lista, int id) {
	Nod* aux = lista->prim;
	while (aux && aux->info.id != id) {
		aux = aux->next;
	}
	if (aux) {
		if (aux->prev) {
			aux->prev->next = aux->next;
			if (aux->next) {
				aux->next->prev = aux->prev;
			}
			else {
				lista->ultim = aux->prev;
			}
		}
		else {
			lista->prim = aux->next;
			if (aux->next) {
				aux->next->prev = NULL;
			}
			else {
				lista->ultim = aux->prev;
			}
		}
		free(aux->info.model);
		free(aux->info.numeSofer);
		free(aux);
	}
}

char* getNumeSoferMasinaScumpa(LD lista) {
	if (lista.prim) {
		Nod* max = lista.prim;
		Nod* aux = lista.prim->next;
	
		while (aux) {
			if (aux->info.pret > max->info.pret) {
				max = aux;
			}
			aux = aux->next;
		}
		char* nume = (char*)malloc(strlen(max->info.numeSofer) + 1);
		strcpy_s(nume, strlen(max->info.numeSofer)+1, max->info.numeSofer);
		return nume;
	}
	else {
		return NULL;
	}
}

int main() {
	LD lista;

	lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasiniInceput(&lista);
	printf("\n---------------------------Afisare de la final---------------------\n");
	afisareListaMasiniFinal(&lista);

	printf("\n---------------------------Calculare pret mediu ---------------------\n");
	float pretMediu = 0;
	pretMediu = calculeazaPretMediu(lista);
	printf("Pretul mediu este: %.2f", pretMediu);
	printf("\n--------------------------- Sterge masina dupa ID ---------------------\n");
	stergeMasinaDupaID(&lista, 6);
	afisareListaMasiniFinal(&lista);

	printf("\n--------------------------- Nume sofer masina scumpa ---------------------\n");
	char* nume = getNumeSoferMasinaScumpa(lista);
	printf("Numele persoanei cu cea mai scumpa masina: %s.\n", nume);

	free(nume);

	dezalocareLDMasini(&lista);

	return 0;
}

