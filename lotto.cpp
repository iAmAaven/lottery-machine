// Aapo Harjunpää - Lottokone

// Lisätään kirjastot
#define NOMINMAX
#include <iostream>
#include <locale>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <thread>
#include <limits>

using namespace std; // Mahdollistaa std-komentojen käytön ilman std::-etuliitettä
using namespace this_thread;	// Mahdollistavat ajastuskomennot,
using namespace chrono;			// kuten sleep_for() -funktion


// LUODAAN GLOBAALIT MUUTTUJAT

// Taulukko numeroista 0-9, jonka avulla hoidetaan niiden graafinen esitys
int possibleNumbers[10][30] = {

	//	Ohjelma tarkistaa tästä taulukosta ensin lottokoneen jokaisen arpoman numeron
	//	ja tulostaa jokaisen arvotun numeron ensimmäisen rivin,
	//	jonka jälkeen se siirtyy seuraavalle riville, kunnes numeroiden viimeisetkin rivit ovat tulostettu

	//	1. RIVI		2. RIVI		3. RIVI		4. RIVI		5. RIVI		6. RIVI
		{2, 1, 3,	1, 0, 1,	1, 0, 1,	1, 0, 1,	1, 0, 1,	2, 1, 3}, // Muodostaa numeron 0
		{0, 1, 0,	2, 1, 0,	0, 1, 0,	0, 1, 0,	0, 1, 0,	1, 1, 1}, // Muodostaa numeron 1
		{2, 1, 3,	1, 0, 1,	0, 2, 1,	2, 1, 0,	1, 3, 0,	1, 1, 1}, // Muodostaa numeron 2
		{1, 1, 3,	0, 0, 1,	0, 1, 3,	0, 0, 1,	1, 0, 1,	2, 1, 3}, // Muodostaa numeron 3
		{0, 2, 1,	0, 1, 1,	2, 3, 1,	1, 1, 1,	0, 0, 1,	0, 0, 1}, // Muodostaa numeron 4
		{1, 1, 1,	1, 0, 0,	1, 1, 3,	0, 0, 1,	1, 0, 1,	2, 1, 3}, // Muodostaa numeron 5
		{2, 1, 3,	1, 0, 1,	1, 0, 0,	1, 1, 3,	1, 0, 1,	2, 1, 3}, // Muodostaa numeron 6
		{1, 1, 1,	0, 0, 1,	0, 2, 3,	0, 1, 0,	0, 1, 0,	0, 1, 0}, // Muodostaa numeron 7
		{2, 1, 3,	1, 0, 1,	2, 1, 3,	1, 0, 1,	1, 0, 1,	2, 1, 3}, // Muodostaa numeron 8
		{2, 1, 3,	1, 0, 1,	2, 1, 1,	0, 0, 1,	1, 0, 1,	2, 1, 3}, // Muodostaa numeron 9

		//	0 tarkoittaa sitä, että ohjelma tulostaa kaksi välilyöntiä					( cout << "  "; )
		//	1 tarkoittaa sitä, että ohjelma tulostaa kaksi nollaa						( cout << "00"; )
		//	2 tarkoittaa sitä, että ohjelma tulostaa yhden välilyönnin ja yhden nollan	( cout << " 0"; )
		//	3 tarkoittaa sitä, että ohjelma tulostaa yhden nollan ja yhden välilyönnin	( cout << "0 "; )
		//	Tämä toiminto on tehty funktiossa PrintSymbols
};
vector<int> paaveikkaus, lisaVeikkaus; // Luodaan kokonaislukuvektorit käyttäjän antamille pää- ja lisänumeroiden veikkauksille
int const PAANUMEROIDEN_MAARA = 7; // Luodaan muuttumaton kokonaisluku, joka määrittää arvottavien päänumeroiden määrän
int paanumerot[PAANUMEROIDEN_MAARA], lisanumerot[3]; // Luodaan taulukot arvotuille pää- ja lisänumeroille
int viikko = 1; // Luodaan kokonaislukumuuttuja, joka pitää kirjaa arvontakerroista
bool osallistunut = false; // Luodaan boolean, joka tarkistaa onko käyttäjä osallistunut viikon lottoarvontaan


// DEKLAROIDAAN FUNKTIOT

void PrintLotto();
void Menu();
void ArvoNumerot();
void BubbleSort(int numerolista[], int maara);
void BubbleSortVector(vector<int>& numerovektori, int maara);
void Arpakauppa();
void PrintNumbers(int numerolista[], int numerolista2[]);
void PrintPaanumerot();
void PrintLisanumerot();
void PrintTulokset();
void PrintSymbols(int index1, int index2);
void PrintLine(int amount);
void PrintEmptyRowWithDividers(int amount);
void PrintDividersAndLines(int amount);
void PrintSumLine(int amount);
void PrintSpaces(int amount);
void FlashNumbers();
void Color(int color);
void GoToXY(int x, int y);
void PrintYlarivi();
void NaytaKursori();
void PiilotaKursori();

