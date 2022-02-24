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
        if(input.empty()) {
            return;
        }
        vector<string> items_to_add = Command::tokenize_command(input);
        if(items_to_add.size() != 1) {
            cout << "Please enter only order name." << endl;
            continue;
        }
        kasa->remove_order(items_to_add[0]);
        cout << "Order to remove: ";
    }
}

//void Remove::read_order_items() {
//    string input;
//    cout << "Enter item: ";
//    while (getline(cin, input)) {
//        if(input.empty()) {
//            return;
//        }
//        vector<string> items_to_add = Command::tokenize_command(input);
//        string amount_temp;
//        if(items_to_add.size() == 1) {
//            amount_temp = "1";
//        }
//        else if(items_to_add.size() == 2) {
//            if(is_decimal(items_to_add[1])) {
//                amount_temp = items_to_add[1];
//            }
//        }
//        else {
//            cout << "Error with reading item." << endl;
//            continue;
//        }
//        kasa->add_to_order(items_to_add[0],  "-" + amount_temp);
//        cout << "Enter item: ";
//    }
//}



//void Remove::remove_from() {
//    if(tokenized.size() == 2) {
//        if(!kasa->active_order_ready()) {
//            cout << "No active order." << endl;
//            return;
//        }
//        kasa->show_order();
//    }
//    else if(tokenized.size() == 3) {
//        if(!kasa->show_order(tokenized[2])) {
//            cout << "Order does not exist." << endl;
//            return;
//        }
//    }
//    else {
//        cout << "Unknown parameters." << endl;
//        return;
//    }
//    read_order_items();
//}

