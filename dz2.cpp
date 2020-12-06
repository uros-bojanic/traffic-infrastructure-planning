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
using namespace std;

class Graf {
private:
	int n;
	vector<string> cvorovi;
	vector<vector<int>> grane;
	// DODATAK: Zadatak 2. (2)
	struct put_za_vatrogasna_kola;
	struct Node_putevi;
public:
	Graf(int dim);
	~Graf() {}	/*	Kako smo za potrebe dinamickog niza koristili vector iz STL,
					odgovarajuci destruktor ce biti automatski pozvan.*/
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
	int dohvati_indeks_cvora_sa_min_rastojanjem(vector<int> rastojanje, vector<bool> poseceno);
	void rekurzija(Node_putevi** putevi, int cvor2, int rastojanje, const put_za_vatrogasna_kola put);
	int dajkstrin_algoritam();
};

Graf::Graf(int dim) {
	n = dim;
	cvorovi.resize(dim);
	grane.resize(dim, vector<int>(dim, 0));
	for (int i = 0; i < dim; i++) {
		cvorovi[i] = "";			// prazan string = marker za nepostojeci cvor
		for (int j = 0; j < dim; j++) {
			grane[i][j] = INT_MAX;	// INT_MAX = marker za nepostojecu granu
		}
	}
}

void Graf::dodaj_cvor(string cvor) {
	for (int i = 0; i < n; i++) {
		if (cvorovi[i] == "") {
			cvorovi[i] = cvor;
			break;
		}
	}
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
	for (int i = 0; i < n; i++) {
		obrisi_granu(cvor, cvorovi[i]);
	}
	cvorovi[dohvati_indeks_cvora(cvor)] = "";
}

void Graf::dodaj_granu(string cvor1, string cvor2, int tezina) {
	int cvor1_ind = dohvati_indeks_cvora(cvor1);
	int cvor2_ind = dohvati_indeks_cvora(cvor2);
	grane[cvor1_ind][cvor2_ind] = tezina;
	grane[cvor2_ind][cvor1_ind] = tezina;
}

void Graf::obrisi_granu(string cvor1, string cvor2) {
	dodaj_granu(cvor1, cvor2, INT_MAX);
}

void Graf::ispisi_reprezentaciju() {
	cout << setw(4) << " ";
	for (int i = 0; i < n; i++) {
		cout << setw(4) << (cvorovi[i] == "" ? "/" : cvorovi[i]);
	}
	cout << endl;
	for (int i = 0; i < n; i++) {
		cout << setw(4) << (cvorovi[i] == "" ? "/" : cvorovi[i]);
		for (int j = 0; j < n; j++) {
			if (grane[i][j] < INT_MAX)
				cout << setw(4) << grane[i][j];
			else
				cout << setw(4) << "-";
		}
		cout << endl;
	}
}

// DODATAK: Zadatak 2. (1)

bool  Graf::proveri_postojanje_cvorova() {
	for (int i = 0; i < n; i++) {
		if (cvorovi[i] == "") {
			return 0;
		}
	}
	return 1;
}

int Graf::primov_algoritam() {
	if (!proveri_postojanje_cvorova()) {
		return -1;
	}

	unordered_set<int> poseceno;

	// Nasumicno biramo pocetak
	int pocetni_cvor = rand() % n;
	poseceno.insert(pocetni_cvor);
	
	// Primov algoritam za nalazenje minimalnog obuhvatnog stabla
	int grana = 1, cena = 0;
	while (grana < n) {
		int min = INT_MAX, cvor1 = -1, cvor2 = -1;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (abs(grane[i][j]) < min) {
					if (poseceno.count(i) + poseceno.count(j) == 1) {
						min = abs(grane[i][j]);
						cvor1 = i;
						cvor2 = j;
					}
				}
			}
		}

		// Stampanje resenja
		if (cvor1 != -1 && cvor2 != -1) {
			cout << "Metro linija " << grana << ": " << cvor1 << "-" << cvor2 << " (" << min << ")" << endl;
			grana++;
			cena += min;
			poseceno.insert(cvor1);
			poseceno.insert(cvor2);
		}
	}
	return cena;
}

// DODATAK: Zadatak 2. (2)

struct Graf::put_za_vatrogasna_kola {
	vector<int> red;
	int cena;
	int broj_izgradnji;
};

