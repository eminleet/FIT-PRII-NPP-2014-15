#include<iostream>
#include<memory>
#include<tuple>

using namespace std;
/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NECE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB160061.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI CETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)


enum VrstaAktivnosti { Tehnike, Kata, Borba };
const char * VrstaAktivnostiChar[] = { "Tehnike", "Kata", "Borba" };
enum Pojas { Zuti, Narandzasti, Zeleni, Plavi, Smedji, Crni };
const char * PojasChar[] = { "Zuti", "Narandzasti", "Zeleni", "Plavi", "Smedji", "Crni" };
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
	char * _nazivOpis;
	int _ocjena; // 1 - 10
	void Unos(VrstaAktivnosti vrsta, Datum * datum, int ocjena, const char * nazivOpis) {
		_vrsta = make_unique<VrstaAktivnosti>(vrsta);
		_datumIzvrsenja.Unos(*datum->_dan, *datum->_mjesec, *datum->_godina);
		_ocjena = ocjena;
		int size = strlen(nazivOpis) + 1;
		_nazivOpis = new char[size];
		strcpy_s(_nazivOpis, size, nazivOpis);
	}
	void Ispis() {
		cout << VrstaAktivnostiChar[*_vrsta] << " " << _ocjena << " " << _nazivOpis;
		_datumIzvrsenja.Ispis();
		cout << endl;
	}
	void Dealociraj() {
		_datumIzvrsenja.Dealociraj();
		delete[] _nazivOpis; _nazivOpis = nullptr;
	}
};
void KopirajAktivnost(Aktivnost & source, Aktivnost & dest) {
	dest._vrsta = make_unique<VrstaAktivnosti>(*source._vrsta);
	dest._datumIzvrsenja.Unos(*source._datumIzvrsenja._dan, *source._datumIzvrsenja._mjesec, *source._datumIzvrsenja._godina);
	source._datumIzvrsenja.Dealociraj();
	int size = strlen(source._nazivOpis) + 1;
	dest._nazivOpis = new char[size];
	strcpy_s(dest._nazivOpis, size, source._nazivOpis);
	delete[] source._nazivOpis; source._nazivOpis = nullptr;
	dest._ocjena = source._ocjena;
}

struct KaratePojas {
	Datum * _datumPolaganja;//datum koji ce se evidentirati kao datum polaganja pojasa tj. kada su ispunjene sve aktivnosti/obaveze
	Pojas _pojas;
	Aktivnost * _listaIzvrsenihAktivnosti;
	int _trenutnoIzvrsenihAktivnosti;
	void Unos(Pojas pojas) {
		_listaIzvrsenihAktivnosti = nullptr;
		_trenutnoIzvrsenihAktivnosti = 0;
		_datumPolaganja = nullptr;
		_pojas = pojas;
	}
	void Dealociraj() {
		if (_datumPolaganja != nullptr) {
			_datumPolaganja->Dealociraj(); _datumPolaganja = nullptr;
		}
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			_listaIzvrsenihAktivnosti[i].Dealociraj();
		delete[] _listaIzvrsenihAktivnosti;
		_listaIzvrsenihAktivnosti = nullptr;
	}
	void Ispis() {
		if (_datumPolaganja != nullptr)
			_datumPolaganja->Ispis();
		cout << PojasChar[_pojas] << endl;
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			_listaIzvrsenihAktivnosti[i].Ispis();
	}
	bool DodajIzvrsenuAktivnost(Aktivnost * aktivnost) {
		if (_datumPolaganja != nullptr)
			return false;
		bool identicna = false;
		int pozicijaPosljednje = -1;
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++) {
			if (_listaIzvrsenihAktivnosti[i]._nazivOpis != nullptr)
				pozicijaPosljednje = i;
			if (*aktivnost->_vrsta == *_listaIzvrsenihAktivnosti[i]._vrsta &&
				istiDatum(aktivnost->_datumIzvrsenja, _listaIzvrsenihAktivnosti[i]._datumIzvrsenja)) {
				identicna = true;
			}
		}

		if (identicna) {
			if (_listaIzvrsenihAktivnosti[pozicijaPosljednje]._ocjena > 6)
				return false;
			int datumPosljednjeAktivnosti = rataDie(*_listaIzvrsenihAktivnosti[pozicijaPosljednje]._datumIzvrsenja._dan,
				*_listaIzvrsenihAktivnosti[pozicijaPosljednje]._datumIzvrsenja._mjesec,
				*_listaIzvrsenihAktivnosti[pozicijaPosljednje]._datumIzvrsenja._godina);
			int datumNoveAktivnosti = rataDie(*aktivnost->_datumIzvrsenja._dan, *aktivnost->_datumIzvrsenja._mjesec,
				*aktivnost->_datumIzvrsenja._godina);
			if (datumNoveAktivnosti - datumPosljednjeAktivnosti < 15)
				return false;
		}

