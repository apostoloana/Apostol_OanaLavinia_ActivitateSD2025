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

typedef struct Lista Lista;
struct Lista {
	Masina info;
	Lista* next;
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
	printf("Serie: %c\n", masina.serie);
}

void afisareListaMasini(Lista* cap) {
	while (cap != NULL) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaMasinaInLista(Lista** cap, Masina masinaNoua) {
	Lista* aux = malloc(sizeof(Lista));
	aux->info = masinaNoua;
	aux->next = NULL;
	if ((*cap) != NULL) {
		Lista* temp = (*cap);
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = aux;
	}
	else {
		(*cap) = aux;
	}
}

void adaugaLaInceputInLista(Lista** cap, Masina masinaNoua) {
    
}

Lista* citireListaMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Lista* lista = NULL;
	while (!feof(f)) {
		Masina m;
		m = citireMasinaDinFisier(f);
		adaugaMasinaInLista(&lista, m);
	}
	fclose(f);
	return lista;
}

void dezalocareListaMasini(Lista** cap) {
	while ((*cap)) {
		Lista* aux = *cap; 
		*cap = (*cap)->next; 
		free(aux->info.model);
		free(aux->info.numeSofer);
		free(aux);
	}
}

float calculeazaPretMediu(Lista* lista) {
	int contor = 0;
	float suma = 0;
	while (lista) {
		suma += lista->info.pret;
		contor++;
		lista = lista->next;
	}
	if (contor == 0) {
		return 0;
	}
	else {
		return suma / contor;

	}
}

void stergeMasiniDinSeria(/*lista masini*/ char serieCautata) {
	//sterge toate masinile din lista care au seria primita ca parametru.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
}

float calculeazaPretulMasinilorUnuiSofer(Lista* lista, const char* numeSofer) {
	float suma = 0;
	while (lista) {
		if (strcmp(lista->info.numeSofer, numeSofer) == 0) {
			suma += lista->info.pret;
		}
		lista = lista->next;
	}
	return suma;
}

int nrUsiMasinaScumpa(Lista* lista) {
	if (lista) {
		float pretMax = lista->info.pret;
		int nrUsi = lista->info.nrUsi;
		lista = lista->next;
		while (lista) {
			if (lista->info.pret > pretMax) {
				nrUsi = lista->info.nrUsi;
				pretMax = lista->info.pret;
			}
			lista = lista->next;
		}
		return nrUsi;
	}
}

int main() {
	Lista* lista;
	lista = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);

	printf("\n--------------------------Pret Mediu----------------------------\n");
	float medie = calculeazaPretMediu(lista);
	printf("Medie: %.2f.", medie);

	printf("\n--------------------------Pret Masina Sofer----------------------------\n");
	float suma = calculeazaPretulMasinilorUnuiSofer(lista, "Ionescu");
	printf("Pretul masinilor lui Ionescu: %.2f.", suma);

	printf("\n--------------------------Nr Usi pt cea mai scumpa masina----------------------------\n");
	int nrUsi = nrUsiMasinaScumpa(lista);
	printf("Nr usi: %d", nrUsi);



	dezalocareListaMasini(&lista);

	return 0;
}



