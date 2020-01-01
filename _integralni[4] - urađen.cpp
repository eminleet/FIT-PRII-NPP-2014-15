#include <iostream>
#include <cmath>

using namespace std;

//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

enum enumPredmeti { Matematika, Historija, Geografia, Hemija, Biologija, Engleski };
const char * enumPredmetiChar[] = { "Matematika", "Historija", "Geografia", "Hemija", "Biologija", "Engleski" };
enum enumRazredi { I, II, III, IV };
const char * enumRazrediChar[] = { "I", "II", "III", "IV" };
const char *crt = "\n--------------------------------------------\n";

struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() { delete _dan; delete _mjesec; delete _godina; _dan = _mjesec = _godina = nullptr; }
};
bool manjiDatum(Datum d1, Datum d2) {
	if (*d1._godina < *d2._godina) return true;
	if (*d1._godina == *d2._godina && *d1._mjesec < *d2._mjesec) return true;
	if (*d1._godina == *d2._godina && *d1._mjesec == *d2._mjesec && *d1._dan < *d2._dan) return true;
	return false;
}
bool manjiIliJednakDatum(Datum d1, Datum d2) {
	if (*d1._godina < *d2._godina) return true;
	if (*d1._godina == *d2._godina && *d1._mjesec < *d2._mjesec) return true;
	if (*d1._godina == *d2._godina && *d1._mjesec == *d2._mjesec && *d1._dan <= *d2._dan) return true;
	return false;
}
int rataDie(int d, int m, int g) {
	if (m < 3)
		g--, m += 12;
	return 365 * g + g / 4 - g / 100 + g / 400 + (153 * m - 457) / 5 + d - 306;
}

struct Kurs {
	enumPredmeti _predmet;
	enumRazredi _razredi;
	Datum * _pocetak;
	Datum * _kraj;
	char * _imePredavaca;
	void Unos(enumPredmeti predmet, enumRazredi razred, Datum pocetak, Datum kraj, const char * imePredavaca) {
		_predmet = predmet;
		_razredi = razred;
		_pocetak = new Datum;
		_pocetak->Unos(*pocetak._dan, *pocetak._mjesec, *pocetak._godina);
		_kraj = new Datum;
		_kraj->Unos(*kraj._dan, *kraj._mjesec, *kraj._godina);
		int size = strlen(imePredavaca) + 1;
		_imePredavaca = new char[size];
		strcpy_s(_imePredavaca, size, imePredavaca);
	}
	void Ispis() {
		cout << enumPredmetiChar[_predmet] << " " << enumRazrediChar[_razredi] << endl;
		_pocetak->Ispis();
		_kraj->Ispis();
		cout << _imePredavaca << endl;
	}
	void Dealociraj() {
		delete[] _imePredavaca; _imePredavaca = nullptr;
		_pocetak->Dealociraj();
		_kraj->Dealociraj();
	}
};
struct Polaznik {
	Datum * _datumRodjenja;
	char * _imePrezime;
	void Unos(Datum datumRodjenja, const char * imePrezime) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		_datumRodjenja = new Datum;
		_datumRodjenja->Unos(*datumRodjenja._dan, *datumRodjenja._mjesec, *datumRodjenja._godina);
	}
	void Dealociraj() {
		_datumRodjenja->Dealociraj();
		delete[] _imePrezime; _imePrezime = nullptr;
	}
	void Ispis() {
		cout << _imePrezime << " ";
		_datumRodjenja->Ispis();
	}
};
struct Polaganja {
	Polaznik * _polaznik;
	Kurs * _kurs;
	Datum * _datumPolaganja;
	int _ocjena; //ocjene su u rasponu od 1 do 5
	void Unos(Polaznik &polaznik, Kurs &kurs, Datum datum, int ocjena) {
		_polaznik = new Polaznik;
		_polaznik->Unos(*polaznik._datumRodjenja, polaznik._imePrezime);
		_kurs = new Kurs;
		_kurs->Unos(kurs._predmet, kurs._razredi, *kurs._pocetak, *kurs._kraj, kurs._imePredavaca);
		_datumPolaganja = new Datum;
		_datumPolaganja->Unos(*datum._dan, *datum._mjesec, *datum._godina);
		_ocjena = ocjena;
	}
	void Ispis() {
		_polaznik->Ispis();
		_kurs->Ispis();
		_datumPolaganja->Ispis();
		cout << _ocjena << endl;
	}
	void Dealociraj() {
		_polaznik->Dealociraj();
		_kurs->Dealociraj();
		_datumPolaganja->Dealociraj();
	}
};

