#include<stdio.h>
#include<malloc.h>
#include<string.h>

typedef struct Masina Masina;

struct Masina {
	int id;
	int anFabricatie;
	char* sofer;
	float kilometriiParcursi;
	char initialaProducator;
};

struct Masina initializare(int id, int anFabricatie, const char* sofer, float kilometriiParcursi, char initialaProducator) {
	Masina m;
	m.id = id;
	m.anFabricatie = anFabricatie;
	if (sofer != NULL) {
		m.sofer = (char*)malloc(strlen(sofer) + 1);
		strcpy_s(m.sofer, strlen(sofer) + 1, sofer);
	}
	else {
		m.sofer = NULL;
	}

	m.kilometriiParcursi = kilometriiParcursi;
	m.initialaProducator = initialaProducator;
	return m;
}

void afisare(Masina m) {
	printf("ID: %d | ", m.id);
	printf("An fabricatie: %d | ", m.anFabricatie);
	printf("Sofer: %s | ", m.sofer);
	printf("Kilometrii Parcursi : %.2f | ", m.kilometriiParcursi);
	printf("Initiala producator: %c \n", m.initialaProducator);
}

void afisareVector(Masina* vector, int nrElemente) {
	if (vector != NULL && nrElemente > 0) {
		for (int i = 0; i < nrElemente; i++) {
			afisare(vector[i]);
		}
	}
}

Masina* copiazaPrimeleNElemente(Masina* vector, int nrElemente, int nrElementeCopiate) {
	Masina* vectorNou = NULL;
	if (vector != NULL && nrElementeCopiate <= nrElemente && nrElementeCopiate > 0) {
		vectorNou = malloc(sizeof(Masina) * nrElementeCopiate);
		for (int i = 0; i < nrElementeCopiate; i++) {
			vectorNou[i] = initializare(vector[i].id, vector[i].anFabricatie, vector[i].sofer, vector[i].kilometriiParcursi, vector[i].initialaProducator);

		}
	}

	return vectorNou;
}

void dezalocare(Masina** vector, int* nrElemente) { 
	if ((*vector) != NULL && (*nrElemente) > 0) {
		for (int i = 0; i < (*nrElemente); i++) {
			free((*vector)[i].sofer);
		}
		free(*vector);
		*vector = NULL;
		*nrElemente = 0;
	}
}

void copiazaAnumiteElemente(Masina* vector, char nrElemente, float prag, Masina** vectorNou, int* dimensiune) {
	if (vector != NULL) {
		(*dimensiune) = 0;
		for (int i = 0; i < nrElemente; i++) {
			if (vector[i].kilometriiParcursi > prag) {
				(*dimensiune)++;
			}
		}

		(*vectorNou) = malloc(sizeof(Masina) * (*dimensiune));
		int contor = 0;
		for (int i = 0; i < nrElemente; i++) {
			if (vector[i].kilometriiParcursi > prag) {
				(*vectorNou)[contor] = vector[i]; //shallow copy
				(*vectorNou)[contor].sofer = malloc(strlen(vector[i].sofer) + 1);
				strcpy_s((*vectorNou)[contor].sofer, strlen(vector[i].sofer) + 1, vector[i].sofer);
				contor++;

			}
		}

	}

}

Masina getPrimaMasinaDupaSofer(Masina* vector, int nrElemente, const char* sofer) {
	if (vector != NULL && nrElemente != NULL) {
		for (int i = 0; i < nrElemente; i++) {
			if (vector[i].sofer != NULL) {
				if (strcmp(vector[i].sofer, sofer) == 0) {
					return vector[i];
				}
			}
			
		}
	}
	else {
		return initializare(-1, 0, NULL, 0, '-');
	}
}



int main() {
	int nrElem = 3;
	Masina* vectorM = malloc(sizeof(Masina) * nrElem); 
	vectorM[0] = initializare(1, 1980, "Lavinia", 200000.00F, 'C');
	vectorM[1] = initializare(2, 2020, "Valentin", 178000.00F, 'H');
	vectorM[2] = initializare(3, 2015, "Lavinia", 4000.00F, 'V');

	afisareVector(vectorM, nrElem);
	int nrElemCopiate = 2;
	Masina* vectNou = copiazaPrimeleNElemente(vectorM, nrElem, nrElemCopiate);
	printf("\n------------------------------Functia de copiere-------------------------\n");
	afisareVector(vectNou, nrElemCopiate);

	printf("\n------------------------------Dezalocare-------------------------\n");
	dezalocare(&vectNou, &nrElemCopiate);
	printf("\n------------------------------Copiere in functie de prag-------------------------\n");
	float prag = 100000.00F;
	copiazaAnumiteElemente(vectorM, nrElem, prag, &vectNou, &nrElemCopiate);
	afisareVector(vectNou, nrElemCopiate);
	dezalocare(&vectNou, &nrElemCopiate);

	printf("\n------------------------------Prima masina condusa de Lavinia-------------------------\n");

	Masina primaMasina = getPrimaMasinaDupaSofer(vectorM, nrElem, "Lavinia");
	afisare(primaMasina);
	dezalocare(&vectorM, &nrElem);

	return 0;
}