		Aktivnost * temp = new Aktivnost[_trenutnoIzvrsenihAktivnosti + 1];
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++) {
			KopirajAktivnost(_listaIzvrsenihAktivnosti[i], temp[i]);
		}
		KopirajAktivnost(*aktivnost, temp[_trenutnoIzvrsenihAktivnosti]);
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
				if (_listaIzvrsenihAktivnosti[i]._ocjena > _listaIzvrsenihAktivnosti[i + 1]._ocjena) {
					swap(_listaIzvrsenihAktivnosti[i], _listaIzvrsenihAktivnosti[i + 1]);
					promjena = true;
				}
			}
			brProlaza--;
		}
	}
	bool DaLiJePolozen() {
		int pojasInt = _pojas;
		pojasInt++;
		int brUspjesnihKata = 0;
		int brUspjesnihBorbi = 0;
		int brUspjenihTehnika = 0;
		int brNegativnihKata = 0;
		int brNegativnihBorbi = 0;
		int brNegativnihTehnika = 0;
		int indeksPosljednjeAktivnosti = -1;

		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++) {
			if (_listaIzvrsenihAktivnosti[i]._nazivOpis != nullptr && _listaIzvrsenihAktivnosti[i]._ocjena > 5) {
				indeksPosljednjeAktivnosti = i;
			}
			if (*_listaIzvrsenihAktivnosti[i]._vrsta == Kata) {
				if (_listaIzvrsenihAktivnosti[i]._ocjena > 5)
					brUspjesnihKata++;
				else
					brNegativnihKata++;
			}
			if (*_listaIzvrsenihAktivnosti[i]._vrsta == Tehnike) {
				if (_listaIzvrsenihAktivnosti[i]._ocjena > 5)
					brUspjenihTehnika++;
				else
					brNegativnihTehnika++;
			}
			if (*_listaIzvrsenihAktivnosti[i]._vrsta == Borba) {
				if (_listaIzvrsenihAktivnosti[i]._ocjena > 5)
					brUspjesnihBorbi++;
				else
					brNegativnihBorbi++;
			}
		}

		if (brUspjenihTehnika < pojasInt || brUspjesnihBorbi < pojasInt || brUspjesnihKata < pojasInt) return false;
		if (brNegativnihBorbi > 3 || brNegativnihKata > 3 || brNegativnihTehnika > 3) return false;
		_datumPolaganja = new Datum;
		_datumPolaganja->Unos(*_listaIzvrsenihAktivnosti[indeksPosljednjeAktivnosti]._datumIzvrsenja._dan,
			*_listaIzvrsenihAktivnosti[indeksPosljednjeAktivnosti]._datumIzvrsenja._mjesec,
			*_listaIzvrsenihAktivnosti[indeksPosljednjeAktivnosti]._datumIzvrsenja._godina);
		return true;
	}
	float PretragaRekrzivno(const char * kljucnaRijec, int brojacAktivnosti = 0, double sveOcjene = 0, int brojacOcjena = 0) {
		if (brojacAktivnosti >= _trenutnoIzvrsenihAktivnosti) return sveOcjene /= brojacOcjena;
		if (strstr(_listaIzvrsenihAktivnosti[brojacAktivnosti]._nazivOpis, kljucnaRijec) != nullptr && _listaIzvrsenihAktivnosti[brojacAktivnosti]._ocjena > 5) {
			sveOcjene += _listaIzvrsenihAktivnosti[brojacAktivnosti]._ocjena;
			return PretragaRekrzivno(kljucnaRijec, brojacAktivnosti + 1, sveOcjene, brojacOcjena + 1);
		}
		return PretragaRekrzivno(kljucnaRijec, brojacAktivnosti + 1, sveOcjene, brojacOcjena);
	}
};