int main() // Ohjelman pääfunktio, josta suoritus alkaa
{
	setlocale(LC_ALL, "fi_FI"); // Mahdollistaa ääkköset
	srand(time(NULL));

	PrintLotto(); // Tulostaa alkunäytölle LOTTO -"logon"

	// Tulostetaan ohjeet käyttäjälle, jotta ohjelma toimii tarkoituksen mukaisesti
	cout << "Aseta ikkuna koko näytölle ja varmista, että alapuolella oleva viiva näkyy kokonaan yhdellä rivillä, kiitos!" << endl << endl << endl;
	for (int i = 0; i < 180; i++)
		cout << "_";

	cout << endl << endl << "(Navigoi valikkoa nuolinäppäimillä)" << endl << endl;

	// Odotetaan syötettä, jotta käyttäjällä on aikaa lukea ohjeet
	system("pause");

	// Tulostetaan "logo" uudelleen, joka myös alustaa terminaalin, sekä vaihdetaan näkymä päävalikkoon
	PrintLotto();
	Menu();

	// Kun päävalikon silmukasta poistutaan, tulostetaan "jäähyväisviesti"
	cout << "    Goodbye!" << endl << endl << endl;

	return EXIT_SUCCESS; // Palauttaa arvon osoittaen onnistuneen suorituksen
}

// Tulostaa LOTTO -"logon"
void PrintLotto()
{
	system("cls"); // Alustaa terminaalin

	// Tulostetaan logo, sekä jokaisen logon rivin eteen 70 välilyöntiä
	cout << endl << endl << endl;
	PrintSpaces(70); cout << "000      0000   000000  000000   0000 " << endl;
	PrintSpaces(70); cout << "000     000000  000000  000000  000000" << endl;
	PrintSpaces(70); cout << "000     00  00    00      00    00  00" << endl;
	PrintSpaces(70); cout << "000     00  00    00      00    00  00" << endl;
	PrintSpaces(70); cout << "000000  000000    00      00    000000" << endl;
	PrintSpaces(70); cout << "000000   0000     00      00     0000 " << endl;
	cout << endl << endl << endl;
}

// Päävalikon funktio
void Menu()
{
	int Set[] = { 3, 7, 7 }; // Tallennetaan päävalikon valintojen värikoodit ( 3 = sininen, 7 = valkoinen )
	int valinta = 1; // Luodaan kokonaislukumuuttuja, joka pitää kirjaa siitä, mikä päävalikon valinta on tällä hetkellä
	char key; // Luodaan kirjainmerkkimuuttuja, joka vastaa käyttäjän syöttämän painikkeen arvoa

	PrintYlarivi(); // Tulostetaan käyttäjän lottorivi vasempaan ylänurkkaan

	// Aloitetaan silmukka, joka loppuu vasta, kun haluamme poistua siitä
	// Tämä tekee valikossa navigoimisesta helpompaa
	for (int i = 0;;)
	{
		// Tulostetaan ensimmäinen valinta päävalikkoon sijaintiin (70, 12) ja tehdään tekstistä sininen
		GoToXY(70, 12);
		Color(Set[0]);
		cout << "Katso viikon lottoarvonta";

		// Tulostetaan toinen valinta päävalikkoon sijaintiin (70, 13) ja tehdään tekstistä valkoinen
		GoToXY(70, 13);
		Color(Set[1]);
		cout << "Osallistu arvontaan";

		// Tulostetaan kolmas valinta päävalikkoon sijaintiin (70, 14) ja tehdään tekstistä valkoinen
		GoToXY(70, 14);
		Color(Set[2]);
		cout << "Poistu";

		PiilotaKursori(); // Piilotetaan kursori, jotta se ei häiritse käyttäjää valikossa
		key = _getch(); // Lukee yhden käyttäjän syöttämän kirjainmerkin ja tallentaa sen "key" -muuttujalle

		cout << endl << endl << endl;


		if (key == 72 && (valinta >= 2 && valinta <= 3)) // Jos käyttäjä painaa ylöspäin-nuolta ja valinta on 2 tai 3...
			valinta--; // ...valitaan valikosta alempi valinta
		else if (key == 80 && (valinta >= 1 && valinta <= 2)) // Jos käyttäjä painaa alaspäin-nuolta ja valinta on 1 tai 2...
			valinta++; // ...valitaan valikosta ylempi valinta
		else if (key == '\r') // Jos käyttäjä painaa Enter-painiketta...
		{
			switch (valinta)
			{
			case 1: // ...ja jos valinta on 1...
				ArvoNumerot(); // ...aloitetaan lottokone
				break;
			case 2: // ...ja jos valinta on 2...
				Arpakauppa(); // ...aloitetaan veikkausrivin tekeminen
				break;
			case 3: // ...ja jos valinta on 3...
				Color(7); // ...muutetaan tekstin väri takaisin valkoiseksi...
				system("cls"); //...ja tyhjennetään terminaali, jonka jälkeen ohjelma päättyy
				break;
			}
			break; // ...poistutaan valikko-silmukasta
		}

		// Päivitetään jokaisen käyttäjän antaman kirjainmerkin jälkeen valikon valintojen värit takaisin valkoisiksi
		for (int i = 0; i < 3; i++)
			Set[i] = 7;

		// Päivitetään tämän hetkisen valinnan värin siniseksi
		switch (valinta)
		{
		case 1:
			Set[0] = 3;
			break;
		case 2:
			Set[1] = 3;
			break;
		case 3:
			Set[2] = 3;
			break;
		}
	}
	cout << endl << endl << endl;
}

