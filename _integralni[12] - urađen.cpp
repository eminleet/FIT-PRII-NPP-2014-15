#include<iostream>
#include<memory>
#include <tuple>
using namespace std;
/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NECE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB160061.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI CETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URA?ENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)


enum VrstaAktivnosti { Poligon, Gradska, Nocna, Parkiranje };
const char * VrstaAktivnostiChar[] = { "Poligon", "Gradska", "Nocna", "Parkiranje" };
enum Kategorija { A, B, C, D, E };
const char * KategorijaChar[] = { "A", "B", "C", "D", "E" };
const int maxKategorija = 5;

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
bool istiDatum(Datum d1, Datum d2) {
	if (*d1._dan == *d2._dan && *d1._mjesec == *d2._mjesec && *d1._godina == *d2._godina) return true;
	return false;
}
int rataDie(int d, int m, int g) {
	if (m < 3)
		g++, m += 12;
	return 365 * g + g / 4 - g / 100 + g / 400 + (153 * m - 457) / 5 + d - 306;
}
struct Aktivnost {
	unique_ptr<VrstaAktivnosti> _vrsta;
	Datum _datumIzvrsenja;
	char * _komentar;
	int _ocjena; // 1 - 5
	void Unos(VrstaAktivnosti vrsta, Datum * datum, int ocjena, const char * komentar) {
		_vrsta = make_unique<VrstaAktivnosti>(vrsta);
		_datumIzvrsenja.Unos(*datum->_dan, *datum->_mjesec, *datum->_godina);
		_ocjena = ocjena;
		int size = strlen(komentar) + 1;
		_komentar = new char[size];
		strcpy_s(_komentar, size, komentar);
	}
	void Ispis() {
		cout << VrstaAktivnostiChar[*_vrsta] << " " << _ocjena << " " << _komentar;
		_datumIzvrsenja.Ispis();
		cout << endl;
	}
	void Dealociraj() {
		_datumIzvrsenja.Dealociraj();
		delete[] _komentar; _komentar = nullptr;
	}
};
void KopirajAktivnost(Aktivnost & source, Aktivnost & dest) {
	dest._vrsta = make_unique<VrstaAktivnosti>(*source._vrsta);
	dest._datumIzvrsenja.Unos(*source._datumIzvrsenja._dan, *source._datumIzvrsenja._mjesec, *source._datumIzvrsenja._godina);
	source._datumIzvrsenja.Dealociraj();
	int len = strlen(source._komentar) + 1;
	dest._komentar = new char[len];
	strcpy_s(dest._komentar, len, source._komentar);
	delete[] source._komentar; source._komentar = nullptr;
	dest._ocjena = source._ocjena;
}
struct VozackaKategorija {
	Datum * _datumPolaganja;//datum koji ce se evidentirati kao datum polaganja kategorije tj. kada su ispunjene sve aktivnosti/obaveze
	Kategorija _kategorija;
	Aktivnost * _listaIzvrsenihAktivnosti;
	int _trenutnoIzvrsenihAktivnosti;
	void Unos(Kategorija kategorija) {
		_listaIzvrsenihAktivnosti = nullptr;
		_trenutnoIzvrsenihAktivnosti = 0;
		_datumPolaganja = nullptr;
		_kategorija = kategorija;
	}
	void Dealociraj() {
		if (_datumPolaganja != nullptr)
			_datumPolaganja->Dealociraj();
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			_listaIzvrsenihAktivnosti[i].Dealociraj();
		delete[] _listaIzvrsenihAktivnosti;
		_listaIzvrsenihAktivnosti = nullptr;
	}
	void Ispis() {
		if (_datumPolaganja != nullptr)
			_datumPolaganja->Ispis();
		cout << KategorijaChar[_kategorija] << endl;
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			_listaIzvrsenihAktivnosti[i].Ispis();
	}
	bool DodajIzvrsenuAktivnost(VrstaAktivnosti vrsta, Datum * datum, int ocjena, const char * komentar) {
		if (_datumPolaganja != nullptr) return false;
		bool identicna = false;
		int indeksIdenticne = -1;
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++) {
			if (istiDatum(_listaIzvrsenihAktivnosti[i]._datumIzvrsenja, *datum) && *_listaIzvrsenihAktivnosti[i]._vrsta == vrsta) {
				identicna = true;
				indeksIdenticne = i;
			}
		}
		if (identicna) {
			if (_listaIzvrsenihAktivnosti[indeksIdenticne]._ocjena > 2) return false;
			int datumIdenticne = rataDie(*_listaIzvrsenihAktivnosti[indeksIdenticne]._datumIzvrsenja._dan,
				*_listaIzvrsenihAktivnosti[indeksIdenticne]._datumIzvrsenja._mjesec,
				*_listaIzvrsenihAktivnosti[indeksIdenticne]._datumIzvrsenja._godina);
			int datumNove = rataDie(*datum->_dan, *datum->_mjesec, *datum->_godina);
			if (datumNove - datumIdenticne < 30) return false;
		}

