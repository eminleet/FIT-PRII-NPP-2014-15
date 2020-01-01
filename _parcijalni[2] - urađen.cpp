#include <iostream>
#include <iomanip>
#include <conio.h>

using namespace std;

char crt[] = "\n-----------------------------------------------\n";

struct Datum {
	int dan, mjesec, godina;
};
struct PolozeniPredmet {
	char* nazivPredmeta;
	int ocjena;
	Datum datumPolaganja;
	char* napomena;
};
struct Student {
	char* imePrezime;
	PolozeniPredmet polozeni[40];
	int brojPolozenih;
};

void Dealociraj(Student** studenti, int trenutnoStudenata) {
	for (int i = 0; i < trenutnoStudenata; i++) {
		delete[] studenti[i]->imePrezime;
		studenti[i]->imePrezime = nullptr;
		for (int j = 0; j < studenti[i]->brojPolozenih; j++) {
			delete[] studenti[i]->polozeni[j].nazivPredmeta;
			studenti[i]->polozeni[j].nazivPredmeta = nullptr;
			delete[] studenti[i]->polozeni[j].napomena;
			studenti[i]->polozeni[j].napomena = nullptr;
		}
		delete studenti[i];
		studenti[i] = nullptr;
	}
}

int PrikaziMenu() {
	int izbor;
	cout << crt << "\t::MENU::" << crt;
	cout << "1. Dodaj studenta" << endl;
	cout << "2. Dodaj polozeni predmet" << endl;
	cout << "3. Pretraga" << endl;
	cout << "4. Kraj" << crt;
	do {
		cout << "Vas izbor -> ";
		cin >> izbor;
	} while (izbor < 1 || izbor>4);
	cin.ignore();
	system("cls");
	return izbor;
}


void Login(char* validPASS) {
	const int max = 15;
	char enteredPASS[max] = {};
	int brojac = 0;
	char unos;
	bool success = false;
	do {
		brojac = 0;
		cout << crt << "\t\t:: LOGIN :: " << crt;
		cout << "Unesite password: " << flush;
		while (brojac < max - 1) {
			unos = _getch();
			if (int(unos) == 13) break;
			else {
				enteredPASS[brojac++] = unos;
				cout << "-" << flush;
			}
		}
		enteredPASS[brojac] = '\0';
		if (strcmp(validPASS, enteredPASS) == 0) success = true;
	} while (!success);
}

void DodajStudenta(Student*& student) {
	student = new Student;
	cout << crt << "\t\t:: DODAVANJE STUDENTA :: " << crt;
	char temp[100];
	cout << "Ime i prezime -> ";
	cin.getline(temp, 99);
	int len = strlen(temp) + 1;
	student->imePrezime = new char[len];
	strcpy_s(student->imePrezime, len, temp);
	do {
		cout << "Koliko predmeta dodajete: " << flush;
		cin >> student->brojPolozenih;
	} while (student->brojPolozenih < 1 || student->brojPolozenih > 40);
	cin.ignore();
	for (int i = 0; i < student->brojPolozenih; i++) {
		cout << "Naziv predmeta -> " << flush;
		cin.getline(temp, 99);
		len = strlen(temp) + 1;
		student->polozeni[i].nazivPredmeta = new char[len];
		strcpy_s(student->polozeni[i].nazivPredmeta, len, temp);
		cout << "Datum polaganja -> " << flush;
		cin >> student->polozeni[i].datumPolaganja.dan >> student->polozeni[i].datumPolaganja.mjesec >>
			student->polozeni[i].datumPolaganja.godina;
		do {
			cout << "Ocjena -> " << flush;
			cin >> student->polozeni[i].ocjena;
		} while (student->polozeni[i].ocjena < 6 || student->polozeni[i].ocjena > 10);
		cin.ignore();
		cout << "Napomena -> " << flush;
		cin.getline(temp, 99);
		len = strlen(temp) + 1;
		student->polozeni[i].napomena = new char[len];
		strcpy_s(student->polozeni[i].napomena, len, temp);
	}
}

void DodajPolozeniPredmet(Student** studenti, int trenutnoStudenata) {
	cout << crt << "\t\t:: DODAVANJE POLOZENOG PREDMETA :: " << crt;
	char temp[100];
	int len = 0;
	bool found = false;
	cout << "Ime i prezime -> ";
	cin.getline(temp, 99);
	for (int i = 0; i < trenutnoStudenata; i++) {
		if (strcmp(studenti[i]->imePrezime, temp) == 0) {
			found = true;
			cout << "STUDENT PRONADJEN ..." << endl;
			cout << "Naziv predmeta -> " << flush;
			cin.getline(temp, 99);
			len = strlen(temp) + 1;
			studenti[i]->polozeni[studenti[i]->brojPolozenih].nazivPredmeta = new char[len];
			strcpy_s(studenti[i]->polozeni[studenti[i]->brojPolozenih].nazivPredmeta, len, temp);
			cout << "Datum polaganja -> " << flush;
			cin >> studenti[i]->polozeni[studenti[i]->brojPolozenih].datumPolaganja.dan >>
				studenti[i]->polozeni[studenti[i]->brojPolozenih].datumPolaganja.mjesec >>
				studenti[i]->polozeni[studenti[i]->brojPolozenih].datumPolaganja.godina;
			do {
				cout << "Ocjena -> " << flush;
				cin >> studenti[i]->polozeni[studenti[i]->brojPolozenih].ocjena;
			} while (studenti[i]->polozeni[studenti[i]->brojPolozenih].ocjena < 6 || studenti[i]->polozeni[studenti[i]->brojPolozenih].ocjena > 10);
			cin.ignore();
			cout << "Napomena -> " << flush;
			cin.getline(temp, 99);
			len = strlen(temp) + 1;
			studenti[i]->polozeni[studenti[i]->brojPolozenih].napomena = new char[len];
			strcpy_s(studenti[i]->polozeni[studenti[i]->brojPolozenih].napomena, len, temp);
			studenti[i]->brojPolozenih++;
		}
	}
	if (!found) cerr << "Student " << temp << " nije pronadjen!" << endl;
}