struct Kandidat {
	char * _imePrezime;
	shared_ptr<KaratePojas> _pojasevi[6];
	void Unos(const char * imePrezime) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		for (size_t i = 0; i < 6; i++)
			_pojasevi[i] = nullptr;
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < 6; i++)
			if (_pojasevi[i] != nullptr) {
				_pojasevi[i]->Dealociraj();
				_pojasevi[i].reset();
			}
	}
	void Ispis() {
		cout << _imePrezime << endl;
		for (size_t i = 0; i < 6; i++)
			if (_pojasevi[i] != nullptr)
				_pojasevi[i]->Ispis();
	}
	bool DodajPojas(KaratePojas &pojas) {
		int naredniSlobodanIndeks = 0;
		int zadnjiPojas = -1;
		for (int i = 0; i < 6; i++) {
			if (_pojasevi[i] == nullptr)
				break;
			zadnjiPojas = _pojasevi[i]->_pojas;
			naredniSlobodanIndeks = i + 1;
		}
		if (zadnjiPojas != -1) {
			zadnjiPojas++;
			if (pojas._pojas != zadnjiPojas) return false;
		}
		if (naredniSlobodanIndeks >= 6) return false;
		_pojasevi[naredniSlobodanIndeks] = make_shared<KaratePojas>();
		_pojasevi[naredniSlobodanIndeks]->Unos(pojas._pojas);
		for (int i = 0; i < pojas._trenutnoIzvrsenihAktivnosti; i++)
			_pojasevi[naredniSlobodanIndeks]->DodajIzvrsenuAktivnost(&pojas._listaIzvrsenihAktivnosti[i]);
		if (pojas._datumPolaganja != nullptr) {
			_pojasevi[naredniSlobodanIndeks]->_datumPolaganja = new Datum;
			_pojasevi[naredniSlobodanIndeks]->_datumPolaganja->Unos(*pojas._datumPolaganja->_dan, *pojas._datumPolaganja->_mjesec, *pojas._datumPolaganja->_godina);
		}
		return true;
	}
	pair<Pojas, float>GetNajbolji() {
		Pojas najboljiPojas;
		float prosjekNajboljegPojasa = 0.0;
		float sveOcjene = 0;
		int brojacOcjena = 0;
		int indeksNajboljegPojasa = 0;
		float prosjekTrenutnog = 0;
		for (int i = 0; i < 6; i++) {
			if (_pojasevi[i] == nullptr)
				break;
			for (int j = 0; j < _pojasevi[i]->_trenutnoIzvrsenihAktivnosti; j++) {
				if (_pojasevi[i]->_listaIzvrsenihAktivnosti[j]._ocjena > 5) {
					sveOcjene += _pojasevi[i]->_listaIzvrsenihAktivnosti[j]._ocjena;
					brojacOcjena++;
				}
			}
			prosjekTrenutnog = sveOcjene / brojacOcjena;
			if (prosjekTrenutnog > prosjekNajboljegPojasa) {
				prosjekNajboljegPojasa = prosjekTrenutnog;
				indeksNajboljegPojasa = i;
			}
			sveOcjene = 0;
			brojacOcjena = 0;
			prosjekTrenutnog = 0;
		}
		return make_pair(_pojasevi[indeksNajboljegPojasa]->_pojas, prosjekNajboljegPojasa);
	}
};

