#include<iostream>
using namespace std;
char * crt = "\n---------------------------\n";
struct Datum {
int dan, mjesec, godina;
};
struct PolozeniPredmet {
char * nazivPredmeta;
int ocjena;
Datum datumPolaganja;
char * napomena;
};
struct Student {
char * imePrezime;
PolozeniPredmet polozeni[40];
int brojPolozenih;
};
int PrikaziMenu() {
int izbor;
cout << crt << "\t::MENU::" << crt;
cout << "1. Dodaj studenta" << endl;
cout << "2. Dodaj polozeni predmet" << endl;
cout << "3. Pretraga" << endl;
cout << "4. Kraj" << crt;
do {
cout << "Vas izbor -> ";
cin >> izbor;
} while (izbor < 1 || izbor>4);
system("cls");
return izbor;
}
/*UKOLIKO SMATRATE DA JE POTREBNO ZADATAK MOZETE PROSIRITI DODATNIM FUNKCIONALNOSTIMA, ALI PROSIRENJA MORAJU PRATITI ADEKVATNI KOMENTARI*/

void main() {
const int maxStudenata = 6;
int trenutnoStudenata = 0, izbor = 0;
Student * studentiIgodina[maxStudenata];
char validPass[] = "ispitPRII2017";
/*VRSITI VALIDACIJU KORISNIKOVIH UNOSA TE GA ONEMOGUCITI DA UNOSI VRIJEDNOSTI KOJE NISU VALIDNE*/
do {
/*
za pristup aplikaciji zahtijevati logiranje,
prilikom unosa password maskirati koristeći crticu (-)
*/
Login(validPass);

izbor = PrikaziMenu();
if (izbor == 1) {
DodajStudenta(studentiIgodina[trenutnoStudenata]);
/*
Ime i prezime -> Denis Mušić
Koliko predmeta dodajete -> 1
Naziv predmeta -> Uvod u informacijske tehnologije
Datum polaganja -> 11 4 2017
Ocjena -> 6
Napomena -> seminarski rad nije sadrzavao sve zahtijevane funkcionalnosti
*/
trenutnoStudenata++;
}
else if (izbor == 2) {
DodajPolozeniPredmet(studentiIgodina, trenutnoStudenata);
/*
Ime i prezime -> Denis Music
STUDENT PRONADJEN....
Naziv predmeta -> Racunarske mreze
Datum polaganja -> 10 4 2017
Ocjena -> 7
Napomena -> studentu dodijeljen seminarski rad na temu OSI modela
*/
}
else if (izbor == 3) {
Pretraga(studentiIgodina, trenutnoStudenata);
/*
Pratraga (1. Datumu polaganja, 2. Napomeni) -> 1
Unesite datum OD -> 12 1 2017
Unesite datum DO -> 14 4 2017
U periodu od 12.1.2017 do 14.4.2017 ispite su polozili sljedeci studenti:
1. Denis Music -> Racunarske mreze (7) Prosjek studenta: 6.5
2. Denis Music -> Uvod u informacijske tehnologije (6) Prosjek studenta: 6.5
3. Adel Handzic -> Baze podataka (9) Prosjek studenta: 8.7

----------UKOLIKO SE ODABERE OPCIJA 2---------------
Pratraga (1. Datumu polaganja, 2. Napomeni) -> 2
Unesite tekst -> nije sadrzavao
Trazeni sadrzaj 'nije sadrzavao' u napomeni posjeduju sljedeci studenti:
1. Denis Music, predmet Uvod u informacijske tehnologije
2. ....
*/
}

} while (izbor != 4);


/*oslobađa zauzetu memoriju*/
Dealociraj(studentiIgodina, trenutnoStudenata);
}