struct Graf::Node_putevi {
	put_za_vatrogasna_kola data;
	Node_putevi* next;
};

int Graf::dohvati_indeks_cvora_sa_min_rastojanjem(vector<int> rastojanje, vector<bool> poseceno) {
	int min = INT_MAX;
	int min_index = -1;
	for (int i = 0; i < n; i++) {
		if (rastojanje[i] <= min && poseceno[i] == 0) {
			min = rastojanje[i];
			min_index = i;
		}
	}
	return min_index;
}

void Graf::rekurzija(Node_putevi** putevi, int cvor2, int rastojanje, const put_za_vatrogasna_kola put) {
	if (cvor2 == 0) {
		// Dodaj tekuci put u moguce puteve za dati cvor, na odgovarajuce mesto u neopadajucoj listi
		if (put.cena == rastojanje) {
			Node_putevi* novi_put = new Node_putevi;
			novi_put->data = put;
			if ((*putevi) == nullptr || (*putevi)->data.broj_izgradnji >= novi_put->data.broj_izgradnji) {
				novi_put->next = (*putevi);
				(*putevi) = novi_put;
			}
			else {
				Node_putevi* tmp = (*putevi);
				while (tmp->next != nullptr && tmp->next->data.broj_izgradnji < novi_put->data.broj_izgradnji) {
					tmp = tmp->next;
				}
				novi_put->next = tmp->next;
				tmp->next = novi_put;
			}
			return;
		}
	}
	// Pronadji rekurzivno sve moguce puteve za dati cvor
	for (int cvor1 = 0; cvor1 < n; cvor1++) {
		if (grane[cvor1][cvor2] < INT_MAX) {
			put_za_vatrogasna_kola put_novi;
			put_novi.red = put.red;
			put_novi.red.push_back(cvor1);
			put_novi.cena = put.cena + abs(grane[cvor1][cvor2]);
			put_novi.broj_izgradnji = put.broj_izgradnji + grane[cvor1][cvor2] < 0;
			rekurzija(putevi, cvor1, rastojanje, put_novi);
		}
	}
}

int Graf::dajkstrin_algoritam() {
	if (!proveri_postojanje_cvorova()) {
		return -1;
	}
	// Inicijalizacija potrebnih markera
	vector<int> rastojanje(n, INT_MAX);
	vector<bool> poseceno(n, 0);
	// Odredjivanje pocetnog cvora (vatrogasne stanice)
	int pocetni_cvor = dohvati_indeks_cvora("V");
	rastojanje[pocetni_cvor] = 0;
	// Dajkstrin algoritam
	for (int grana = 1; grana < n; grana++) {
		int j = dohvati_indeks_cvora_sa_min_rastojanjem(rastojanje, poseceno);
		poseceno[j] = true;
		for (int i = 0; i < n; i++) {
			if (!poseceno[i] && grane[j][i] < INT_MAX && rastojanje[j] != INT_MAX && rastojanje[j] + grane[j][i] < rastojanje[i]) {
				rastojanje[i] = rastojanje[j] + abs(grane[j][i]);
			}
		}
	}
	// Pripisivanje smerova granama (zbog dalje modifikacije Dajkstrinog algoritma)
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (grane[i][j] != INT_MAX) {
				if (rastojanje[i] < rastojanje[j]) {
					grane[j][i] = INT_MAX;
				}
				else {
					grane[i][j] = INT_MAX;
				}
			}
		}
	}
	// Stampanje puteva
	for (int cvor2 = 1; cvor2 < n; cvor2++) {
		Node_putevi* putevi = nullptr;
		cout << "Putevi za cvor " << cvor2 << ":" << endl;
		// Rekurzivno nalazenje mogucih puteva do datog cvora
		put_za_vatrogasna_kola put;
		put.red.push_back(cvor2);
		put.cena = put.broj_izgradnji = 0;
		rekurzija(&putevi, cvor2, rastojanje[cvor2], put);
		// Stampanje puteva
		Node_putevi* tmp = putevi;
		while (tmp != nullptr) {
			cout << "\t";
			for (int i = tmp->data.red.size() - 1; i > 0 ; i--) {
				cout << tmp->data.red[i] << "-";
			}
			cout << tmp->data.red[0] << endl;
			cout << "\tDuzina puta: " << tmp->data.cena << " (Potrebnih izgradnji puteva: " << tmp->data.broj_izgradnji << ")" << endl;
			// Oslobadjanje memorije
			Node_putevi* obr = tmp;
			tmp = tmp->next;
			free(obr);
		}
	}
	// Popravljanje privremeno usmerenih grana
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (grane[i][j] == INT_MAX && grane[j][i] != INT_MAX) {
				grane[i][j] = grane[j][i];
			}
			if (grane[i][j] != INT_MAX && grane[j][i] == INT_MAX) {
				grane[j][i] = grane[i][j];
			}
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
/// Funkcije korisnickog interfejsa
///////////////////////////////////////////////////////////////////////////////////////////

