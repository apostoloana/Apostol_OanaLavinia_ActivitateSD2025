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
typedef struct NodLS NodLS;
typedef struct ListaDubla LD;

struct Nod {
	Masina info;
	Nod* prev;
	Nod* next;
};

struct NodLS {
	Masina info;
	NodLS* next;
};

struct ListaDubla {
	Masina info;
	Nod* prim;
	Nod* ultim;
};

Masina initMasina(int id, int nrUsi, float pret, const char* model, const char* numeSofer, unsigned char serie) {
	Masina m;
	m.id = id;
	m.nrUsi = nrUsi;
	m.pret = pret;
	m.model = (char*)malloc(strlen(model) + 1);
	strcpy(m.model, model);
	m.numeSofer = (char*)malloc(strlen(numeSofer) + 1);
	strcpy(m.numeSofer, numeSofer);
	m.serie = serie;
	return m;
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

void afisareListaSimpla(NodLS* cap) {
	NodLS* temp = cap;
	while (temp) {
		afisareMasina(temp->info);
		temp = temp->next;
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

void inserareLSFinal(NodLS** cap, Masina m) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->info = initMasina(m.id, m.nrUsi, m.pret, m.model, m.numeSofer, m.serie);
	nou->next = NULL;

	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		NodLS* temp = *cap;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void inserareLSInceput(NodLS** cap, Masina m) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->info = initMasina(m.id, m.nrUsi, m.pret, m.model, m.numeSofer, m.serie);
	nou->next = *cap;
	*cap = nou;
}



void citireDinFisier(LD* lista, const char* numeFisier) {
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
		addListaSfarsit(lista, m);
	}
	fclose(f);
}

void dezalocare(LD* lista) {
	Nod* aux = lista->prim;
	while (aux) {
		Nod* p = aux;
		aux = aux->next;
		free(p->info.model);
		free(p->info.numeSofer);
		free(p);
	}
	lista->prim = NULL;
	lista->ultim = NULL;
}

void dezalocareLS(NodLS** cap) {
	while (*cap) {
		NodLS* temp = *cap;
		*cap = (*cap)->next;
		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);
	}
}


float calculeazaPretMediu(LD lista) {
	float suma = 0;
	int count = 0;
	Nod* aux = lista.prim;
	while (aux) {
		suma += aux->info.pret;
		count++;
		aux = aux->next;
	}
	if (count > 0) {
		return suma / count;
	}
}

//sterge nod
void stergeNod(Nod* nod, LD* lista) {
	if (nod->prev) {
		nod->prev->next = nod->next;
	}
	else {
		lista->prim = nod->next;
	}
	if (nod->next) {
		nod->next->prev = nod->prev;
	}
	else {
		lista->ultim = nod->prev;
	}
	free(nod->info.model);
	free(nod->info.numeSofer);
	free(nod);
	nod = NULL;
}

void stergeMasinaDupaID(LD* lista, int id) {
	Nod* aux = lista->prim;
	if (!aux) {
		return;
	}
	while (aux) {
		if (aux->info.id == id) {
			Nod* sters = aux;
			aux = aux->next;
			stergeNod(sters, lista);
			return;
		}
		else {
			aux = aux->next;
		}
	}
}

void stergeNodPoz(LD* lista, int poz) {
	Nod* nod = lista->prim;
	int index = 0;
	while (nod && index < poz) {
		nod = nod->next;
		index++;
	}
	if (poz > index) {
		printf("Pozitia %d nu exista.\n", poz);
	}
	if (nod) {
		stergeNod(nod, lista);
	}
}

char* getNumeSoferMasinaScumpa(LD lista) {
	if (lista.prim) {
		Nod* max = lista.prim;
		Nod* temp = lista.prim->next;
		while (temp) {
			if (temp->info.pret > max->info.pret) {
				max = temp;
			}
			temp = temp->next;
		}
		char* nume = (char*)malloc(strlen(max->info.numeSofer) + 1);
		strcpy(nume, max->info.numeSofer);
		return nume;
	}
	else {
		return NULL;
	}
}

