#include<iostream>
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


//Napisati program koji će, na osnovu odabranih komandi, omogućiti pomijeranje kocke unutar definisanog okvira.Prilikom pravljenja programa voditi računa o sljedećem :
//1. Program treba izgledati kao na narednoj slici
//2. Početna veličina matrice je 20 x 20, a početna veličina kocke je 2
//3. Veličina matrice se povećava sa povećavanjem kocke(komanda P)
//4. Veličina kocke ne smije biti manja od 1
//5. Komande za pomijeranje kocke su :
/// Lijevo - L / Desno - R / Gore - U / Dole - D / Povecaj - P / Smanji - S /
//6. Komande mogu biti unesene velikim ili malim slovima
//7. Kocka ne smije izaći iz definisanog okvira.U slučaju da korisnik 3 puta dotakne okvir igra se prekida.
//8. Voditi računa o pravilnoj alokaciji i dealokaciji memorije
struct Kocka {
	int lokacijaX;
	int lokacijaY;
	int velicina;
};

void main() {
	char** matrica = nullptr;
	Kocka kocka;
	char komanda;
	do {
		cout << "/Lijevo - L/Desno - R/Gore - U/Dole - D/" << endl;
		cout << "/Povecaj - P/ Smanji - S/" << endl;
		cout << "Komanda -> ";
		cin >> komanda;
		system("cls");
	} while (1);
}