/* Uros Bojanic
* 2019/0077
* i = 1
* j = 1
*/

#include <iostream>
#include <iomanip>
#include <climits>
#include <string>
#include <algorithm>
#include <vector>
#include <queue>
#include <deque>
#include <utility>
#include <unordered_set>
//#include <bits/stdc++.h>
using namespace std;

// Cvorovi su predstavljeni kao string, a tezine grana kao int
constexpr auto PRAZAN_CVOR = "";
constexpr int PRAZNA_GRANA = INT_MAX;

class Graf {
private:
    int n;
    vector<string> cvorovi;
    vector<vector<int>> grane;
public:
    Graf(int dim);
    ~Graf() {}  /*  Kako smo za potrebe dinamickog niza koristili vector iz STL,
                    odgovarajuci destruktor ce biti automatski pozvan.*/
    void povecaj();
    void dodaj_cvor(string cvor);
    int dohvati_indeks_cvora(string cvor);
    void obrisi_cvor(string cvor);
    void dodaj_granu(string cvor1, string cvor2, int tezina);
    void obrisi_granu(string cvor1, string cvor2);
    void ispisi_reprezentaciju();
    // DODATAK: Zadatak 2. (1)
    bool proveri_postojanje_cvorova();
    int primov_algoritam();
    // DODATAK: Zadatak 2. (2)
    void dajkstrin_algoritam_stampaj(priority_queue<pair<int, deque<int>>, vector<pair<int, deque<int>>>, greater<pair<int, deque<int>>>>& putevi, deque<int>put, int pocetni_cvor, vector<deque<int>> prethodnik, int rastojanje);
    void dajkstrin_algoritam();
};

Graf::Graf(int dim) {
    n = dim;
    cvorovi.resize(dim, PRAZAN_CVOR);
    grane.resize(dim, vector<int>(dim, PRAZNA_GRANA));
}

void Graf::povecaj() {
    for (int i = 0; i < n; i++) {
        grane[i].push_back(PRAZNA_GRANA);
    }
    n++;
    grane.push_back(vector<int>(n, PRAZNA_GRANA));
    cvorovi.push_back(PRAZAN_CVOR);
}

void Graf::dodaj_cvor(string cvor) {
    for (int i = 0; i < n; i++) {
        if (cvorovi[i] == PRAZAN_CVOR) {
            cvorovi[i] = cvor;
            break;
        }
    }
    grane[dohvati_indeks_cvora(cvor)][dohvati_indeks_cvora(cvor)] = 0;
}

int Graf::dohvati_indeks_cvora(string cvor) {
    for (int i = 0; i < n; i++) {
        if (cvorovi[i] == cvor) {
            return i;
        }
    }
    return -1;
}

void Graf::obrisi_cvor(string cvor) {
    int ind = dohvati_indeks_cvora(cvor);
    for (int i = 0; i < n; i++) {
        grane[i].erase(grane[i].begin() + ind);
    }
    grane.erase(grane.begin() + ind);
    n--;
    cvorovi.erase(cvorovi.begin() + ind);
}

void Graf::dodaj_granu(string cvor1, string cvor2, int tezina) {
    int cvor1_ind = dohvati_indeks_cvora(cvor1);
    int cvor2_ind = dohvati_indeks_cvora(cvor2);
    grane[cvor1_ind][cvor2_ind] = tezina;
    grane[cvor2_ind][cvor1_ind] = tezina;
}

void Graf::obrisi_granu(string cvor1, string cvor2) {
    dodaj_granu(cvor1, cvor2, PRAZNA_GRANA);
}

void Graf::ispisi_reprezentaciju() {
    cout << setw(4) << " ";
    for (int i = 0; i < n; i++) {
        cout << setw(4) << (cvorovi[i] == PRAZAN_CVOR ? "/" : cvorovi[i]);
    }
    cout << endl;
    for (int i = 0; i < n; i++) {
        cout << setw(4) << (cvorovi[i] == PRAZAN_CVOR ? "/" : cvorovi[i]);
        for (int j = 0; j < n; j++) {
            if (grane[i][j] < PRAZNA_GRANA)
                cout << setw(4) << abs(grane[i][j]);
            else
                cout << setw(4) << "-";
        }
        cout << endl;
    }
}

// DODATAK: Zadatak 2. (1)
bool Graf::proveri_postojanje_cvorova() {
    for (int i = 0; i < n; i++) {
        if (cvorovi[i] == "") {
            return 0;
        }
    }
    return 1;
}

