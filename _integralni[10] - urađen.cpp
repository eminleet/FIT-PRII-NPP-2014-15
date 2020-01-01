#include<iostream>
#include<memory>
#include <cmath>
/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NEÆE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB160061.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ÆETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/

using namespace std;
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

enum VrstaObaveze { Seminarski, Parcijalni1, Parcijalni2, Integralni, Prakticni };
const char * VrstaObavezeChar[] = { "Seminarski", "Parcijalni1", "Parcijalni2", "Integralni", "Prakticni" };
struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() {
		cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl;
	}
	void Dealociraj() { delete _dan; delete _mjesec; delete _godina; _dan = _mjesec = _godina = nullptr; }
};
bool manjiDatum(Datum d1, Datum d2) {
	if (*d1._godina < *d2._godina) return true;
	if (*d1._godina == *d2._godina && *d1._mjesec < *d2._mjesec) return true;
	if (*d1._godina == *d2._godina && *d1._mjesec == *d2._mjesec && *d1._dan < *d2._dan) return true;
	return false;
}
int rataDie(int d, int m, int g) {
	if (m < 3)
		g--, m += 12;
	return 365 * g + g / 4 - g / 100 + g / 400 + (153 * m - 457) / 5 + d - 306;
}

struct ObavezeNaPredmetu {
	shared_ptr<VrstaObaveze> _vrstaObaveze;
	Datum _datumIzvrsenja;
	char * _napomena;
	int _ocjena; // 5 - 10 
	void Unos(VrstaObaveze vrsta, Datum datum, int ocjena, const char * napomena) {
		_vrstaObaveze = make_shared<VrstaObaveze>(vrsta);
		_datumIzvrsenja.Unos(*datum._dan, *datum._mjesec, *datum._godina);
		_ocjena = ocjena;
		_napomena = new char[strlen(napomena) + 1];
		strcpy_s(_napomena, strlen(napomena) + 1, napomena);
	}
	void Ispis() {
		cout << VrstaObavezeChar[*_vrstaObaveze] << " " << _ocjena << " " << _napomena;
		_datumIzvrsenja.Ispis();
		cout << endl;
	}
	void Dealociraj() {
		_datumIzvrsenja.Dealociraj();
		delete[] _napomena; _napomena = nullptr;
	}
};

const char * _kljucneRijeci[15] = { NULL }; /*da bi se odredjena obaveza na predmetu smatrala uspjesno izvrsenom, u napomeni
											(objekta ObavezeNaPredmetu) ne smije stajati niti jedna od rijeci koja se nalazi u nizu _kljucneRijeci*/

struct PolozeniPredmet {
	Datum _datumPolaganja;//datum koji ce se evidentirati kao datum kada je predmet polozen tj. kada je formirana konacna ocjena
	char * _nazivPredmeta;
	ObavezeNaPredmetu * _listaIzvrsenihObaveza;
	int _trenutnoIzvrsenihObaveza;
	int _minimalanBrojDana; //odnosi se na minimalan broj dana koji mora proci od izvrsenja pojedinih obaveza na predmetu (npr. 7 dana)
	int _konacnaOcjena; //formira se na osnovu ocjena izvrsenih obaveza

