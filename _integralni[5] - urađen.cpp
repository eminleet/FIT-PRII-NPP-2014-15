#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

enum enumKursevi { HtmlCSSJavaScript, SoftwareEngeneeringFundamentals, MasteringSQL, WindowsSecurity };
const char * enumKurseviChar[] = { "HtmlCSSJavaScript", "SoftwareEngeneeringFundamentals", "MasteringSQL", "WindowsSecurity" };
const char * crt = "\n--------------------------------------------\n";

struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina; }
	void Dealociraj() { delete _dan; delete _mjesec; delete _godina; _dan = _mjesec = _godina = nullptr; }
};

struct Kurs {
	enumKursevi _kurs;
	Datum * _pocetak;
	Datum * _kraj;
	char * _imePredavaca;
	float _cijenaKursa;

	void Unos(enumKursevi kurs, Datum pocetak, Datum kraj, const char * imePredavaca, float cijenaKursa) {
		_kurs = kurs;
		_pocetak = new Datum;
		_pocetak->Unos(*pocetak._dan, *pocetak._mjesec, *pocetak._godina);
		_kraj = new Datum;
		_kraj->Unos(*kraj._dan, *kraj._mjesec, *kraj._godina);
		int size = strlen(imePredavaca) + 1;
		_imePredavaca = new char[size];
		strcpy_s(_imePredavaca, size, imePredavaca);
		_cijenaKursa = cijenaKursa;
	}
	void Ispis() {
		cout << enumKurseviChar[_kurs] << " " << _imePredavaca << " " << _cijenaKursa << " (";
		_pocetak->Ispis(); cout << " - "; _kraj->Ispis(); cout << ")" << endl;
	}
	void Dealociraj() {
		delete[] _imePredavaca; _imePredavaca = nullptr;
		_pocetak->Dealociraj();
		_kraj->Dealociraj();
		delete _pocetak; _pocetak = nullptr;
		delete _kraj; _kraj = nullptr;
		_cijenaKursa = 0;
	}
};
struct Polaznik {
	char * _imePrezime;
	Datum * _datumRodjenja;
	void Unos(const char * imePrezime, Datum datumRodjenja) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		_datumRodjenja = new Datum;
		_datumRodjenja->Unos(*datumRodjenja._dan, *datumRodjenja._mjesec, *datumRodjenja._godina);
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		_datumRodjenja->Dealociraj();
		delete _datumRodjenja; _datumRodjenja = nullptr;
	}
	void Ispis() { cout << _imePrezime << " ("; _datumRodjenja->Ispis(); cout << ")" << endl; }
};
struct Prijava {
	Polaznik * _polaznik; //prijava moze biti pojedinacna ili grupna
	float * _uplaceniIznos;
	/*za svakog polaznika ce se cuvati podatak o iznosu uplate. uplaceni iznos svakog pojedinog polaznika treba biti jednak punoj cijeni
	kursa osim u sljedecim slucajevima:
	1. prijave 2 ili 3 polaznika dobijaju 10% popusta na ukupnu cijenu kursa
	2. prijave za vise od 3 polaznika dobijaju 15% na ukupnu cijenu kursa
	*/
	int _trenutnoPolaznika;
	Kurs * _kurs; //prijava treba cuvati adresu (pokazivati) na postojeci kurs, a ne kreirati novi
	void Unos(Kurs * kurs) {
		_polaznik = nullptr;
		_uplaceniIznos = nullptr;
		_trenutnoPolaznika = 0;
		_kurs = kurs;
	}
	void Ispis() {
		cout << crt << "Kurs: ";
		_kurs->Ispis();
		for (int i = 0; i < _trenutnoPolaznika; i++)
			_polaznik[i].Ispis();
	}
	bool DodajPolaznika(Polaznik polaznik, float uplaceniIznos) {
		double potrebanIznos = _kurs->_cijenaKursa;
		if (_trenutnoPolaznika == 2 || _trenutnoPolaznika == 3) potrebanIznos -= potrebanIznos * 0.1;
		if (_trenutnoPolaznika > 3) potrebanIznos -= potrebanIznos * 0.15;
		potrebanIznos /= 2.;
		if (uplaceniIznos < potrebanIznos) return false;
		for (int i = 0; i < _trenutnoPolaznika; i++)
			if (strcmp(_polaznik[i]._imePrezime, polaznik._imePrezime) == 0)
				return false;

		float * tempFloat = new float[_trenutnoPolaznika + 1];
		Polaznik * temp = new Polaznik[_trenutnoPolaznika + 1];
		for (int i = 0; i < _trenutnoPolaznika; i++) {
			tempFloat[i] = _uplaceniIznos[i];
			temp[i].Unos(_polaznik[i]._imePrezime, *_polaznik[i]._datumRodjenja);
			_polaznik[i].Dealociraj();
		}
		tempFloat[_trenutnoPolaznika] = uplaceniIznos;
		temp[_trenutnoPolaznika].Unos(polaznik._imePrezime, *polaznik._datumRodjenja);
		_trenutnoPolaznika++;
		delete[] _uplaceniIznos; _uplaceniIznos = nullptr;
		delete[] _polaznik; _polaznik = nullptr;
		_uplaceniIznos = tempFloat;
		_polaznik = temp;
		return true;
	}
	void Dealociraj() {
		_polaznik->Dealociraj(); delete _polaznik; _polaznik = nullptr;
		delete[] _uplaceniIznos; _uplaceniIznos = nullptr;
		_kurs = nullptr;
		_trenutnoPolaznika = 0;
	}
	bool DodajUplatu(Polaznik polaznik, float uplaceniIznos) {
		int index = -1;
		for (int i = 0; i < _trenutnoPolaznika; i++)
			if (strcmp(_polaznik[i]._imePrezime, polaznik._imePrezime) == 0)
				index = i;

		if (index == -1) return false;
		_uplaceniIznos[index] += uplaceniIznos;
		return true;

	}
	float StatusUplate(Polaznik polaznik) {
		int index = -1;
		for (int i = 0; i < _trenutnoPolaznika; i++)
			if (strcmp(polaznik._imePrezime, _polaznik[i]._imePrezime) == 0)
				index = i;
		float razlika = _kurs->_cijenaKursa - _uplaceniIznos[index];
		if (razlika > 0) return -razlika;
		return razlika;
	}
};