bool racunanjeDatuma(Datum OD, Datum DO, PolozeniPredmet predmet) {
	int razlikaGodine = DO.godina - OD.godina;
	int razlikaMjeseci = DO.mjesec - OD.mjesec;
	int razlikaDani = DO.dan - OD.dan;

	if (DO.godina - predmet.datumPolaganja.godina < razlikaGodine)
		return true;
	else if (DO.godina - predmet.datumPolaganja.godina == razlikaGodine) {
		if (DO.mjesec - predmet.datumPolaganja.mjesec < razlikaMjeseci)
			return true;
		else if (DO.mjesec - predmet.datumPolaganja.mjesec == razlikaMjeseci)
			if (DO.dan - predmet.datumPolaganja.dan < razlikaDani)
				return true;

	}
	return false;
}

double Prosjek(Student* student) {
	double suma = 0.0;
	for (int i = 0; i < student->brojPolozenih; i++)
		suma += student->polozeni[i].ocjena;
	return suma / student->brojPolozenih;
}

void Pretraga(Student** studenti, int trenutnoStudenata) {
	cout << crt << "\t\t:: PRETRAGA :: " << crt;
	int izbor = 0;
	do {
		cout << "Pretraga (1 - po datumu polaganja | 2 - po napomeni) -> " << flush;
		cin >> izbor;
	} while (izbor != 1 && izbor != 2);
	if (izbor == 1) {
		Datum OD;
		cout << "Unesite datum OD -> ";
		cin >> OD.dan >> OD.mjesec >> OD.godina;
		Datum DO;
		cout << "Unesite datum DO -> ";
		cin >> DO.dan >> DO.mjesec >> DO.godina;
		cout << "U periodu od " << OD.dan << "." << OD.mjesec << "." << OD.godina << " do " << DO.dan << "." << DO.mjesec << "." << DO.godina
			<< " ispite su polozili sljedeci studenti: " << endl;

		unsigned int l = 1;
		for (int i = 0; i < trenutnoStudenata; i++) {
			for (int j = 0; j < studenti[i]->brojPolozenih; j++) {
				if (racunanjeDatuma(OD, DO, studenti[i]->polozeni[j]))
					cout << l++ << ". " << studenti[i]->imePrezime << " -> " << studenti[i]->polozeni[j].nazivPredmeta
					<< " (" << studenti[i]->polozeni[j].ocjena << ") Prosjek studenta: " << Prosjek(studenti[i]) << endl;
			}
		}

	}
	else if (izbor == 2) {
		cin.ignore();
		cout << "Unesite tekst -> " << flush;
		char temp[100];
		unsigned int l = 1;
		char* pok = nullptr;
		cin.getline(temp, 99);
		cout << "Trazeni sadrzaj '" << temp << "' u napomeni posjeduju sljedeci studenti: " << endl;
		for (int i = 0; i < trenutnoStudenata; i++) {
			for (int j = 0; j < studenti[i]->brojPolozenih; j++) {
				pok = strstr(studenti[i]->polozeni[j].napomena, temp);
				if (pok != nullptr)
					cout << l++ << ". " << studenti[i]->imePrezime << ", predmet " << studenti[i]->polozeni[j].nazivPredmeta << endl;
			}
		}
	}
}

int main() {
	const int maxStudenata = 6;
	int trenutnoStudenata = 0, izbor = 0;
	Student* studentiIgodina[maxStudenata];
	char validPass[] = "ispitPRII2017";
	do {
		Login(validPass);

		izbor = PrikaziMenu();
		if (izbor == 1) {
			DodajStudenta(studentiIgodina[trenutnoStudenata]);
			trenutnoStudenata++;
		}
		else if (izbor == 2) {
			DodajPolozeniPredmet(studentiIgodina, trenutnoStudenata);
		}
		else if (izbor == 3) {
			Pretraga(studentiIgodina, trenutnoStudenata);
		}

	} while (izbor != 4);


	Dealociraj(studentiIgodina, trenutnoStudenata);

	cout << crt << setw(33) << ":: END OF PROGRAM :: " << crt << flush;
	cin.ignore();
	cin.get();
}