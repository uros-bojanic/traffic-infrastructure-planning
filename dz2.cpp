/* Uros Bojanic
 * 2019/0077
 * i = 1
 * j = 1
*/

#include <bits/stdc++.h>
using namespace std;

class Graf {
private:
	int n;
	vector<string> cvorovi;
	vector<vector<int>> grane;
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
	int primov_algoritam();
	int dohvati_min_index(vector<int> dist, vector<bool> poseceno);
	void stampaj_put(vector<int> prethodnik, int i);
	void dajkstrin_algoritam();
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
	cout << setw(3) << " ";
	for (int i = 0; i < n; i++) {
		cout << setw(3) << (cvorovi[i] == "" ? "/" : cvorovi[i]);
	}
	cout << endl;
	for (int i = 0; i < n; i++) {
		cout << setw(3) << (cvorovi[i] == "" ? "/" : cvorovi[i]);
		for (int j = 0; j < n; j++) {
			if(grane[i][j] < INT_MAX)
				cout << setw(3) << grane[i][j];
			else
				cout << setw(3) << "-";
		}
		cout << endl;
	}
}

int Graf::primov_algoritam() {
	unordered_set<int> poseceno;
	poseceno.insert(0);
	int grana = 1, cena_puta = 0;
	while (grana < n) {
		int min = INT_MAX, cvor1 = -1, cvor2 = -1;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (grane[i][j] < min) {
					if (poseceno.count(i) + poseceno.count(j) == 1) {
						min = grane[i][j];
						cvor1 = i;
						cvor2 = j;
					}
				}
			}
		}
		if (cvor1 != -1 && cvor2 != -1) {
			cout << "Metro linija " << grana << ": " << cvor1 << "-" << cvor2 << " (" << min << ")" << endl;
			grana++;
			cena_puta += min;
			poseceno.insert(cvor1);
			poseceno.insert(cvor2);
		}
	}
	return cena_puta;
}

int Graf::dohvati_min_index(vector<int> dist, vector<bool> poseceno) {
	int min = INT_MAX, min_index;
	for (int i = 0; i < n; i++) {
		if (dist[i] <= min && poseceno[i] == 0) {
			min = dist[i];
			min_index = i;
		}
	}
	return min_index;
}

void Graf::stampaj_put(vector<int> prethodnik, int i) {
	if(prethodnik[i] == -1) return;
	stampaj_put(prethodnik, prethodnik[i]);
	cout << "-" << i;
}

void Graf::dajkstrin_algoritam() {
	vector<int> dist(n, INT_MAX);
	vector<bool> poseceno(n, 0);
	vector<int> prethodnik(n, -1);
	dist[0] = 0;
	for (int grana = 1; grana < n; grana++) {
		int j = dohvati_min_index(dist, poseceno);
		poseceno[j] = true;
		for (int i = 0; i < n; i++) {
			if (!poseceno[i] && grane[j][i] < INT_MAX && dist[j] != INT_MAX && dist[j] + grane[j][i] < dist[i]) {
				prethodnik[i] = j;
				dist[i] = dist[j] + grane[j][i]; 
			}
		}
	}
	cout << "cvor\trastojanje\tput" << endl;
	for (int i = 0; i < n; i++) {
		cout << i << "\t" << dist[i] << "\t\t0";
		stampaj_put(prethodnik, i);
		cout << endl;
	}
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

	if(graf->dohvati_indeks_cvora(cvor) != -1) {
		cout << "Vec postoji taj cvor!";
		return 1;
	}

	graf->dodaj_cvor(cvor);

	if(graf->dohvati_indeks_cvora(cvor) == -1) {
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

bool dodaj_granu(Graf* graf) {
	if (graf == nullptr) {
		cout << "Graf ne postoji!";
		return 1;
	}

	cout << "Dodavanje grane... Unesite prvi cvor: ";
	string cvor1, cvor2;
	cin >> cvor1;
	cout << "Unesite drugi cvor: ";
	cin >> cvor2;
	int tezina;
	cout << "Unesite tezinu: ";
	cin >> tezina;

	if (graf->dohvati_indeks_cvora(cvor1) == -1 || graf->dohvati_indeks_cvora(cvor2) == -1) {
		cout << "Nepravilan unos cvora!";
		return 1;
	}
	if (tezina < 0) {
		cout << "Nepravilan unos tezine!";
		return 1;
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
	delete *graf;
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
	cout << "Ukupna cena izgradnje metroa: " << cena_puta << "." << endl;
	return 0;
}

bool dajkstrin_algoritam(Graf* graf) {
	if (graf == nullptr) {
		cout << "Graf ne postoji!";
		return 1;
	}

	cout << "Dajkstrin algoritam..." << endl;
	graf->dajkstrin_algoritam();
	return 0;
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
		cout << "\t8. Primov algoritam" << endl;
		cout << "\t9. Dajkstrin algoritam" << endl;
		cout << "\t0. Izadji iz programa (EXIT)" << endl;
		cout << "Unesite zeljeni broj i pritisnite ENTER... ";

		int opcija;
		cin >> opcija;

		switch(opcija){
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
				exit_flag = dodaj_granu(graf);
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
			case 9:
				exit_flag = dajkstrin_algoritam(graf);
				break;
			default:
				cout << "Nepravilan izbor opcije.";
				exit_flag = true;
		}
	}

	delete graf;
	cout << endl << "Program zavrsen.";

	return 0;
}