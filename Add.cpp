//
//  Add.cpp
//  Pokladna
//
//  Created by Tobias Vavroch on 20/12/2021.
//
#include "Command.hpp"

/*Procesovani commandu Add probiha tak, ze rozpozname druhy token a podle toho zavolame odpovidajici funkci.*/
void Add::process_command() {
    
    //prikaz musi mit dalsi tokeny
    if(tokenized.size() == 1) {
        return;
    }
    //druhy token je item(nebo jiny odpovidajici token - viz zkratky), chci nacitat itemy do databaze
    if(find(acros_item.begin(), acros_item.end(), tokenized[1]) != acros_item.end()) {
        add_items();
    }
    
    //order
    else if(find(acros_order.begin(), acros_order.end(), tokenized[1]) != acros_order.end()) {
        add_order();
    }
    
    //druhy token je "to", uzivatel chce pridat polozky do objednavky
    else if(tokenized[1] == "to") {
        add_to();
    }
    
    //druhy token je cash
    else if(find(acros_cash.begin(), acros_cash.end(), tokenized[1]) != acros_cash.end()) {
        add_cash();
    }
    
    //parametr pro Add jsem nepoznal
    else {
        cout << "Uknown parameter" << endl;
    }
}

//funkce na nacitani polozek do databaze kasy - predpokladam nacitani polozek ve tvaru jmeno a cena, tedy ocekava 2 tokeny
void Add::add_items() {
    
    //kontrola, ze 2 tokeny, tedy prikaz je "add item" nebo pomoci zkratek
    if (tokenized.size() != 2) {
        cout << "Error: Uknown parameters." << endl;
        return;
    }
    
    string input;
    cout << "Enter item: ";

    //nasledne probiha nacitani itemu
    while (getline(cin, input)) {
        
        //prazdny input ukonci nacitani itemu
        if(input.empty()) {
            return;
        }
        
        //jinak si naparsuji item
        vector<string> item_to_add = Command::tokenize_command(input);
        
        //pokud nejsou 2 tokeny tak chyba a nacitam dale
        if(item_to_add.size() != 2) {
            cout << "Error: please enter product name and price" << endl;
            cout << "Enter item: ";
            continue;
        }
    
        //pokud druhy token neni v decimalnim tvaru, chyba a nacitam dale
        if(!is_decimal(item_to_add[1])) {
            cout << "Error: price must be a real number" << endl;
            cout << "Enter item: ";
            continue;
        }
        
        /* jinak je vstup na urovni UI ok a pridavam polozku do kasy.
            pridani se samozrejme na urovni kasy nemusi podarit/*/
        kasa->add_item(item_to_add[0], item_to_add[1]);
        cout << "Enter item: ";

    }
}

/* funkce na nacitani itemu do objednavky
   funkce je podobna te predchozi, ale ve vice detailech je jina. Pripadna jejich kombinace by nebyla prilis citelna. */
void Add::read_order_items() {
    string input;
    cout << "Enter item: ";
    while (getline(cin, input)) {
        if(input.empty()) {
            return;
        }
        vector<string> items_to_add = Command::tokenize_command(input);
        string amount_temp;
        
        //nenutim uzivatele, aby specifikoval mnozstvi pridavane polozky, defaultne je tam 1
        if(items_to_add.size() == 1) {
            amount_temp = "1";
        }
        else if(items_to_add.size() == 2) {
            if(!is_decimal(items_to_add[1])) {
                cout << "Invalid amount." << endl;
                cout << "Enter item: ";
                continue;;
            }
            amount_temp = items_to_add[1];
        }
        else {
            cout << "Error with reading item." << endl;
            continue;
        }
        
        //pri pouziti teto funkce ocekavam, ze je ohlidano to, aby funkce mela nastavenou spravnou active_order, do ktere itemy nacitam
        kasa->add_to_order(items_to_add[0], amount_temp);
        cout << "Enter item: ";
    }
}

/* Pridani objednavky do kasy.
   Vola se tedy pri vstupu ve formatu "add order" + pripadna specifikace jmena objednavky.*/
void Add::add_order() {
    
    //jmeno objednavky nespecifikovane
    if(tokenized.size() < 3) {
        kasa->add_order();
        //nacitam polozky do objednavky
        read_order_items();
        return;
    }
    //jinak se pokusim pridat objednavku se specifikovanym jmenem
    if(!kasa->add_order(tokenized[2])) {
        return;
    }
    //pokud se podari ji pridat, nacitam do ni itemy
    read_order_items();
}


/* Nastaveni poctu penez v kase. Nepridava penize, ale primo nastavuje.
 Vyzaduje alespon 3 tokeny - add cash + castka*/
void Add::add_cash() {
    if(tokenized.size() < 3) {
        cout << "Cash must be specified." << endl;
        return;
    }
    if(!is_decimal(tokenized[2])) {
        cout << "Cash not in correct format." << endl;
        return;
    }
    kasa->set_cash(tokenized[2]);
}

//funkce na pridani polozek do objednavky
void Add::add_to() {
    
    //uzivatel nespecifikoval objednavku, chce vyuzit aktivni objednavku
    if(tokenized.size() == 2) {
        
        //opet, kasa by si ohlidala, ze nepridava polozky do nespecifikovane objednavky, ale je snazsi to v ramci UI odchytit i zde
        if(!kasa->active_order_ready()) {
            cout << "Cashdesk has no active order." << endl;
            return;
        }
        //ukazu aktivni objednavku
        kasa->show_order();
    }
    else if(tokenized.size() == 3) {
        
        //v pripade specifikovane objednavky ji ukazu, cimz zaroven nastavim active_order v kase, cehoz vyuzivam ve funkci read_order_items
        if(!kasa->show_order(tokenized[2])) {
            return;
        }
    }
    else {
        cout << "Unknown parameters." << endl;
        return;
    }
    
    //nactu polozky do active_order
    read_order_items();
}



