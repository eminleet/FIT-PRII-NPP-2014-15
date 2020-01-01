#include <iostream>
#include <fstream>

using namespace std;
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

enum Opstina { Opstina1, Opstina2, Opstina3, Opstina4 };
const char * OpstineChar[] = { "Opstina1", "Opstina2", "Opstina3", "Opstina4" };
enum Kandidati { Kandidat1, Kandidat2, Kandidat3, Kandidat4 };
const char * KandidatiChar[] = { "Kandidat1", "Kandidat2", "Kandidat3", "Kandidat4" };

struct Datum {
	int _dan, _mjesec, _godina;
	void Unos(int d, int m, int g) { _dan = d; _mjesec = m; _godina = g; }
	void Ispis() { cout << _dan << "/" << _mjesec << "/" << _godina << endl; }
};
bool manjiDatum(Datum d1, Datum d2) {
	if (d1._godina < d2._godina) return true;
	if (d1._godina == d2._godina && d1._mjesec < d2._mjesec) return true;
	if (d1._godina == d2._godina && d1._mjesec == d2._mjesec && d1._dan < d2._dan) return true;
	return false;
}
bool Punoljetni(Datum dRodjenja, Datum dIzbora) {
	if (dIzbora._godina - dRodjenja._godina > 18) return true;
	if (dIzbora._godina - dRodjenja._godina == 18 && dRodjenja._mjesec < dIzbora._mjesec) return true;
	return false;
}
struct Osoba {
	Datum _datumRodjenja;
	char * _imePrezime;
	char _JMBG[14];
	Opstina _Prebivaliste;
	void unos(Datum d, const char * ip, const char * jmbg, Opstina p) {
		_datumRodjenja.Unos(d._dan, d._mjesec, d._godina);
		_imePrezime = new char[strlen(ip) + 1];
		strcpy_s(_imePrezime, strlen(ip) + 1, ip);
		strcpy_s(_JMBG, strlen(jmbg) + 1, jmbg);
		_Prebivaliste = p;
	}
	void Dealociraj() { delete[] _imePrezime; _imePrezime = nullptr; }
	void Info() {
		_datumRodjenja.Ispis();
		cout << _imePrezime << " " << _JMBG << " " <<
			OpstineChar[_Prebivaliste] << endl;
	}
};
struct Glas {
	Osoba * _glasac;
	Kandidati * _glasZa;
	void Unos(Osoba o, Kandidati k) {
		_glasac = new Osoba;
		_glasac->unos(o._datumRodjenja, o._imePrezime, o._JMBG, o._Prebivaliste);
		_glasZa = new Kandidati(k);
	}
	void Ispis() {
		cout << endl;
		_glasac->Info();
		cout << KandidatiChar[*_glasZa] << endl;
	}
	void Dealociraj() { _glasac->Dealociraj(); delete _glasZa; _glasZa = nullptr; }
};
struct KandidatiGlas {
	Kandidati k;
	int brGlasova;
	void Unos(Kandidati kandidat) {
		k = kandidat;
		brGlasova = 0;
	}
};
struct Izbori2016 {
	Opstina * _glasackoMjesto;
	Glas * _glasovi[1000];
	int _doSadaGlasalo;
	void Unos(Opstina opstina) {
		_glasackoMjesto = new Opstina(opstina);
		_doSadaGlasalo = 0;
	}
	void Dealociraj() {
		delete _glasackoMjesto; _glasackoMjesto = nullptr;
		for (int i = 0; i < _doSadaGlasalo; i++)
			_glasovi[i]->Dealociraj();
	}
	bool DodajGlas(Glas &glas, Datum datumIzbora) {
		for (int i = 0; i < _doSadaGlasalo; i++) {
			if (strcmp(_glasovi[i]->_glasac->_JMBG, glas._glasac->_JMBG) == 0) {
				return false;
			}
		}
		if (!Punoljetni(glas._glasac->_datumRodjenja, datumIzbora))
			return false;
		if (glas._glasac->_Prebivaliste != *_glasackoMjesto)
			return false;

		_glasovi[_doSadaGlasalo] = new Glas;
		_glasovi[_doSadaGlasalo]->Unos(*glas._glasac, *glas._glasZa);
		_doSadaGlasalo++;
		return true;
	}
	void Sortiraj(KandidatiGlas * niz) {
		bool promjena = true;
		int brProlaza = 3; //Broj kandidata umanjen za 1
		while (promjena) {
			promjena = false;
			for (int i = 0; i < brProlaza; i++) {
				if (niz[i].brGlasova < niz[i + 1].brGlasova) {
					swap(niz[i], niz[i + 1]);
					promjena = true;
				}
			}
			brProlaza--;
		}
	}
	void Ispis() {
		KandidatiGlas niz[4];
		niz[0].Unos(Kandidat1);
		niz[1].Unos(Kandidat2);
		niz[2].Unos(Kandidat3);
		niz[3].Unos(Kandidat4);
		for (int i = 0; i < _doSadaGlasalo; i++) {
			if (*_glasovi[i]->_glasZa == Kandidat1)
				niz[0].brGlasova++;
			if (*_glasovi[i]->_glasZa == Kandidat2)
				niz[1].brGlasova++;
			if (*_glasovi[i]->_glasZa == Kandidat3)
				niz[2].brGlasova++;
			if (*_glasovi[i]->_glasZa == Kandidat4)
				niz[3].brGlasova++;
		}
		Sortiraj(niz);
		cout << endl << OpstineChar[*_glasackoMjesto] << endl;
		for (int i = 0; i < 4; i++) {
			cout << endl << KandidatiChar[niz[i].k] << " Procenat glasova: " << niz[i].brGlasova / float(_doSadaGlasalo) * 100. << " %." << endl;
		}	
	}
	int BrojGlasacaPoRodjenju(Datum OD, Datum DO) {
		if (!manjiDatum(OD, DO))
			swap(OD, DO);

		int brojac = 0;
		for (int i = 0; i < _doSadaGlasalo; i++) {
			if (manjiDatum(OD, _glasovi[i]->_glasac->_datumRodjenja) && manjiDatum(_glasovi[i]->_glasac->_datumRodjenja, DO)) {
				brojac++;
			}
		}
		return brojac;
	}
	/*
	1. Unos - na osnovu vrijednosti primljenih parametara, inicijalizovati vrijednosti atributa strukture.
	2. DodajGlas - funkcija treba da omoguci dodavanje informacija o novom glasu i tom prilikom onemoguciti: da ista osoba glasa vise puta,
	glasanje osobama mladjim od 18 godina (uzeti u obzir samo mjesec i godinu rodjenja), glasanje osobama iz drugih opstina. U zavisnosti od
	uspjesnosti operacije funkcija vraca true ili false
	3. Ispis - u zavisnosti od postignutog uspjeha funkcija treba ispisati listu kandidata sortiranu opadajucim redoslijedom, a uz svakog
	kandidata je potrebno ispisati i osvojeni procenat glasova.
	Funkcija vraca opstinu u kojoj je pobjednicki kandidat ostvario najveci broj glasova, a ukoliko je pobjednicki kandidat ostvario podjednak broj glasova u vise opstina, funkcija vraca prvu koja se nalazi u nizu na koji pokazuje pokazivac izbornaMjesta
	4. BrojGlasacaPoRodjenju - funkcija vraca broj glasaca koji su do tog momenta glasali, a koji su rodjeni u parametrima definisanom periodu 
	(parametri su tipa datum, period OD i DO)
	*/
};

