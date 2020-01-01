#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

const char * crt = "\n----------------------------------------------------\n";
struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() {
		delete _dan; delete _mjesec; delete _godina;
		_dan = _mjesec = _godina = nullptr;
	}

};
bool manjiDatum(Datum d1, Datum d2) {
	if (*d1._godina < *d2._godina) return true;
	if (*d1._godina == *d2._godina && *d1._mjesec < *d2._mjesec) return true;
	if (*d1._godina == *d2._godina && *d1._mjesec == *d2._mjesec && *d1._dan < *d2._dan) return true;
	return false;
}
bool istiDatum(Datum d1, Datum d2) {
	if (*d1._godina == *d2._godina && *d1._mjesec && *d2._mjesec && *d1._dan == *d2._dan) return true;
	return false;
}
struct Izostanak {
	char * _razlogIzostanka;
	Datum _datumIzostanka;
	int _brojSati;
	bool _opravdano;//pravdanje zahtijeva da se evidentira i razlog izostanka	
	void Unos(Datum datumIzostanka, int brojSati) {
		_datumIzostanka.Unos(*datumIzostanka._dan, *datumIzostanka._mjesec, *datumIzostanka._godina);
		_brojSati = brojSati;
		_opravdano = false;
		_razlogIzostanka = new char;
		_razlogIzostanka[0] = '\0';
	}
	void Dealociraj() { _datumIzostanka.Dealociraj();  delete[] _razlogIzostanka; _razlogIzostanka = nullptr; }
	void Ispis() { _datumIzostanka.Ispis(); cout << _razlogIzostanka; }
	void Opravdaj(const char * razlogIzostanka) {
		if (_razlogIzostanka != nullptr) {
			delete[] _razlogIzostanka;
			_razlogIzostanka = nullptr;
		}
		int vel = strlen(razlogIzostanka) + 1;
		_razlogIzostanka = new char[vel];
		strcpy_s(_razlogIzostanka, vel, razlogIzostanka);
		_opravdano = true;
	}
};
void KopirajIzostanak(Izostanak & source, Izostanak & dest) {
	int len = strlen(source._razlogIzostanka) + 1;
	dest._razlogIzostanka = new char[len];
	strcpy_s(dest._razlogIzostanka, len, source._razlogIzostanka);
	delete[] source._razlogIzostanka; source._razlogIzostanka = nullptr;
	dest._datumIzostanka.Unos(*source._datumIzostanka._dan, *source._datumIzostanka._mjesec, *source._datumIzostanka._godina);
	source._datumIzostanka.Dealociraj();
	dest._brojSati = source._brojSati;
	dest._opravdano = source._opravdano;
}
struct Student {
	//format broja indeksa: IB150051
	//osigurati automatsko generisanje broja indeksa pocevsi od IB150001
	char _brojIndeksa[10];
	char * _imePrezime;
	Izostanak * _izostanci;
	int _brojIzostanaka;
	void Unos(char * brIndeksa, const char * imePrezime) {
		strcpy_s(_brojIndeksa, 9, brIndeksa);
		int len = strlen(imePrezime) + 1;
		_imePrezime = new char[len];
		strcpy_s(_imePrezime, len, imePrezime);
		_izostanci = nullptr;
		_brojIzostanaka = 0;
	}
	void Ispis() {
		cout << _brojIndeksa << " " << _imePrezime << endl;
		for (int i = 0; i < _brojIzostanaka; i++)
			_izostanci[i].Ispis();
	}
	void DodajIzostanak(Izostanak &izostanak) {
		for (int i = 0; i < _brojIzostanaka; i++) {
			if (istiDatum(izostanak._datumIzostanka, _izostanci[i]._datumIzostanka))
				_izostanci[i]._brojSati += izostanak._brojSati;
		}
		Izostanak * temp = new Izostanak[_brojIzostanaka + 1];
		for (int i = 0; i < _brojIzostanaka; i++) {
			KopirajIzostanak(_izostanci[i], temp[i]);
		}
		KopirajIzostanak(izostanak, temp[_brojIzostanaka]);
		delete[] _izostanci; _izostanci = nullptr;
		_izostanci = temp;
		_brojIzostanaka++;
	}
	Izostanak * BinarnaPretraga(Datum &datum) {
		int prvi = 0;
		int zadnji = _brojIzostanaka - 1;
		bool found = false;
		int srednji = 0;
		while (prvi <= zadnji && !found) {
			srednji = (prvi + zadnji) / 2;
			if (istiDatum(_izostanci[srednji]._datumIzostanka, datum))
				return &_izostanci[srednji];
			if (manjiDatum(datum, _izostanci[srednji]._datumIzostanka))
				prvi = srednji + 1;
			if (manjiDatum(_izostanci[srednji]._datumIzostanka, datum))
				zadnji = srednji - 1;
		}
		return nullptr;
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (int i = 0; i < _brojIzostanaka; i++)
			_izostanci[i].Dealociraj();
		if (_izostanci != nullptr) {
			delete[] _izostanci; _izostanci = nullptr;
		}
		_brojIzostanaka = 0;
	}
	//Potpisi funkcija trebaju odgovarati onima u main-u
	//1. Unos
	//2. Dealociraj
	/*3. DodajIzostanak - dodaje novi izostanak u listu izostanaka.	Onemoguciti ponavljanje izostanaka na nacin da se uvecava samo broj sati 
	ukoliko*/
	//4. Ispis - ispisuje izostanke u formatu prikazanom na slici (nije obavezno da ispis bude identican, ali je svakako pozeljno). Prilikom svakog ispisa, izostanke je potrebno spasiti u tekstualni fajl sa istim nazivom kao i broj indeksa.
	//5. BinarnaPretraga - koristeci binarnu pretragu, na osnovu datuma, pronalazi i vraca pokazivac na izostanak. Ukoliko trazeni izostanak 
	//ne postoji funkcija vraca nullptr.

};

