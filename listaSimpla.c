#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

typedef struct Masina Masina;
typedef struct Nod Nod;

struct Masina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};

struct Nod {
	Masina info;
	Nod* next;
};

Masina initMasina(int id, int nrUsi, float pret, const char* model, const char* numeSofer, unsigned char serie) {
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

Masina afisareMasina(Masina m) {
	printf("Id masina: %d | ", m.id);
	printf("Nr usi: %d | ", m.nrUsi);
	printf("Pret: %.2f | ", m.pret);
	printf("Model: %s | ", m.model);
	printf("Sofer: %s | ", m.numeSofer);
	printf("Serie: %c \n", m.serie);
}

void inserareLSInceput(Nod** cap, Masina m) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = m;
	nod->next = (*cap);
	*cap = nod;
}

void inserareLaFinal(Nod** cap, Masina m) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = m;
	nod->next = NULL;
	if (*cap) {
		Nod* temp = (*cap);
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nod;
	}else {
		*cap = nod;
	}
}

void afisareLS(Nod* cap) {
	while (cap) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void citireDinFisier(Nod** lista, const char* fisier) {
	FILE* f = fopen(fisier, "r");
	if (!f) {
		return;
	}
	char buffer[50];
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
		inserareLaFinal(lista, m);
	}
	fclose(f);
}

void dezalocare(Nod** cap) {
	while (*cap) {
		Nod* aux = *cap;
		(*cap) = aux->next;
		free(aux->info.model);
		free(aux->info.numeSofer);
		free(aux);
	}
}

float pretMediu(Nod* cap) {
	float suma = 0;
	int contor = 0;
	Nod* aux = cap;
	while (aux) {
		suma += aux->info.pret;
		contor++;
		aux = aux->next;
	}
	if (contor > 0) {
		return suma / contor;
	}
}

void stergeMasiniDupaSerie(Nod** cap, char serie) {
	while ((*cap) && (*cap)->info.serie == serie) {
		Nod* nod = (*cap);
		(*cap) = nod->next;
		free(nod->info.model);
		free(nod->info.numeSofer);
		free(nod);
	}
	if ((*cap) != NULL) {
		Nod* temp = (*cap);
		while (temp) {
			while (temp->next && temp->next->info.serie != serie) {
				temp = temp->next;
			}
			if (temp->next) {
				Nod* p = temp->next;
				temp->next = p->next;
				free(p->info.model);
				free(p->info.numeSofer);
				free(p);
			}
			else {
				temp = NULL;
			}

		}
	}
}

void stergeMasinaSofer(Nod** cap, const char* numeSofer) {
	while ((*cap) && (strcmp((*cap)->info.numeSofer, numeSofer)==0)) {
		Nod* nod = (*cap);
		(*cap) = nod->next;
		free(nod->info.model);
		free(nod->info.numeSofer);
		free(nod);
	}
	if ((*cap) != NULL) {
		Nod* temp = (*cap);
		while (temp) {
			while (temp->next && (strcmp(temp->next->info.numeSofer, numeSofer) != 0)) {
				temp = temp->next;
			}
			if (temp->next) {
				Nod* p = temp->next;
				temp->next = p->next;
				free(p->info.model);
				free(p->info.numeSofer);
				free(p);
			}
			else {
				temp = NULL;
			}
		}
	}
}

float pretMasiniSofer(Nod* cap, const char* sofer) {
	float suma = 0;
	while (cap) {
		if (strcmp(cap->info.numeSofer, sofer) == 0) {
			suma += cap->info.pret;
		}
		cap = cap->next;
	}
	return suma;
}

Masina* conversieLaVector(Nod* cap, int* dim) {
	int count = 0;
	Nod* temp = cap;
	while (temp) {
		count++;
		temp = temp->next;
	}
	
	Masina* vector = (Masina*)malloc(sizeof(Masina) * count);
	if (!vector) {
		*dim = 0;
		return NULL;
	}

	temp = cap;
	for (int i = 0; i < count; i++) {
		vector[i].id = temp->info.id;
		vector[i].nrUsi = temp->info.nrUsi;
		vector[i].pret = temp->info.pret;
		vector[i].model = (char*)malloc(strlen(temp->info.model)+1);
		strcpy(vector[i].model, temp->info.model);
		vector[i].numeSofer = (char*)malloc(strlen(temp->info.numeSofer)+1);
		strcpy(vector[i].numeSofer, temp->info.numeSofer);
		vector[i].serie = temp->info.serie;

		temp = temp->next;
	}
	*dim = count;
	return vector;


}

void main() {
	Nod* lista = NULL;

	citireDinFisier(&lista, "masini.txt");
	afisareLS(lista);

	printf("\n--------------------------------EX 1-----------------------------\n");
	printf("Pretul mediu este: %.2f.", pretMediu(lista));

	printf("\n--------------------------------EX 2-----------------------------\n");
	stergeMasiniDupaSerie(&lista, 'A');
	afisareLS(lista);

	printf("\n--------------------------------EX 3-----------------------------\n");
	stergeMasinaSofer(&lista, "Vasilescu");
	afisareLS(lista);

	printf("\n--------------------------------EX 4-----------------------------\n");
	printf("Pretul masinilor lui Gigel: %.2f.", pretMasiniSofer(lista, "Gigel"));

	printf("\n--------------------------------EX 5-----------------------------\n");
	int dim = 0;
	Masina* vector = conversieLaVector(lista, &dim);
	for (int i = 0; i < dim; i++) {
		afisareMasina(vector[i]);
	}
	for (int i = 0; i < dim; i++) {
		free(vector[i].model);
		free(vector[i].numeSofer);
	}
	free(vector);


	printf("\n--------------------------------Dezalocare-----------------------------\n");
	dezalocare(&lista);
}