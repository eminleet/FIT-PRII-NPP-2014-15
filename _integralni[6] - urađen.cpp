#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

const char * crt = "\n--------------------------------------------\n";

enum VrstaObaveze { Seminarski, Parcijalni1, Parcijalni2, Integralni, Prakticni };
const char * VrstaObavezeChar[] = { "Seminarski", "Parcijalni1", "Parcijalni2", "Integralni", "Prakticni" };

struct Datum {
	int *_dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d); _mjesec = new int(m); _godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() { delete _dan; delete _mjesec; delete _godina; _dan = _mjesec = _godina = nullptr; }
};
struct ObavezeNaPredmetu {
	VrstaObaveze _vrstaObaveze;
	Datum * _datumIzvrsenja;
	char * _napomena;
	int _ocjena; // 5 - 10
	void Unos(VrstaObaveze vrsta, Datum * datum, int ocjena, const char * napomena) {
		_vrstaObaveze = vrsta;
		_datumIzvrsenja = new Datum;
		_datumIzvrsenja->Unos(*datum->_dan, *datum->_mjesec, *datum->_godina);
		_ocjena = ocjena;
		int size = strlen(napomena) + 1;
		_napomena = new char[size];
		strcpy_s(_napomena, size, napomena);
	}
	void Ispis() {
		cout << VrstaObavezeChar[_vrstaObaveze] << " " << _ocjena << " " << _napomena << " ";
		_datumIzvrsenja->Ispis();
		cout << endl;
	}
	void Dealociraj() {
		_datumIzvrsenja->Dealociraj();
		delete _datumIzvrsenja; _datumIzvrsenja = nullptr;
		delete[] _napomena; _napomena = nullptr;
	}
	void DodajNapomenu(const char * napomena) {
		int size = strlen(_napomena) + strlen(napomena) + 3;
		char * tempNapomena = new char[size];
		strcpy_s(tempNapomena, size, _napomena);
		strcat_s(tempNapomena, size, "; ");
		strcat_s(tempNapomena, size, napomena);
		delete[] _napomena; _napomena = nullptr;
		_napomena = tempNapomena;
	}
};

void KopirajObavezu(ObavezeNaPredmetu & source, ObavezeNaPredmetu & dest) {
	dest._vrstaObaveze = source._vrstaObaveze;
	dest._datumIzvrsenja = new Datum;
	dest._datumIzvrsenja->Unos(*source._datumIzvrsenja->_dan, *source._datumIzvrsenja->_mjesec, *source._datumIzvrsenja->_godina);
	int len = strlen(source._napomena) + 1;
	dest._napomena = new char[len];
	strcpy_s(dest._napomena, len, source._napomena);
	dest._ocjena = source._ocjena;
}

int rataDie(int d, int m, int g) {
	if (m < 3)
		g--, m += 12;
	return 365 * g + g / 4 - g / 100 + g / 400 + (153 * m - 457) / 5 + d - 306;
}

struct PolozeniPredmet {
	char * _nazivPredmeta;
	ObavezeNaPredmetu * _listaIzvrsenihObaveza[10];//ogranicen je broj izvrsenih obavezan na predmetu
	int _trenutnoIzvrsenihObaveza;
	int _konacnaOcjena; //formira se na osnovu ocjena izvrsenih obaveza
	void Unos(const char * naziv) {
		int size = strlen(naziv) + 1;
		_nazivPredmeta = new char[size];
		strcpy_s(_nazivPredmeta, size, naziv);
		_trenutnoIzvrsenihObaveza = 0;
		_konacnaOcjena = 5;
	}
	void Dealociraj() {
		delete[] _nazivPredmeta; _nazivPredmeta = nullptr;
		for (size_t i = 0; i < _trenutnoIzvrsenihObaveza; i++) {
			_listaIzvrsenihObaveza[i]->Dealociraj();
			delete _listaIzvrsenihObaveza[i];
			_listaIzvrsenihObaveza[i] = nullptr;
		}
	}
	void Ispis() {
		cout << _nazivPredmeta << "(" << _konacnaOcjena << ")" << endl;
		for (size_t i = 0; i < _trenutnoIzvrsenihObaveza; i++)
			_listaIzvrsenihObaveza[i]->Ispis();
	}
	bool DodajIzvrsenuObavezu(ObavezeNaPredmetu predmet) {
		if (_trenutnoIzvrsenihObaveza >= 10) return false;
		for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++)
			if (_listaIzvrsenihObaveza[i]->_vrstaObaveze == predmet._vrstaObaveze && (_listaIzvrsenihObaveza[i]->_vrstaObaveze != Prakticni &&
				_listaIzvrsenihObaveza[i]->_vrstaObaveze != Seminarski || _listaIzvrsenihObaveza[i]->_ocjena == 5))
				return false;