// Tulostaa käyttäjän veikkaukset vasempaan yläkulmaan ja tämän hetkisen viikon oikeaan yläkulmaan
void PrintYlarivi()
{
	// Tulostetaan käyttäjän lottorivi sijaintiin (20, 8)
	GoToXY(20, 8);
	cout << "Lottorivisi: ";

	if (osallistunut) // Jos käyttäjä on jo antanut veikkauksensa tämän viikon arvontaan...
	{
		// ...tulostetaan pelaajan päänumeroveikkaukset
		for (int i = 0; i < 7; i++)
			cout << paaveikkaus[i] << " ";
		cout << "   ";
		// ...tulostetaan pelaajan lisänumeroveikkaukset
		for (int i = 0; i < 3; i++)
			cout << lisaVeikkaus[i] << " ";
	}
	else // Jos käyttäjä ei ole osallistunut tämän viikon arvontaan...
		cout << "__ __ __ __ __ __ __   __ __ __"; // ...tulostetaan paikkamerkkirivi veikkauksen luvuille


	// Tulostetaan tämänhetkinen viikko sijaintiin (108, 8)
	GoToXY(108, 8);
	cout << "      __  __  __  __  __  __  __  __  Viikko " << viikko;
}

// Funktio voittorivin arpomiseen
void ArvoNumerot()
{
	int numero; // Luodaan kokonaislukumuuttuja arvotulle numerolle

	// Arvotaan päänumerot 1:n ja 39:n väliltä päänumeroiden määrän perusteella
	for (int i = 0; i < PAANUMEROIDEN_MAARA; i++)
	{
		numero = 1 + rand() % 39;

		// Jos arvottu numero löytyy jo aiemmin arvotuista numeroista, arvotaan numero uudelleen,
		// kunnes numerolle ei löydy vastaavaa jo arvotuista numeroista
		while (find(begin(paanumerot), end(paanumerot), numero) != end(paanumerot))
			numero = 1 + rand() % 39;

		paanumerot[i] = numero; // Lisätään arvottu numero lottoriviin
	}

	BubbleSort(paanumerot, PAANUMEROIDEN_MAARA); // Järjestetään päänumerot suuruusjärjestykseen bubble sortilla

	// Arvotaan 3 lisänumeroa
	for (int i = 0; i < 3; i++)
	{
		numero = 1 + rand() % 39;

		// Jos arvottu numero löytyy jo aiemmin arvotuista pää- tai lisänumeroista, arvotaan numero uudelleen,
		// kunnes numerolle ei löydy vastaavaa jo arvotuista numeroista
		while (find(begin(paanumerot), end(paanumerot), numero) != end(paanumerot)
			|| find(begin(lisanumerot), end(lisanumerot), numero) != end(lisanumerot))
			numero = 1 + rand() % 39;

		lisanumerot[i] = numero; // Lisätään arvottu numero lottoriviin
	}

	BubbleSort(lisanumerot, 3); // Järjestetään lisänumerot suuruusjärjestykseen bubble sortilla
	PrintNumbers(paanumerot, lisanumerot); // Tulostetaan arvotut numerot

	cout << endl << endl << endl;
}

// Funktio, joka järjestää annetun taulukon numerot suuruusjärjestykseen
void BubbleSort(int numerolista[], int maara)
{
	int talteen;
	for (int i = 0; i < maara - 1; i++)
		for (int j = 0; j < maara - 1; j++)
			if (numerolista[j] > numerolista[j + 1])
			{
				talteen = numerolista[j];
				numerolista[j] = numerolista[j + 1];
				numerolista[j + 1] = talteen;
			}
}

// Funktio, joka järjestää annetun vektorin numerot suuruusjärjestykseen
void BubbleSortVector(vector<int>& numerovektori, int maara)
{
	int talteen;
	for (int i = 0; i < maara - 1; i++)
		for (int j = 0; j < maara - 1; j++)
			if (numerovektori[j] > numerovektori[j + 1])
			{
				talteen = numerovektori[j];
				numerovektori[j] = numerovektori[j + 1];
				numerovektori[j + 1] = talteen;
			}
}