int PretragaRekurzivno(Izbori2016 &izbori, Kandidati k, int brGlasova = 0, int brojacGlasova = 0) {
	if (brojacGlasova >= izbori._doSadaGlasalo) return brGlasova;
	if (*izbori._glasovi[brojacGlasova]->_glasZa == k) {
		brojacGlasova++;
		return(izbori, k, brGlasova, brojacGlasova + 1);
	}
	return(izbori, k, brGlasova, brojacGlasova + 1);
}

void UpisiUBinarniFajl(Izbori2016 &izbori) {
	ofstream outfile(OpstineChar[*izbori._glasackoMjesto]);
	if (outfile.is_open()) {
		outfile.write((char*)(&izbori), sizeof(izbori));
		outfile.close();
	}
	else
		cout << "Error !" << endl;
}

void UcitajIzBinarnogFajla(Izbori2016 &izbori, const char * opstina) {
	ifstream infile(opstina);
	if (infile.is_open()) {
		infile.read((char*)(&izbori), sizeof(izbori));
		infile.close();
	}
	else
		cout << "Error!" << endl;
}

/*
1. UpisiUBinarniFajl - objekat koji je primljen kao parametar upisuje u binarni fajl. Naziv fajla treba biti identican nazivu opstine u kojoj
se odrzavaju izbori
2. UcitajIzBinarnogFajla - u objekat koji je primljen kao parametar ucitava sadrzaj binarnog fajla (naziv fajla je takodjer proslijedjen kao 
parametar)
3. PretragaRekurzivno - rekurzivna funkcija koja treba da vrati broj glasova koje je na izborima ostvario odredjeni kandidat (kandidat se proslijedjuje kao parametar)
*/