		_listaIzvrsenihObaveza[_trenutnoIzvrsenihObaveza] = new ObavezeNaPredmetu;
		_listaIzvrsenihObaveza[_trenutnoIzvrsenihObaveza]->Unos(predmet._vrstaObaveze, predmet._datumIzvrsenja, predmet._ocjena, predmet._napomena);
		_trenutnoIzvrsenihObaveza++;
		return true;
	}
	int FormirajKonacnuOcjenu() {
		double ocjene = 0.0;
		bool imaPrvi = false, imaDrugi = false, imaIntegralni = false, imaPrakticni = false;
		int brValidnihSeminarskih = 0;
		int brOcjena = 0, indeksIntegralni = -1, indeksPrakticni = -1;
		for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++) {
			if (_listaIzvrsenihObaveza[i]->_vrstaObaveze == Parcijalni1 && _listaIzvrsenihObaveza[i]->_ocjena > 5)
				imaPrvi = true;
			if (_listaIzvrsenihObaveza[i]->_vrstaObaveze == Parcijalni2 &&_listaIzvrsenihObaveza[i]->_ocjena > 5)
				imaDrugi = true;
			if (_listaIzvrsenihObaveza[i]->_vrstaObaveze == Integralni && _listaIzvrsenihObaveza[i]->_ocjena > 5) {
				imaIntegralni = true;
				indeksIntegralni = i;
			}
			if (_listaIzvrsenihObaveza[i]->_vrstaObaveze == Prakticni && _listaIzvrsenihObaveza[i]->_ocjena > 5) {
				imaPrakticni = true;
				indeksPrakticni = i;
			}
			if (_listaIzvrsenihObaveza[i]->_vrstaObaveze == Seminarski && _listaIzvrsenihObaveza[i]->_ocjena > 5 &&
				strstr(_listaIzvrsenihObaveza[i]->_napomena, "nije dobro") == 0 &&
				strstr(_listaIzvrsenihObaveza[i]->_napomena, "kopirano") == 0 &&
				strstr(_listaIzvrsenihObaveza[i]->_napomena, "nije ispravljeno") == 0)
				brValidnihSeminarskih++;
		}
		if (imaPrvi && imaDrugi && brValidnihSeminarskih >= 2) {
			for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++)
				if (_listaIzvrsenihObaveza[i]->_ocjena > 5) {
					ocjene += _listaIzvrsenihObaveza[i]->_ocjena;
					brOcjena++;
				}
			ocjene /= brOcjena;
			_konacnaOcjena = ceil(ocjene);
			return _konacnaOcjena;
		}
		else if (imaIntegralni && imaPrakticni) {
			int brDanaIntegralni = rataDie(*_listaIzvrsenihObaveza[indeksIntegralni]->_datumIzvrsenja->_dan,
				*_listaIzvrsenihObaveza[indeksIntegralni]->_datumIzvrsenja->_mjesec,
				*_listaIzvrsenihObaveza[indeksIntegralni]->_datumIzvrsenja->_godina);
			int brDanaPrakticni = rataDie(*_listaIzvrsenihObaveza[indeksPrakticni]->_datumIzvrsenja->_dan,
				*_listaIzvrsenihObaveza[indeksPrakticni]->_datumIzvrsenja->_mjesec,
				*_listaIzvrsenihObaveza[indeksPrakticni]->_datumIzvrsenja->_godina);
			if (brDanaPrakticni - brDanaIntegralni <= 15) {
				for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++)
					if (_listaIzvrsenihObaveza[i]->_ocjena > 5) {
						ocjene += _listaIzvrsenihObaveza[i]->_ocjena;
						brOcjena++;
					}
				ocjene /= brOcjena;
				_konacnaOcjena = ceil(ocjene);
				return _konacnaOcjena;
			}
		}
		_konacnaOcjena = 5;
		return _konacnaOcjena;

	}
	/*
	1. DodajIzvrsenuObavezu – na osnovu vrijednosti primljenog parametra osigurati dodavanje novoizvrsene obaveze na predmetu.
	Potrebno je onemoguciti dodavanje identicnih obaveza. Identicna vrsta obaveze se moze dodati samo u slucaju da se radi o Prakticno radu
	ili je prethodno dodana obaveza (identicne vrste) imala ocjenu 5.
	2. FormirajKonacnuOcjenu - formiranje konacne ocjene se vrsi prema sljedecim pravilima:
	student mora posjedovati polozen integralni ili dva parijcalna ispita;
	ukoliko je ispit polozen putem parcijalnih ispita, student takodjer mora imati pozitivno (ocjenom vecom od 5) ocijenjena najmanje dva
	seminarska rada u okviru kojih nije evidentirana losa napomena (rijeci koje oznacavaju losu napomenu: nije dobro, kopirano,
	nije ispravljeno); u slucaju da je ispit polozen putem integralnog ispita student mora imati polozen i prakticni ispit
	(polozen najkasnije 15 dana od dana polaganja integralnog ispita).
	*/
};

