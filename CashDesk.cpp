//
//  CashDesk.cpp
//  Pokladna
//
//  Created by Tobias Vavroch on 21/10/2021.
//

#include <iostream>
#include "CashDesk.hpp"

//Pri vytvoreni kasy jen nactu data
CashDesk::CashDesk(){
    cout << "Loading data, please wait." << endl;
    data.load_data(items, orders);
    cout << "CashDesk ready to use." << endl;
}

//Pridani polozky do objednavky
void CashDesk::add_item(const string& item_name, const string& item_price) {
    
    //nejprve kontrola, zda kasa umoznuje pridani dalsiho itemu z hledika maximalniho poctu itemu v kase
    if((int)items.size() >= max_number_of_items) {
        cout << "Maximum number of items in this CashDesk has been reached." << endl;
        return;
    }
    
    //zkontroluju jmeno
    if(!item_order_name_ok(item_name)) {
        return;
    }
    
    /*zkontroluju cenu – zde se jedna pouze o takovou kontrolu, abych dale mohl vytvorit objekt typu Money, ktery nepretece a ma danou maximalni hodnotu
      momentalne se jedna o hodnotu 1 000 000 000
      mohu to tedy prevest a provadet aritmeticke operace
     */
    if(!price_ok(item_price)) {
        cout << "Amount not supported." << endl;
        return;
    }
    
    //vyrobim cenu
    Money m(item_price);
    
    //zde kontroluji, zda cena neni prilis velka
    if(m > max_item_price) {
        cout << "Cashdesk doesnt support items more expensive than 999 999 CZK." << endl;
        return;
    }
    
    //nebo neni zaporna
    if(m < 0) {
        cout << "Cashdesk doesnt support items with negative value." << endl;
        return;
    }

    //overim, zda item s danym jmenem jiz neexistuje
    observer_ptr<Item> item = get_item_pointer_by_name(item_name, true, false);
    if(item != nullptr) {
        return;
    }
    
    //item vytvorim a pridam
    ;
    this->items.push_back(Item(item_name, m));
    
    //reknu ulozisti dat, ze byl pridan item
    data.add_item(item_name, m);
}

/*Pridani objednavky do kasy.
 Funkce vraci hodnotu bool proto, aby oznamila uzivatelskemu interface, zda se pridani objednavky zdarilo.
 Proc? Protoze casto po pridani objednavky ji hned chceme zacit plnit polozkami. Interface musi vedet, zda polozky
 muze zacit nacitat.
 */
bool CashDesk::add_order(const string& order_name) {
    
    //kontrola jmena objednavky
    if(!item_order_name_ok(order_name)) {
        return false;
    }
    
    //kontrola poctu objednavek
    if((int)orders.size() == max_number_of_orders) {
        cout << "Maximum number of orders in this CashDesk has been reached." << endl;
        return false;
    }
    
    //kontrola zda objednavka jiz neexistuje
    observer_ptr<Order> order = get_order_pointer_by_name(order_name, false, false);
    if(order != nullptr) {
        return false;
    }
    
    //vytvoreni a pridani objednavky
    orders.push_back(Order(order_name));
    
    //nastaveni aktivni objednavky na prave pridanou
    active_order = get_order_pointer_by_name(order_name, false, false);
    
    //oznameni ulozisti dat, ze byla pridana objednavka
    data.add_order(order_name);
    return true;
}

//pridani objednavky bez specifikace jmena
void CashDesk::add_order() {
    
    //kontrola poctu objednavek
    if((int)orders.size() == max_number_of_orders) {
        cout << "Maximum number of orders in this CashDesk has been reached." << endl;
        return;
    }
    
    //postupne generuji jmena pomoci get_unique tridy Identificator a pomoci predchozi funkce zkousim objednavku pridat
    while(true) {
        if(add_order(orders_id.get_unique())) {
            break;
        }
    }
}

//pridani polozky do aktivni objednavky
void CashDesk::add_to_order(const string& item_name, const string& amount) {
    add_to_order(active_order->order_name(), item_name, amount);
}