void main() {

	//BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA
	//STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
	Datum datumPolaganja1, datumPolaganja2, datumPolaganja3, datumPolaganja4;
	datumPolaganja1.Unos(10, 6, 2018);
	datumPolaganja2.Unos(18, 6, 2018);
	datumPolaganja3.Unos(22, 3, 2018);
	datumPolaganja4.Unos(22, 7, 2018);

	Aktivnost aktivnost1, aktivnost2, aktivnost3, aktivnost4, aktivnost5;
	aktivnost1.Unos(Tehnike, &datumPolaganja1, 6, "Tehnike za zuti pojas");
	aktivnost2.Unos(Kata, &datumPolaganja1, 8, "Taiki joko shodan - zuti pojas");
	aktivnost3.Unos(Borba, &datumPolaganja1, 2, "Jednostavne borbene tehnike sa partnerom");
	aktivnost4.Unos(Borba, &datumPolaganja1, 6, "Jednostavne borbene tehnike sa partnerom");
	aktivnost5.Unos(Borba, &datumPolaganja4, 6, "Jednostavne borbene tehnike sa partnerom");

	KaratePojas pojasZuti, pojasNarandzasti, pojasZeleni;
	pojasZuti.Unos(Zuti);
	pojasNarandzasti.Unos(Narandzasti);
	pojasZeleni.Unos(Zeleni);

	//Na osnovu vrijednosti primljenog parametra osigurati dodavanje novoizvrsene aktivnosti za potrebe stjecanja odredjenog pojasa. 
	//Broj aktivnosti nije ogranicen.
	//Identicna aktivnost se moze dodati jedino u slucaju kada je prethodna (identivna aktivnost po vrsti i datumu izvrsenja) imala ocjenu 
	//	manju od 6.
	//Uspjesnom aktivnoscu se smatraju one aktivnosti koje imaju ocjenu vecu od 5, a svaka naredna identicna aktivnost, bez obzira da li je
	//	uspjesna ili ne,
	//moze biti dodana jedino ako je proslo najmanje 15 dana od izvrsenja prethodne. Onemoguciti dodavanje aktivnosti uspjesno polozenom
	//	pojasu.
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost1))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost2))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost3))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost4))//15 dana...
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost5))
		cout << "Aktivnost uspjesno dodana!" << endl;

	//Koristeci neki od obradjenih algoritama, po ocjeni sortirati aktivnosti u okviru odredjenog pojasa
	pojasZuti.Sortiraj();

	/* Karate pojas zahtijeva uspjesnu realizaciju svih planiranih aktivnosti, a one se za jedan povecavaju sa svakim novim pojasom, npr.
	zuti pojas: 1 x tehnika, 1 x kata, 1 x borba; narandzasti pojas: 2 x tehnika, 2 x kata, 2 x borba; itd...
	Funkcija vraca false u slucaju da: su kandidatu u listu aktivnosti evidentirane tri negativno ocijenjene identicne vrste aktivnosti
	(npr. tri negativne ocjene iz borbi), onda se taj pojas ne moze smatrati uspjesno stecenim
	i, te ukoliko nedostaje bilo koja od aktivnosti zahtijevanih u okviru tog pojasa. Ukoliko je kandidat uspjesno realizovao sve aktivnost,
		datum polaganja se postavlja na datum posljednje uspjesno realizovane aktivnosti*/
	if (pojasZuti.DaLiJePolozen())
		pojasZuti.Ispis();

	/*Funkcija vraca prosjecnu ocjenu svih uspjesno realizovanih aktivnosti koja u nazivu ili opisu sadrze vrijednost primljenog parametra.
	Ukoliko smatrate da je potrebno, mozete dodati i druge parametre za potrebe implementacije ove funkcije*/
	cout << "Prosjecna ocjena za zuti pojas -> " << pojasZuti.PretragaRekrzivno("pojas") << endl;

	//ispisuje sve dostupne podatke o pojasu
	pojasZuti.Ispis();

	Kandidat jasmin;
	jasmin.Unos("Jasmin Azemovic");
	/*
	Karate pojasevi se moraju dodavati po redoslijedu tj. ne smije se dozvoliti dodavanje zelenog pojasa ukoliko prethodno nije dodan zuti i
	narandzasti. Za provjeru lokacije (unutar funkcije DodajPojas) na koju ce se dodati novi karate pojas, te da li su nizi pojasevi prethodno
	dodani koristiti lambda funkciju.
	*/
	if (jasmin.DodajPojas(pojasZuti))
		cout << "Pojas uspjesno dodan!" << endl;
	if (jasmin.DodajPojas(pojasZeleni))//prethodno je trebao biti dodan narandzasti pojas
		cout << "Pojas uspjesno dodan!" << endl;
	if (jasmin.DodajPojas(pojasNarandzasti))
		cout << "Pojas uspjesno dodan!" << endl;

	float prosjek = 0;
	Pojas pojas;
	//Funkcija GetNajbolji vraca par koji sadrzi oznaku i prosjecnu ocjenu (uspjesno okoncanih aktivnosti) pojasa sa najvecim prosjekom
	tie(pojas, prosjek) = jasmin.GetNajbolji();
	cout << "Najbolji rezultat od " << prosjek << " je ostvaren tokom stjecanja pojasa " << PojasChar[pojas] << endl;


	datumPolaganja1.Dealociraj(); datumPolaganja2.Dealociraj(); datumPolaganja3.Dealociraj(); datumPolaganja4.Dealociraj();
	aktivnost1.Dealociraj(); aktivnost2.Dealociraj(); aktivnost3.Dealociraj(); aktivnost4.Dealociraj(); aktivnost5.Dealociraj();
	pojasZuti.Dealociraj(); pojasNarandzasti.Dealociraj(); pojasZeleni.Dealociraj();
	jasmin.Dealociraj();

	system("pause");
}