int rataDie(int d, int m, int g) {
	if (m < 3)
		g--, m += 12;
	return 365 * g + g / 4 - g / 100 + g / 400 + (153 * m - 457) / 5 + d - 306;
}

double potrebanIznos(Prijava p) {
	double potrebanIznos = 0.0;
	if (p._trenutnoPolaznika == 2 || p._trenutnoPolaznika == 3)
		potrebanIznos = p._kurs->_cijenaKursa - (p._kurs->_cijenaKursa * 0.1);
	else if (p._trenutnoPolaznika > 3)
		potrebanIznos = p._kurs->_cijenaKursa - (p._kurs->_cijenaKursa * 0.15);
	return potrebanIznos;
}

struct SkillsCentar {
	char * _nazivCentra;
	Kurs * _kursevi[50];//JEDAN EDUKACIJSKI CENTAR MOZE NUDITI NAJVISE 50 KURSEVA
	int _trenutnoKurseva;
	Prijava * _prijave;
	int _trenutnoPrijava;
	void Unos(const char * nazivCentra) {
		int len = strlen(nazivCentra) + 1;
		_nazivCentra = new char[len];
		strcpy_s(_nazivCentra, len, nazivCentra);
		_trenutnoKurseva = 0;
		_prijave = nullptr;
		_trenutnoPrijava = 0;
	}
	void Dealociraj() {
		delete[] _nazivCentra; _nazivCentra = nullptr;
		for (int i = 0; i < _trenutnoKurseva; i++) {
			_kursevi[i]->Dealociraj();
			delete _kursevi[i]; _kursevi[i] = nullptr;
		}
		for (int i = 0; i < _trenutnoPrijava; i++)
			_prijave[i].Dealociraj();
		delete[] _prijave; _prijave = nullptr;
		_trenutnoKurseva = 0; _trenutnoPrijava = 0;
	}
	bool DodajKurs(Kurs kurs) {
		int index = -1;
		for (int i = 0; i < _trenutnoKurseva; i++)
			if (kurs._kurs == _kursevi[i]->_kurs)
				index = i;
		if (index != -1) {
			int brDanaTrenutnog = rataDie(*_kursevi[index]->_kraj->_dan, *_kursevi[index]->_kraj->_mjesec, *_kursevi[index]->_kraj->_godina);
			int brDanaNovog = rataDie(*kurs._pocetak->_dan, *kurs._pocetak->_mjesec, *kurs._pocetak->_godina);
			if (brDanaNovog - brDanaTrenutnog < 15) return false;
		}
		_kursevi[_trenutnoKurseva] = new Kurs;
		_kursevi[_trenutnoKurseva]->Unos(kurs._kurs, *kurs._pocetak, *kurs._kraj, kurs._imePredavaca, kurs._cijenaKursa);
		_trenutnoKurseva++;
		return true;
	}
	bool DodajPrijavu(Prijava prijava) {
		bool found = false;
		for (int i = 0; i < _trenutnoKurseva; i++)
			if (prijava._kurs->_kurs == _kursevi[i]->_kurs)
				found = true;
		if (!found) return false;

		float ukupnoPlaceno = 0.0;
		for (int i = 0; i < prijava._trenutnoPolaznika; i++)
			ukupnoPlaceno += prijava._uplaceniIznos[i];

		double potrebanIznosUkupno = potrebanIznos(prijava);
		if (ukupnoPlaceno <= (potrebanIznosUkupno * 0.8))
			return false;

		Prijava * temp = new Prijava[_trenutnoPrijava + 1];
		for (int i = 0; i < _trenutnoPrijava; i++) {
			temp[i].Unos(_prijave[i]._kurs);
			for (int j = 0; j < _prijave[i]._trenutnoPolaznika; j++)
				temp[i].DodajPolaznika(_prijave[i]._polaznik[j], _prijave[i]._uplaceniIznos[j]);
		}
		temp[_trenutnoPrijava].Unos(prijava._kurs);
		for (int i = 0; i < prijava._trenutnoPolaznika; i++)
			temp[_trenutnoPrijava].DodajPolaznika(prijava._polaznik[i], prijava._uplaceniIznos[i]);

		if (_prijave != nullptr) {
			_prijave->Dealociraj(); delete[] _prijave; _prijave = nullptr;
		}

		_prijave = temp;
		_trenutnoPrijava++;
		return true;

	}
	void Ispis() {
		cout << crt << "Naziv centra: " << _nazivCentra << endl;
		cout << crt << ":: KURSEVI " << endl;
		for (int i = 0; i < _trenutnoKurseva; i++)
			_kursevi[i]->Ispis();
		cout << crt << ":: PRIJAVE " << endl;
		for (int i = 0; i < _trenutnoPrijava; i++)
			_prijave[i].Ispis();
	}
	Polaznik * PolazniciDugovanja(int &brPolaznika) {
		for (int i = 0; i < _trenutnoPrijava; i++)
			for (int j = 0; j < _prijave[i]._trenutnoPolaznika; j++)
				if (_prijave[i].StatusUplate(_prijave[i]._polaznik[j]) < 0)
					brPolaznika++;
		Polaznik * noviNiz = new Polaznik[brPolaznika];

		unsigned int l = 0;
		for (int i = 0; i < _trenutnoPrijava; i++)
			for (int j = 0; j < _prijave[i]._trenutnoPolaznika; j++)
				if (_prijave[i].StatusUplate(_prijave[i]._polaznik[j]) < 0)
					noviNiz[l++].Unos(_prijave[i]._polaznik[j]._imePrezime, *_prijave[i]._polaznik[j]._datumRodjenja);
		return noviNiz;
	}
	void Sortiraj() {
		for (int i = 0; i < _trenutnoPrijava; i++) {
			int brProlaza = _prijave[i]._trenutnoPolaznika - 1;
			bool promjena = true;
			while (promjena) {
				promjena = false;
				for (int j = 0; j < brProlaza; j++) {
					if (_prijave[i].StatusUplate(_prijave[i]._polaznik[j]) > _prijave[i].StatusUplate(_prijave[i]._polaznik[j + 1])) {
						swap(_prijave[i]._polaznik[j], _prijave[i]._polaznik[j + 1]);
						promjena = true;
					}
				}
				brProlaza--;
			}
		}
	}
};

