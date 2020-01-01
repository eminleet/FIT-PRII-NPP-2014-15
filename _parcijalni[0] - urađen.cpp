#include <iostream>
#include <iomanip>
#include <assert.h>
#include <fstream>
#include <string>
#include <conio.h>

using namespace std;

char crt[] = "\n----------------------------------------------\n";

int redovi = 20;
int kolone = 20;

struct Kocka {
	int lokacijaX;
	int lokacijaY;
	int velicina;
};

void ocisti_Matricu(char ** matrica) {
	for (int i = 0; i < redovi; i++) {
		for (int j = 0; j < kolone; j++) {
			matrica[i][j] = ' ';
		}
	}
}

void Ispis(char ** matrica, Kocka kocka) {
	ocisti_Matricu(matrica);

	for (int i = kocka.lokacijaX; i < kocka.lokacijaX + kocka.velicina; i++) {
		for (int j = kocka.lokacijaY; j < kocka.lokacijaY + kocka.velicina; j++) {
			matrica[i][j] = '*';
		}
		cout << endl;
	}

	for (int i = 0; i < kolone; i++)
		cout << "+-";
	cout << "+-" << endl;
	for (int i = 0; i < redovi; i++) {
		cout << "|" << flush;
		for (int j = 0; j < kolone; j++) {
			cout << setw(2) << matrica[i][j];
		}
		cout << "|" << endl;
	}
	for (int i = 0; i < kolone; i++)
		cout << "+-";
	cout << "+-" << endl;

}

void Dealokacija(char **& matrica) {
	for (int i = 0; i < redovi; i++)
		delete[] matrica[i];
	delete[] matrica;
	matrica = nullptr;
}

void Alokacija(char **& matrica) {
	if (matrica != NULL)
		Dealokacija(matrica);
	matrica = new char*[redovi];
	for (int i = 0; i < redovi; i++)
		matrica[i] = new char[kolone];
	ocisti_Matricu(matrica);
}

int main() {

	char ** matrica = nullptr;
	Kocka kocka;
	kocka.lokacijaX = redovi / 2;
	kocka.lokacijaY = kolone / 2;
	kocka.velicina = 2;
	char komanda;
	Alokacija(matrica);
	do {
		Ispis(matrica, kocka);
		cout << "/Lijevo - L/Desno - R/Gore - U/Dole - D/" << endl;
		cout << "/Povecaj - P/ Smanji - S/" << endl;
		cout << "Komanda >>> " << flush;
		komanda = _getch();
		switch (toupper(komanda))
		{
		case 'L':
			if (kocka.lokacijaY <= 0)
				break;
			kocka.lokacijaY--;
			break;
		case 'R':
			if (kocka.lokacijaY >= kolone - kocka.velicina)
				break;
			kocka.lokacijaY++;
			break;
		case 'U':
			if (kocka.lokacijaX <= 0)
				break;
			kocka.lokacijaX--;
			break;
		case 'D':
			if (kocka.lokacijaX >= redovi - kocka.velicina)
				break;
			kocka.lokacijaX++;
			break;
		case 'P':
			Dealokacija(matrica);
			redovi++;
			kolone++;
			kocka.velicina++;
			Alokacija(matrica);
			break;
		case 'S':
			if (redovi <= 11) {
				break;
			}
			Dealokacija(matrica);
			redovi--;
			kolone--;
			kocka.velicina--;
			if (kocka.velicina < 1)
				kocka.velicina = 1;
			Alokacija(matrica);
			break;
		}
		system("CLS");
	} while (1);
	Dealokacija(matrica);

	cout << crt << setw(33) << ":: Kraj programa :: " << crt << endl;
	system("PAUSE > NULL");
	return 0;
}

