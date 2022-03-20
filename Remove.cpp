//
//  Remove.cpp
//  Pokladna
//
//  Created by Tobias Vavroch on 20/12/2021.
//

#include "Command.hpp"


/*procesovani prikazu remove, mozne parametry jsou item a order, podle kterych se nasledne volaji funkce
  prikazy se porovnavaji s moznymi zkratkami nebo akronymy */
void Remove::process_command() {
    if(find(acros_item.begin(), acros_item.end(), tokenized[1]) != acros_item.end()) {
        remove_item();
         return;
    }
    if(find(acros_order.begin(), acros_order.end(), tokenized[1]) != acros_order.end()) {
        remove_order();
        return;
    }
    else {
        cout << "Uknown command" << endl;
    }
}

//funkce nacita itemy, ktere se maji odstranit z kasy
void Remove::remove_item() {
    string input;
    cout << "Item to remove: ";
    while (getline(cin, input)) {
        
        if(!is_input_ascii(input)) {
            cout << "Not supported input" << endl;
            cout << "Item to remove: ";
            continue;
        }
        
        //prazdny vstup ukonci nacitani itemu
        if(input.empty()) {
            return;
        }
        //jinak se vstup rozparsuje
        vector<string> items_to_add = Command::tokenize_command(input);
        if(items_to_add.size() != 1) {
            cout << "Please enter only item name." << endl;
            continue;
        }
        //funkce to posle do kasy a o nic dalsiho se nestara
        kasa->remove_item(items_to_add[0]);
        cout << "Item to remove: ";

    }
}

//smazani objednavky, jinak funkce velice podobna te predchozi, akorat jine chybove hlasky a jina funkce volana na kase
void Remove::remove_order() {
    string input;
    cout << "Order to remove: ";
    while (getline(cin, input)) {
        
        if(!is_input_ascii(input)) {
            cout << "Not supported input" << endl;
            cout << "Order to remove: ";

            continue;
        }
        
        if(input.empty()) {
            return;
        }
        vector<string> items_to_add = Command::tokenize_command(input);
        if(items_to_add.size() != 1) {
            cout << "Please enter only order name." << endl;
            cout << "Order to remove: ";

            continue;
        }
        kasa->remove_order(items_to_add[0]);
        cout << "Order to remove: ";
    }
}