// Funktio käyttäjän veikkausrivin vastaanottamista varten
void Arpakauppa()
{
	int numero, talteen, rivivaihtojenMaara = 0; // Luodaan tarvittavat kokonaislukumuuttujat

	// Alustetaan mahdolliset aiemmat veikkaukset ja vaihdetaan "osallistunut" todeksi
	paaveikkaus.clear();
	lisaVeikkaus.clear();
	osallistunut = true;

	PrintLotto(); // Tulostetaan LOTTO -logo

	NaytaKursori(); // Näytetään kursori, jotta käyttäjä voi syöttää veikkauksensa

	// Pyydetään käyttäjältä päänumeroveikkaukset ja aloitetaan tulostaminen sijainnista (70, 12)
	GoToXY(70, 12);
	cout << "SYÖTÄ 7 PÄÄNUMEROA (1-39)" << endl;

	for (int laskuri = 1; laskuri <= PAANUMEROIDEN_MAARA; laskuri++)
	{
		// Vastaanotetaan käyttäjän syöte sijainnista (70, 13 + rivivaihtojenMaara),
		// jossa "rivivaihtojenMaara" pitää kirjaa siitä, kuinka monta kertaa rivi vaihtuu
		GoToXY(70, 13 + rivivaihtojenMaara);
		cout << laskuri << ". numero: ";
		cin >> numero;

		// Jos numero ei ole 1:n ja 39:n väliltä,
		// TAI jos käyttäjän antama numero löytyy jo aiemmin annetuista päänumeroveikkauksista,
		// pyydetään numeroa uudelleen, kunnes numerolle ei löydy vastaavaa jo annetuista numeroista
		while (cin.fail() || (numero < 1 || numero > 39) || find(begin(paaveikkaus), end(paaveikkaus), numero) != end(paaveikkaus))
		{
			cin.clear(); // Tyhjennä virhetila
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Tyhjennä puskurin sisältö

			rivivaihtojenMaara++;
			GoToXY(70, 13 + rivivaihtojenMaara); // Vaihtaa riviä seuraavalle, pitäen x-akselin sijainnin 70:ssä
			cout << "Antamasi numero oli virheellinen." << endl;
			rivivaihtojenMaara++;
			GoToXY(70, 13 + rivivaihtojenMaara);
			cout << "Syötä " << laskuri << ". numero uudelleen: ";
			cin >> numero;
		}
		rivivaihtojenMaara++;
		paaveikkaus.push_back(numero); // Lisätään käyttäjän syöttämä numero päänumeroveikkaus-vektoriin
	}

	BubbleSortVector(paaveikkaus, PAANUMEROIDEN_MAARA); // Järjestetään päänumeroveikkaus-vektorin numerot suuruusjärjestykseen bubble sortilla

	cout << endl << endl << endl;
	rivivaihtojenMaara += 3;

	// Pyydetään käyttäjältä lisänumeroveikkaukset ja aloitetaan tulostaminen sijainnista (70, 13 + rivivaihtojenMaara)
	GoToXY(70, 13 + rivivaihtojenMaara);
	cout << "SYÖTÄ 3 LISÄNUMEROA (1-39)" << endl;
	rivivaihtojenMaara++;

	for (int laskuri = 1; laskuri <= 3; laskuri++)
	{
		// Vastaanotetaan käyttäjän syöte sijainnista (70, 13 + rivivaihtojenMaara)
		GoToXY(70, 13 + rivivaihtojenMaara);
		cout << laskuri << ". numero: ";
		cin >> numero;

		// Jos numero ei ole 1:n ja 39:n väliltä,
		// TAI jos käyttäjän antama numero löytyy jo aiemmin annetuista pää- TAI lisänumeroveikkauksista,
		// pyydetään numeroa uudelleen, kunnes numerolle ei löydy vastaavaa jo annetuista numeroista
		while (cin.fail() || (numero < 1 || numero > 39)
			|| (find(begin(paaveikkaus), end(paaveikkaus), numero) != end(paaveikkaus)
				|| find(begin(lisaVeikkaus), end(lisaVeikkaus), numero) != end(lisaVeikkaus)))
		{
			cin.clear(); // Tyhjennä virhetila
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Tyhjennä puskurin sisältö

			rivivaihtojenMaara++;
			GoToXY(70, 13 + rivivaihtojenMaara);
			cout << "Antamasi numero oli virheellinen." << endl;
			rivivaihtojenMaara++;
			GoToXY(70, 13 + rivivaihtojenMaara);
			cout << "Syötä " << laskuri << ". numero uudelleen: ";
			cin >> numero;
		}
		rivivaihtojenMaara++;
		lisaVeikkaus.push_back(numero); // Lisätään käyttäjän syöttämä numero päänumeroveikkaus-vektoriin
	}

	BubbleSortVector(lisaVeikkaus, 3); // Järjestetään lisänumeroveikkaus-vektorin numerot suuruusjärjestykseen bubble sortilla

	cout << endl << endl << endl;
	rivivaihtojenMaara += 3;


	// Tulostetaan käyttäjän tekemä lottorivi

	GoToXY(70, 13 + rivivaihtojenMaara);
	cout << "Tässä sinun arpasi. Onnea peliin!" << endl;
	rivivaihtojenMaara++;
	GoToXY(70, 13 + rivivaihtojenMaara);

	for (int i = 0; i < 7; i++)
		cout << paaveikkaus[i] << " ";
	cout << "   ";
	for (int i = 0; i < 3; i++)
		cout << lisaVeikkaus[i] << " ";


	cout << endl << endl << endl;

	// Pysäytetään ohjelma odottamaan käyttäjän syötettä, jonka jälkeen ohjelma palaa päävalikkoon
	system("pause");
	PrintLotto();
	Menu();
}