double PretragaRekurzivno(SkillsCentar centar, int brojacPrijava = 0, int brojacPolaznika = 0, double maxDug = 0) {
	if (brojacPrijava >= centar._trenutnoPrijava) return maxDug;
	if (brojacPolaznika >= centar._prijave[brojacPrijava]._trenutnoPolaznika)
		return PretragaRekurzivno(centar, brojacPrijava + 1, 0, maxDug);
	if (centar._prijave[brojacPrijava].StatusUplate(centar._prijave[brojacPrijava]._polaznik[brojacPolaznika]) < maxDug) {
		maxDug = centar._prijave[brojacPrijava].StatusUplate(centar._prijave[brojacPrijava]._polaznik[brojacPolaznika]);
		return PretragaRekurzivno(centar, brojacPrijava, brojacPolaznika + 1, maxDug);
	}
	return PretragaRekurzivno(centar, brojacPrijava, brojacPolaznika + 1, maxDug);
}

int main() {

	/*
	1. BROJ I TIPOVI PARAMETARA MORAJU BITI IDENTICNI ONIMA U TESTNOM CODE-U (OSIM UKOLIKO IMATE OPRAVDANU POTREBU ZA MODIFIKACIJOM). U SUPROTNOM SE RAD NEÆE BODOVATI
	2. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA.
	NE PREDAVATI .TXT ILI .CPP FAJLOVE. TAKOÐER, UKLONITE SVE DIJELOVE CODE-A KOJI NISU IMPLEMENTIRANI
	3. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
	4. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ÆETE KOPIRATI VAŠA RJEŠENJA)
	5. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
	*/

	Datum datum1, datum2, datum3, datum4, datum5, datum6, rodjendan;
	datum1.Unos(26, 11, 2015);
	datum2.Unos(29, 5, 2016);
	datum3.Unos(5, 6, 2016);
	datum4.Unos(15, 8, 2016);
	datum5.Unos(13, 7, 2016);
	datum6.Unos(22, 9, 2016);
	rodjendan.Unos(15, 6, 1995);

	Kurs softverski, html, sql, security;
	softverski.Unos(SoftwareEngeneeringFundamentals, datum1, datum2, "Emina Junuz", 1800);
	html.Unos(HtmlCSSJavaScript, datum2, datum3, "Larisa Tipura", 1000);
	sql.Unos(MasteringSQL, datum3, datum4, "Jasmin Azemovic", 2230);
	security.Unos(WindowsSecurity, datum3, datum4, "Adel Handzic", 4000);

	Polaznik denis, zanin, indira, dubravka;
	denis.Unos("Denis Music", rodjendan);
	zanin.Unos("Zanin Vejzovic", rodjendan);
	indira.Unos("Indira Hamulic", rodjendan);
	dubravka.Unos("Dubravka Plosko", rodjendan);


	Prijava denisZaninDubravkaHtml, zaninIndiraHtml, indiraSql;
	denisZaninDubravkaHtml.Unos(&html);
	//700 predstavlja uplatu polaznika koji se prijavljuje na kurs
	//tokom prijave polaznika nije potrebno uplatiti kompletan iznos, ali on mora biti veci od 50% iznosa kojeg je u tom momentu 
	//potrebno platiti (u zavisnosti od broja prijavljenih tj. ostvarenog popusta)
	if (denisZaninDubravkaHtml.DodajPolaznika(denis, 700))
		cout << "Polaznik uspjesno dodan!" << endl;
	if (denisZaninDubravkaHtml.DodajPolaznika(zanin, 650))
		cout << "Polaznik uspjesno dodan!" << endl;
	if (denisZaninDubravkaHtml.DodajPolaznika(zanin, 650))//onemoguciti dupliranje polaznika na istoj prijavi
		cout << "Polaznik uspjesno dodan!" << endl;
	if (denisZaninDubravkaHtml.DodajPolaznika(dubravka, 430))//uplata ne zadovoljava kriterije
		cout << "Polaznik uspjesno dodan!" << endl;

	if (denisZaninDubravkaHtml.DodajUplatu(zanin, 200))//polaznik mora biti prethodno dodan prijavi da bi mogao dodati novu uplatu
		cout << "Uplata uspjesno dodana" << endl;

	zaninIndiraHtml.Unos(&html);
	if (zaninIndiraHtml.DodajPolaznika(zanin, 500))
		cout << "Polaznik uspjesno dodan!" << endl;
	if (zaninIndiraHtml.DodajPolaznika(indira, 750))
		cout << "Polaznik uspjesno dodan!" << endl;

	//ako polaznik duguje novca funkcija vraca negativan iznos potrebne uplate
	float stanjeUplate = denisZaninDubravkaHtml.StatusUplate(zanin);
	cout << "Polaznik "; zanin.Ispis();
	if (stanjeUplate > 0)
		cout << " je u pretplati " << stanjeUplate << " KM" << endl;
	else
		cout << " je duzan uplatiti " << stanjeUplate * -1 << " KM" << endl;

	SkillsCentar mostar;
	mostar.Unos("Skills Center Mostar");
	//jedan edukacijski centar moze moze nuditi vise istih kurseva (npr. MasteringSQL), ali novi kurs moze poceti tek 15 dana nakon zavrsetka
	//prethodnog kursa npr.
	//ako prethodni kurs MasteringSQL traje 01.03.2016 - 01.04.2016, onda se novi kurs MasteringSQL moze poceti nakon 15.04.2016
	if (mostar.DodajKurs(softverski)) {
		cout << crt << "KURS USPJESNO REGISTROVAN" << crt;
		softverski.Ispis();
		cout << crt;
	}
	if (mostar.DodajKurs(html)) {
		cout << crt << "KURS USPJESNO REGISTROVAN" << crt;
		html.Ispis();
		cout << crt;
	}
	//nije moguce dodati prijavu onog kursa koji nije evidentiran (registrovan, ponudjen) u tom edukacijskom centru
	//moguce je dodati samo one prijave koje kod kojih ukupna uplata iznosi vise od 80% cijene kursa (za prijavljeni broj polaznika)
	if (mostar.DodajPrijavu(denisZaninDubravkaHtml)) {
		cout << crt << "PRIJAVA EVIDENTIRANA" << crt;
		denisZaninDubravkaHtml.Ispis();
	}
	if (mostar.DodajPrijavu(zaninIndiraHtml)) {
		cout << crt << "PRIJAVA EVIDENTIRANA" << crt;
		zaninIndiraHtml.Ispis();
	}
	int brojPolaznika = 0;
	//vraca niz polaznika koji nisu izmirili uplate
	Polaznik * polazniciDugovanja = mostar.PolazniciDugovanja(brojPolaznika);

	cout << crt << "Pronadjeno " << brojPolaznika << " polaznika koji nisu kompletirali svoje uplate --->>> " << endl;
	for (size_t i = 0; i < brojPolaznika; i++)
		polazniciDugovanja[i].Ispis();

	//PretragaRekurzivno - rekurzivna funkcija pronalazi najveci iznos duga ostvarenog u odredjenom edukacijskom centru
	cout << crt << "Najveci dug iznosi " << PretragaRekurzivno(mostar) << endl;
	//ispisuje sve informacije o edukacijskom centru. prije ispisa sortirati sve prijave po visini dugovanja njenih polaznika
	mostar.Sortiraj();
	mostar.Ispis();

	//izvrsiti potrebne dealokacije
	datum1.Dealociraj(); datum2.Dealociraj(); datum3.Dealociraj(); datum4.Dealociraj(); datum5.Dealociraj(); datum6.Dealociraj();
	rodjendan.Dealociraj();
	softverski.Dealociraj(); html.Dealociraj(); sql.Dealociraj(); security.Dealociraj();
	denis.Dealociraj(); zanin.Dealociraj(); indira.Dealociraj(); dubravka.Dealociraj();
	denisZaninDubravkaHtml.Dealociraj(); zaninIndiraHtml.Dealociraj();
	mostar.Dealociraj();
	polazniciDugovanja->Dealociraj();

	system("PAUSE > NULL");
}