		Aktivnost * temp = new Aktivnost[_trenutnoIzvrsenihAktivnosti + 1];
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++) {
			KopirajAktivnost(_listaIzvrsenihAktivnosti[i], temp[i]);
		}
		temp[_trenutnoIzvrsenihAktivnosti].Unos(vrsta, datum, ocjena, komentar);
		delete[] _listaIzvrsenihAktivnosti; _listaIzvrsenihAktivnosti = nullptr;
		_listaIzvrsenihAktivnosti = temp;
		_trenutnoIzvrsenihAktivnosti++;
		return true;
	}
	void Sortiraj() {
		bool promjena = true;
		int brProlaza = _trenutnoIzvrsenihAktivnosti - 1;
		while (promjena) {
			promjena = false;
			for (int i = 0; i < brProlaza; i++) {
				if (_listaIzvrsenihAktivnosti[i]._ocjena < _listaIzvrsenihAktivnosti[i + 1]._ocjena) {
					swap(_listaIzvrsenihAktivnosti[i], _listaIzvrsenihAktivnosti[i + 1]);
					promjena = true;
				}
			}
			brProlaza--;
		}
	}
	bool DaLiJePolozena() {
		int potrebno = _kategorija;
		potrebno++;
		int indeksPosljednjePolozene = -1;
		int brUspjesnihPoligona = 0;
		int brUspjesnihGradskih = 0;
		int brUspjesnihNocnih = 0;
		int brUspjesnihParkiranja = 0;
		int brNegativnihPoligona = 0;
		int brNegativnihGradskih = 0;
		int brNegativnihNocnih = 0;
		int brNegativnihParkiranja = 0;
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++) {
			if (_listaIzvrsenihAktivnosti[i]._ocjena > 2)
				indeksPosljednjePolozene = i;
			if (*_listaIzvrsenihAktivnosti[i]._vrsta == Poligon) {
				if (_listaIzvrsenihAktivnosti[i]._ocjena > 2)
					brUspjesnihPoligona++;
				else
					brNegativnihPoligona++;
			}
			if (*_listaIzvrsenihAktivnosti[i]._vrsta == Gradska) {
				if (_listaIzvrsenihAktivnosti[i]._ocjena > 2)
					brUspjesnihGradskih++;
				else
					brNegativnihGradskih++;
			}
			if (*_listaIzvrsenihAktivnosti[i]._vrsta == Nocna) {
				if (_listaIzvrsenihAktivnosti[i]._ocjena > 2)
					brUspjesnihNocnih++;
				else
					brNegativnihNocnih++;
			}
			if (*_listaIzvrsenihAktivnosti[i]._vrsta == Parkiranje) {
				if (_listaIzvrsenihAktivnosti[i]._ocjena > 2)
					brUspjesnihParkiranja++;
				else
					brNegativnihParkiranja++;
			}
		}
		if (brUspjesnihGradskih < potrebno || brUspjesnihNocnih < potrebno || brUspjesnihParkiranja < potrebno || brUspjesnihPoligona < potrebno)
			return false;
		if (brNegativnihGradskih > 3 || brNegativnihNocnih > 3 || brNegativnihParkiranja > 3 || brNegativnihPoligona > 3) return false;
		_datumPolaganja = new Datum;
		_datumPolaganja->Unos(*_listaIzvrsenihAktivnosti[indeksPosljednjePolozene]._datumIzvrsenja._dan,
			*_listaIzvrsenihAktivnosti[indeksPosljednjePolozene]._datumIzvrsenja._mjesec,
			*_listaIzvrsenihAktivnosti[indeksPosljednjePolozene]._datumIzvrsenja._godina);
		return true;
	}
	float PretragaRekrzivno(const char * kljucnaRijec, int brojacAktivnosti = 0, double sveOcjene = 0, int brojacOcjena = 0) {
		if (brojacAktivnosti >= _trenutnoIzvrsenihAktivnosti) return sveOcjene /= brojacOcjena;
		if (strstr(_listaIzvrsenihAktivnosti[brojacAktivnosti]._komentar, kljucnaRijec) != nullptr && _listaIzvrsenihAktivnosti[brojacAktivnosti]._ocjena > 2) {
			sveOcjene += _listaIzvrsenihAktivnosti[brojacAktivnosti]._ocjena;
			return PretragaRekrzivno(kljucnaRijec, brojacAktivnosti + 1, sveOcjene, brojacOcjena + 1);
		}
		return PretragaRekrzivno(kljucnaRijec, brojacAktivnosti + 1, sveOcjene, brojacOcjena);
	}
};

