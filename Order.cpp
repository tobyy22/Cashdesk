//
//  Order.cpp
//  Pokladna
//
//  Created by Tobias Vavroch on 21/10/2021.
//

#include "Order.hpp"
#include "Item.hpp"
#include <iostream>


Order::Order(const string& name) {
    this->OrderName = name;
}


bool Order::add_item_to_order(observer_ptr<Item> item) {
    return add_item_to_order(item, Money(1));
}

/*
 Trochu obsahlejsi funkce starajici se o pridani polozky do objednavky.
 Trida CashDesk volajici tuto funkci zarucuje, ze parametr amount nebude vetsi nez 1 000 000 000, coz je dulezite proto,
 ze jednak parametr amount urcite nepreteka a zaroven si diky tomu hlidam, aby mi dalsi pripadne operace s timto parametrem take nepretekly.
 Funkce vraci bool, zda se zbozi podarilo nebo nepodarilo pridat
 */
bool Order::add_item_to_order(observer_ptr<Item> item, const Money& amount) {
    if(amount > maximum_number_of_items_of_single_type_in_order) { //pokud bych chtel pridat vice nez maximalni povoleny pocet kusu jednoho druhu zbozi do objednavky, hned odmitam
        cout << "Order cannot contain more than 99 pieces of a single item type." << endl;
        return false;
    }
    for (size_t i = 0; i < ItemsInOrder.size(); i++) { //musim zkontrolovat, zda pridavany Item uz je v objednavce – cyklim pres Itemy v objednavce
        
        /* V nasledujici funkci pouzivam:
            get<0>(ItemsInOrder[i]), coz je samotna i-ta polozka v objednavce
            get<1>(ItemsInOrder[i]), coz je mnozstvi i-te polozky v objednavce */
        
        observer_ptr<Item> ith_item_in_order = get<0>(ItemsInOrder[i]);
        if(item == ith_item_in_order) { //porovnavam pridavany Item s i-tym Itemem v objednavce
            
            // Pokud by pridavane mnozstvi + stavajici mnozstvi presahlo maximalni pocet druhu jednoho zbozi v objednavce, odmitam
            if(get<1>(ItemsInOrder[i]) +  amount > maximum_number_of_items_of_single_type_in_order) {
                cout << "Order cannot contain more than 99 pieces of a single item type." << endl;
                return false;
            }
            
            // Pokud by stejna honota klesla pod nulu, zbozi uplne odebiram
            if(get<1>(ItemsInOrder[i]) + amount <= 0) {
                ItemsInOrder.erase(ItemsInOrder.begin() + (int)i);
                cout << "Deleting item from order." << endl;
                return true;
            }
            
            //kontrola, ze stavajici hodnota objednavkzy + hodnota noveho zbozi nepresahne maximalni moznou hodnotu objednavky
            if(sum_order() + (amount)*item->getPrice() > maximum_order_price) {
                cout << "Maximum order price would be reached." << endl;
                return false;
            }
            
            //pricitam zbozi do objednavky
            get<1>(ItemsInOrder[i]) += amount;
            return true;
        }
    }
    
    //pokud zbozi v objednavce nenim dostaneme se sem.
    
    // kontrola, ze pridavaneho zbozi neni mensi nez nula
    if(amount < 0) {
        cout << "Order can not contain negative amount of items." << endl;
        return false;
    }
    
    //kontrola, ze nova hodnota objednavky je ok
    if(sum_order() + amount*item->getPrice() > maximum_order_price) {
        cout << "Maximum order price would be reached." << endl;
        return false;
    }
    
    //pridam zbozi do objednavky
    ItemsInOrder.push_back(make_tuple(item, amount));
    return true;
}

// Funkce, ktera o objednavce rekne, zda obsahuje konkretni polozku
bool Order::contains_item(observer_ptr<Item> item) {
    for (size_t i = 0; i < ItemsInOrder.size(); ++i) {
        if(item == get<0>(ItemsInOrder[i])) {
            return true;
        }
    }
    return false;
}

// Funkce na vypsani objednavky
void Order::show_order() {
    std::cout << OrderName << " " << std::endl;
    for (size_t i = 0; i < ItemsInOrder.size(); ++i) {
        get<0>(ItemsInOrder[i])->ShowItem(get<1>(ItemsInOrder[i]));
    }
    std::cout <<"Order sum: " << sum_order() << endl;
    std::cout << std::endl;
}

Money Order::sum_order() {
    Money sum(0);
    for (size_t i = 0; i < ItemsInOrder.size(); ++i) {
        sum += get<1>(ItemsInOrder[i])*get<0>(ItemsInOrder[i])->getPrice();
    }
    return sum;
}

const string& Order::order_name() {
    return OrderName;
}

//Funkce vraci velikost objednavky – pocet polozek v objednavce
int Order::order_size() {
    return (int)ItemsInOrder.size(); //diky tomu, ze maximalni velikost objednavky je jasne omezena, muzu si dovolit velikost vektoru pretypovat na int
}