void KopirajPredmet(PolozeniPredmet & source, PolozeniPredmet & destination) {
	int len = strlen(source._nazivPredmeta) + 1;
	destination._nazivPredmeta = new char[len];
	strcpy_s(destination._nazivPredmeta, len, source._nazivPredmeta);
	for (int i = 0; i < source._trenutnoIzvrsenihObaveza; i++) {
		destination._listaIzvrsenihObaveza[i] = new ObavezeNaPredmetu;
		KopirajObavezu(*source._listaIzvrsenihObaveza[i], *destination._listaIzvrsenihObaveza[i]);
	}
	destination._trenutnoIzvrsenihObaveza = source._trenutnoIzvrsenihObaveza;
	destination._konacnaOcjena = source._konacnaOcjena;
}

struct Student {
	char * _imePrezime;
	char * _brojIndeksa;
	PolozeniPredmet * _polozeniPredmeti;
	int _brojPolozenih;
	void Unos(const char * IB, const char * imePrezime) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		size = strlen(IB) + 1;
		_brojIndeksa = new char[size];
		strcpy_s(_brojIndeksa, size, IB);
		_brojPolozenih = 0;
		_polozeniPredmeti = nullptr;
	}
	bool DodajPolozeniPredmet(PolozeniPredmet &predmet) {
		for (int i = 0; i < _brojPolozenih; i++) {
			if (strcmp(_polozeniPredmeti[i]._nazivPredmeta, predmet._nazivPredmeta) == 0)
				return false;
		}
		
		if (predmet._konacnaOcjena <= 5)
			return false;
		
		for (int j = 0; j < predmet._trenutnoIzvrsenihObaveza; j++) {
			if (strstr(predmet._listaIzvrsenihObaveza[j]->_napomena, "prepisivao") != 0 ||
				strstr(predmet._listaIzvrsenihObaveza[j]->_napomena, "izbacen") != 0)
				return false;
		}
		PolozeniPredmet * temp = new PolozeniPredmet[_brojPolozenih + 1];
		for (int i = 0; i < _brojPolozenih; i++) {
			_polozeniPredmeti[i].Dealociraj();
			KopirajPredmet(_polozeniPredmeti[i], temp[i]);
		}
		KopirajPredmet(predmet, temp[_brojPolozenih]);
		delete[] _polozeniPredmeti; _polozeniPredmeti = nullptr;
		_polozeniPredmeti = temp;
		_brojPolozenih++;
		return true;

	}
	PolozeniPredmet * PronadjiNajuspjesnijuObavezu(VrstaObaveze obaveza) {
		int indeks = -1;
		int najvecaOcjena = 5;
		for (int i = 0; i < _brojPolozenih; i++) {
			for (int j = 0; j < _polozeniPredmeti[i]._trenutnoIzvrsenihObaveza; j++) {
				if (_polozeniPredmeti[i]._listaIzvrsenihObaveza[j]->_vrstaObaveze == obaveza &&
					_polozeniPredmeti[i]._listaIzvrsenihObaveza[j]->_ocjena >= najvecaOcjena) {
					najvecaOcjena = _polozeniPredmeti[i]._listaIzvrsenihObaveza[j]->_ocjena;
					indeks = i;
				}
			}
		}
		if (indeks != -1)
			return &_polozeniPredmeti[indeks];
		return nullptr;
	}
	/*
	1. DodajPolozeniPredmet - na osnovu vrijednosti primljenog parametra osigurati dodavanje novopolozenog predmeta.
	Potrebno je onemoguciti dodavanje identicnih predmeta, predmeta koja ne posjeduju formiranu konacnu ocjenu,
	te predmeta kod kojih je u napomeni neke od izvrsenih obaveza navedena rijec 'prepisivao' ili 'izbacen'.
	2. PronadjiNajuspjesnijuObavezu - vraca pokazivac na predmet u okviru koga je student ostvario najvecu ocjenu tokom realizacije 
	odredjene obaveze (vrsta obaveze je proslijedjena kao parametar). Ukoliko postoje dva identicna predmeta, onda funkcija vraca pokazivac 
	na posljednje pronadjeni predmet. U slucaju da obaveza nije pronadjena, funkcija vraca nullptr.
	*/

	void Ispis() {
		cout << _brojIndeksa << " " << _imePrezime << endl;
		for (size_t i = 0; i < _brojPolozenih; i++)
			_polozeniPredmeti[i].Ispis();
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		delete[] _brojIndeksa; _brojIndeksa = nullptr;
		for (size_t i = 0; i < _brojPolozenih; i++)
			_polozeniPredmeti[i].Dealociraj();
		delete[] _polozeniPredmeti;
		_polozeniPredmeti = nullptr;

	}
};

