#include <iostream>
#include <cmath>

using namespace std;

//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

enum enumKursevi { HtmlCSSJavaScript, SoftwareEngeneeringFundamentals, MasteringSQL, WindowsSecurity };
const char * enumKurseviChar[] = { "HtmlCSSJavaScript", "SoftwareEngeneeringFundamentals", "MasteringSQL", "WindowsSecurity" };
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

int rataDie(int d, int m, int g) {
	if (m < 3)
		g--, m += 12;
	return 365 * g + g / 4 - g / 100 + g / 400 + (153 * m - 457) / 5 + d - 306;
}
bool manjiDatum(Datum d1, Datum d2) {
	if (*d1._godina < *d2._godina) return true;
	if (*d1._godina == *d2._godina && *d1._mjesec < *d2._mjesec) return true;
	if (*d1._godina == *d2._godina && *d1._mjesec == *d2._mjesec && *d1._dan < *d2._dan) return true;
	return false;
}

struct Kurs {
	enumKursevi _kurs;
	Datum _pocetak;
	Datum _kraj;
	char * _imePredavaca;
	bool _aktivan;//SVAKI KURS JE NA POCETKU AKTIVAN
	void Unos(enumKursevi kurs, Datum pocetak, Datum kraj, const char * imepredavaca) {
		_kurs = kurs;
		_pocetak.Unos(*pocetak._dan, *pocetak._mjesec, *pocetak._godina);
		_kraj.Unos(*kraj._dan, *kraj._mjesec, *kraj._godina);
		int len = strlen(imepredavaca) + 1;
		_imePredavaca = new char[len];
		strcpy_s(_imePredavaca, len, imepredavaca);
		_aktivan = true;
	}
	void Ispis() {
		cout << enumKurseviChar[_kurs] << " ";
		_pocetak.Ispis();
		_kraj.Ispis();
		cout << _imePredavaca << endl;
	}
	void Dealociraj() {
		_pocetak.Dealociraj();
		_kraj.Dealociraj();
		delete[] _imePredavaca; _imePredavaca = nullptr;
	}
};
struct Polaznik {
	int _polaznikID;
	char * _imePrezime;
	void Unos(int polaznikID, const char * imePrezime) {
		_polaznikID = polaznikID;
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
	}
	void Ispis() { cout << _polaznikID << " " << _imePrezime << endl; }
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
};
struct Polaganja {
	Polaznik _polaznik;
	Kurs _kurs;
	Datum _datumPolaganja;
	float _ostvareniUspjeh;
	void Unos(Polaznik &polaznik, Kurs &kurs, Datum datum, float uspjeh) {
		_polaznik.Unos(polaznik._polaznikID, polaznik._imePrezime);
		_kurs.Unos(kurs._kurs, kurs._pocetak, kurs._kraj, kurs._imePredavaca);
		_datumPolaganja.Unos(*datum._dan, *datum._mjesec, *datum._godina);
		_ostvareniUspjeh = uspjeh;
	}
	void Ispis() {
		_polaznik.Ispis();
		_kurs.Ispis();
		_datumPolaganja.Ispis();
		cout << _ostvareniUspjeh << endl;
	}
	void Dealociraj() {
		_polaznik.Dealociraj();
		_kurs.Dealociraj();
		_datumPolaganja.Dealociraj();
	}
};
struct SkillsCentar {
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
		_trenutnoPolaganja = 0;
		_polaganja = nullptr;
	}
	bool DodajKurs(Kurs &kurs) {
		bool identicanKurs = false;
		int indeksIdenticnogKursa = -1;
		for (int i = 0; i < _trenutnoKurseva; i++) {
			if (_kursevi[i]->_kurs == kurs._kurs) {
				identicanKurs = true;
				indeksIdenticnogKursa = i;
			}
		}
		if (identicanKurs) {
			if (!manjiDatum(_kursevi[indeksIdenticnogKursa]->_kraj, kurs._pocetak))
				return false;
		}
		_kursevi[_trenutnoKurseva] = new Kurs;
		_kursevi[_trenutnoKurseva]->Unos(kurs._kurs, kurs._pocetak, kurs._kraj, kurs._imePredavaca);
		_trenutnoKurseva++;
		return true;
	}
	void Ispis() {
		cout << _nazivCentra << endl;
		cout << crt << "KURSEVI" << endl;
		for (int i = 0; i < _trenutnoKurseva; i++)
			_kursevi[i]->Ispis();
		cout << crt << "POLAGANJA" << endl;
		for (int i = 0; i < _trenutnoPolaganja; i++)
			_polaganja[i].Ispis();
	}
	bool DodajPolaganje(Polaganja &polaganje) {
		bool found = false;
		int indeksKursa = -1;
		for (int i = 0; i < _trenutnoKurseva; i++) {
			if (polaganje._kurs._kurs == _kursevi[i]->_kurs) {
				found = true;
				indeksKursa = i;
			}
		}
		if (!found) return false;
		else {
			int datumPolaganje = rataDie(*polaganje._datumPolaganja._dan, *polaganje._datumPolaganja._mjesec, *polaganje._datumPolaganja._godina);
			int datumKursa = rataDie(*_kursevi[indeksKursa]->_kraj._dan, *_kursevi[indeksKursa]->_kraj._mjesec, *_kursevi[indeksKursa]->_kraj._godina);
			if (abs(datumPolaganje - datumKursa) > 5) return false;
		}
		bool ponovoPolaze = false;
		int indeksProslogPolaganje = -1;
		for (int i = 0; i < _trenutnoPolaganja; i++) {
			if (_polaganja[i]._polaznik._polaznikID == polaganje._polaznik._polaznikID && _polaganja[i]._kurs._kurs == polaganje._kurs._kurs) {
				ponovoPolaze = true;
				indeksProslogPolaganje = -1;
			}
		}
		if (ponovoPolaze) {
			if (_polaganja[indeksProslogPolaganje]._ostvareniUspjeh > polaganje._ostvareniUspjeh) return false;
		}
		Polaganja * temp = new Polaganja[_trenutnoPolaganja + 1];
		for (int i = 0; i < _trenutnoPolaganja; i++) {
			temp[i].Unos(_polaganja[i]._polaznik, _polaganja[i]._kurs, _polaganja[i]._datumPolaganja, _polaganja[i]._ostvareniUspjeh);
			_polaganja[i].Dealociraj();
		}
		temp[_trenutnoPolaganja].Unos(polaganje._polaznik, polaganje._kurs, polaganje._datumPolaganja, polaganje._ostvareniUspjeh);
		delete[] _polaganja; _polaganja = nullptr;
		_polaganja = temp;
		_trenutnoPolaganja++;
		return true;
	}
	Polaganja * PolaganjaByDatum(Datum OD, Datum DO, int &brojPolaganja) {
		for (int i = 0; i < _trenutnoPolaganja; i++) {
			if (manjiDatum(OD, _polaganja[i]._datumPolaganja) && manjiDatum(_polaganja[i]._datumPolaganja, DO))
				brojPolaganja++;
		}

		Polaganja * noviNiz = new Polaganja[brojPolaganja];
		unsigned int l = 0;
		for (int i = 0; i < _trenutnoPolaganja; i++) {
			if (manjiDatum(OD, _polaganja[i]._datumPolaganja) && manjiDatum(_polaganja[i]._datumPolaganja, DO)) {
				noviNiz[l++].Unos(_polaganja[i]._polaznik, _polaganja[i]._kurs, _polaganja[i]._datumPolaganja, _polaganja[i]._ostvareniUspjeh);
			}
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

float PretragaRekurzivno(SkillsCentar &mostar, enumKursevi kurs, int godina, int brojacPolaganja = 0, float sviUspjesi = 0, int brojacUspjeha = 0) {
	if (brojacPolaganja >= mostar._trenutnoPolaganja) return sviUspjesi / brojacUspjeha;
	if (mostar._polaganja[brojacPolaganja]._kurs._kurs == kurs && *mostar._polaganja[brojacPolaganja]._datumPolaganja._godina == godina) {
		sviUspjesi += mostar._polaganja[brojacPolaganja]._ostvareniUspjeh;
		brojacUspjeha++;
		return PretragaRekurzivno(mostar, kurs, godina, brojacPolaganja + 1, sviUspjesi, brojacUspjeha);
	}
	return PretragaRekurzivno(mostar, kurs, godina, brojacPolaganja + 1, sviUspjesi, brojacUspjeha);
}

void main() {

	/*
	1. BROJ I TIPOVI PARAMETARA MORAJU BITI IDENTICNI ONIMA U TESTNOM CODE-U (OSIM UKOLIKO IMATE OPRAVDANU POTREBU ZA MODIFIKACIJOM). U SUPROTNOM SE RAD NEÆE BODOVATI
	2. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE. TAKOÐER, UKLONITE SVE DIJELOVE CODE-A KOJI NISU IMPLEMENTIRANI
	3. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
	4. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ÆETE KOPIRATI VAŠA RJEŠENJA)
	5. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
	*/

	Datum datum1, datum2, datum3, datum4, datum5, datum6;
	datum1.Unos(26, 11, 2015);
	datum2.Unos(29, 5, 2016);
	datum3.Unos(5, 6, 2016);
	datum4.Unos(15, 8, 2016);
	datum5.Unos(13, 7, 2016);
	datum6.Unos(22, 9, 2016);
	Datum datumEmin;
	datumEmin.Unos(19, 8, 2016);
	Kurs softverski, html, sql, security;
	softverski.Unos(SoftwareEngeneeringFundamentals, datum1, datum2, "Emina Junuz");
	html.Unos(HtmlCSSJavaScript, datum2, datum3, "Larisa Tipura");
	sql.Unos(MasteringSQL, datum3, datum4, "Jasmin Azemovic");
	security.Unos(WindowsSecurity, datum3, datum4, "Adel Handzic");

	Polaznik denis, zanin, indira;
	denis.Unos(1, "Denis Music");
	zanin.Unos(2, "Zanin Vejzovic");
	indira.Unos(3, "Indira Hamulic");
	Polaganja denisHtml, zaninHtml, indiraSql;
	denisHtml.Unos(denis, html, datum5, 61);
	//61 predstavlja ostvareni uspjeh/procenat, a uspjesno polozenim se smatra svaki kurs na kome je polaznik ostvari vise od 55%
	zaninHtml.Unos(zanin, html, datum6, 93);

	Polaganja denisSQL;
	denisSQL.Unos(denis, sql, datumEmin, 61);
	SkillsCentar mostar;
	mostar.Unos("Skills Center Mostar");
	//jedan edukacijski centar moze nuditi vise istih kurseva (npr. MasteringSQL), ali se oni moraju realizovati u razlictim periodima 
	//(pocetak-kraj) tj. ne smiju se preklapati
	//novi kurs istog tipa (npr. MasteringSQL) se mora realizovati nakon prethodno dodanog npr. ako prethodni kurs MasteringSQL traje 
	//01.03.2016 - 01.04.2016, onda se novi kurs MasteringSQL moze dodati jedino ako pocinje nakon 01.04.2016
	if (mostar.DodajKurs(softverski)) {
		cout << crt << "KURS USPJESNO REGISTROVAN" << crt;
		softverski.Ispis();
		cout << crt;
	}
	if (mostar.DodajKurs(sql)) {
		cout << crt << "KURS USPJESNO REGISTROVAN" << crt;
		sql.Ispis();
		cout << crt;
	}
	//nije moguce dodati polaganje onog kursa koji nije evidentiran (registrovan, ponudjen) u tom edukacijskom centru
	//moguce je polagati samo aktivne kurseve i to najkasnije 5 dana od dana zavrsetka (kraja) posljednje dodanog (aktivnog) kursa iste vrste 
	//(npr. MasteringSQL)
	//jedan polaznik moze vise puta polagati isti kurs, ali novi procenat mora biti veci od prethodno dodanog (na istom kursu)
	if (mostar.DodajPolaganje(denisHtml)) {
		cout << crt << "POLAGANJE EVIDENTIRANO" << crt;
		denisHtml.Ispis();
	}
	if (mostar.DodajPolaganje(denisSQL)) {
		cout << crt << "POLAGANJE EVIDENTIRANO" << crt;
		denisSQL.Ispis();
	}
	Datum OD, DO;
	int brojPolaganja = 0;
	OD.Unos(1, 6, 2016);
	DO.Unos(1, 8, 2016);
	//PolaganjaByDatum – vraca niz polaganja koja su uspjesno realizovana u periodu OD-DO
	Polaganja * polaganjaByDatum = mostar.PolaganjaByDatum(OD, DO, brojPolaganja);
	cout << "U periodu od ";
	OD.Ispis();
	cout << " do ";
	DO.Ispis();
	cout << " uspjesno je realizovano " << brojPolaganja << " polaganja--->>>";
	for (size_t i = 0; i < brojPolaganja; i++)
		polaganjaByDatum[i].Ispis();

	//PretragaRekurzivno - rekurzivna funkcija pronalazi prosjecni uspjeh koji su polaznici tokom godine (npr.2016) ostvarili na odredjenom 
	//kursu (npr. MasteringSQL)
	cout << "Prosjecan uspjeh na kursu MasteringSQL u 2016 godini je " << PretragaRekurzivno(mostar, MasteringSQL, 2016) << endl;
	//Ispis - ispisuje sve informacije o edukacijskom centru. prije ispisa sortirati sve kurseve na osnovu pocetka odrzavanja kursa
	mostar.Ispis();

	//izvrsiti potrebne dealokacije
	datum1.Dealociraj(); datum2.Dealociraj(); datum3.Dealociraj(); datum4.Dealociraj(); datum5.Dealociraj(); datum6.Dealociraj(); datumEmin.Dealociraj();
	softverski.Dealociraj(); html.Dealociraj(); sql.Dealociraj(); security.Dealociraj();
	denis.Dealociraj(); zanin.Dealociraj(); indira.Dealociraj();
	denisHtml.Dealociraj(); zaninHtml.Dealociraj(); denisSQL.Dealociraj();
	OD.Dealociraj(); DO.Dealociraj();
	mostar.Dealociraj();
	system("pause");
}