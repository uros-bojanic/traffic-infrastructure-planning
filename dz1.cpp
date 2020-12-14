/* Uros Bojanic
* 2019/0077
* i = 1
* j = 1
*/

#include <iostream>
#include <iomanip>
#include <climits>
#include <string>
#include <vector>
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
                cout << setw(4) << grane[i][j];
            else
                cout << setw(4) << "-";
        }
        cout << endl;
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

///////////////////////////////////////////////////////////////////////////////////////////
/// Glavni program
///////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    Graf* graf = nullptr;
    bool exit_flag = false;

    while (!exit_flag) {
        cout << "--------------------------------------------" << endl;
        cout << "Izaberite opciju:" << endl;
        cout << "\t1. Kreiraj graf" << endl;
        cout << "\t2. Dodaj cvor u graf" << endl;
        cout << "\t3. Obrisi cvor iz grafa" << endl;
        cout << "\t4. Dodaj granu izmedju dva cvora" << endl;
        cout << "\t5. Obrisi granu izmedju dva cvora" << endl;
        cout << "\t6. Ispisi reprezentaciju grafa" << endl;
        cout << "\t7. Obrisi graf" << endl;
        cout << "\t0. Izadji iz programa (EXIT)" << endl;
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
        default:
            cout << "Nepravilan izbor opcije.";
            exit_flag = true;
        }
    }

    delete graf;
    graf = nullptr;
    cout << endl << "Program zavrsen.";

    return 0;
}