struct EdukacijskiCentar {
	char * _nazivCentra;
	Kurs * _kursevi[50];//JEDAN EDUKACIJSKI CENTAR MOZE NUDITI NAJVISE 50 KURSEVA
	int _trenutnoKurseva;
	Polaganja * _polaganja;
	int _trenutnoPolaganja;
	void Unos(const char * nazivcentra) {
		int len = strlen(nazivcentra) + 1;
		_nazivCentra = new char[len];
		strcpy_s(_nazivCentra, len, nazivcentra);
		_trenutnoKurseva = 0;
		_polaganja = nullptr;
		_trenutnoPolaganja = 0;
	}
	void Ispis() {
		cout << crt << _nazivCentra << endl;
		cout << "KURSEVI" << endl;
		for (int i = 0; i < _trenutnoKurseva; i++)
			_kursevi[i]->Ispis();
		cout << "POLAGANJA" << endl;
		for (int i = 0; i < _trenutnoPolaganja; i++)
			_polaganja[i].Ispis();
	}
	bool DodajKurs(Kurs &kurs) {
		bool identican = false;
		int indeksIdenticnog = -1;
		for (int i = 0; i < _trenutnoKurseva; i++) {
			if (_kursevi[i]->_predmet == kurs._predmet) {
				identican = true;
				indeksIdenticnog = i;
			}
		}
		if (identican) {
			if (!manjiDatum(*_kursevi[indeksIdenticnog]->_kraj, *kurs._pocetak))
				return false;
		}
		_kursevi[_trenutnoKurseva] = new Kurs;
		_kursevi[_trenutnoKurseva]->Unos(kurs._predmet, kurs._razredi, *kurs._pocetak, *kurs._kraj, kurs._imePredavaca);
		_trenutnoKurseva++;
		return true;
	}
	bool DodajPolaganje(Polaganja &polaganje) {
		bool pronadjenKurs = false;
		int indeksPronadjenogKursa = -1;
		for (int i = 0; i < _trenutnoKurseva; i++) {
			if (_kursevi[i]->_predmet == polaganje._kurs->_predmet) {
				indeksPronadjenogKursa = i;
				pronadjenKurs = true;
			}
		}
		if (!pronadjenKurs) return false;
		int datumNovogKursa = rataDie(*polaganje._datumPolaganja->_dan, *polaganje._datumPolaganja->_mjesec, *polaganje._datumPolaganja->_godina);
		int zadnjeDodanogKursa = rataDie(*_kursevi[indeksPronadjenogKursa]->_kraj->_dan, *_kursevi[indeksPronadjenogKursa]->_kraj->_mjesec,
			*_kursevi[indeksPronadjenogKursa]->_kraj->_godina);
		if (abs(datumNovogKursa - zadnjeDodanogKursa) > 15) return false;

		for (int i = 0; i < _trenutnoPolaganja; i++) {
			if (strcmp(polaganje._polaznik->_imePrezime, _polaganja[i]._polaznik->_imePrezime) == 0 && _polaganja[i]._kurs->_predmet == polaganje._kurs->_predmet) {
				if (_polaganja[i]._ocjena > polaganje._ocjena)
					return false;
			}
		}

		Polaganja * temp = new Polaganja[_trenutnoPolaganja + 1];
		for (int i = 0; i < _trenutnoPolaganja; i++) {
			temp[i].Unos(*_polaganja[i]._polaznik, *_polaganja[i]._kurs, *_polaganja[i]._datumPolaganja, _polaganja[i]._ocjena);
			_polaganja[i].Dealociraj();
		}
		temp[_trenutnoPolaganja].Unos(*polaganje._polaznik, *polaganje._kurs, *polaganje._datumPolaganja, polaganje._ocjena);
		delete[] _polaganja; _polaganja = nullptr;
		_polaganja = temp;
		_trenutnoPolaganja++;
		return true;
	}
	Polaganja * PolaganjaByDatumIRazred(Datum OD, Datum DO, enumRazredi razred, int &brojPolaganja) {
		for (int i = 0; i < _trenutnoPolaganja; i++) {
			if (manjiIliJednakDatum(OD, *_polaganja[i]._datumPolaganja) && manjiIliJednakDatum(*_polaganja[i]._datumPolaganja, DO) &&
				_polaganja[i]._kurs->_razredi == razred)
				brojPolaganja++;
		}
		Polaganja * noviNiz = new Polaganja[brojPolaganja];
		unsigned int l = 0;
		for (int i = 0; i < _trenutnoPolaganja; i++) {
			if (manjiIliJednakDatum(OD, *_polaganja[i]._datumPolaganja) && manjiIliJednakDatum(*_polaganja[i]._datumPolaganja, DO) &&
				_polaganja[i]._kurs->_razredi == razred)
				noviNiz[l++].Unos(*_polaganja[i]._polaznik, *_polaganja[i]._kurs, *_polaganja[i]._datumPolaganja, _polaganja[i]._ocjena);
		}
		return noviNiz;
	}
	void Dealociraj() {
		delete[] _nazivCentra; _nazivCentra = nullptr;
		for (int i = 0; i < _trenutnoKurseva; i++)
			_kursevi[i]->Dealociraj();
		for (int i = 0; i < _trenutnoPolaganja; i++)
			_polaganja[i].Dealociraj();
		delete[] _polaganja; _polaganja = nullptr;
	}
};