// Funktio lottonumeroiden graafisen tulostuksen käsittelyä varten
void PrintNumbers(int numerolista[], int numerolista2[])
{
	FlashNumbers(); // Välkyttää voittorivin tyhjiä ruutuja
	PrintPaanumerot(); // Tulostaa lottorivin päänumerot ruutuihin
	PrintLisanumerot(); // Tulostaa lottorivin lisänumerot ruutuihin
	PrintTulokset(); // Tulostaa lopuksi voittorivin sekä pelaajan rivin, ja katsoo kuinka monta numeroa käyttäjä veikkasi oikein

	// Pysäytetään ohjelma odottamaan käyttäjän syötettä, jonka jälkeen ohjelma palaa päävalikkoon
	system("pause");
	PrintLotto();
	Menu();
}

// Funktio päänumeroiden graafista tulostusta varten
void PrintPaanumerot()
{
	// Aloitetaan tulostaminen sijainnista (84, 11), joka on keskellä verrattaessa "PÄÄNUMEROT" tekstiä LOTTO-logoon
	GoToXY(84, 11);
	cout << "PÄÄNUMEROT";

	// Tulostetaan graafisten ruutujen kaksi ensimmäistä riviä sijainnista (0, 12)
	GoToXY(0, 12);
	PrintLine(PAANUMEROIDEN_MAARA);
	PrintEmptyRowWithDividers(PAANUMEROIDEN_MAARA);

	// Luodaan kokonaislukumuuttuja, joka pitää kirjaa "possibleNumbers"-taulukon rivien indeksistä,
	// joka varmistaa lukujen graafisen tulostuksen
	int symbolit = 0;

	// Aloitetaan silmukka, joka käy jokaisen "possibleNumbers" -taulukon rivin kerrallaan
	for (int rivi = 0; rivi < 6; rivi++)
	{
		cout << "    ||"; // Luodaan jokaisen rivin eteen ensimmäisen graafisen ruudun vasemman "seinän" pala

		// Käydään läpi jokaisen päänumeron arvot
		for (int lottonumero = 0; lottonumero < PAANUMEROIDEN_MAARA; lottonumero++)
		{
			// Jos tämän hetkisen päänumeron arvo on yksinumeroinen, eli alle 10...
			if (paanumerot[lottonumero] < 10)
			{
				cout << "        "; // ...tulostetaan graafisen ruudun sisäpuolelle 8 välilyöntiä ennen päänumeroa

				// ...tulostetaan päänumeron tämänhetkisellä rivillä olevat merkit
				for (int i = symbolit; i < symbolit + 3; i++)
					PrintSymbols(paanumerot[lottonumero], i);

				cout << "        ||"; // ...tulostetaan graafisen ruudun sisäpuolelle 8 välilyöntiä ja oikea seinä päänumeron jälkeen
			}
			else // Jos tämän hetkisen päänumeron arvo on kaksinumeroinen, eli yli 9...
			{

				cout << "    "; // ...tulostetaan graafisen ruudun sisäpuolelle 4 välilyöntiä ennen päänumeroa


				// ...halkaistaan luvun kaksi numeroa ja annetaan ne kahden kokonaislukumuuttujan arvoiksi
				// Esim. jos luku on '23', "split1":n arvo olisi '2', ja "split2":n arvo olisi '3'
				int split1 = paanumerot[lottonumero] / 10 % 10; // Tallentaa luvun ensimmäisen numeron
				int split2 = paanumerot[lottonumero] % 10; // Tallentaa luvun toisen numeron

				// ...tulostetaan luvun ensimmäisen numeron tämänhetkisellä rivillä olevat merkit
				for (int i = symbolit; i < symbolit + 3; i++)
					PrintSymbols(split1, i);

				cout << "  ";

				// ...tulostetaan luvun toisen numeron tämänhetkisellä rivillä olevat merkit
				for (int j = symbolit; j < symbolit + 3; j++)
					PrintSymbols(split2, j);

				cout << "    ||"; // ...tulostetaan graafisen ruudun sisäpuolelle 4 välilyöntiä ja oikea seinä päänumeron jälkeen
			}
		}

		// Koska yhdessä "possibleNumbers" numeron rivissä on 3 arvoa, lisätään "symbolit" muuttujaan 3
		symbolit += 3;
		cout << endl;
	}

	// Tulostetaan graafisten ruutujen alemmat rivit
	PrintDividersAndLines(PAANUMEROIDEN_MAARA);
	PrintSumLine(PAANUMEROIDEN_MAARA);

	cout << endl << endl;
}