//pridani polozky do specifikovane objednavky
void CashDesk::add_to_order(const string& order_name, const string& item_name, const string& amount) {
    if(!item_order_name_ok(order_name)) {
        return;
    }
    if(!price_ok(amount)) {
        cout << "Amount not supported." << endl;
        return;
    }
    
    //pokud objednavka nebo item neexituji, nic nedelam, o vypis chyby se staraji funkce get_order_pointer_by_name a get_item_pointer_by_name
    observer_ptr<Order> order = get_order_pointer_by_name(order_name, false, true);
    observer_ptr<Item> item = get_item_pointer_by_name(item_name, false, true);

    if(order == nullptr || item == nullptr) {
        return;
    }
    
    //vytvorim si objekt Money
    Money m(amount);

    //zkusim pridat polozku do objednavky, v pripade uspechu to oznamim ulozisti
    if(order->add_item_to_order(item, m)) {
        data.add_item_to_order(item_name, order->order_name(), m);
    }
    //nastavim active_order na tuto objednavku
    active_order = order;
    return;
}

bool CashDesk::active_order_ready() {
    return active_order != nullptr;
}

//odstraneni objednavky
void CashDesk::remove_order(const string& name){
    
    //projdu objednavky, najdu ji a odstranim
    for (size_t i = 0; i < orders.size(); ++i) {
        if(orders[i].order_name() == name) {
            orders.erase(orders.begin() + (int)i);
            
            //aktivni objednavku nechci definovat po teto operaci
            active_order = nullptr;
            
            //oznamim smazani objednavky ulozisti
            data.delete_order(name);
            return;
        }
    }
    
    cout << "Order does not exist." << endl;
}

//odstraneni itemu
void CashDesk::remove_item(const string& name) {
    
    /*Nejprve kontrola, zda item existuje.
      Trochu neefektivni, protoze item hledam dvakrat, ale zprehledni to nasledujici kod. */
    observer_ptr<Item> item = get_item_pointer_by_name(name, false, true);
    if(item == nullptr) {
        return;
    }
    
    /* Zkontroluji, zda exsituje objednavka obsahujici tento item.
       Pokud ano, nechci ho odstranit.*/
    for (size_t i = 0; i < orders.size(); i++) {
        if(orders[i].contains_item(item)) {
            cout << "All orders containing this item must be processed." << endl;;
            return;
        }
    }
    /* Oznamim ot ulozisti. Uloziste se samo postara, ze item bude smazan i z objesnavek.*/
    data.delete_item(name);
    
    //nasledne najdu a smazu samotny item
    for (size_t i = 0; i < this->items.size(); ++i) {
        if(this->items[i].getName() == name) {
            this->items.erase(items.begin() + (int)i);
            return;
        }
    }
}

//funkce na nalezeni itemu
void CashDesk::find_item(const string& item) {
    for (size_t i = 0; i < items.size(); i++) {
        if(items[i].getName().find(item) != string::npos) {
            items[i].ShowItem();
        }
    }
}

//vypsani vsech itemu v databazi
void CashDesk::show_all_items(){
    if(items.empty()) {
        cout << "No items added." << endl;
        return;
    }
    for (size_t i = 0; i < items.size(); ++i) {
        items[i].ShowItem();
    }
}

//nalezeni objednavky
void CashDesk::find_order(const string& order_name) {
    for (size_t i = 0; i < orders.size(); ++i) {
        if(orders[i].order_name().find(order_name) != string::npos) {
            orders[i].show_order();
        }
    }
}

//vypsani vhsech objednavek
void CashDesk::show_all_orders() {
    if(orders.empty()) {
        cout << "No orders added." << endl;
        return;
    }
    for (size_t i = 0; i < orders.size(); ++i) {
        orders[i].show_order();
    }
}

//najde item v databazi a vrati na nej pointer
observer_ptr<Item> CashDesk::get_item_pointer_by_name(const string& name, bool success, bool fail) {
    for (size_t i = 0; i < items.size(); ++i) {
        if(items[i].getName() == name) {
            if(success) {
                cout << "Item exists." << endl;
            }
            return &items[i];
        }
    }
    if(fail) {
        cout << "Item does not exist." << endl;
    }
    return nullptr;
}

//nastavi penize v kase
void CashDesk::set_cash(const string& cash) {
    if(!price_ok(cash)) {
        return;
    }
    this->cash = Money(cash);
}

