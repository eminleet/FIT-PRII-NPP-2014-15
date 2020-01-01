#include<iostream>
using namespace std;
const char* crt = "\n---------------------------\n";
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
	system("cls");
	return izbor;
}
void main() {
	const int maxPitanja = 6;
	int trenutnoPitanja = 0, izbor = 0;
	Pitanje* ispitPRII[maxPitanja];
	int* odgovoriNaPitanja = nullptr;
	/*VRSITI VALIDACIJU KORISNIKOVIH UNOSA TE GA ONEMOGUCITI DA UNOSI VRIJEDNOSTI KOJE NISU VALIDNE*/
	do {
		izbor = PrikaziMenu();
		if (izbor == 1) {
			DodajPitanje(ispitPRII[trenutnoPitanja]);
			/*
			Unesite tekst pitanja -> U kojem gradu se nalazi FIT?
			Koliko odgovora unosite -> 4
			Odgovor 1 -> Mostar
			Odgovor 2 -> Sarajevo
			Odgovor 3 -> Tuzla
			Odgovor 4 -> Bihac
			Koji od unesenih je tacan odgovor -> 1
			Koliko bodova nosi ovo pitanje -> 3
			*/
			trenutnoPitanja++;
		}
		else if (izbor == 2) {
			/*PITANJA SE MOGU MIJENJATI SAMO AKO KVIZ NIJE URADJEN!*/
			PromijeniOdgovor(ispitPRII, trenutnoPitanja);
			/*
			Unesite odgovor ili dio odgovora za pretragu -> ac
			Odgovor Bihac pronadjen u pitanju: U kojem gradu se nalazi FIT?
			Da li je ovo odgovor koji biste zeljeli promijeniti (d/n) -> d
			Unesite novi odgovor -> Zenica
			Da li zelite postaviti novi odgovor kao tacan (d/n) -> d
			*/
		}
		else if (izbor == 3) {
			/*ONEMOGUCITI VISESTRUKO POKRETANJE KVIZA!*/
			if (trenutnoPitanja > 3) /*KVIZ MORA IMAT NAJMANJE 3 PITANJA*/
				odgovoriNaPitanja = PokreniKviz(ispitPRII, maxPitanja);
			/*
			-------------------------------
			1. U kojem gradu se nalazi FIT?
			-------------------------------
			1. Mostar
			2. Sarajevo
			3. Tuzla
			4. Zenica
			-------------------------------
			Vas odgovor -> 1
			-------------------------------
			2. Naredno pitanje...
			*/
		}
		else if (izbor == 4) {
			/*REZULTATI SE PRIKAZUJU SAMO AKO JE KVIZ URADJEN*/
			PrikaziRezultat(ispitPRII, odgovoriNaPitanja, maxPitanja);
			/*
			-------------------------------
			REZULTATI
			-------------------------------
			1. Pitanje -> TACNO
			2. Pitanje -> NETACNO
			3. Pitanje -> NETACNO
			...
			-------------------------------
			USPJEH: 10/20 BODOVA (50%)
			-------------------------------
			*/
		}

	} while (izbor != 5);


	/*oslobađa zauzetu memoriju*/
	Dealociraj(ispitPRII, odgovoriNaPitanja, trenutnoPitanja);
}