void main() {
	/*
	1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NEÆE BODOVATI
	2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
	3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
	4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
	5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ÆETE KOPIRATI VAŠA RJEŠENJA)
	6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
	*/

	Datum datumGlasac1, datumGlasac2, datumGlasac3, datumGlasac4, datumIzbora;
	datumIzbora.Unos(14, 7, 2016);
	datumGlasac1.Unos(18, 5, 1990);
	datumGlasac2.Unos(5, 3, 1982);
	datumGlasac3.Unos(12, 8, 1958);
	datumGlasac4.Unos(22, 6, 2000);

	Osoba a, b, c, d, e;
	a.unos(datumGlasac1, "Denis Music", "1111111111111", Opstina1);
	b.unos(datumGlasac2, "Jasmin Azemovic", "1111111111112", Opstina1);
	c.unos(datumGlasac3, "Indira Hamulic", "1111111111113", Opstina1);
	d.unos(datumGlasac4, "Adel Handzic", "1111111111114", Opstina2);
	e.unos(datumGlasac4, "Zanin Vejzovic", "1111111111115", Opstina1);

	Glas g1, g2, g3, g4, g5;
	g1.Unos(a, Kandidat1);
	g2.Unos(b, Kandidat1);
	g3.Unos(c, Kandidat2);
	g4.Unos(d, Kandidat1);
	g5.Unos(e, Kandidat2);

	Izbori2016 izbori, izbori_FromFile;
	izbori.Unos(Opstina1);

	izbori.DodajGlas(g1, datumIzbora);
	izbori.DodajGlas(g2, datumIzbora);
	izbori.DodajGlas(g3, datumIzbora);
	izbori.DodajGlas(g4, datumIzbora);
	izbori.DodajGlas(g5, datumIzbora);
	izbori.Ispis();

	Datum OD, DO;
	OD.Unos(17, 7, 1998);
	DO.Unos(17, 7, 1986);
	cout << "Glasaca u starost od 18 - 30 godina->" << izbori.BrojGlasacaPoRodjenju(OD, DO) << endl;
	cout << "Broj glasova za Kandidat1->" << PretragaRekurzivno(izbori, Kandidat1) << endl;
	UpisiUBinarniFajl(izbori);
	UcitajIzBinarnogFajla(izbori_FromFile, "Opstina1");
	izbori_FromFile.Ispis();
	system("pause");
}