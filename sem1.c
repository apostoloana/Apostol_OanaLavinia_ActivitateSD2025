//#include<stdio.h>
//#include<stdlib.h>
//#include<malloc.h>
//
//typedef struct Masina Masina;
//
//struct Masina {
//	int id;
//	int anFabricatie;
//	char* sofer; //char* este un sir// dpdv al memoriei - char* ocupa 4/8 octeti (64/32)
//	float kmParcursi;
//	char initialaProducator; //char este un singur caracter // dpdv al memoriei - char* ocupa 1 octet- 8 biti unde sunt 0 si 1
//};
//
////unsigned char - 0-512
//
//Masina initializare(int id, int anFabricatie, const char* sofer, float kmParcursi, char initialaProducator) {
//	Masina m;
//	m.id = id;
//	m.anFabricatie = anFabricatie;
//	//m.sofer = malloc(strlen(sofer) + 1);
//	m.sofer = (char*)malloc(strlen(sofer) + 1); //(char*) //malloc-functie cu tip returnat de tip void*(adica poate fi pointer la orice) - folosim char* ca sa facem voidul de tip char, am facut cast la char*
//	//malloc primeste dimensiunea in octeti
//	strcpy_s(m.sofer, strlen(sofer) + 1, sofer); //strcpy_s - verifica dimensiunea, cat ii dau atat copiaza, strcpy scrie in continuare daca ii dai un nume mai mare decat memoria
//	m.kmParcursi = kmParcursi;
//	m.initialaProducator = initialaProducator;
//	return m;
//}
//
////conts - stiva de memorie si nu va putea fi modificat
////dif dintre functie si metoda -> metoda primeste pointer la adresa obiectului pt care apelam( adresa o retinem in pointerul this) 
//
//void afisare(Masina m) {
//	printf("Id: %d | ", m.id);
//	printf("An Fabricatie: %d | ", m.anFabricatie);
//	printf("Nume Sofer: %s | ", m.sofer);
//	printf("Km Parcursi: %.2f | ", m.kmParcursi);
//	printf("Producator: %c. \n", m.initialaProducator);
//
//}
//
//void modificaSofer(Masina* m, char* soferNou) { //transmitem prin pointer
//	if (strlen(soferNou) > 2){
//		if (m->sofer != NULL) {
//			free(m->sofer);
//		}
//		m->sofer = (char*)malloc(strlen(soferNou) + 1);
//		strcpy_s(m->sofer, strlen(soferNou) + 1, soferNou);
//	}
//}
//
//void dezalocare(Masina* m) {
//	if (m->sofer != NULL) {
//		free(m->sofer);
//		m->sofer = NULL;
//	}
//}
//
//void main() {
//	Masina masina;
//	masina = initializare(1, 2004, "Lavinia", 200.00F, 'H'); //"P"
//	afisare(masina);
//	modificaSofer(&masina, "Oana"); //param1 - adresa de masina
//	afisare(masina);
//	printf("\n-------------------------------dezalocare------------------------------\n");
//	dezalocare(&masina);
//	afisare(masina);
//
//
//	return 0;
//}
//
////nu am dreptul sa dezaloc masina creata pe stiva(stack) - programatorul are voie sa dezaloce sau sa aloce pe heap.
//
//
//
////structuri de date - containere
////in C - avem structuri, nu avem functii/metode in structuri
//
////Examen - 60%
////Sem - 40% - 2 lucrari practice 7-13 aprilie, 26-30 mai, 15% fiecare
////10% - activitate
////Git - repo public - activitate o data pe sapt - 15 martie de facut 