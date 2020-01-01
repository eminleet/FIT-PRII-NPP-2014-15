#include<iostream>

using namespace std;
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

enum enumKursevi { HtmlCSSJavaScript, SoftwareEngeneeringFundamentals, MasteringSQL, WindowsSecurity };
const char* crt = "\n--------------------------------------------\n";

struct Datum {
	int* _dan, * _mjesec, * _godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina; }
	void Dealociraj() { delete _dan; delete _mjesec; delete _godina; }
};

struct Kurs {
	enumKursevi _kurs;
	Datum* _pocetak;
	Datum* _kraj;
	char* _imePredavaca;
	float _cijenaKursa;

	void Unos(enumKursevi kurs, Datum pocetak, Datum kraj, const char* imePredavaca, float cijenaKursa) {
		_kurs = kurs;
		_pocetak->Unos(*pocetak._dan, *pocetak._mjesec, *pocetak._godina);
		_kraj->Unos(*kraj._dan, *kraj._mjesec, *kraj._godina);
		int size = strlen(imePredavaca) + 1;
		imePredavaca = new char[size];
		strcpy_s(imePredavaca, size, _imePredavaca);
		_cijenaKursa = cijenaKursa;
	}
	void Ispis() {
		cout << _kurs << " " << _imePredavaca << " " << _cijenaKursa << " (";
		_pocetak->Ispis(); cout << " - "; _kraj->Ispis(); cout << ")";
	}
	void Dealociraj() {
		delete[] _imePredavaca; _imePredavaca = nullptr;
		_pocetak->Dealociraj();
		_kraj->Dealociraj();
		delete _pocetak; _pocetak = nullptr;
		delete _kraj; _kraj = nullptr;
	}
};
struct Polaznik {
	char* _imePrezime;
	Datum* _datumRodjenja;
	void Unos(const char* imePrezime, Datum datumRodjenja) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		_datumRodjenja->Unos(*datumRodjenja._dan, *datumRodjenja._mjesec, *datumRodjenja._godina);
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		delete _datumRodjenja; _datumRodjenja = nullptr;
	}
	void Ispis() { cout << _imePrezime << "("; _datumRodjenja->Ispis(); cout << ")"; }
};
struct Prijava {
	Polaznik* _polaznik; //prijava moze biti pojedinacna ili grupna
	float* _uplaceniIznos;
	/*za svakog polaznika ce se cuvati podatak o iznosu uplate. uplaceni iznos svakog pojedinog polaznika treba biti jednak punoj cijeni kursa osim u sljedecim slucajevima:
	1. prijave 2 ili 3 polaznika dobijaju 10% popusta na ukupnu cijenu kursa
	2. prijave za vise od 3 polaznika dobijaju 15% na ukupnu cijenu kursa
	*/
	int _trenutnoPolaznika;
	Kurs* _kurs; //prijava treba cuvati adresu (pokazivati) na postojeci kurs, a ne kreirati novi
};

struct SkillsCentar {
	char* _nazivCentra;
	Kurs* _kursevi[50];//JEDAN EDUKACIJSKI CENTAR MOZE NUDITI NAJVISE 50 KURSEVA
	int _trenutnoKurseva;
	Prijava* _prijave;
	int _trenutnoPrijava;
};

void main() {

	/*
	1. BROJ I TIPOVI PARAMETARA MORAJU BITI IDENTICNI ONIMA U TESTNOM CODE-U (OSIM UKOLIKO IMATE OPRAVDANU POTREBU ZA MODIFIKACIJOM). U SUPROTNOM SE RAD NEĆE BODOVATI
	2. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA.
	NE PREDAVATI .TXT ILI .CPP FAJLOVE. TAKOĐER, UKLONITE SVE DIJELOVE CODE-A KOJI NISU IMPLEMENTIRANI
	3. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
	4. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ĆETE KOPIRATI VAŠA RJEŠENJA)
	5. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
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
	if (denisZaninDubravkaHtml.DodajPolaznika(denis, 700))//tokom prijave polaznika nije potrebno uplatiti kompletan iznos, ali on mora biti veci od 50% iznosa kojeg je u tom momentu potrebno platiti (u zavisnosti od broja prijavljenih tj. ostvarenog popusta)
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
	if (zaninIndiraHtml.DodajPolaznika(zanin, 800))
		cout << "Polaznik uspjesno dodan!" << endl;
	if (zaninIndiraHtml.DodajPolaznika(indira, 750))
		cout << "Polaznik uspjesno dodan!" << endl;

	float stanjeUplate = denisZaninDubravkaHtml.StatusUplate(zanin);//ako polaznik duguje novca funkcija vraca negativan iznos potrebne uplate
	cout << "Polaznik "; zanin.Ispis();
	if (stanjeUplate > 0)
		cout << " je u pretplati " << stanjeUplate << " KM" << endl;
	else
		cout << " je duzan uplatiti " << stanjeUplate * -1 << " KM" << endl;

	SkillsCentar mostar;
	mostar.Unos("Skills Center Mostar");
	//jedan edukacijski centar moze moze nuditi vise istih kurseva (npr. MasteringSQL), ali novi kurs moze poceti tek 15 dana nakon zavrsetka prethodnog kursa npr.
	//ako prethodni kurs MasteringSQL traje 01.03.2016 - 01.04.2016, onda se novi kurs MasteringSQL moze poceti nakon 15.04.2016
	if (mostar.DodajKurs(softverski)) {
		cout << crt << "KURS USPJESNO REGISTROVAN" << crt;
		softverski.Ispis();
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
	Polaznik* polazniciDugovanja = mostar.PolazniciDugovanja(brojPolaznika);

	cout << "Pronadjeno " << brojPolaznika << " polaznika koji nisu kompletirali svoje uplate --->>> ";
	for (size_t i = 0; i < brojPolaznika; i++)
		polazniciDugovanja[i].Ispis();

	//PretragaRekurzivno - rekurzivna funkcija pronalazi najveci iznos duga ostvarenog u odredjenom edukacijskom centru
	cout << "Najveci dug iznosi " << PretragaRekurzivno(/*potrebni parametri*/) << endl;
	//ispisuje sve informacije o edukacijskom centru. prije ispisa sortirati sve prijave po visini dugovanja njenih polaznika
	mostar.Ispis();

	//izvrsiti potrebne dealokacije
	system("pause");
}