void UpisiUBinarniFajl(PolozeniPredmet & predmet) {
	char Konacna[3];
	_itoa_s(predmet._konacnaOcjena, Konacna, 10);
	const int max = 20;
	char nazivFajla[max];
	strcpy_s(nazivFajla, max, predmet._nazivPredmeta);
	strcat_s(nazivFajla, max, "_");
	strcat_s(nazivFajla, max, Konacna);
	ofstream output(nazivFajla, ios::binary);
	if (output.is_open()) {
		output.write((char*)(&predmet), sizeof(predmet));
		cout << crt << "Sadrzaj upisan u fajl!" << endl;
		output.close();
	}
	else
		cerr << crt << "Error! " << endl;
}

void UcitajIzBinarnogFajla(PolozeniPredmet & predmet, const char * nazivFajla) {
	ifstream input(nazivFajla);
	if (input.is_open()) {
		input.read((char*)(&predmet), sizeof(predmet));
		cout << crt << "Sadrzaj ucitan iz fajla!" << endl;
		input.close();
	}
	else
		cerr << crt << "Error! " << endl;
}
/*
1. UpisiUBinarniFajl - objekat koji je primljen kao parametar upisuje u binarni fajl. Naziv fajla treba biti sastavljen od naziva predmeta
i konacne ocjena npr. "Programiranje II_6"
2. UcitajIzBinarnogFajla - u objekat koji je primljen kao parametar ucitava sadrzaj binarnog fajla
(naziv fajla je takodjer proslijedjen kao parametar)
*/
int main() {
	/*
	1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NEÆE BODOVATI
	2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
	3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
	4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
	5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ÆETE KOPIRATI VAŠA RJEŠENJA)
	6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
	*/


	Datum datumSeminarski1, datumSeminarski2, datumParcijalni1, datumParcijalni2,
		datumIntegralni;
	datumSeminarski1.Unos(10, 6, 2016);
	datumSeminarski2.Unos(18, 6, 2016);
	datumParcijalni1.Unos(22, 3, 2016);
	datumParcijalni2.Unos(22, 6, 2016);


	ObavezeNaPredmetu parcijalni1, parcijalni2, seminarski1, seminarski2;
	parcijalni1.Unos(Parcijalni1, &datumParcijalni1, 6, "uslovno polozen");

	parcijalni2.Unos(Parcijalni2, &datumParcijalni2, 6, "previse gresaka");
	parcijalni2.DodajNapomenu("sugerisati da se vise paznje obrati na rad sa dinamickim nizovima");
	parcijalni2.Ispis();

	seminarski1.Unos(Seminarski, &datumSeminarski1, 8, "implementirani svi algoritmi");
	seminarski1.DodajNapomenu("predloziti ucesce na FITCC 2017");//omogucava dodavanje dodatnih napomena vezano za obeveze na predmetu

	seminarski2.Unos(Seminarski, &datumSeminarski2, 6, "rad slican kao kod studenta IB150388");

	//polozeni predmet
	PolozeniPredmet prII, prII_FromFile;
	prII.Unos("Programiranje II");
	if (prII.DodajIzvrsenuObavezu(parcijalni1))
		cout << "Parcijalni 1...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(parcijalni2))
		cout << "Parcijalni 2...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(seminarski1))
		cout << "Seminarski1...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(seminarski2))
		cout << "Seminarski2...dodan" << endl;

	datumParcijalni2.Unos(22, 7, 2016);
	parcijalni2._datumIzvrsenja->Unos(*datumParcijalni2._dan, *datumParcijalni2._mjesec, *datumParcijalni2._godina);

	if (prII.DodajIzvrsenuObavezu(parcijalni2))
		cout << "Parcijalni 2...dodan" << endl;
	cout << "Konacna ocjena iz predmeta PRII je: " << prII.FormirajKonacnuOcjenu() << endl;

	//manipulacija fajlovima
	UpisiUBinarniFajl(prII);
	prII.Ispis();
	UcitajIzBinarnogFajla(prII_FromFile, "Programiranje II_7");
	prII_FromFile.Ispis();
	Student jasmin;
	jasmin.Unos("IB130031", "Jasmin Azemovic");
	if (jasmin.DodajPolozeniPredmet(prII))
		cout << "PRII uspjesno dodan!" << endl;
	PolozeniPredmet * p = jasmin.PronadjiNajuspjesnijuObavezu(Seminarski);
	cout << "Najuspjesniji seminarski je realizovan u okviru predmeta "; p->Ispis();
	jasmin.Ispis();

	//izvrsiti potrebne dealokacije
	datumSeminarski1.Dealociraj(); datumSeminarski2.Dealociraj(); datumParcijalni1.Dealociraj(); datumParcijalni2.Dealociraj();
	parcijalni1.Dealociraj(); parcijalni2.Dealociraj(); seminarski1.Dealociraj(); seminarski2.Dealociraj();
	prII.Dealociraj();
	jasmin.Dealociraj();

	system("PAUSE > NULL");

}