//functie care sa insereze elem crescator dupa un criteriu
void inserareCrescatorDupaPret(LD* lista, Masina masina) {
	Nod* nod = malloc(sizeof(Nod));
	nod->info = masina;
	nod->next = NULL;
	nod->prev = NULL;
	Nod* aux = lista->prim;
	while (aux != NULL && aux->info.pret);
}

void stergeMasinaPretMIC(LD* lista) {
	if (lista->prim) {
		Nod* aux = lista->prim;
		Nod* min = lista->prim->next;
		while (aux) {
			if (aux->info.pret < min->info.pret) {
				min = aux;
			}
			aux = aux->next;
		}
		if (min) {
			stergeNod(min, lista);
		}
	}
	else {
		return NULL;
	}
}

Masina* conversieLaVector(LD* lista, int* dim) {
	Nod* temp = lista->prim;
	int count = 0;
	while (temp) {
		count++;
		temp = temp->next;
	}
	Masina* vector = (Masina*)malloc(sizeof(Masina) * count);
	if (!vector) {
		*dim = 0;
		return;
	}
	temp = lista->prim;
	for (int i = 0; i < count; i++) {
		vector[i].id = temp->info.id;
		vector[i].nrUsi = temp->info.nrUsi;
		vector[i].pret = temp->info.pret;
		vector[i].model = (char*)malloc(strlen(temp->info.model) + 1);
		strcpy(vector[i].model, temp->info.model);
		vector[i].numeSofer = (char*)malloc(strlen(temp->info.numeSofer) + 1);
		strcpy(vector[i].numeSofer, temp->info.numeSofer);
		vector[i].serie = temp->info.serie;

		temp = temp->next;
	}
	*dim = count;
	return vector;
}

NodLS* salvareInLS(LD lista, float prag) {
	NodLS* cap = NULL;
	Nod* temp = lista.prim;
	while (temp) {
		if (temp->info.pret > prag) {
			Masina copie = initMasina(temp->info.id, temp->info.nrUsi, temp->info.pret, temp->info.model, temp->info.numeSofer, temp->info.serie);
			inserareLSInceput(&cap, copie);
		}
		temp = temp->next;
	}
	return cap;
}



int main() {
	LD lista;
	lista.prim = NULL;
	lista.ultim = NULL;

	citireDinFisier(&lista, "masini.txt");
	afisareListaMasiniInceput(&lista);
	printf("\n---------------------------Afisare de la final---------------------\n");
	afisareListaMasiniFinal(&lista);
	printf("\n---------------------------Calculare pret mediu ---------------------\n");
	printf("Pretul mediu este: %.2f", calculeazaPretMediu(lista));
	printf("\n--------------------------- Sterge masina dupa ID ---------------------\n");
	stergeMasinaDupaID(&lista, 7);
	afisareListaMasiniFinal(&lista);
	printf("\n--------------------------- Nume sofer masina scumpa ---------------------\n");
	char* nume = getNumeSoferMasinaScumpa(lista);
	printf("Soferul cu cea mai scumpa masina este: %s", nume);
	free(nume);

	printf("\n--------------------------- Sterge Nod dupa Poz ---------------------\n");
	stergeNodPoz(&lista, 8);
	afisareListaMasiniFinal(&lista);

	printf("\n--------------------------- Sterge Masina Pret Mic ---------------------\n");
	stergeMasinaPretMIC(&lista);
	afisareListaMasiniFinal(&lista);

	int dim = 0;
	Masina* vector = conversieLaVector(&lista, &dim);
	printf("\n--------------------------- Conversie la Vector ---------------------\n");
	for (int i = 0; i < dim; i++) {
		afisareMasina(vector[i]);
	}
	for (int i = 0; i < dim; i++) {
		free(vector[i].model);
		free(vector[i].numeSofer);
	}
	free(vector);

	printf("\n--------------------------- Salvare in LS ---------------------\n");
	NodLS* listaSimpla;
	listaSimpla = salvareInLS(lista, 4000.00F);
	afisareListaSimpla(listaSimpla);
	dezalocareLS(&listaSimpla);

	printf("\n--------------------------- DEZALOCARE ---------------------\n");
	dezalocare(&lista);

	return 0;
}