// Funktio lisänumeroiden graafista tulostusta varten
void PrintLisanumerot()
{
	// Aloitetaan tulostaminen sijainnista (82, 24), joka on keskellä verrattaessa "LISÄNUMEROT..." tekstiä LOTTO-logoon
	GoToXY(82, 24);
	cout << "LISÄNUMEROT";

	// Tulostetaan "LISÄNUMEROT" tekstin perään '.' -merkki kahden sekunnin välein
	for (int i = 0; i < 3; i++)
	{
		sleep_for(seconds(2));
		cout << ".";
	}

	// Odotetaan vielä kaksi sekuntia pisteiden tulostuksen jälkeen
	sleep_for(seconds(2));

	cout << endl << endl;

	// Tulostetaan lisänumeroiden graafisten ruutujen kaksi ensimmäistä riviä
	PrintSpaces(48); PrintLine(3);
	PrintSpaces(48); PrintEmptyRowWithDividers(3);

	// Luodaan kokonaislukumuuttuja, joka pitää kirjaa "possibleNumbers"-taulukon rivien indeksistä,
	// joka varmistaa lukujen graafisen tulostuksen
	int symbolit = 0;

	// Aloitetaan silmukka, joka käy jokaisen "possibleNumbers" -taulukon rivin kerrallaan
	for (int rivi = 0; rivi < 6; rivi++)
	{
		// Luodaan jokaisen rivin eteen ensimmäisen graafisen ruudun vasemman "seinän" pala ja asetetaan se keskelle LOTTO-logoon verrattaessa
		PrintSpaces(52); cout << "||";

		// Käydään läpi jokaisen lisänumeron arvot
		for (int lottonumero = 0; lottonumero < 3; lottonumero++)
		{
			// Jos tämän hetkisen päänumeron arvo on yksinumeroinen, eli alle 10...
			if (lisanumerot[lottonumero] < 10)
			{
				cout << "        "; // ...Tulostetaan graafisen ruudun sisäpuolelle 8 välilyöntiä ennen päänumeroa

				// ...tulostetaan lisänumeron tämänhetkisellä rivillä olevat merkit
				for (int i = symbolit; i < symbolit + 3; i++)
					PrintSymbols(lisanumerot[lottonumero], i);

				cout << "        ||"; // ...tulostetaan graafisen ruudun sisäpuolelle 8 välilyöntiä ja oikea seinä lisänumeron jälkeen
			}
			else // Jos tämän hetkisen päänumeron arvo on kaksinumeroinen, eli yli 9...
			{
				cout << "    "; // ...tulostetaan graafisen ruudun sisäpuolelle 4 välilyöntiä ennen päänumeroa

				// ...halkaistaan luvun kaksi numeroa ja annetaan ne kahden kokonaislukumuuttujan arvoiksi
				// Esim. jos luku on '23', "split1":n arvo olisi '2', ja "split2":n arvo olisi '3'
				int split1 = lisanumerot[lottonumero] / 10 % 10; // Tallentaa luvun ensimmäisen numeron
				int split2 = lisanumerot[lottonumero] % 10; // Tallentaa luvun toisen numeron

				// ...tulostetaan luvun ensimmäisen numeron tämänhetkisellä rivillä olevat merkit
				for (int i = symbolit; i < symbolit + 3; i++)
					PrintSymbols(split1, i);

				cout << "  ";

				// ...tulostetaan luvun toisen numeron tämänhetkisellä rivillä olevat merkit
				for (int j = symbolit; j < symbolit + 3; j++)
					PrintSymbols(split2, j);

				cout << "    ||"; // ...tulostetaan graafisen ruudun sisäpuolelle 4 välilyöntiä ja oikea seinä lisänumeron jälkeen
			}
		}

		// Koska yhdessä "possibleNumbers" numeron rivissä on 3 arvoa, lisätään "symbolit" muuttujaan 3
		symbolit += 3;
		cout << endl;
	}

	// Tulostetaan graafisten ruutujen viimeiset rivit
	PrintSpaces(48); PrintDividersAndLines(3);
	PrintSpaces(48); PrintSumLine(3);

	cout << endl << endl;
}

