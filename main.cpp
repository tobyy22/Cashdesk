//
//  main.cpp
//  Pokladna
//
//  Created by Tobias Vavroch on 21/10/2021.
//

#include <iostream>
#include <string>
#include <memory>
#include <algorithm>

#include <ctime>
#include <chrono>

#include "lib/decimal.h"
#include "Command.hpp"



using namespace std;

/*
 Knihovna Decimal.h pro vhodnou reprezentaci penez. <2> znamena, ze pouzivam cisla se 2 desetinnymi misty, coz je aplikovatelne pro
 naprostou vetsinu men.
 */


int main(int argc, const char * argv[]) {
    /*
     Pointer na abstraktni tridu Command, tato trida ma potomky jednotlive zakladni commandy,
     ktere se rozpoznaji podle prvniho naparsovaneho tokenu
     */
    unique_ptr<Command> command;
    
    /*
     Zde vytvarim objekt CashDesk, ke kteremu chci pristupovat pomoci pointeru z toho duvodu, ze k tomu objektu pristupuji
     napriklad rozhrani jednotlivych commandu, ktere po naparsovani volaji na tomto objektu spravne funkce.
     Jrdnolive commandy si vytvari na kasu vlastni pointer.
     */
    
    CashDesk kasa;
//
//    for (int i = 211; i < 500; i++) {
//        kasa.add_to_order("188", to_string(i), "4");
//    }

    /*
     Zde je algoritmus cele pokladny – nacte se input od uzivatele do promenne input, z inputu se vytvori spravny command,
     ktery nasledne virtualni funkce process_command zprocesuje
     
     Je dulezite zminit, ze ne nutne vsechen uzivateluv vstup projde pres tento cyklus – jednotlive commandy mohou vyzadovat
     treba jeste nejake upresneni od uzivatele – to si resi jednotlive commandy samy.
     
     */
    string input;
    cout << endl << "Command: ";

    while (getline(cin, input)) {
        command = Command::correct_command(input, kasa);
        command->process_command();
        
        cout << endl << "Command: ";

    }
}

