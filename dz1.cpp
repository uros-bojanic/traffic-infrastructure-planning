#include <bits/stdc++.h>
using namespace std;

/* Uros Bojanic
 * 2019/0077
 * i = 1
 * j = 1
*/

class Graf {
private:
	int n;
	vector<string> cvorovi;
	vector<vector<int>> grane;

public:
	// Konstruktor
	Graf(int dim) {
		n = dim;
		// Cvorovi
		cvorovi.resize(dim);
		for (int i = 0; i < dim; i++) {
			cvorovi[i] = "";
		}
		// Grane
		grane.resize(dim, vector<int>(dim, 0));
		for (int i = 0; i < dim; i++) {
			for (int j = 0; j < dim; j++) {
				grane[i][j] = 0;
			}
		}
	}

	// Destruktor
	~Graf() {
		/*	Kako smo za potrebe dinamickog niza koristili vector iz STL,
			nema potrebe za "rucnim" oslobadjanjem memorije.*/
	}

	// Metode
	void dodaj_cvor(string cvor){
		for(int i = 0; i < n; i++){
			if(cvorovi[i] == ""){
				cvorovi[i] = cvor;
				break;
			}
		}
	}

	int dohvati_indeks_cvora(string cvor){
		for(int i = 0; i < n; i++){
			if(cvorovi[i]==cvor){
				return i;
			}
		}
		return -1;
	}

	void obrisi_cvor(string cvor){
		for(int i = 0; i < n; i++){
			obrisi_granu(cvor, cvorovi[i]);
		}
		cvorovi[dohvati_indeks_cvora(cvor)] = "";
	}

	void dodaj_granu(string cvor1, string cvor2, int tezina) {
		int cvor1_ind = dohvati_indeks_cvora(cvor1);
		int cvor2_ind = dohvati_indeks_cvora(cvor2);
		grane[cvor1_ind][cvor2_ind] = tezina;
		grane[cvor2_ind][cvor1_ind] = tezina;
	}

	void obrisi_granu(string cvor1, string cvor2) {
		dodaj_granu(cvor1, cvor2, 0);
	}

	void ispisi_reprezentaciju() {
		cout << setw(3) << " ";
		for (int i = 0; i < n; i++) {
			cout << setw(3) << (cvorovi[i] == "" ? "/" : cvorovi[i]);
		}
		cout << endl;
		for (int i = 0; i < n; i++) {
			cout << setw(3) << (cvorovi[i] == "" ? "/" : cvorovi[i]);
			for (int j = 0; j < n; j++) {
				cout << setw(3) << grane[i][j];
			}
			cout << endl;
		}
	}
};

///////////////////////////////////////////////////////////////////////////////////////////
/// Funkcije korisnickog interfejsa														///
///////////////////////////////////////////////////////////////////////////////////////////

Graf* kreiraj_graf(){
	cout << "Kreiranje grafa... Unesite velicinu grafa: ";

	int dim;
	cin >> dim;

	if(dim<=0){
		cout << "Nepravilan unos!";
		exit(1);
	}

	Graf* graf = new Graf{dim};
	cout << "Graf uspesno kreiran!" << endl;
	return graf;
}

void dodaj_cvor(Graf* graf){
	cout << "Dodavanje cvora... Unesite naziv cvora: ";
	if(graf==nullptr){
		cout << "Graf ne postoji!";
		exit(1);
	}

	string cvor;
	cin >> cvor;

	if(cvor==""){
		cout << "Nepravilan unos!";
		exit(1);
	}

	graf->dodaj_cvor(cvor);
	cout << "Cvor: " << cvor << " uspesno dodat!" << endl;
}

void obrisi_cvor(Graf* graf){
	cout << "Brisanje cvora... Unesite naziv cvora: ";
	if(graf==nullptr){
		cout << "Graf ne postoji!";
		exit(1);
	}

	string cvor;
	cin >> cvor;

	if(graf->dohvati_indeks_cvora(cvor)==-1){
		cout << "Nepravilan unos!";
		exit(1);
	}

	graf->obrisi_cvor(cvor);
	cout << "Cvor: " << cvor << " uspesno obrisan!" << endl;
}