	void Unos(const char * naziv, int minimalanBrojDana) {
		_nazivPredmeta = new char[strlen(naziv) + 1];
		strcpy_s(_nazivPredmeta, strlen(naziv) + 1, naziv);
		_listaIzvrsenihObaveza = nullptr;
		_trenutnoIzvrsenihObaveza = 0;
		_minimalanBrojDana = minimalanBrojDana;
		_konacnaOcjena = 0;
	}
	void Dealociraj() {
		if (_konacnaOcjena > 5)
			_datumPolaganja.Dealociraj();
		delete[] _nazivPredmeta; _nazivPredmeta = nullptr;
		for (size_t i = 0; i < _trenutnoIzvrsenihObaveza; i++)
			_listaIzvrsenihObaveza[i].Dealociraj();
		delete[]_listaIzvrsenihObaveza; _listaIzvrsenihObaveza = nullptr;
	}
	bool DodajIzvrsenuObavezu(VrstaObaveze vrsta, Datum datum, int ocjena, const char * napomena) {
		bool identicna = false;
		int indeksIdenticne = -1;
		int indeksZadnjeObaveze = -1;
		for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++) {
			if (_listaIzvrsenihObaveza[i]._napomena != nullptr)
				indeksZadnjeObaveze = i;
			if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == vrsta) {
				identicna = true;
				indeksIdenticne = i;
			}
		}
		if (identicna) {
			if (_listaIzvrsenihObaveza[indeksIdenticne]._ocjena > 5)
				return false;
			for (int i = 0; i < 15; i++) {
				if (_kljucneRijeci[i] == nullptr)
					break;
				if (strstr(_listaIzvrsenihObaveza[indeksIdenticne]._napomena, _kljucneRijeci[i]) == 0)
					return false;
			}
		}

		if (indeksZadnjeObaveze != -1) {
			int datumNoveObaveze = rataDie(*datum._dan, *datum._mjesec, *datum._godina);
			int datumZadnjeObaveze = rataDie(*_listaIzvrsenihObaveza[indeksZadnjeObaveze]._datumIzvrsenja._dan,
				*_listaIzvrsenihObaveza[indeksZadnjeObaveze]._datumIzvrsenja._mjesec,
				*_listaIzvrsenihObaveza[indeksZadnjeObaveze]._datumIzvrsenja._godina);
			if (abs(datumNoveObaveze - datumZadnjeObaveze) < _minimalanBrojDana) return false;
		}

		ObavezeNaPredmetu * temp = new ObavezeNaPredmetu[_trenutnoIzvrsenihObaveza + 1];
		for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++) {
			temp[i].Unos(*_listaIzvrsenihObaveza[i]._vrstaObaveze, _listaIzvrsenihObaveza[i]._datumIzvrsenja, _listaIzvrsenihObaveza[i]._ocjena,
				_listaIzvrsenihObaveza[i]._napomena);
			_listaIzvrsenihObaveza[i].Dealociraj();
		}
		temp[_trenutnoIzvrsenihObaveza].Unos(vrsta, datum, ocjena, napomena);
		delete[] _listaIzvrsenihObaveza; _listaIzvrsenihObaveza = nullptr;
		_listaIzvrsenihObaveza = temp;
		_trenutnoIzvrsenihObaveza++;
		return true;
	}
	int FormirajKonacnuOcjenu() {
		for (int i = 0; i < 15; i++) {
			if (_kljucneRijeci[i] == nullptr)
				break;
			if (strstr(_listaIzvrsenihObaveza[i]._napomena, _kljucneRijeci[i]) != 0) {
				_konacnaOcjena = 5;
				return _konacnaOcjena;
			}
		}

		int indeksPrvogParc = -1;
		int indeksDrugogParc = -1;
		for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++) {
			if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Parcijalni1) {
				indeksPrvogParc = i;
			}
			if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Parcijalni2) {
				indeksDrugogParc = i;
			}
		}
		if ((indeksPrvogParc != 1) && (indeksDrugogParc != 1)) {
			if (manjiDatum(_listaIzvrsenihObaveza[indeksDrugogParc]._datumIzvrsenja, _listaIzvrsenihObaveza[indeksPrvogParc]._datumIzvrsenja)) {
				_konacnaOcjena = 5;
				return _konacnaOcjena;
			}
		}

		bool imaIntegralni = false;
		bool imaPrviParc = false;
		bool imaDrugiParc = false;
		bool imaPrakticni = false;
		int brojSeminarskih = 0;
		int najvecaOcjena = 5;
		int indeksNajveceOcjene = -1;
		int sveOcjene = 0;
		int brojacOcjena = 0;
		for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++) {
			if (_listaIzvrsenihObaveza[i]._ocjena > najvecaOcjena) {
				najvecaOcjena = _listaIzvrsenihObaveza[i]._ocjena;
				indeksNajveceOcjene = i;
			}
			if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Parcijalni1 && _listaIzvrsenihObaveza[i]._ocjena > 5) {
				sveOcjene += _listaIzvrsenihObaveza[i]._ocjena;
				brojacOcjena++;
				imaPrviParc = true;
			}
			if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Parcijalni2 && _listaIzvrsenihObaveza[i]._ocjena > 5) {
				sveOcjene += _listaIzvrsenihObaveza[i]._ocjena;
				brojacOcjena++;
				imaDrugiParc = true;
			}
			if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Integralni && _listaIzvrsenihObaveza[i]._ocjena > 5) {
				sveOcjene += _listaIzvrsenihObaveza[i]._ocjena;
				brojacOcjena++;
				imaIntegralni = true;
			}
			if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Prakticni && _listaIzvrsenihObaveza[i]._ocjena > 5) {
				sveOcjene += _listaIzvrsenihObaveza[i]._ocjena;
				brojacOcjena++;
				imaPrakticni = true;
			}
			if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Seminarski && _listaIzvrsenihObaveza[i]._ocjena > 5) {
				sveOcjene += _listaIzvrsenihObaveza[i]._ocjena;
				brojacOcjena++;
				brojSeminarskih++;
			}
		}

		if ((imaPrviParc && imaDrugiParc && imaPrakticni && (brojSeminarskih > 2) || imaIntegralni)) {
			_konacnaOcjena = sveOcjene / brojacOcjena;
			if (_konacnaOcjena > 5) {
				_datumPolaganja.Unos(*_listaIzvrsenihObaveza[indeksNajveceOcjene]._datumIzvrsenja._dan,
					*_listaIzvrsenihObaveza[indeksNajveceOcjene]._datumIzvrsenja._mjesec,
					*_listaIzvrsenihObaveza[indeksNajveceOcjene]._datumIzvrsenja._godina);
			}
			return _konacnaOcjena;
		}
	}
};

