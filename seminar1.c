#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

typedef struct Masina Masina;

struct Masina {
	int id;
	int anFabricatie;
	char* sofer; 
	float kmParcursi;
	char initialaProducator; 
};

Masina initializare(int id, int anFabricatie, const char* sofer, float kmParcursi, char initialaProducator) {
	Masina m;
	m.id = id;
	m.anFabricatie = anFabricatie;
	m.sofer = (char*)malloc(strlen(sofer) + 1); 
	strcpy_s(m.sofer, strlen(sofer) + 1, sofer); 
	m.kmParcursi = kmParcursi;
	m.initialaProducator = initialaProducator;
	return m;
}


void afisare(Masina m) {
	printf("Id: %d | ", m.id);
	printf("An Fabricatie: %d | ", m.anFabricatie);
	printf("Nume Sofer: %s | ", m.sofer);
	printf("Km Parcursi: %.2f | ", m.kmParcursi);
	printf("Producator: %c. \n", m.initialaProducator);

}

void modificaSofer(Masina* m, char* soferNou) {
	if (strlen(soferNou) > 2) {
		if (m->sofer != NULL) {
			free(m->sofer);
		}
		m->sofer = (char*)malloc(strlen(soferNou) + 1);
		strcpy_s(m->sofer, strlen(soferNou) + 1, soferNou);
	}
}

void dezalocare(Masina* m) {
	if (m->sofer != NULL) {
		free(m->sofer);
		m->sofer = NULL;
	}
}

int main() {
	Masina masina;
	masina = initializare(1, 2004, "Lavinia", 200.00F, 'H'); 
	afisare(masina);
	modificaSofer(&masina, "Oana");
	afisare(masina);
	printf("\n-------------------------------dezalocare------------------------------\n");
	dezalocare(&masina);
	afisare(masina);


	return 0;
}