int Graf::primov_algoritam() {
    // Inicijalizacija
    vector<int> prethodnik(n, INT_MAX);
    vector<int> rastojanje(n, INT_MAX);
    unordered_set<int> poseceno;
    // Odredjivanje pocetnog cvora (nasumicno)
    int pocetni_cvor = rand() % n;
    rastojanje[pocetni_cvor] = 0;
    prethodnik[pocetni_cvor] = -1;
    // Primov algoritma
    for (int grana = 1; grana < n; grana++) {
        // Odabrati neposeceni cvor sa najmanjim rastojanjem
        int cvor1 = -1;
        int rastojanje_min = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (poseceno.count(i) == 0 && rastojanje[i] < rastojanje_min) {
                cvor1 = i;
                rastojanje_min = rastojanje[i];
            }
        }
        // Posetiti ga
        poseceno.insert(cvor1);
        for (int cvor2 = 0; cvor2 < n; cvor2++) {
            if (poseceno.count(cvor2) == 0 && grane[cvor1][cvor2] < rastojanje[cvor2]) {
                prethodnik[cvor2] = cvor1;
                rastojanje[cvor2] = grane[cvor1][cvor2];
            }
        }
    }
    // Stampanje resenja
    int cena = 0;
    for (int cvor2 = 0; cvor2 < n; cvor2++) {
        if (cvor2 != pocetni_cvor) {
            int cvor1 = prethodnik[cvor2];
            cena += grane[cvor1][cvor2];
        }
    }
    return cena;
}

// DODATAK: Zadatak 2. (2)
void Graf::dajkstrin_algoritam_stampaj(priority_queue<pair<int, deque<int>>, vector<pair<int, deque<int>>>, greater<pair<int, deque<int>>>>& putevi, deque<int>put, int pocetni_cvor, vector<deque<int>> prethodnik, int rastojanje) {
    int trenutni_cvor = put.back();
    if (trenutni_cvor == pocetni_cvor) {
        //cout << "\t";
        int rastojanje_ukupno = 0, broj_izgradnji = 0;
        for (int i = put.size() - 1; i > 0; i--) {
            rastojanje_ukupno += abs(grane[put[i]][put[i-1]]);
            broj_izgradnji += grane[put[i]][put[i-1]] < 0;
        }
        if (rastojanje_ukupno == rastojanje) {
            putevi.push(make_pair(broj_izgradnji, put));
        }
        //cout << put[0] << endl << "\tDuzina puta: " << rastojanje_ukupno << " (Potrebnih izgradnji puteva: " << broj_izgradnji << ")" << endl;
    }
    else {
        deque<int>put_novi = put;
        vector<deque<int>> prethodnik_novi = prethodnik;
        while (!prethodnik_novi[trenutni_cvor].empty()) {
            put_novi.push_back(prethodnik_novi[trenutni_cvor].front());
            prethodnik_novi[trenutni_cvor].pop_front();
            dajkstrin_algoritam_stampaj(putevi, put_novi, pocetni_cvor, prethodnik_novi, rastojanje);
             put_novi.pop_back();
        }
    }
}