float PretragaRekurzivno(PolozeniPredmet &predmet, VrstaObaveze vrsta, Datum OD, Datum DO, int brojacObaveza, float prosjek, int brojUspjesnihObaveza = 0) {
	if (brojacObaveza >= predmet._trenutnoIzvrsenihObaveza) return prosjek / brojUspjesnihObaveza;
	if (*predmet._listaIzvrsenihObaveza[brojacObaveza]._vrstaObaveze == vrsta && predmet._listaIzvrsenihObaveza[brojacObaveza]._ocjena > 5 &&
		manjiDatum(OD, predmet._listaIzvrsenihObaveza[brojacObaveza]._datumIzvrsenja) &&
		manjiDatum(predmet._listaIzvrsenihObaveza[brojacObaveza]._datumIzvrsenja, DO)) {
		prosjek += predmet._listaIzvrsenihObaveza[brojacObaveza]._ocjena;
		brojUspjesnihObaveza++;
		return PretragaRekurzivno(predmet, vrsta, OD, DO, brojacObaveza + 1, prosjek, brojUspjesnihObaveza);
	}
	return PretragaRekurzivno(predmet, vrsta, OD, DO, brojacObaveza + 1, prosjek, brojUspjesnihObaveza);
}

int GetBrojObavezaIznadProsjeka(PolozeniPredmet &predmet, float prosjek) {
	auto pok = [predmet, prosjek]() -> int {
		int brojac = 0;
		for (int i = 0; i < predmet._trenutnoIzvrsenihObaveza; i++) {
			if (predmet._listaIzvrsenihObaveza[i]._ocjena > prosjek)
				brojac++;
		}
		return brojac;
	};
	int brojacObaveza = pok();
	return brojacObaveza;
}

