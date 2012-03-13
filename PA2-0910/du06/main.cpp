/* 
 * File:   main.cpp
 * Author: Enzy
 *
 * Created on 25. duben 2010, 15:53
 */

#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

#include "menu.cpp"

/*
 * 
 */
int main() {

    cout << "# START" << endl;

    // Test výpisu do streamu:
	const Menu &menu1 = Menu(
			new Polozka(1, "pol1",
			new Polozka(2, "pol2",
			new Submenu(3, "submenu1",
				new Polozka(4, "pol3",
				new Polozka(5, "pol4")),
			new Polozka(6, "pol5")))));
	cout << "Vypis menu:" << endl << menu1;
/* Má vypsat:
Vypis menu:
pol1
pol2
submenu1
   pol3
   pol4
pol5
*/

// Test hledání cesty:
	string cesta;
	menu1.najdi(1, cesta);  // vrátí true; cesta == "pol1"
	menu1.najdi(3, cesta);  // vrátí true; cesta == "submenu1"
	menu1.najdi(5, cesta);  // vrátí true; cesta == "submenu1/pol4"
	menu1.najdi(7, cesta);  // vrátí false; cesta je nedefinována

// Test vkládání:
	Menu menu2(new Polozka(1, "pol1"));
	menu2.vlozZa(1, new Submenu(2, "submenu1"));
	menu2.vlozZa(2, new Polozka(3, "pol2"));
	menu2.vlozZa(1, new Polozka(4, "pol3"));
	menu2.vlozDo(2, new Submenu(5, "submenu2"));
	menu2.vlozDo(5, new Polozka(6, "pol2"));  // O.K. duplicita jména nevadí
	menu2.vlozZa(1, new Polozka(4, "pol5"));  // neuspěje: id = 4 už existuje
	menu2.vlozDo(1, new Polozka(7, "pol6"));  // neuspěje: id = 1 není submenu
	cout << "Vypis menu:" << endl << menu2;
/* Má vypsat:
Vypis menu:
pol1
pol3
submenu1
   submenu2
      pol2
pol2
*/

// Test odebírání:
	menu2.odeber(1); // O.K. odebere "pol1"
	menu2.odeber(5); // O.K. odebere "submenu2" včetně vnořené "pol2"
	menu2.odeber(6); // neuspěje: dané id (již neexistuje)



    return (EXIT_SUCCESS);
}

