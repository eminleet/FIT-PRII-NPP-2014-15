#include <iostream>
#include <iomanip>

using namespace std;

char crt[] = "\n-----------------------------------------------\n";

struct Odgovor {
	bool tacan;
	char* tekstOdgovora;
};
struct Pitanje {
	char* tekstPitanja;
	Odgovor odgovori[5];
	int brojOdgovora;
	int brojBodova;
};

void Dealociraj(Pitanje** ispitPRII, int*& odgovoriNaPitanja, int trenutnoPitanja) {
	for (int i = 0; i < trenutnoPitanja; i++) {
		delete[] ispitPRII[i]->tekstPitanja;
		ispitPRII[i]->tekstPitanja = nullptr;
		for (int j = 0; j < ispitPRII[i]->brojOdgovora; j++) {
			delete[] ispitPRII[i]->odgovori[j].tekstOdgovora;
			ispitPRII[i]->odgovori[j].tekstOdgovora = nullptr;
		}
		delete ispitPRII[i];
		ispitPRII[i] = nullptr;
	}
	delete[] odgovoriNaPitanja;
	odgovoriNaPitanja = nullptr;
}


void DodajPitanje(Pitanje*& ispitPRII) {
	ispitPRII = new Pitanje;
	cout << crt << "\t\t:: DODAVANJE PITANJA ::" << crt;
	cout << "Unesite tekst pitanja -> " << flush;
	char temp[50];
	cin.getline(temp, 49);
	int len = strlen(temp) + 1;
	ispitPRII->tekstPitanja = new char[len];
	strcpy_s(ispitPRII->tekstPitanja, len, temp);
	do {
		cout << "Koliko odgovora unosite -> ";
		cin >> ispitPRII->brojOdgovora;
	} while (ispitPRII->brojOdgovora > 5 || ispitPRII->brojOdgovora < 1);
	cin.ignore();
	for (int i = 0; i < ispitPRII->brojOdgovora; i++) {
		cout << "Odgovor " << i + 1 << " -> " << flush;
		cin.getline(temp, 49);
		len = strlen(temp) + 1;
		ispitPRII->odgovori[i].tekstOdgovora = new char[len];
		strcpy_s(ispitPRII->odgovori[i].tekstOdgovora, len, temp);
		ispitPRII->odgovori[i].tacan = false;
	}
	int tacan = 0;
	do {
		cout << "Koji od unesenih je tacan odgovor -> " << flush;
		cin >> tacan;
	} while (tacan > ispitPRII->brojOdgovora || tacan < 1);
	ispitPRII->odgovori[--tacan].tacan = true;
	cout << "Koliko bodova nosi ovo pitanje -> " << flush;
	cin >> ispitPRII->brojBodova;
}

int* PokreniKviz(Pitanje** ispitPRII, int maxPitanja) {
	cout << crt << "\t\t:: POKRETANJE KVIZA ::" << crt;
	int* odgovori = new int[maxPitanja];
	unsigned int l = 0;
	for (int i = 0; i < maxPitanja; i++) {
		cout << crt;
		cout << ispitPRII[i]->tekstPitanja << endl;
		cout << crt;
		for (int j = 0; j < ispitPRII[i]->brojOdgovora; j++)
			cout << j + 1 << ". " << ispitPRII[i]->odgovori[j].tekstOdgovora << endl;
		cout << crt;
		int odgovor = 0;
		do {
			cout << "Vas odgovor -> ";
			cin >> odgovor;
		} while (odgovor < 1 || odgovor > ispitPRII[i]->brojOdgovora);
		odgovori[l++] = odgovor;
	}
	return odgovori;
}