void main() {
	_kljucneRijeci[0] = "prepisivao";
	_kljucneRijeci[1] = "ometao";
	_kljucneRijeci[2] = "nije dostavio";

	Datum datumSeminarski1, datumSeminarski2, datumParcijalni1, datumParcijalni2, datumIntegralni;
	datumSeminarski1.Unos(10, 6, 2017);
	datumSeminarski2.Unos(18, 6, 2017);
	datumParcijalni1.Unos(22, 6, 2017);
	datumParcijalni2.Unos(30, 6, 2017);
	PolozeniPredmet prII;
	//naziv predmeta, minimalan broj dana
	prII.Unos("Programiranje II", 7);

	/*Na osnovu vrijednosti primljenih parametara osigurati dodavanje novoizvrsene obaveze na predmetu. Potrebno je onemoguciti dodavanje
	identicnih obaveza, a izmedju izvrsenja pojedinih obaveza mora proci najmanje onoliko dana koliko je definisano vrijednoscu atributa
	_minimalanBrojDana. Identicna vrsta obaveze se moze dodati samo u slucaju da je prethodno dodana obaveza (identicne vrste) imala ocjenu
	5 ili je u napomeni sadrzavala neku od zabranjenih rijeci (definisanih nizom _kljucneRijeci).*/
	if (prII.DodajIzvrsenuObavezu(Parcijalni1, datumParcijalni1, 6, "uslovno polozen ispit"))
		cout << "Parcijalni 1...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Parcijalni2, datumParcijalni2, 5, "previse gresaka, a tokom ispita ometao druge studente"))
		cout << "Parcijalni 2...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Seminarski, datumSeminarski1, 8, "implementirani svi algoritmi"))
		cout << "Seminarski1...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Seminarski, datumSeminarski2, 6, "rad slican kao kod studenta IB150388"))
		cout << "Seminarski2...dodan" << endl;

	/*Konacna ocjene predstavlja prosjecnu ocjenu na predmetu (prosjecnu ocjenu izvrsenih obaveza koje ispunjavaju uslove za uspjesno
	polaganje predmeta),a za njeno formiranje student mora posjedovati polozen integralni ili dva parijcalna ispita. Ukoliko je ispit polozen
	putem parcijalnih ispita, student takodjer mora imati pozitivno (ocjenom vecom od 5) ocijenjena najmanje dva seminarska i jedan prakticni
	rad. Takodjer, datum polaganja drugog parcijalnog ispita ne smije biti raniji (manji) od datuma polaganja prvog parcijalnog ispita.
	Konacna ocjena ne smije biti formirana u slucaju da napomena bilo koje odbaveze (koja je ucestvovala u formiranju konacne ocjene) sadrzi
	neku od zabranjenih rijeci (niz _kljucneRijeci). U slucaju da neki od navedenih uslova nije zadovoljen konacna ocjena treba biti
	postavljena na vrijednost 5. Ukoliko su ispunjeni svi uslovi, osim formiranja konacne ocjene, datum polaganja je potrebno postaviti na
	vrijednost datuma posljednje izvrsene obaveze sa najvecom ocjenom.
	*/
	cout << "Konacna ocjena iz predmeta PRII je: " << prII.FormirajKonacnuOcjenu() << endl;

	datumParcijalni2.Unos(22, 7, 2017);
	if (prII.DodajIzvrsenuObavezu(Parcijalni2, datumParcijalni2, 6, ""))
		cout << "Parcijalni 2...dodan" << endl;
	cout << "Konacna ocjena iz predmeta PRII je: " << prII.FormirajKonacnuOcjenu() << endl;
	Datum OD, DO; OD.Unos(1, 1, 2017); DO.Unos(1, 7, 2017);

	/*Rekurzivna funkcija treba vratiti prosjecnu ocjenu uspjesno izvrsenih obaveze na predmetu odredjene vrste u definisanom periodu.
	Izbjegavati koristenje globalnih varijabli*/
	float prosjek = PretragaRekurzivno(prII, Seminarski, OD, DO, 0, 0);
	cout << "Prosjecna ocjena izvrsenih obaveza je: " << prosjek << endl;

	/*Koristeci lambda-u, unutar funkcije GetBrojObavezaIznadProsjeka pronaci broj izvrsenih obaveza koje imaju istu ili vecu ocjenu od
	prosjecne*/
	int iznadProsjeka = GetBrojObavezaIznadProsjeka(prII, prosjek);
	cout << "Broj izvrsenih obaveza iznad prosjeka je: " << iznadProsjeka << endl;

	datumParcijalni1.Dealociraj();
	datumParcijalni2.Dealociraj();
	datumSeminarski1.Dealociraj();
	datumSeminarski2.Dealociraj();
	//datumIntegralni.Dealociraj();
	prII.Dealociraj();
	system("pause");
}