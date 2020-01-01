/*
1. ZADATAK

Izvršiti definiciju funkcija na naèin koji odgovara opisu (komentarima) datom neposredno uz pozive ili nazive funkcija. Možete dati komentar na bilo koju liniju code-a koju smatrate da bi trebalo unaprijediti ili da æe eventualno uzrokovati grešku prilikom kompajliranja. Takoðer, možete dodati dodatne funkcije koje æe vam olakšati implementaciju programa.


1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NEÆE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ÆETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/
#include<iostream>
using namespace std;

//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

const char *crt = "\n-------------------------------------------\n";

enum vrstaDokumenta { PDF, DOC, TXT, HTML };
const char * vrstaDokumentaChar[] = { "PDF", "DOC", "TXT", "HTML" };
enum prioritet { VISOK, SREDNJI, NIZAK };
const char * prioritetChar[] = { "VISOK", "SREDNJI", "NIZAK" };
struct DatumVrijeme {
	int *_dan, *_mjesec, *_godina, *_sati, *_minuti;
	void Unos(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);
	}
	void Dealociraj() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati; _sati = nullptr;
		delete _minuti; _minuti = nullptr;

	}
	void Ispis() {
		cout << *_dan << "." << *_mjesec << "." << *_godina << " " << *_sati << ":" << *_minuti << endl;
	}
};
bool manjiIliJedanDatum(DatumVrijeme d1, DatumVrijeme d2) {
	if (*d1._godina < *d2._godina) return true;
	if (*d1._godina == *d2._godina && *d1._mjesec < *d2._mjesec) return true;
	if (*d1._godina == *d2._godina && *d1._mjesec == *d2._mjesec && *d1._dan <= *d2._dan) return true;
	return false;
}

struct Filter {
	char * _sadrzaj;
	prioritet _prioritet;
	void Unos(const char * sadrzaj, prioritet prioritet) {
		int vel = strlen(sadrzaj) + 1;
		_sadrzaj = new char[vel];
		strcpy_s(_sadrzaj, vel, sadrzaj);
		_prioritet = prioritet;
	}
	void Dealociraj() {
		delete[] _sadrzaj; _sadrzaj = nullptr;
	}
	void Ispis() { cout << _sadrzaj << " (" << prioritetChar[_prioritet] << ")" << endl; }

};

struct Dokument {
	vrstaDokumenta _vrsta;
	char * _naziv;
	char * _sadrzaj;
	DatumVrijeme _kreiran;
	int _brojStranica;

	void Unos(vrstaDokumenta vrsta, const char * naziv, DatumVrijeme kreiran) {
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);
		_vrsta = vrsta;
		_sadrzaj = new char('\0');
		_kreiran.Unos(*kreiran._dan, *kreiran._mjesec, *kreiran._godina, *kreiran._sati, *kreiran._minuti);
		_brojStranica = 0;//broj stranica se odredjuje prilikom dodavanja svakog novog sadrzaja dokumentu
	}
	void Dealociraj() {
		delete[] _naziv; _naziv = nullptr;
		delete[] _sadrzaj; _sadrzaj = nullptr;
		_kreiran.Dealociraj();
	}
	void Ispis() {
		cout << crt << _naziv << " " << vrstaDokumentaChar[_vrsta] << " kreiran ";
		_kreiran.Ispis();
		cout << crt << _sadrzaj << crt << " br.stranica (" << _brojStranica << ")" << crt;
	}
	void DodajSadrzaj(const char * noviSadrzaj) {
		int len = strlen(_sadrzaj) + strlen(noviSadrzaj) + 2;
		char * temp = new char[len];
		strcpy_s(temp, len, _sadrzaj);
		strcat_s(temp, len, noviSadrzaj);
		delete[] _sadrzaj; _sadrzaj = nullptr;
		_sadrzaj = temp;
		_brojStranica = ceil(len / 30.);
	}
};
void KopirajDokument(Dokument & source, Dokument & dest) {
	dest._vrsta = source._vrsta;
	int len = strlen(source._naziv) + 1;
	dest._naziv = new char[len];
	strcpy_s(dest._naziv, len, source._naziv);
	len = strlen(source._sadrzaj) + 1;
	dest._sadrzaj = new char[len];
	strcpy_s(dest._sadrzaj, len, source._sadrzaj);
	dest._kreiran.Unos(*source._kreiran._dan, *source._kreiran._mjesec, *source._kreiran._godina, *source._kreiran._sati, *source._kreiran._minuti);
	dest._brojStranica = source._brojStranica;
}
bool ispunjavaUsloveZaPrint(Dokument &doc, int _trenutnoFiltera, Filter * _filteri) {
	char * pok = strstr(doc._naziv, ".");
	if (strcmp(pok, ".pdf") != 0 && strcmp(pok, ".doc") != 0 && strcmp(pok, ".txt") != 0 && strcmp(pok, ".html") != 0)
		return false;

	int len = strlen(doc._naziv);
	int duzinaPok = strlen(pok);
	int duzinaNaziva = len - duzinaPok;
	for (int i = 0; i < duzinaNaziva; i++) {
		if (!isalpha(doc._naziv[i]))
			return false;
	}

	for (int i = 0; i < _trenutnoFiltera; i++) {
		if (strstr(doc._naziv, _filteri[i]._sadrzaj) != nullptr)
			return false;
		if (strstr(doc._sadrzaj, _filteri[i]._sadrzaj) != nullptr && _filteri[i]._prioritet != NIZAK)
			return false;
	}
	return true;
}

int brPrintanihDoc = 0;
struct Printer {
	char * _proizvodjac;
	char * _model;
	Dokument * _dokumenti;
	int _trenutnoDokumenata;
	Filter * _filteri;
	int _trenutnoFiltera;

	void Unos(const char * proizvodjac, const char * model) {
		int vel = strlen(proizvodjac) + 1;
		_proizvodjac = new char[vel];
		strcpy_s(_proizvodjac, vel, proizvodjac);
		vel = strlen(model) + 1;
		_model = new char[vel];
		strcpy_s(_model, vel, model);
		_trenutnoDokumenata = 0;
		_trenutnoFiltera = 0;
		_filteri = nullptr;
		_dokumenti = nullptr;
	}
	void Dealociraj() {
		delete[] _proizvodjac; _proizvodjac = nullptr;
		delete[] _model; _model = nullptr;
		for (size_t i = 0; i < _trenutnoDokumenata; i++)
			_dokumenti[i].Dealociraj();
		delete[]_dokumenti; _dokumenti = nullptr;
		for (size_t i = 0; i < _trenutnoFiltera; i++)
			_filteri[i].Dealociraj();
		delete[]_filteri; _filteri = nullptr;
	}
	bool DodajFilter(const char * filter, prioritet priority) {
		for (int i = 0; i < _trenutnoFiltera; i++) {
			if (strcmp(_filteri[i]._sadrzaj, filter) == 0)
				return false;
		}
		Filter * temp = new Filter[_trenutnoFiltera + 1];
		for (int i = 0; i < _trenutnoFiltera; i++) {
			temp[i].Unos(_filteri[i]._sadrzaj, _filteri[i]._prioritet);
			_filteri[i].Dealociraj();
		}
		temp[_trenutnoFiltera].Unos(filter, priority);
		delete[] _filteri; _filteri = nullptr;
		_filteri = temp;
		_trenutnoFiltera++;
		return true;
	}
	void Ispis() {
		cout << _proizvodjac << " " << _model << endl;
		for (int i = 0; i < _trenutnoDokumenata; i++) {
			if (ispunjavaUsloveZaPrint(_dokumenti[i], _trenutnoFiltera, _filteri))
				_dokumenti[i].Ispis();
		}
		for (int i = 0; i < _trenutnoFiltera; i++)
			_filteri[i].Ispis();
	}
	void Printaj(Dokument &doc) {
		if (strlen(doc._sadrzaj) == 0)
			return;

		Dokument * temp = new Dokument[_trenutnoDokumenata + 1];
		for (int i = 0; i < _trenutnoDokumenata; i++) {
			KopirajDokument(_dokumenti[i], temp[i]);
			_dokumenti[i].Dealociraj();
		}
		KopirajDokument(doc, temp[_trenutnoDokumenata]);
		delete[] _dokumenti; _dokumenti = nullptr;
		_dokumenti = temp;
		_trenutnoDokumenata++;

		if (!ispunjavaUsloveZaPrint(doc, _trenutnoFiltera, _filteri)) return;

		brPrintanihDoc++;
		for (int i = 0; i < strlen(doc._sadrzaj); i++) {
			if (i % 30 == 0)
				cout << crt;
			cout << doc._sadrzaj[i];
		}

	}
	int GetProsjecanBrojStranicaPoDatumuIVrsti(DatumVrijeme OD, DatumVrijeme DO, vrstaDokumenta vrsta) {
		int ukupanBrojStranica = 0;
		for (int i = 0; i < _trenutnoDokumenata; i++) {
			if (manjiIliJedanDatum(OD, _dokumenti[i]._kreiran) && manjiIliJedanDatum(_dokumenti[i]._kreiran, DO) && _dokumenti[i]._vrsta == vrsta)
				ukupanBrojStranica += _dokumenti[i]._brojStranica;
		}
		return ukupanBrojStranica / brPrintanihDoc;
	}
	Filter * GetNakoristenijiFilter() {
		int * koristenjeFiltera = new int[_trenutnoFiltera];
		for (int i = 0; i < _trenutnoDokumenata; i++) {
			for (int j = 0; j < _trenutnoFiltera; j++) {
				if (strstr(_dokumenti[i]._sadrzaj, _filteri[j]._sadrzaj) != nullptr) {
					koristenjeFiltera[j] += 1;
				}
			}
		}
		int indeks = -1;
		for (int i = 0; i < _trenutnoFiltera - 1; i++) {
			if (koristenjeFiltera[i] > koristenjeFiltera[i + 1]) {
				indeks = i;
			}
		}
		return &_filteri[indeks];
	}
};

void ZamijeniIPrintaj(Printer &printer, char znak, int brojacDoc = 0, int brojacFiltera = 0) {
	if (brojacDoc >= printer._trenutnoDokumenata) return;
	if (brojacFiltera >= printer._trenutnoFiltera) {
		cout << printer._dokumenti[brojacDoc]._sadrzaj << endl;
		return ZamijeniIPrintaj(printer, znak, brojacDoc + 1, 0);
	}
	else {
		char * pok = strstr(printer._dokumenti[brojacDoc]._sadrzaj, printer._filteri[brojacFiltera]._sadrzaj);
		if (pok != nullptr) {
			int lenDoc = strlen(printer._dokumenti[brojacDoc]._sadrzaj);
			int lenPok = strlen(pok);
			int mjestoPronalaska = lenDoc - lenPok;
			int lenPronadjenogFiltera = strlen(printer._filteri[brojacFiltera]._sadrzaj);
			for (int i = mjestoPronalaska; i < mjestoPronalaska + lenPronadjenogFiltera; i++) {
				printer._dokumenti[brojacDoc]._sadrzaj[i] = znak;
			}
		}
		return ZamijeniIPrintaj(printer, znak, brojacDoc, brojacFiltera + 1);
	}
}


void main() {
	DatumVrijeme prije3Dana; prije3Dana.Unos(3, 2, 2017, 10, 15);
	DatumVrijeme danas1; danas1.Unos(6, 2, 2017, 10, 15);
	DatumVrijeme danas2; danas2.Unos(6, 2, 2017, 10, 16);
	DatumVrijeme za10Dana; za10Dana.Unos(16, 2, 2017, 10, 15);

	Dokument ispitPRII, ispitMAT, ispitUIT, ispitUITDrugiRok;
	ispitPRII.Unos(DOC, "ispitPRII.doc", prije3Dana);
	ispitMAT.Unos(DOC, "ispitMAT.doc", danas1);
	ispitUIT.Unos(DOC, "ispitUIT.doc", danas2);
	ispitUITDrugiRok.Unos(PDF, "ispitUITDrugiRok.pdf", za10Dana);

	/*BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREÐUJE PRILIKOM DODAVANJA SADRŽAJA. ZA POTREBE ISPITA PRETPOSTAVLJAMO DA NA JEDNU
	STRANICU MOŽE STATI 30 ZNAKOVA UKLJUÈUJUÆI I RAZMAKE*/
	ispitPRII.DodajSadrzaj("Programiranje ili racunarsko programiranje (engl. programming) jeste vjestina pomocu koje ");
	ispitPRII.DodajSadrzaj("korisnik stvara i izvrsava algoritme koristeci odredjene programske jezike da bi ... ");
	cout << "Broj stranica -> " << ispitPRII._brojStranica << endl;

	ispitPRII.Ispis();//ISPISUJE SVE PODATKE O DOKUMENTU

	ispitMAT.DodajSadrzaj("matematika se razvila iz potrebe da se obavljaju proracuni u trgovini, vrse mjerenja zemljista i predvidjaju ");
	ispitMAT.DodajSadrzaj("astronomski dogadjaji, i ove tri primjene se mogu dovesti u vezu sa grubom podjelom matematike na RAT i ");

	Printer hp3200; hp3200.Unos("HP", "3200");
	/*PRINTER NEÆE DOZVOLITI PRINTANJE DOKUMENATA U ÈIJEM SADRŽAJU SE NALAZI NEKA OD ZABRANJENIH RIJEÈI DEFINISANIH FILTERIMA*/
	if (hp3200.DodajFilter("RE", NIZAK)) //VISOK :)
		cout << "Filter dodan!" << endl;
	if (hp3200.DodajFilter("RAT", VISOK))
		cout << "Filter dodan!" << endl;
	if (hp3200.DodajFilter("UBITI", VISOK))
		cout << "Filter dodan!" << endl;
	if (hp3200.DodajFilter("MRZITI", SREDNJI))
		cout << "Filter dodan!" << endl;
	if (hp3200.DodajFilter("RE", SREDNJI))/* ONEMOGUÆITI PONAVLJANJE FILTERA, BEZ OBZIRA STO SU RAZLICITOG PRIORITETA*/
		cout << "Filter dodan!" << endl;

	/*
	DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDEÆA PRAVILA:
	1. NAZIV DOKUMENTA MOŽE SADRŽAVATI SAMO SLOVA, A EKSTENZIJA MOŽE BITI SAMO NEKA OD OZNAÈENIH SA vrstaDokumenta
	2. SADRŽAJ DOKUMENTA U SEBI NE SMIJE POSJEDOVATI ZABRANJENE RIJEÈ DEFINISANE FILTERIMA OSIM AKO SU ONE OZNAÈENE SA NISKIM PRIORITETOM
	(PRIRITET NIZAK)

	UKOLIKO NEKI OD NAVEDENIH USLOVA NIJE ZADOVOLJEN FUNKCIJA PRINTAJ DOKUMENT TREBA SAÈUVATI U NIZ _dokumenti, ALI TAJ DOKUMENT NE TREBA
	PRINTATI.
	UKOLIKO DOKUMENT ISPUNJAVA SVE USLOVE ZA PRINTANJE POTREBNO JE NA KONZOLU ISPISATI SADRŽAJ DOKUMENTA KOJI SE PRINTA, A SA CRT LINIJOM
	ODVAJATI STRANICE DOKUMENTA
	*/
	hp3200.Printaj(ispitPRII);
	hp3200.Printaj(ispitMAT);
	hp3200.Printaj(ispitUIT);
	hp3200.Printaj(ispitUITDrugiRok);

	//PROSJEÈAN BROJ STRANICA = UKUPAN BROJ STRANICA / BROJ PRINTANIH DOKUMENATA
	cout << "Prosjecan broj printanih stranica je -> " << hp3200.GetProsjecanBrojStranicaPoDatumuIVrsti(prije3Dana, danas2, DOC) << crt;
	//VRAÆA FILTER KOJI SE NAJÈEŠÆE KORISTIO U DOKUMENTIMA
	Filter * f = hp3200.GetNakoristenijiFilter();
	cout << "Najcesce koristeni sadrzaj filtera je -> " << f->_sadrzaj << crt;
	//PORED VRIJEDNOSTI SVIH OSTALIH ATRIBUTA, ISPISUJE SAMO ONE DOKUMENTE KOJI ZADOVOLJAVAJU SVA PRETHODNO DEFINISANA PRAVILA
	hp3200.Ispis();
	///*MIJENJA ZNAKOVE ZABRANJENOG SADRŽAJA DOKUMENTA SA POSLANIM KARAKTEROM
	//(BEZ OBZIRA NA PRIORITET) I NAKON TOGA PRINTA SADRŽAJ DOKUMENATA; npr.
	//UBITI POSTAJE -----.
	//RIJEŠITI REKURZIJOM*/
	ZamijeniIPrintaj(hp3200, '-'); //OSTALE PARAMETRE DODAJTE PO POTREBI
	prije3Dana.Dealociraj(); danas1.Dealociraj(); danas2.Dealociraj(); za10Dana.Dealociraj();
	ispitMAT.Dealociraj(); ispitPRII.Dealociraj(); ispitUIT.Dealociraj(); ispitUITDrugiRok.Dealociraj();

	hp3200.Dealociraj();
	system("pause");
}