void Graf::dajkstrin_algoritam() {
    // Inicijalizacija
    vector<deque<int>> prethodnik(n);
    vector<int> rastojanje(n, INT_MAX);
    unordered_set<int> poseceno;
    // Odredjivanje pocetnog cvora (vatrogasne stanice)
    int pocetni_cvor = dohvati_indeks_cvora("V");
    rastojanje[pocetni_cvor] = 0;
    prethodnik[pocetni_cvor].push_front(-1);
    // Dajkstrin algoritam
    for (int grana = 1; grana < n; grana++) {
        // Odabrati neposeceni cvor sa najmanjim rastojanjem
        int cvor1 = -1;
        int rastojanje_min = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (poseceno.count(i) == 0 && rastojanje[i] < rastojanje_min) {
                cvor1 = i;
                rastojanje_min = rastojanje[i];
            }
        }
        // Posetiti ga
        poseceno.insert(cvor1);
        for (int cvor2 = 0; cvor2 < n; cvor2++) {
            if (poseceno.count(cvor2) == 0 && grane[cvor1][cvor2] < INT_MAX && rastojanje[cvor1] != INT_MAX && rastojanje[cvor1] + abs(grane[cvor1][cvor2]) < rastojanje[cvor2]) {
                prethodnik[cvor2].push_front(cvor1);
                rastojanje[cvor2] = rastojanje[cvor1] + abs(grane[cvor1][cvor2]);
            }
            else if (poseceno.count(cvor2) == 0 && grane[cvor1][cvor2] < INT_MAX && rastojanje[cvor1] != INT_MAX && rastojanje[cvor1] + abs(grane[cvor1][cvor2]) == rastojanje[cvor2]) {
                prethodnik[cvor2].push_back(cvor1);
            }
        }
    }
    // Stampanje resenja
    for (int krajnji_cvor = 0; krajnji_cvor < n; krajnji_cvor++) {
        priority_queue<pair<int, deque<int>>, vector<pair<int, deque<int>>>, greater<pair<int, deque<int>>>> putevi;
        if (krajnji_cvor != pocetni_cvor) {
            cout << "Putevi za cvor " << cvorovi[krajnji_cvor] << ":" << endl;
            deque<int>put;
            put.push_back(krajnji_cvor);
            dajkstrin_algoritam_stampaj(putevi, put, pocetni_cvor, prethodnik, rastojanje[krajnji_cvor]);
        }
        while(!putevi.empty()){
            cout << "\t";
            for (int i = putevi.top().second.size() - 1; i > 0; i--) {
                cout << cvorovi[putevi.top().second[i]] << "-";
            }
            cout << cvorovi[putevi.top().second[0]] << endl << "\tDuzina puta: " << rastojanje[krajnji_cvor] << " (Potrebnih izgradnji puteva: " << putevi.top().first << ")" << endl;
            putevi.pop();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
/// Funkcije korisnickog interfejsa
///////////////////////////////////////////////////////////////////////////////////////////

void kreiraj_graf(Graf** graf) {
    if (*graf != nullptr) {
        cout << "Graf vec postoji!" << endl;
        return;
    }

    cout << "Kreiranje grafa... Unesite velicinu grafa: ";
    int dim;
    cin >> dim;

    if (dim <= 0) {
        cout << "Nepravilan unos velicine grafa! Velicina grafa mora biti pozitivan ceo broj." << endl;
        return;
    }

    *graf = new Graf{ dim };
    cout << "Graf uspesno kreiran!" << endl;
}

void dodaj_cvor(Graf* graf) {
    if (graf == nullptr) {
        cout << "Graf ne postoji!" << endl;
        return;
    }

    cout << "Dodavanje cvora..." << endl;
    cout << "Unesite naziv cvora: ";
    string cvor;
    cin >> cvor;

    if (cvor.length() > 3) {
        cout << "Nepravilan unos cvora! Naziv cvora mora biti string ne duzi od 3 karaktera." << endl;
        return;
    }

    if (graf->dohvati_indeks_cvora(cvor) != -1) {
        cout << "Vec postoji taj cvor!" << endl;
        return;
    }

    graf->dodaj_cvor(cvor);

    if (graf->dohvati_indeks_cvora(cvor) == -1) {
        cout << "Nema vise slobodnih cvorova u grafu!" << endl;
        cout << "Da li zelite da prosirite graf? Unesite 1(da) ili 0(ne): ";
        int povecaj;
        cin >> povecaj;
        if (povecaj == 1) {
            graf->povecaj();
            graf->dodaj_cvor(cvor);
        }
        else if (povecaj == 0) {
            return;
        }
        else {
            cout << "Nepravilan izbor opcije." << endl;
            return;
        }
    }

    cout << "Cvor: " << cvor << " uspesno dodat!" << endl;
}

void obrisi_cvor(Graf* graf) {
    if (graf == nullptr) {
        cout << "Graf ne postoji!" << endl;
        return;
    }

    cout << "Brisanje cvora... Unesite naziv cvora: ";
    string cvor;
    cin >> cvor;

    if (graf->dohvati_indeks_cvora(cvor) == -1) {
        cout << "Nepravilan unos cvora! Cvor ne postoji u grafu." << endl;
        return;
    }

    graf->obrisi_cvor(cvor);
    cout << "Cvor: " << cvor << " uspesno obrisan!" << endl;
}

void dodaj_granu(Graf* graf, bool samo_pozitivne_tezine) {
    if (graf == nullptr) {
        cout << "Graf ne postoji!" << endl;
        return;
    }

    cout << "Dodavanje grane..." << endl << "Unesite prvi cvor: ";
    string cvor1, cvor2;
    cin >> cvor1;

    if (graf->dohvati_indeks_cvora(cvor1) == -1) {
        cout << "Nepravilan unos cvora! Cvor ne postoji u grafu." << endl;
        return;
    }

    cout << "Unesite drugi cvor: ";
    cin >> cvor2;

    if (graf->dohvati_indeks_cvora(cvor2) == -1) {
        cout << "Nepravilan unos cvora! Cvor ne postoji u grafu." << endl;
        return;
    }

    int tezina;
    cout << "Unesite tezinu: ";
    cin >> tezina;

    if (tezina <= 0) {
        cout << "Nepravilan unos tezine! Tezina mora biti pozitivna vrednost." << endl;
        return;
    }

    if (!samo_pozitivne_tezine) {
        cout << "Da li je put izgradjen(1) ili izgradljiv(0)? Uneti 1 ili 0: ";
        int izgradjen;
        cin >> izgradjen;
        if (izgradjen == 0) {
            tezina *= -1;
        }
        else if (izgradjen != 1) {
            cout << "Nepravilan izbor opcije." << endl;
            return;
        }
    }

    graf->dodaj_granu(cvor1, cvor2, tezina);
    cout << "Cvorovi: " << cvor1 << " i " << cvor2 << " uspesno povezani! (" << abs(tezina) << ")" << endl;
}

void obrisi_granu(Graf* graf) {
    if (graf == nullptr) {
        cout << "Graf ne postoji!" << endl;
        return;
    }

    cout << "Brisanje grane..." << endl;
    cout << "Unesite prvi cvor: ";
    string cvor1, cvor2;
    cin >> cvor1;

    if (graf->dohvati_indeks_cvora(cvor1) == -1) {
        cout << "Nepravilan unos cvora! Cvor ne postoji u grafu." << endl;
        return;
    }

    cout << "Unesite drugi cvor: ";
    cin >> cvor2;

    if (graf->dohvati_indeks_cvora(cvor2) == -1) {
        cout << "Nepravilan unos cvora! Cvor ne postoji u grafu." << endl;
        return;
    }

    graf->obrisi_granu(cvor1, cvor2);
    cout << "Cvorovi: " << cvor1 << " i " << cvor2 << " uspesno 'odvezani'!" << endl;
}

void ispisi_reprezentaciju(Graf* graf) {
    if (graf == nullptr) {
        cout << "Graf ne postoji!" << endl;
        return;
    }

    cout << "Ispisivanje reprezentacije grafa..." << endl;
    graf->ispisi_reprezentaciju();
}

void obrisi_graf(Graf** graf) {
    if (*graf == nullptr) {
        cout << "Graf ne postoji!" << endl;
        return;
    }

    cout << "Brisanje grafa..." << endl;
    delete* graf;
    *graf = nullptr;
    cout << "Graf uspesno obrisan!" << endl;
}

// DODATAK: Zadatak 2. (1)
void primov_algoritam(Graf* graf) {
    if (graf == nullptr) {
        cout << "Graf ne postoji!" << endl;
        return;
    }

    if (!graf->proveri_postojanje_cvorova()) {
        cout << "Cvorovi nisu inicijalizovani! Postoje prazni cvorovi u grafu." << endl;
        return;
    }

    cout << "Primov algoritam..." << endl;
    int cena_metroa = graf->primov_algoritam();
    cout << "Ukupna cena izgradnje metroa: " << cena_metroa << "." << endl;
}

// DODATAK: Zadatak 2. (2)
void dajkstrin_algoritam(Graf* graf) {
    if (graf == nullptr) {
        cout << "Graf ne postoji!" << endl;
        return;
    }

    if (!graf->proveri_postojanje_cvorova()) {
        cout << "Cvorovi nisu inicijalizovani! Postoje prazni cvorovi u grafu." << endl;
        return;
    }

    if(graf->dohvati_indeks_cvora("V") == -1) {
        cout << "Graf ne sadrzi cvor V (vatrogasnu stanicu)!" << endl;
        return;
    }

    cout << "Dajkstrin algoritam..." << endl;
    graf->dajkstrin_algoritam();
}

///////////////////////////////////////////////////////////////////////////////////////////
/// Modovi rada glavnog programa
///////////////////////////////////////////////////////////////////////////////////////////

void mod1_metro() {
    Graf* graf = nullptr;
    bool exit_flag = false;

    while (!exit_flag) {
        cout << "--------------------------------------------" << endl;
        cout << "Izaberite opciju:" << endl;
        cout << "\t1. Kreiraj grad (graf)" << endl;
        cout << "\t2. Dodaj lokaciju (cvor) u grad (graf)" << endl;
        cout << "\t3. Obrisi lokaciju (cvor) iz grada (grafa)" << endl;
        cout << "\t4. Dodaj metro liniju (granu) izmedju dve lokacije (cvora)" << endl;
        cout << "\t5. Obrisi metro liniju (granu) izmedju dve lokacije (cvora)" << endl;
        cout << "\t6. Ispisi reprezentaciju grada (grafa)" << endl;
        cout << "\t7. Obrisi grad (graf)" << endl;
        cout << "\t8. Izracunaj cenu izgradnje metroa (Primov algoritam)" << endl;
        cout << "\t0. Povratak u glavni meni (BACK)" << endl;
        cout << "Unesite zeljeni broj i pritisnite ENTER... ";

        int opcija;
        cin >> opcija;

        switch (opcija) {
        case 0:
            exit_flag = true;
            break;
        case 1:
            kreiraj_graf(&graf);
            break;
        case 2:
            dodaj_cvor(graf);
            break;
        case 3:
            obrisi_cvor(graf);
            break;
        case 4:
            dodaj_granu(graf, 1);
            break;
        case 5:
            obrisi_granu(graf);
            break;
        case 6:
            ispisi_reprezentaciju(graf);
            break;
        case 7:
            obrisi_graf(&graf);
            break;
        case 8:
            primov_algoritam(graf);
            break;
        default:
            cout << "Nepravilan izbor opcije." << endl;
        }
    }

    delete graf;
    graf = nullptr;
    return;
}

void mod2_vatrogasci() {
    Graf* graf = nullptr;
    bool exit_flag = false;

    while (!exit_flag) {
        cout << "--------------------------------------------" << endl;
        cout << "Izaberite opciju:" << endl;
        cout << "\t1. Kreiraj grad (graf)" << endl;
        cout << "\t2. Dodaj lokaciju (cvor) u grad (graf)" << endl;
        cout << "\t3. Obrisi lokaciju (cvor) iz grada (grafa)" << endl;
        cout << "\t4. Dodaj put (postojeci ili izgradljiv) izmedju dve lokacije (cvora)" << endl;
        cout << "\t5. Obrisi put (postojeci ili izgradljiv) izmedju dve lokacije (cvora)" << endl;
        cout << "\t6. Ispisi reprezentaciju grada (grafa)" << endl;
        cout << "\t7. Obrisi grad (graf)" << endl;
        cout << "\t8. Izracunaj najkrace putanje od vatrogasne stanice do svih lokacija" << endl;
        cout << "\t0. Povratak u glavni meni (BACK)" << endl;
        cout << "Unesite zeljeni broj i pritisnite ENTER... ";

        int opcija;
        cin >> opcija;

        switch (opcija) {
        case 0:
            exit_flag = true;
            break;
        case 1:
            kreiraj_graf(&graf);
            break;
        case 2:
            dodaj_cvor(graf);
            break;
        case 3:
            obrisi_cvor(graf);
            break;
        case 4:
            dodaj_granu(graf, 0);
            break;
        case 5:
            obrisi_granu(graf);
            break;
        case 6:
            ispisi_reprezentaciju(graf);
            break;
        case 7:
            obrisi_graf(&graf);
            break;
        case 8:
            dajkstrin_algoritam(graf);
            break;
        default:
            cout << "Nepravilan izbor opcije." << endl;
        }
    }

    delete graf;
    graf = nullptr;
    return;
}

///////////////////////////////////////////////////////////////////////////////////////////
/// Glavni program
///////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    int opcija = 1;
    while(opcija != 0) {
        cout << "--------------------------------------------" << endl;
        cout << "Izaberite mod rada programa:" << endl;
        cout << "\t1. Stvaranje i ispis metroa" << endl;
        cout << "\t2. Pronalazenje najkrace putanje za vatrogasna kola" << endl;
        cout << "\t0. Izadji iz programa (EXIT)" << endl;
        cout << "Unesite zeljeni broj i pritisnite ENTER... ";
        cin >> opcija;
        switch (opcija) {
        case 0:
            break;
        case 1:
            mod1_metro();
            break;
        case 2:
            mod2_vatrogasci();
            break;
        default:
            cout << "Nepravilan izbor opcije." << endl;
        }
    }
    
    cout << "Program zavrsen.";
    return 0;
}