// Funktio lottoarvonnan tulosten tulostamiseen
void PrintTulokset()
{
	// Aloitetaan tulostus sijainnista (70, 38)
	GoToXY(70, 38);

	// Tulostetaan 3 '.' -merkkiä sekunnin välein
	for (int i = 0; i < 3; i++)
	{
		sleep_for(seconds(1));
		cout << ".";
	}
	sleep_for(seconds(1));

	int oikeatPaanumerot = 0, oikeatLisanumerot = 0; // Luodaan kokonaislukumuuttujat, jotka pitävät kirjaa oikein arvatuista numeroista

	// Aloitetaan voittorivin tulostus sijainnista (70, 39)
	GoToXY(70, 39);
	cout << "VOITTORIVI: ";

	// Tulostaa voittorivin päänumerot
	for (int i = 0; i < PAANUMEROIDEN_MAARA; i++)
		cout << paanumerot[i] << " ";

	cout << "   ";

	// Tulostaa voittorivin lisänumerot
	for (int j = 0; j < 3; j++)
		cout << lisanumerot[j] << " ";

	if (osallistunut == true) // Jos käyttäjä osallistui arvontaan, eli antoi omat veikkauksensa...
	{
		// ...aloitetaan käyttäjän rivin tulostus sijainnista (70, 40)
		GoToXY(70, 40);
		cout << "SINUN RIVI: ";

		// ...tulostetaan käyttäjän veikkaamat päänumerot
		for (int i = 0; i < PAANUMEROIDEN_MAARA; i++)
		{
			// Alustetaan numeron väri valkoiseksi
			Color(7);

			if (find(begin(paanumerot), end(paanumerot), paaveikkaus[i]) != end(paanumerot)) // Jos pelaaja sai jonkin päänumeron oikein...
			{
				// ...lisätään "oikeatPaanumerot" muuttujaan 1, ja vaihdetaan numeron väri vihreäksi
				oikeatPaanumerot++;
				Color(2);
			}

			cout << paaveikkaus[i] << " ";
		}
		cout << "   ";

		// ...tulostetaan käyttäjän veikkaamat lisänumerot
		for (int i = 0; i < 3; i++)
		{
			// Alustetaan numeron väri valkoiseksi
			Color(7);

			if (find(begin(lisanumerot), end(lisanumerot), lisaVeikkaus[i]) != end(lisanumerot)) // Jos pelaaja sai jonkin lisänumeron oikein...
			{
				// ...lisätään "oikeatLisanumerot" muuttujaan 1, ja vaihdetaan numeron väri vihreäksi
				Color(2);
				oikeatLisanumerot++;
			}

			cout << lisaVeikkaus[i] << " ";
		}

		// ...aloitetaan lopullisten tulosten tulostus sijainnista (70, 42) ja vaihdetaan seuraavien tekstien väriksi valkoinen
		GoToXY(70, 42);
		Color(7);

		if (oikeatPaanumerot + oikeatLisanumerot > 0) // Jos oikein arvattujen numeroiden yhteenlaskettu summa on yli 0...
		{
			if (oikeatPaanumerot + oikeatLisanumerot >= 10) //...ja jos summa on 10 tai yli (vaikka sen ei pitäisi olla mahdollista)...
			{
				cout << "SAIT KAIKKI OIKEIN! PÄÄVOITTO ON SINUN! ONNEKSI OLKOON!"; // ...tulostetaan onnitteluviesti päävoitosta
			}
			else //...ja jos summa on alle 10...
			{
				// ...tulostetaan tulokset ja käytettään hieman logiikkaa oikeanlaisen kieliopin nimissä

				cout << "Sait ";
				if (oikeatPaanumerot > 0)
				{
					cout << oikeatPaanumerot;
					if (oikeatPaanumerot == 1)
						cout << ":n päänumeron ";
					else
						cout << " päänumeroa ";
				}

				if (oikeatLisanumerot > 0)
				{
					if (oikeatPaanumerot > 0)
						cout << "ja ";

					cout << oikeatLisanumerot;

					if (oikeatLisanumerot == 1)
						cout << ":n lisänumeron ";
					else
						cout << " lisänumeroa ";
				}

				cout << "oikein!";
			}
		}
		else // Jos oikein arvattujen numeroiden yhteenlaskettu summa on 0 (tai alle)...
			cout << "Et saanut yhtään numeroa oikein..."; // ...tulostetaan pettynyt viesti
	}
	else // Jos käyttäjä ei osallistunut arvontaan...
	{
		// ...tulostetaan informatiivinen viesti sijainnille (70, 41)
		GoToXY(70, 41);
		cout << "Et osallistunut arvontaan.";
	}

	viikko++; // Lisätään viikkoon 1, kun tämän viikon lottokone on puhunut

	// Tulostetaan vielä viesti sijainnille (70, 43)
	GoToXY(70, 43);
	cout << "Ensi viikkoon!";

	if (osallistunut == true) // Jos käyttäjä osallistui arvontaan...
	{
		// ...tyhjennetään veikkauslistat ja muutetaan "osallistunut" epätodeksi
		paaveikkaus.clear();
		lisaVeikkaus.clear();
		osallistunut = false;
	}

	cout << endl << endl << endl;
}