struct Kandidat {
	char * _imePrezime;
	shared_ptr<VozackaKategorija> _kategorije[maxKategorija];
	int _trenutnoKategorija;
	void Unos(const char * imePrezime) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		_trenutnoKategorija = 0;
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < _trenutnoKategorija; i++) {
			_kategorije[i]->Dealociraj();
			_kategorije[i].reset();
		}
	}
	void Ispis() {
		cout << _imePrezime << endl;
		for (size_t i = 0; i < _trenutnoKategorija; i++)
			_kategorije[i]->Ispis();
	}
	bool DodajKategoriju(VozackaKategorija &kategorija) {
		if (_trenutnoKategorija >= maxKategorija) return false;
		int prethodnaPolozenaKategorija = -1;
		for (int i = 0; i < maxKategorija; i++) {
			if (_kategorije[i] == nullptr)
				break;
			prethodnaPolozenaKategorija = _kategorije[i]->_kategorija;
		}
		if (prethodnaPolozenaKategorija != -1) {
			if (kategorija._kategorija <= prethodnaPolozenaKategorija)
				return false;
		}
		_kategorije[_trenutnoKategorija] = make_shared<VozackaKategorija>();
		_kategorije[_trenutnoKategorija]->Unos(kategorija._kategorija);
		for (int i = 0; i < kategorija._trenutnoIzvrsenihAktivnosti; i++) {
			_kategorije[_trenutnoKategorija]->DodajIzvrsenuAktivnost(*kategorija._listaIzvrsenihAktivnosti[i]._vrsta,
				&kategorija._listaIzvrsenihAktivnosti[i]._datumIzvrsenja, kategorija._listaIzvrsenihAktivnosti[i]._ocjena,
				kategorija._listaIzvrsenihAktivnosti[i]._komentar);
		}
		if (kategorija._datumPolaganja != nullptr) {
			_kategorije[_trenutnoKategorija]->_datumPolaganja->Unos(*kategorija._datumPolaganja->_dan, *kategorija._datumPolaganja->_mjesec,
				*kategorija._datumPolaganja->_godina);
		}
		_trenutnoKategorija++;
		return true;
	}
	pair<Kategorija, float>GetNajboljuKategoriju() {
		double sveOcjene = 0.0;
		int brojacOcjena = 0;
		float prosjekNajboljeKategorije = 0.0;
		float prosjekTrenutnog = 0.0;
		Kategorija najboljaKategorija;
		for (int i = 0; i < _trenutnoKategorija; i++) {
			for (int j = 0; j < _kategorije[i]->_trenutnoIzvrsenihAktivnosti; j++) {
				if (_kategorije[i]->_listaIzvrsenihAktivnosti[j]._ocjena > 2) {
					sveOcjene += _kategorije[i]->_listaIzvrsenihAktivnosti[j]._ocjena;
					brojacOcjena++;
				}
			}
			prosjekTrenutnog = sveOcjene / brojacOcjena;
			if (prosjekTrenutnog > prosjekNajboljeKategorije) {
				prosjekNajboljeKategorije = prosjekTrenutnog;
				najboljaKategorija = _kategorije[i]->_kategorija;
			}
			sveOcjene = 0;
			brojacOcjena = 0;
			prosjekTrenutnog = 0;
		}
		return make_pair(najboljaKategorija, prosjekNajboljeKategorije);
	}
};