float PretragaRekurzivno(EdukacijskiCentar &mostar, int godina, enumPredmeti predmet, int brojacPolaganja = 0, float prosjecanUspjeh = 0, int brojacOcjena = 0) {
	if (brojacPolaganja >= mostar._trenutnoPolaganja) return prosjecanUspjeh / brojacOcjena;
	if(mostar._polaganja[brojacPolaganja]._kurs->_predmet == predmet && *mostar._polaganja[brojacPolaganja]._datumPolaganja->_godina == godina) {
		prosjecanUspjeh += mostar._polaganja[brojacPolaganja]._ocjena;
		brojacOcjena++;
		return PretragaRekurzivno(mostar, godina, predmet, brojacPolaganja + 1, prosjecanUspjeh, brojacOcjena);
	}
	return PretragaRekurzivno(mostar, godina, predmet, brojacPolaganja + 1, prosjecanUspjeh, brojacOcjena);
}

void main() {

	/*
	1. BROJ I TIPOVI PARAMETARA MORAJU BITI IDENTICNI ONIMA U TESTNOM CODE-U (OSIM UKOLIKO IMATE OPRAVDANU POTREBU ZA MODIFIKACIJOM). U SUPROTNOM SE RAD NEÆE BODOVATI
	2. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA.
	NE PREDAVATI .TXT ILI .CPP FAJLOVE. TAKOÐER, UKLONITE SVE DIJELOVE CODE-A KOJI NISU IMPLEMENTIRANI
	3. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
	4. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ÆETE KOPIRATI VAŠA RJEŠENJA)
	5. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
	*/

	Datum datum1, datum2, datum3, datum4, datum5, datum6;
	datum1.Unos(26, 11, 2015);
	datum2.Unos(29, 5, 2016);
	datum3.Unos(5, 6, 2016);
	datum4.Unos(15, 8, 2016);
	datum5.Unos(2, 6, 2016);
	datum6.Unos(22, 9, 2016);

	Kurs matematika, hemija, biologija, engleski;
	matematika.Unos(Matematika, III, datum1, datum2, "Nina Bijedic");
	hemija.Unos(Hemija, III, datum2, datum3, "Murat Praso");
	biologija.Unos(Biologija, IV, datum3, datum4, "Jasmin Azemovic");
	engleski.Unos(Engleski, II, datum3, datum4, "Iris Memic");

	Datum rodjenje1, rodjenje2;
	rodjenje1.Unos(22, 8, 1982);
	rodjenje2.Unos(13, 3, 7);
	Polaznik denis, zanin, indira;
	denis.Unos(rodjenje1, "Denis Music");
	zanin.Unos(rodjenje2, "Zanin Vejzovic");
	Polaznik emin;
	emin.Unos(rodjenje2, "Emin Hajdarevic");

	Polaganja denisMatematika, zaninHemija, denisBiologija;
	denisMatematika.Unos(denis, matematika, datum5, 3);
	//3 predstavlja ostvareni uspjeh/ocjenu, a uspjesno polozenim se smatra svaki kurs na kome je polaznik ostvario ocjenu veæu od 1
	zaninHemija.Unos(zanin, hemija, datum6, 93);
	Polaganja eminMatematika;
	eminMatematika.Unos(emin, matematika, datum5, 99);
	EdukacijskiCentar mostar;
	mostar.Unos("ECM - Edukacijski Center Mostar");
	//jedan edukacijski centar moze nuditi vise istih kurseva (npr. Matematika), ali se oni moraju realizovati u razlictim periodima 
	//(pocetak-kraj) tj. ne smiju se preklapati
	//novi kurs istog tipa (npr. Matematika) se mora realizovati nakon prethodno dodanog npr. ako prethodni kurs Matematika traje 01.03.2016 - 
	//01.04.2016, onda se novi kurs Matematika moze dodati jedino ako pocinje nakon 01.04.2016
	if (mostar.DodajKurs(matematika)) {
		cout << crt << "KURS USPJESNO REGISTROVAN" << crt;
		matematika.Ispis();
		cout << crt;
	}
	//nije moguce dodati polaganje onog kursa koji nije evidentiran (registrovan, ponudjen) u tom edukacijskom centru
	//kurseve je moguce najkasnije 15 dana od dana zavrsetka (kraja) posljednje dodanog kursa
	//jedan polaznik moze vise puta polagati isti kurs, ali nova ocjena mora biti veca od prethodne ocjene na tom istom kursu
	if (mostar.DodajPolaganje(denisMatematika)) {
		cout << crt << "POLAGANJE EVIDENTIRANO" << crt;
		denisMatematika.Ispis();
	}
	if (mostar.DodajPolaganje(eminMatematika)) {
		cout << crt << "POLAGANJE EVIDENTIRANO" << crt;
		eminMatematika.Ispis();
	}
	mostar.Ispis();
	Datum OD, DO;
	int brojPolaganja = 0;
	OD.Unos(1, 6, 2016);
	DO.Unos(1, 8, 2016);
	//PolaganjaByDatumIRazred - funkcija vraca niz uspjesnih polaganja koja su realizovana u periodu OD - DO, a odnose se na bilo koji predmet
	//u definisanom razredu (npr. II)
	Polaganja * polaganjaByDatumiRazred = mostar.PolaganjaByDatumIRazred(OD, DO, III, brojPolaganja);
	cout << "U periodu od ";
	OD.Ispis();
	cout << " do ";
	DO.Ispis();
	cout << " uspjesno je polozeno " << brojPolaganja << " predmeta --->>> ";
	for (size_t i = 0; i < brojPolaganja; i++)
		polaganjaByDatumiRazred[i].Ispis();

	//PretragaRekurzivno - rekurzivna funkcija koja pronalazi prosjecni uspjeh koji su polaznici tokom godine (npr.2016) ostvarili na 
	//odredjenom kursu (npr. Matematika)
	cout << "Prosjecan uspjeh na kursu iz predmeta Matematika u 2016 godini je " << PretragaRekurzivno(mostar, 2016, Matematika) << endl;
	//ispisuje sve informacije o edukacijskom centru. prije ispisa sortirati sve kurseve
	mostar.Ispis();

	//izvrsiti potrebne dealokacije
	datum1.Dealociraj(); datum2.Dealociraj(); datum3.Dealociraj(); datum4.Dealociraj(); datum5.Dealociraj(); datum6.Dealociraj(); rodjenje1.Dealociraj(); rodjenje2.Dealociraj();
	matematika.Dealociraj(); hemija.Dealociraj(); biologija.Dealociraj(); engleski.Dealociraj();
	denis.Dealociraj(); zanin.Dealociraj(); emin.Dealociraj();
	denisMatematika.Dealociraj(); zaninHemija.Dealociraj(); eminMatematika.Dealociraj();
	OD.Dealociraj(); DO.Dealociraj();
	mostar.Dealociraj();
	
	system("pause");
}