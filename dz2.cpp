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
	int primov_algoritam();
	// DODATAK: Zadatak 2. (2)
	int dohvati_min_index(vector<int> dist, vector<bool> poseceno);
	void stampaj_put(vector<int> prethodnik, int i);
	void rekurzija(Node_putevi** putevi, int cvor2, put_za_vatrogasna_kola* put);
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

// DODATAK: Zadatak 2. (1)

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

// DODATAK: Zadatak 2. (2)

struct Graf::put_za_vatrogasna_kola{
	vector<int> red;
	int cena;
	int broj_izgradnji;
};

struct Graf::Node_putevi{
	put_za_vatrogasna_kola data;
	Node_putevi* next;
};

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

void Graf::rekurzija(Node_putevi** putevi, int cvor2, put_za_vatrogasna_kola* put) {
	if(cvor2 == 0) {
		cout << "Konacno: " << endl;
		for(int i = 0; i < put->red.size() - 1; i++) {
			cout << put->red[i] << "-";
		}
		cout << put->red[put->red.size() - 1] << endl;
		cout << "Cena: " << put->cena << "(" << put->broj_izgradnji << ")" << endl << "-------------------" << endl;

		Node_putevi* novi_put = new Node_putevi;
		novi_put->data = (*put);
		novi_put->next = (*putevi);
		(*putevi) = novi_put;

		return;
	}
	for(int cvor1 = 0; cvor1 < n; cvor1++) {
		if(grane[cvor1][cvor2] < INT_MAX) {
			put_za_vatrogasna_kola put_copy = (*put);
			put_copy.red.push_back(cvor1);
			put_copy.cena += abs(grane[cvor1][cvor2]);
			put_copy.broj_izgradnji += grane[cvor1][cvor2] < 0;

			cout << "{";
			for(int i = 0; i < put_copy.red.size(); i++) {
				cout << put_copy.red[i] << "-";
			}
			cout << "}" << endl;

			rekurzija(putevi, cvor1, &put_copy);
		}
	}
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

	ispisi_reprezentaciju();

	// Pripisivanje smerova
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			if(grane[i][j] != INT_MAX) {
				if(dist[i] < dist[j]) {
					grane[j][i] = INT_MAX;
				}
				else {
					grane[i][j] = INT_MAX;
				}
			}
		}
	}

	ispisi_reprezentaciju();

	// Stampanje puteva
	for(int cvor2 = 1; cvor2 < n; cvor2++) {
		Node_putevi* putevi = nullptr;
		cout << "Putevi za cvor " << cvor2 << ":" << endl;
		put_za_vatrogasna_kola put;
		put.red.push_back(cvor2);
		rekurzija(&putevi, cvor2, &put);

		Node_putevi* tmp = putevi;
		cout << "*************************" << endl;
		cout << "JEBENO KONACNO: " << endl;
		while(tmp != nullptr){
			cout << tmp->data.cena << "(" << tmp->data.broj_izgradnji << ")" << endl;
			tmp = tmp->next;
		}
		cout << "*************************" << endl;

		cout << endl;
	}

	// Popravljanje smerova
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			if(grane[i][j] == INT_MAX && grane[j][i] != INT_MAX) {
				grane[i][j] = grane[j][i];
			}
			if(grane[i][j] != INT_MAX && grane[j][i] == INT_MAX) {
				grane[j][i] = grane[i][j];
			}
		}
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