void main() {

	//BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA
	//STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
	Datum datumPolaganja1, datumPolaganja2, datumPolaganja3, datumPolaganja4;
	datumPolaganja1.Unos(10, 6, 2018);
	datumPolaganja2.Unos(18, 6, 2018);
	datumPolaganja3.Unos(22, 3, 2018);
	datumPolaganja4.Unos(28, 7, 2018);

	VozackaKategorija kategorijaA, kategorijaB, kategorijaC;
	kategorijaA.Unos(A);
	kategorijaB.Unos(B);
	kategorijaC.Unos(C);

	/*Na osnovu vrijednosti primljenog parametra osigurati dodavanje novoizvrsene aktivnosti za potrebe stjecanja odredjene vozacke kategorije.
	Broj aktivnosti nije ogranicen.
	Identicna aktivnost se moze dodati jedino u slucaju kada je prethodna (identivna aktivnost po vrsti i datumu izvrsenja) imala ocjenu manju
	od 3.
	Uspjesnom aktivnoscu se smatraju one aktivnosti koje imaju ocjenu vecu od 2, a svaka naredna identicna aktivnost, bez obzira da li je
	uspjesna ili ne,
	moze biti dodana jedino ako je proslo najmanje 30 dana od izvrsenja prethodne. Onemoguciti dodavanje aktivnosti uspjesno polozenoj
	kategoriji.*/
	if (kategorijaA.DodajIzvrsenuAktivnost(Poligon, &datumPolaganja1, 3, "izuzetno dobra orijentacija"))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (kategorijaA.DodajIzvrsenuAktivnost(Gradska, &datumPolaganja1, 2, "nesigurnost u raskrsnicama"))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (kategorijaA.DodajIzvrsenuAktivnost(Gradska, &datumPolaganja2, 4, "raskrsnice savladane"))//datum?
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (kategorijaA.DodajIzvrsenuAktivnost(Gradska, &datumPolaganja4, 4, "raskrsnice savladane"))
		cout << "Aktivnost uspjesno dodana!" << endl;

	//koristeci neki od obradjenih algoritama, po ocjeni sortirati aktivnosti u okviru odredjene kategorije
	kategorijaA.Sortiraj();


	/*Stjecanje kategorije zahtijeva uspjesnu realizaciju svih planiranih aktivnosti, a one se za jedan povecavaju sa svakom novom kategorijom,
	npr.
	A: 1 x poligon, 1 x gradska, 1 x nocna, 1 x parkiranje;
	B: 2 x poligon, 2 x gradska, 2 x nocna, 2 x parkiranje; i td...
	Funkcija vraca false u slucaju da: su kandidatu u listu aktivnosti evidentirane tri negativno ocijenjene identicne vrste aktivnosti
	(npr. tri negativne ocjene iz gradske voznje), onda se ta kategorija ne moze smatrati uspjesno polozenom
	,te ukoliko nedostaje bilo koja od aktivnosti zahtijevanih u okviru te kategorije. Ukoliko je kandidat uspjesno realizovao sve aktivnost,
	datum polaganja se postavlja na datum posljednje uspjesno realizovane aktivnosti*/
	if (kategorijaA.DaLiJePolozena())
		kategorijaA.Ispis();

	/*Funkcija vraca prosjecnu ocjenu svih uspjesno realizovanih aktivnosti koja u nazivu ili opisu sadrze vrijednost primljenog parametra.
	Ukoliko smatrate da je potrebno, mozete dodati i druge parametre za potrebe implementacije ove funkcije*/
	cout << "Prosjecna ocjena za kategoriju A -> " << kategorijaA.PretragaRekrzivno("nesigurno") << endl;

	//ispisuje sve dostupne podatke o kategoriji
	kategorijaA.Ispis();

	Kandidat jasmin;
	jasmin.Unos("Jasmin Azemovic");

	/*
	Pretpostavka je da se kategorije moraju polagati po redoslijedu tj. ne smije se dozvoliti dodavanje kategorije C ukoliko prethodno nisu
	uspjesno polozene kategorije B i C. Za provjeru ispunjenosti preduslova dodavanja nove kategorije, unutar funkcije DodajKategoriju,
	kreirati lambda funkciju.
	*/
	if (jasmin.DodajKategoriju(kategorijaA))
		cout << "Kategorija uspjesno dodan!" << endl;
	if (jasmin.DodajKategoriju(kategorijaC))
		cout << "Kategorija uspjesno dodan!" << endl;
	if (jasmin.DodajKategoriju(kategorijaB))
		cout << "Kategorija uspjesno dodan!" << endl;

	float prosjek = 0;
	Kategorija kategorija;
	//Funkcija GetNajboljuKategoriju  vraca par koji sadrzi oznaku i prosjecnu ocjenu (uspjesno okoncanih aktivnosti) kategorije sa najvecim 
	//prosjekom
	tie(kategorija, prosjek) = jasmin.GetNajboljuKategoriju();
	cout << "Najbolji rezultat od " << prosjek << " je ostvaren tokom polaganja kategorije " << KategorijaChar[kategorija] << endl;


	datumPolaganja1.Dealociraj(), datumPolaganja2.Dealociraj(), datumPolaganja3.Dealociraj(), datumPolaganja4.Dealociraj();
	kategorijaA.Dealociraj(), kategorijaB.Dealociraj(), kategorijaC.Dealociraj();
	jasmin.Dealociraj();

	system("pause");
}
