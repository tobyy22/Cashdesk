//
//  Order.cpp
//  Pokladna
//
//  Created by Tobias Vavroch on 21/10/2021.
//

#include "Order.hpp"


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
    for (size_t i = 0; i < items_in_order.size(); i++) { //musim zkontrolovat, zda pridavany Item uz je v objednavce – cyklim pres Itemy v objednavce
        
        observer_ptr<Item> ith_item_in_order = items_in_order[i].item;
        
        if(item == ith_item_in_order) { //porovnavam pridavany Item s i-tym Itemem v objednavce
            
            // Pokud by pridavane mnozstvi + stavajici mnozstvi presahlo maximalni pocet druhu jednoho zbozi v objednavce, odmitam
            if(items_in_order[i].amount + amount > maximum_number_of_items_of_single_type_in_order) {
                cout << "Order cannot contain more than 99 pieces of a single item type." << endl;
                return false;
            }
            
            // Pokud by stejna honota klesla pod nulu, zbozi uplne odebiram
            if(items_in_order[i].amount + amount <= 0) {
                items_in_order.erase(items_in_order.begin() + (int)i);
                cout << "Deleting item from order." << endl;
                sum_order();
                return true;
            }
            
            //kontrola, ze stavajici hodnota objednavkzy + hodnota noveho zbozi nepresahne maximalni moznou hodnotu objednavky
            if(order_sum + (amount)*item->get_price() > maximum_order_price) {
                cout << "Maximum order price would be reached." << endl;
                return false;
            }
            
            //pricitam zbozi do objednavky
            items_in_order[i].amount += amount;
            sum_order();
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
    if(order_sum + amount*item->get_price() > maximum_order_price) {
        cout << "Maximum order price would be reached." << endl;
        return false;
    }
    
    //pridam zbozi do objednavky
    items_in_order.push_back(item_in_order(item, amount));
    sum_order();
    return true;
}

// Funkce, ktera o objednavce rekne, zda obsahuje konkretni polozku
bool Order::contains_item(observer_ptr<Item> item) {
    for (size_t i = 0; i < items_in_order.size(); ++i) {
        if(item == items_in_order[i].item) {
            return true;
        }
    }
    return false;
}

// Funkce na vypsani objednavky
void Order::show_order() {
    std::cout << OrderName << " " << std::endl;
    for (size_t i = 0; i < items_in_order.size(); ++i) {
        cout << items_in_order[i].item << ' ' << items_in_order[i].amount << endl;;
    }
    cout <<"Order sum: " << order_sum << endl;
    cout << endl;
}

void Order::sum_order() {
    Money sum(0);
    for (size_t i = 0; i < items_in_order.size(); ++i) {
        sum += items_in_order[i].amount*items_in_order[i].item->get_price();
    }
    order_sum = sum;
}

const string& Order::order_name() {
    return OrderName;
}

//Funkce vraci velikost objednavky – pocet polozek v objednavce
int Order::order_size() {
    return (int)items_in_order.size(); //diky tomu, ze maximalni velikost objednavky je jasne omezena, muzu si dovolit velikost vektoru pretypovat na int
}

/*
 Vypsani cele objednavky na ostream.
 */
ostream& operator<<(ostream& os, const Order& order) {
    os << order.OrderName << " " << std::endl;

    for (size_t i = 0; i < order.items_in_order.size() ; i++) {
        
        //nutno pristoupit k hodnote pointeru pres *
        os << *order.items_in_order[i].item << ' ' << order.items_in_order[i].amount << endl;

    }
    os << "Order sum: " << order.order_sum << endl;
    os << std::endl;
    return os;
}