// Funktio graafisten lukujen tulostamista varten
void PrintSymbols(int index1, int index2)
{
	switch (possibleNumbers[index1][index2]) // Jos "possibleNumbers" taulukon rivin "index1" sarakkeen "index2" arvo on...
	{
	case 0: // ...0, niin tulostetaan kaksi välilyöntiä
		cout << "  ";
		break;
	case 1: // ...1, niin tulostetaan kaksi nollaa
		cout << "00";
		break;
	case 2: // ...2, niin tulostetaan yksi välilyönti ja yksi nolla
		cout << " 0";
		break;
	case 3: // ...3, niin tulostetaan yksi nolla ja yksi välilyönti
		cout << "0 ";
		break;
	}
}

// Funktio, joka tulostaa viivan graafiselle ruudulle. "amount" tarkoittaa ruutujen haluttua määrää
void PrintLine(int amount)
{
	cout << "    __"; // Tulostetaan graafisen ruudun vasemman yläkulman reuna
	int symbolsToPrint = 0;
	for (int i = 0; i < amount; i++)
	{
		symbolsToPrint += 24; // Yhden ruudun leveys on 22, johon lisätään ruutujen välissä olevan jakajan leveys 2. Eli 22 + 2 = 24
	}
	for (int j = 0; j < symbolsToPrint; j++)
		cout << "_";

	cout << endl;
}

// Funktio, joka tulostaa tyhjän rivin jakajilla graafiselle ruudulle. "amount" tarkoittaa ruutujen haluttua määrää
void PrintEmptyRowWithDividers(int amount)
{
	cout << "    ||"; // Tulostetaan graafisen ruudun vasenta "seinää"
	for (int k = 0; k < amount; k++)
	{
		PrintSpaces(22);	// Tulostetaan välilyöntejä yhden ruudun leveyden verran...
		cout << "||";		// ...ja sen jälkeen ruutujen jakaja
	}
	cout << endl;
}

// Funktio, joka tulostaa rivin viivoja jakajilla graafiselle ruudulle. "amount" tarkoittaa ruutujen haluttua määrää
void PrintDividersAndLines(int amount)
{
	cout << "    ||"; // Tulostetaan graafisen ruudun vasenta "seinää"
	for (int k = 0; k < amount; k++)
	{
		cout << "______________________";	// Tulostetaan viivoja yhden ruudun leveyden verran...
		cout << "||";						// ...ja sen jälkeen ruutujen jakaja
	}
	cout << endl;
}

// Funktio, joka tulostaa rivin "yhtä suuri kuin"-merkkejä jakajilla graafiselle ruudulle. "amount" tarkoittaa ruutujen haluttua määrää
void PrintSumLine(int amount)
{
	cout << "    ||"; // Tulostetaan graafisen ruudun vasenta "seinää"
	for (int k = 0; k < amount; k++)
	{
		cout << "======================";	// Tulostetaan merkkejä yhden ruudun leveyden verran...
		cout << "||";						// ...ja sen jälkeen ruutujen jakaja
	}
	cout << endl;
}

// Funktio joka tulostaa annetun määrän välilyöntejä
void PrintSpaces(int amount)
{
	for (int i = 0; i < amount; i++)
		cout << " ";
}

// Funktio, joka piirtää ja väläyttää lottorivin tyhjää graafista ruudukkoa 3 kertaa
void FlashNumbers()
{
	for (int i = 0; i < 3; i++)
	{
		PrintLotto();
		PrintYlarivi();

		cout << endl << endl << endl << endl;

		sleep_for(seconds(1));
		PrintLine(PAANUMEROIDEN_MAARA);

		for (int j = 0; j < 7; j++)
			PrintEmptyRowWithDividers(PAANUMEROIDEN_MAARA);

		PrintDividersAndLines(PAANUMEROIDEN_MAARA);
		PrintSumLine(PAANUMEROIDEN_MAARA);
		sleep_for(seconds(1));
	}

	PrintLotto();
	PrintYlarivi();
	cout << endl << endl << endl;
	sleep_for(seconds(1));
}

// Funktio, joka vaihtaa tekstin värin
void Color(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Funktio, joka määrittää tulostettavan tekstin sijainnin
void GoToXY(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void PiilotaKursori() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	cursorInfo.bVisible = false; // Piilottaa kursorin
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

void NaytaKursori() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	cursorInfo.bVisible = true; // Näyttää kursorin
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}