void PromijeniOdgovor(Pitanje** ispitPRII, int trenutnoPitanja) {
	cout << crt << "\t\t:: PROMJENA ODGOVORA :: " << crt;
	char* pok = nullptr;
	int pitanje = -1;
	int odg = -1;
	char potvrda;
	char tacan;
	char temp[50];
	cout << "Unesite odgovor ili dio odgovora za pretragu: " << flush;
	cin.getline(temp, 49);
	for (int i = 0; i < trenutnoPitanja; i++) {
		for (int j = 0; j < ispitPRII[i]->brojOdgovora; j++) {
			pok = strstr(ispitPRII[i]->odgovori[j].tekstOdgovora, temp);
			if (pok != nullptr) {
				pitanje = i;
				odg = j;
				break;
			}
		}
	}
	if (pok == nullptr) cerr << "Trazene rijeci ne sadrzi niti jedan odgovor!" << endl;
	else {
		cout << "Odgovor " << ispitPRII[pitanje]->odgovori[odg].tekstOdgovora << " pronadjen u pitanju: "
			<< ispitPRII[pitanje]->tekstPitanja << endl;
		cout << "Da li je ovo odgovor koji zelite promijeniti (d/n) -> ";
		cin >> potvrda;
		cin.ignore();
		if (potvrda == 'D' || potvrda == 'd') {
			cout << "Unesite novi odgovor -> " << flush;
			cin.getline(temp, 49);
			int len = strlen(temp) + 1;
			strcpy_s(ispitPRII[pitanje]->odgovori[odg].tekstOdgovora, len, temp);
			cout << "Da li zelite postaviti novi odgovor kao tacan (d/n) -> ";
			cin >> tacan;
			if (tacan == 'D' || tacan == 'd') {
				for (int i = 0; i < ispitPRII[pitanje]->brojOdgovora; i++) {
					if (ispitPRII[pitanje]->odgovori[i].tacan)
						ispitPRII[pitanje]->odgovori[i].tacan = false;
				}
				ispitPRII[pitanje]->odgovori[odg].tacan = true;
			}
		}
	}
}

void PrikaziRezultat(Pitanje** ispitPRII, int* odgovoriNaPitanja, int maxPitanja) {
	int brojBodova = 0;
	int ukupnoBodova = 0;
	bool tacanOdg = false;
	int tacan = 0;
	cout << crt << "\t\t:: REZULTATI :: " << crt;
	for (int i = 0; i < maxPitanja; i++) {
		ukupnoBodova += ispitPRII[i]->brojBodova;
		for (int j = 0; j < ispitPRII[i]->brojOdgovora; j++) {
			if (ispitPRII[i]->odgovori[j].tacan) {
				tacan = j;
				tacan++;;
				if (odgovoriNaPitanja[i] == tacan) {
					cout << i + 1 << ". Pitanje -> TACNO" << endl;
					brojBodova += ispitPRII[i]->brojBodova;
				}
				else
					cout << i + 1 << ". Pitanje -> NETACNO" << endl;
			}
		}
	}
	cout << crt << "USPJEH: " << brojBodova << '/' << ukupnoBodova << " (" << (float)brojBodova / ukupnoBodova * 100. << "%)" << crt;
}

int PrikaziMenu() {
	int izbor;
	cout << crt << "\t::MENU::" << crt;
	cout << "1. Dodaj pitanje" << endl;
	cout << "2. Promijeni odgovor na pitanje" << endl;
	cout << "3. Pokreni kviz" << endl;
	cout << "4. Prikazi rezultat" << crt;
	cout << "5. Kraj" << crt;
	do {
		cout << "Vas izbor -> ";
		cin >> izbor;
	} while (izbor < 1 || izbor>5);
	cin.ignore();
	system("cls");
	return izbor;
}


int main() {
	const int maxPitanja = 6;
	int trenutnoPitanja = 0, izbor = 0;
	bool kvizPokrenut = false;
	Pitanje* ispitPRII[maxPitanja];
	int* odgovoriNaPitanja = nullptr;

	do {
		izbor = PrikaziMenu();
		if (izbor == 1) {
			DodajPitanje(ispitPRII[trenutnoPitanja]);
			trenutnoPitanja++;
		}
		else if (izbor == 2) {
			if (kvizPokrenut == true) {
				cerr << crt << "Kviz je vec jednom pokrenut!" << endl;
				break;
			}
			else if (trenutnoPitanja > 0) {
				PromijeniOdgovor(ispitPRII, trenutnoPitanja);
			}
			else
				cerr << crt << "Kviz ne sadrzi nijedno pitanje!" << endl;
		}
		else if (izbor == 3) {
			if (kvizPokrenut == false && trenutnoPitanja > 3) {
				odgovoriNaPitanja = PokreniKviz(ispitPRII, trenutnoPitanja);
				kvizPokrenut = true;
			}

			else if (trenutnoPitanja <= 3) cerr << crt << "Kviz nema vise od 3 pitanja!" << endl;
			else if (kvizPokrenut == true) {
				cerr << crt << "Kviz je vec jednom pokrenut!" << endl;
				continue;
			}
		}
		else if (izbor == 4) {
			if (kvizPokrenut == true) {
				PrikaziRezultat(ispitPRII, odgovoriNaPitanja, trenutnoPitanja);
			}
			else
				cerr << crt << "Kviz nije pokrenut!" << endl;
		}

	} while (izbor != 5);


	Dealociraj(ispitPRII, odgovoriNaPitanja, trenutnoPitanja);

	cout << crt << setw(33) << ":: END OF PROGRAM :: " << crt << flush;
	cin.ignore();
	cin.get();
}