bool kreiraj_graf(Graf** graf) {
	if (*graf != nullptr) {
		cout << "Graf vec postoji!";
		return 1;
	}

	cout << "Kreiranje grafa... Unesite velicinu grafa: ";
	int dim;
	cin >> dim;

	if (dim <= 0) {
		cout << "Nepravilan unos velicine grafa!";
		return 1;
	}

	*graf = new Graf{ dim };
	cout << "Graf uspesno kreiran!" << endl;
	return 0;
}

bool dodaj_cvor(Graf* graf) {
	if (graf == nullptr) {
		cout << "Graf ne postoji!";
		return 1;
	}

	cout << "Dodavanje cvora... Unesite naziv cvora: ";
	string cvor;
	cin >> cvor;

	if (cvor == "") {
		cout << "Nepravilan unos cvora!";
		return 1;
	}

	if (graf->dohvati_indeks_cvora(cvor) != -1) {
		cout << "Vec postoji taj cvor!";
		return 1;
	}

	graf->dodaj_cvor(cvor);

	if (graf->dohvati_indeks_cvora(cvor) == -1) {
		cout << "Nema vise slobodnih cvorova u grafu!";
		return 1;
	}

	cout << "Cvor: " << cvor << " uspesno dodat!" << endl;
	return 0;
}

bool obrisi_cvor(Graf* graf) {
	if (graf == nullptr) {
		cout << "Graf ne postoji!";
		return 1;
	}

	cout << "Brisanje cvora... Unesite naziv cvora: ";
	string cvor;
	cin >> cvor;

	if (graf->dohvati_indeks_cvora(cvor) == -1) {
		cout << "Nepravilan unos cvora!";
		return 1;
	}

	graf->obrisi_cvor(cvor);
	cout << "Cvor: " << cvor << " uspesno obrisan!" << endl;
	return 0;
}

bool dodaj_granu(Graf* graf, bool samo_pozitivne_tezine) {
	if (graf == nullptr) {
		cout << "Graf ne postoji!";
		return 1;
	}

	cout << "Dodavanje grane..." << endl << "Unesite prvi cvor: ";
	string cvor1, cvor2;
	cin >> cvor1;
	cout << "Unesite drugi cvor: ";
	cin >> cvor2;

	if (graf->dohvati_indeks_cvora(cvor1) == -1 || graf->dohvati_indeks_cvora(cvor2) == -1) {
		cout << "Nepravilan unos cvora!";
		return 1;
	}

	int tezina;
	cout << "Unesite tezinu: ";
	cin >> tezina;

	if (tezina < 0) {
		cout << "Nepravilan unos tezine!";
		return 1;
	}

	if (!samo_pozitivne_tezine) {
		cout << "Da li je put izgradjen(1) ili izgradljiv(0)? Uneti 1 ili 0: ";
		int izgradjen;
		cin >> izgradjen;
		if (izgradjen == 0) {
			tezina *= -1;
		}
	}

	graf->dodaj_granu(cvor1, cvor2, tezina);
	cout << "Cvorovi: " << cvor1 << " i " << cvor2 << " uspesno povezani! (" << tezina << ")" << endl;
	return 0;
}

bool obrisi_granu(Graf* graf) {
	if (graf == nullptr) {
		cout << "Graf ne postoji!";
		return 1;
	}

	cout << "Brisanje grane... Unesite prvi cvor: ";
	string cvor1, cvor2;
	cin >> cvor1;
	cout << "Unesite drugi cvor: ";
	cin >> cvor2;

	if (graf->dohvati_indeks_cvora(cvor1) == -1 || graf->dohvati_indeks_cvora(cvor2) == -1) {
		cout << "Nepravilan unos cvora!";
		return 1;
	}

	graf->obrisi_granu(cvor1, cvor2);
	cout << "Cvorovi: " << cvor1 << " i " << cvor2 << " uspesno odvezani!" << endl;
	return 0;
}