void dodaj_granu(Graf* graf){
	cout << "Dodavanje grane... Unesite prvi cvor: ";
	if(graf==nullptr){
		cout << "Graf ne postoji!";
		exit(1);
	}

	string cvor1, cvor2;
	cin >> cvor1;
	cout << "Unesite drugi cvor: ";
	cin >> cvor2;
	int tezina;
	cout << "Unesite tezinu: ";
	cin >> tezina;

	if(graf->dohvati_indeks_cvora(cvor1)==-1 || graf->dohvati_indeks_cvora(cvor2)==-1 || tezina < 0){
		cout << "Nepravilan unos!";
		exit(1);
	}

	graf->dodaj_granu(cvor1, cvor2, tezina);
	cout << "Cvorovi: " << cvor1 << " i " << cvor2 << " uspesno povezani! (" << tezina << ")" << endl;
}

void obrisi_granu(Graf* graf){
	cout << "Brisanje grane... Unesite prvi cvor: ";
	if(graf==nullptr){
		cout << "Graf ne postoji!";
		exit(1);
	}

	string cvor1, cvor2;
	cin >> cvor1;
	cout << "Unesite drugi cvor: ";
	cin >> cvor2;

	if(graf->dohvati_indeks_cvora(cvor1)==-1 || graf->dohvati_indeks_cvora(cvor2)==-1){
		cout << "Nepravilan unos!";
		exit(1);
	}

	graf->obrisi_granu(cvor1, cvor2);
	cout << "Cvorovi: " << cvor1 << " i " << cvor2 << " uspesno odvezani!" << endl;
}

void ispisi_reprezentaciju(Graf* graf){
	cout << "Ispisivanje reprezentacije grafa..." << endl;
	if(graf==nullptr){
		cout << "Graf ne postoji!";
		exit(1);
	}

	graf->ispisi_reprezentaciju();
}

void obrisi_graf(Graf* graf){
	cout << "Brisanje grafa..." << endl;
	if(graf==nullptr){
		cout << "Graf ne postoji!";
		exit(1);
	}

	delete graf;
	graf = nullptr;
}

///////////////////////////////////////////////////////////////////////////////////////////
/// Glavni program																		///
///////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	Graf* graf;
	bool exit_flag = false;

	while(!exit_flag){
		cout << "--------------------------------------------" << endl;
		cout << "Izaberite opciju:" << endl;
		cout << "\t1. Kreiraj graf" << endl;
		cout << "\t2. Dodaj cvor u graf" << endl;
		cout << "\t3. Obrisi cvor iz grafa" << endl;
		cout << "\t4. Dodaj granu izmedju dva cvora" << endl;
		cout << "\t5. Obrisi granu izmedju dva cvora" << endl;
		cout << "\t6. Ispisi reprezentaciju grafa" << endl;
		cout << "\t7. Obrisi graf" << endl;
		cout << "\t8. Izadji iz programa (EXIT)" << endl;
		cout << "Unesite zeljeni broj i pritisnite ENTER... ";
		
		int opcija;
		cin >> opcija;

		if(!(opcija>=1 && opcija<=8)){
			cout << "Nepravilan unos." << endl;
			exit(1);
		}

		if(opcija==1){
			graf = kreiraj_graf();
		}
		if(opcija==2){
			dodaj_cvor(graf);
		}
		if(opcija==3){
			obrisi_cvor(graf);
		}
		if(opcija==4){
			dodaj_granu(graf);
		}
		if(opcija==5){
			obrisi_granu(graf);
		}
		if(opcija==6){
			ispisi_reprezentaciju(graf);
		}
		if(opcija==7){
			obrisi_graf(graf);
		}
		if(opcija==8){
			exit_flag = true;
		}
	}
	
	return 0;
}