int defaultIndeks = 170000;
char * GenerisiSljedeciBrojIndeksa() {
	const int maxlen = 10;
	char * noviIndeks = new char[maxlen];
	char temp[maxlen] = "";
	strcpy_s(noviIndeks, maxlen, "IB");
	_itoa_s(defaultIndeks, temp, 10);
	strcat_s(noviIndeks, maxlen, temp);
	defaultIndeks++;
	return noviIndeks;
}

Izostanak * PronadjiNajveciNeopravdaniIzostanak(Student * student, int brojacIzostanaka = 0, int indeksNajvecegIzostanka = 0, int najveciIzostanak = 0) {
	if (brojacIzostanaka >= student->_brojIzostanaka) return &student->_izostanci[indeksNajvecegIzostanka];
	if (student->_izostanci[brojacIzostanaka]._opravdano == false && student->_izostanci[brojacIzostanaka]._brojSati > najveciIzostanak) {
		najveciIzostanak = student->_izostanci[brojacIzostanaka]._brojSati;
		indeksNajvecegIzostanka = brojacIzostanaka;
		return PronadjiNajveciNeopravdaniIzostanak(student, brojacIzostanaka + 1, indeksNajvecegIzostanka, najveciIzostanak);
	}
	return PronadjiNajveciNeopravdaniIzostanak(student, brojacIzostanaka + 1, indeksNajvecegIzostanka, najveciIzostanak);
}

//1. PronadjiNajveciNeopravdaniIzostanak - rekurzivna funkcija koja vraca pokazivac na neopravdani izostanak koji ima najveci broj 
//2. GenerisiSljedeciBrojIndeksa - generise i vraca naredni broj indeksa


void main() {
	Datum jucer, prije5Dana, prije10Dana;
	jucer.Unos(12, 6, 2016);
	prije5Dana.Unos(8, 6, 2016);
	prije10Dana.Unos(3, 6, 2016);

	Student denis;
	denis.Unos(GenerisiSljedeciBrojIndeksa(), "Denis Music");

	Izostanak izostanakJucer, izostanakPrije5Dana, izostanakPrije10Dana;

	izostanakJucer.Unos(jucer, 5);
	denis.DodajIzostanak(izostanakJucer);

	izostanakPrije5Dana.Unos(prije5Dana, 3);
	izostanakPrije5Dana.Opravdaj("Odsutan zbog bolesti - gripa");
	denis.DodajIzostanak(izostanakPrije5Dana);
	izostanakPrije5Dana.Unos(prije5Dana, 2);
	denis.DodajIzostanak(izostanakPrije5Dana);

	izostanakPrije10Dana.Unos(prije10Dana, 1);
	denis.DodajIzostanak(izostanakPrije10Dana);
	izostanakPrije10Dana.Unos(prije10Dana, 1);
	denis.DodajIzostanak(izostanakPrije10Dana);

	denis.Ispis();
	Izostanak * p = denis.BinarnaPretraga(jucer);
	cout << "Binarnom pretragom pronadjen izostanak -> ";
	p->Ispis();
	denis.Ispis();
	Izostanak * p2 = PronadjiNajveciNeopravdaniIzostanak(&denis);
	cout << "Rekurzivno pronadjen najveci neopravdani izostanak -> ";
	p2->Ispis();
	cout << endl;
	////dealocirati zauzetu memoriju
	jucer.Dealociraj();
	prije5Dana.Dealociraj();
	prije10Dana.Dealociraj();
	izostanakJucer.Dealociraj();
	izostanakPrije5Dana.Dealociraj();
	izostanakPrije10Dana.Dealociraj();
	denis.Dealociraj();
	system("pause");
}