bool ispisi_reprezentaciju(Graf* graf) {
	if (graf == nullptr) {
		cout << "Graf ne postoji!";
		return 1;
	}

	cout << "Ispisivanje reprezentacije grafa..." << endl;
	graf->ispisi_reprezentaciju();
	return 0;
}

bool obrisi_graf(Graf** graf) {
	if (*graf == nullptr) {
		cout << "Graf ne postoji!";
		return 1;
	}

	cout << "Brisanje grafa..." << endl;
	delete* graf;
	*graf = nullptr;
	cout << "Graf uspesno obrisan!" << endl;
	return 0;
}

bool primov_algoritam(Graf* graf) {
	if (graf == nullptr) {
		cout << "Graf ne postoji!";
		return 1;
	}

	cout << "Primov algoritam..." << endl;
	int cena_puta = graf->primov_algoritam();

	if (cena_puta == -1) {
		cout << "Cvorovi nisu inicijalizovani!";
		return 1;
	}

	cout << "Ukupna cena izgradnje metroa: " << cena_puta << "." << endl;
	return 0;
}

bool dajkstrin_algoritam(Graf* graf) {
	if (graf == nullptr) {
		cout << "Graf ne postoji!";
		return 1;
	}

	cout << "Dajkstrin algoritam..." << endl;
	int rezultat = graf->dajkstrin_algoritam();
	if (rezultat == -1) {
		cout << "Cvorovi nisu inicijalizovani!";
		return 1;
	}
	return 0;
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
		cout << "\t0. Izadji iz programa (EXIT)" << endl;
		cout << "Unesite zeljeni broj i pritisnite ENTER... ";

		int opcija;
		cin >> opcija;

		switch (opcija) {
		case 0:
			exit_flag = true;
			break;
		case 1:
			exit_flag = kreiraj_graf(&graf);
			break;
		case 2:
			exit_flag = dodaj_cvor(graf);
			break;
		case 3:
			exit_flag = obrisi_cvor(graf);
			break;
		case 4:
			exit_flag = dodaj_granu(graf, 1);
			break;
		case 5:
			exit_flag = obrisi_granu(graf);
			break;
		case 6:
			exit_flag = ispisi_reprezentaciju(graf);
			break;
		case 7:
			exit_flag = obrisi_graf(&graf);
			break;
		case 8:
			exit_flag = primov_algoritam(graf);
			break;
		default:
			cout << "Nepravilan izbor opcije.";
			exit_flag = true;
		}
	}

	delete graf;
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
		cout << "\t0. Izadji iz programa (EXIT)" << endl;
		cout << "Unesite zeljeni broj i pritisnite ENTER... ";

		int opcija;
		cin >> opcija;

		switch (opcija) {
		case 0:
			exit_flag = true;
			break;
		case 1:
			exit_flag = kreiraj_graf(&graf);
			break;
		case 2:
			exit_flag = dodaj_cvor(graf);
			break;
		case 3:
			exit_flag = obrisi_cvor(graf);
			break;
		case 4:
			exit_flag = dodaj_granu(graf, 0);
			break;
		case 5:
			exit_flag = obrisi_granu(graf);
			break;
		case 6:
			exit_flag = ispisi_reprezentaciju(graf);
			break;
		case 7:
			exit_flag = obrisi_graf(&graf);
			break;
		case 8:
			exit_flag = dajkstrin_algoritam(graf);
			break;
		default:
			cout << "Nepravilan izbor opcije.";
			exit_flag = true;
		}
	}

	delete graf;
	return;
}

///////////////////////////////////////////////////////////////////////////////////////////
/// Glavni program
///////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	cout << "--------------------------------------------" << endl;
	cout << "Izaberite mod rada programa:" << endl;
	cout << "\t1. Stvaranje i ispis metroa" << endl;
	cout << "\t2. Pronalazenje najkrace putanje za vatrogasna kola" << endl;
	cout << "\t0. Izadji iz programa (EXIT)" << endl;
	cout << "Unesite zeljeni broj i pritisnite ENTER... ";

	int opcija;
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
	
	cout << endl << "Program zavrsen.";
	return 0;
}