//zobrazi objednavku a vraci hodnotu bool podle toho, zda objednavka existuje
bool CashDesk::show_order(const string& order_name) {
    observer_ptr<Order> order = get_order_pointer_by_name(order_name, false, true);
    if(order == nullptr) {
        return false;
    }
    active_order = order;
    order->show_order();
    return true;
}

void CashDesk::show_order() {
    if(active_order == nullptr) {
        cout << "Please specify order." << endl;
        return;
    }
    active_order->show_order();
}

/* Funkce na zprocesovani objednavky, dostane jmeno zaplacene objednavky a placenou castku.
   Vraci hodnotu bool podle toho, zda se to podarilo – muze byt dobre info pro user interface.
 */
bool CashDesk::process_order(const string& order_name, const string& paid) {
    
    if(!price_ok(paid)) {
        return false;
    }
    //zkonstroluje, ze objednavka existuje
    observer_ptr<Order> order = get_order_pointer_by_name(order_name, false, true);
    if(order == nullptr) {
        return false;
    }
    
    //prevede si placenou castku a rekne si o celkovou sumu objednavky
    Money paid_converted(paid);
    Money order_price = order->sum_order();
    
    //zkontroluje, ze castka staci
    if(paid_converted < order_price) {
        cout << "No sufficient amount!" << endl;
        return false;
    }
    
    //rekne, kolik zakaznikovi vracime, pricte penize do kasy, odebere objednavku
    cout << "Return: " << paid_converted - order_price << endl;
    this->cash += order_price;
    remove_order(order_name);
    return true;
}

/* Funkce najde objednavku podle jmena, pokud ji nenajdem vrati nullptr.
    Je mozne specifikovat, zda ma neco vypsat na vystup v pripade neuspechu.*/
observer_ptr<Order> CashDesk::get_order_pointer_by_name(const string& order_name, bool success, bool fail) {
    for (size_t i = 0; i < orders.size(); i++) {
        if(orders[i].order_name() == order_name) {
            if(success) {
                cout << "Order exists." << endl;
            }
            return &orders[i];
        }
    }
    if (fail){
        cout << "Order does not exist." << endl;
    }
    return nullptr;
}

void CashDesk::show_cash() {
    cout << cash << endl;
}

//funkce kontroluje, zda amount reprezentuje nezaporne cislo mensi rovne 10 na (n + 1)
//ocekavam, ze amount je libovolne cislo v decimalnim formatu, spatny vstup resi user interface
//ve skutecnosti kontroluji, zda pocet cifer pred desetinnou teckou je maximalne n a nasledne pocet cifer za desetinnou teckou jiz neresim ->splnuje predchozi podminku
bool CashDesk::positive_price_ok(string amount, int n) {
    
    //zaporne nebereme
    if(amount[0] == '-') {
        return false;
    }
    //nasledne prochazim cifry a hledam desetinnu tecku
    for (size_t i = 0; i < amount.size(); ++i) {
        
        //jiz jsem na n + 1 cifre a desetinnou tecku jsem nenasel -> cislo je prilis velke
        if((int)i > n - 1) {
            return false;
        }
        //cislo neni prilis velke a nasel jsem tecku -> ok
        if(amount[i] == '.') {
            return true;
        }
    }
    return true;
}

//stejne jako predchozi, jen cele posunute kvuli minus na zacatku
bool CashDesk::negative_price_ok(string amount, int n) {
    if(amount[0] != '-') {
        return false;
    }
    for (size_t i = 0; i < amount.size(); ++i) {
        if((int)i > n) {
            return false;
        }
        if(amount[i] == '.') {
            return true;
        }
    }
    return true;
}

//funkce kontroluje, zda amount odpovida cislu v rozpeti od -1 000 000 000 do +1 000 000 000
bool CashDesk::price_ok(string amount) {
    
    //konkretne kontroluje zvlast kladne a zaporne moznosti – viz implementace vyse
    if(positive_price_ok(amount, 8)) {
        return true;
    }
    if(negative_price_ok(amount, 8)) {
        return true;
    }
    return false;
}

//funkce na kontrolu jmena itemu a objednavek. plati pro ne stejna omezeni – v soucasne chvili jen omezeni delky 
bool CashDesk::item_order_name_ok(const string &item_name) {
    if(item_name.length() > item_name_max_length) {
        cout << "Name too long." << endl;
        return false;
    }
    return true;
}


