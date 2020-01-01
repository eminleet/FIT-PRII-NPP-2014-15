#include<iostream>

using namespace std;
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

enum VrstaObaveze { Seminarski, Parcijalni1, Parcijalni2, Integralni, Prakticni };
struct Datum {
	int* _dan, * _mjesec, * _godina;
	void Unos(int d, int m, int g) {
		_dan = &d; _mjesec = &m; _godina = &g;
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() { delete _dan; delete _mjesec; delete _godina; }
};
struct ObavezeNaPredmetu {
	VrstaObaveze _vrstaObaveze;
	Datum* _datumIzvrsenja;
	char* _napomena;
	int _ocjena; // 5 - 10
	void Unos(VrstaObaveze vrsta, Datum* datum, int ocjena, const char* napomena) {
		_vrstaObaveze = vrsta;
		_datumIzvrsenja = new Datum;
		_datumIzvrsenja->Unos(*datum->_dan, *datum->_mjesec, *datum->_godina);
		_ocjena = ocjena;
		int size = strlen(napomena) + 1;
		_napomena = new char[size];
		strcpy_s(_napomena, size, napomena);
	}
	void Ispis() {
		cout << _vrstaObaveze << " " << _ocjena << " " << _napomena;
		_datumIzvrsenja->Ispis();
		cout << endl;
	}
	void Dealociraj() {
		_datumIzvrsenja->Dealociraj();
		delete[] _datumIzvrsenja; _datumIzvrsenja = nullptr;
		delete[] _napomena; _napomena = nullptr;
	}
};
struct PolozeniPredmet {
	char* _nazivPredmeta;
	ObavezeNaPredmetu* _listaIzvrsenihObaveza[10];//ogranicen je broj izvrsenih obavezan na predmetu
	int _trenutnoIzvrsenihObaveza;
	int _konacnaOcjena; //formira se na osnovu ocjena izvrsenih obaveza
	void Unos(const char* naziv) {
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





	/*
	1. DodajIzvrsenuObavezu – na osnovu vrijednosti primljenog parametra osigurati dodavanje novoizvrsene obaveze na predmetu. Potrebno je onemoguciti dodavanje identicnih obaveza. Identicna vrsta obaveze se moze dodati samo u slucaju da se radi o Prakticno radu ili je prethodno dodana obaveza (identicne vrste) imala ocjenu 5.
	2. FormirajKonacnuOcjenu - formiranje konacne ocjene se vrsi prema sljedecim pravilima: student mora posjedovati polozen integralni ili dva parijcalna ispita; ukoliko je ispit polozen putem parcijalnih ispita, student takodjer mora imati pozitivno (ocjenom vecom od 5) ocijenjena najmanje dva seminarska rada u okviru kojih nije evidentirana losa napomena (rijeci koje oznacavaju losu napomenu: nije dobro, kopirano, nije ispravljeno); u slucaju da je ispit polozen putem integralnog ispita student mora imati polozen i prakticni ispit (polozen najkasnije 15 dana od dana polaganja integralnog ispita).
	*/
};

struct Student {
	char* _imePrezime;
	char* _brojIndeksa;
	PolozeniPredmet* _polozeniPredmeti;
	int _brojPolozenih;
	void Unos(const char* IB, const char* imePrezime) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		size = strlen(IB) + 1;
		_brojIndeksa = new char[size];
		strcpy_s(_brojIndeksa, size, IB);
		_brojPolozenih = 0;
	}

	/*
	1. DodajPolozeniPredmet - na osnovu vrijednosti primljenog parametra osigurati dodavanje novopolozenog predmeta. Potrebno je onemoguciti dodavanje identicnih predmeta, predmeta koja ne posjeduju formiranu konacnu ocjenu, te predmeta kod kojih je u napomeni neke od izvrsenih obaveza navedena rijec 'prepisivao' ili 'izbacen'.
	2. PronadjiNajuspjesnijuObavezu - vraca pokazivac na predmet u okviru koga je student ostvario najvecu ocjenu tokom realizacije odredjene obaveze (vrsta obaveze je proslijedjena kao parametar). Ukoliko postoje dva identicna predmeta, onda funkcija vraca pokazivac na posljednje pronadjeni predmet. U slucaju da obaveza nije pronadjena, funkcija vraca nullptr.
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

/*
1. UpisiUBinarniFajl - objekat koji je primljen kao parametar upisuje u binarni fajl. Naziv fajla treba biti sastavljen od naziva predmeta i konacne ocjena npr. "Programiranje II_6"
2. UcitajIzBinarnogFajla - u objekat koji je primljen kao parametar ucitava sadrzaj binarnog fajla (naziv fajla je takodjer proslijedjen kao parametar)
*/
void main() {
	/*
	1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NEĆE BODOVATI
	2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
	3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
	4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
	5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ĆETE KOPIRATI VAŠA RJEŠENJA)
	6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
	*/


	Datum datumSeminarski1, datumSeminarski2, datumParcijalni1, datumParcijalni2,
		datumIntegralni;
	datumSeminarski1.Unos(10, 6, 2016);
	datumSeminarski2.Unos(18, 6, 2016);
	datumParcijalni1.Unos(22, 3, 2016);
	datumParcijalni2.Unos(22, 6, 2016);

	ObavezeNaPredmetu parcijalni1, parcijalni2, seminarski1, seminarski2;
	parcijalni1.Unos(Parcijalni1, &datumParcijalni1, 6, "uslovno polozen");

	parcijalni2.Unos(Parcijalni2, &datumParcijalni2, 5, "previse gresaka");
	parcijalni2.DodajNapomenu("sugerisati da se vise paznje obrati na rad sa dinamickim nizovima");

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
	parcijalni2._datumIzvrsenja = &datumParcijalni2;

	if (prII.DodajIzvrsenuObavezu(parcijalni2))
		cout << "Parcijalni 2...dodan" << endl;
	cout << "Konacna ocjena iz predmeta PRII je: " << prII.FormirajKonacnuOcjenu() << endl;

	//manipulacija fajlovima
	UpisiUBinarniFajl(prII);
	prII.Ispis();
	UcitajIzBinarnogFajla(prII_FromFile, "Programiranje II_6");
	prII_FromFile.Ispis();

	Student jasmin;
	jasmin.Unos("IB130031", "Jasmin Azemovic");
	if (jasmin.DodajPolozeniPredmet(prII))
		cout << "PRII uspjesno dodan!" << endl;
	PolozeniPredmet* p = jasmin.PronadjiNajuspjesnijuObavezu(Seminarski);
	cout << "Najuspjesniji seminarski je realizovan u okviru predmeta "; p->Ispis();
	jasmin.Ispis();

	//izvrsiti potrebne dealokacije

}
