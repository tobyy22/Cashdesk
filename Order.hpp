//
//  Order.hpp
//  Pokladna
//
//  Created by Tobias Vavroch on 21/10/2021.
//


#include <stdio.h>
#include <vector>
#include <tuple>
#include <iostream>

#include "Item.hpp"
#include "lib/decimal.h"


/*
 Konstanty definujici omezeni objednavky.
 */
const int maximum_number_of_items_of_single_type_in_order = 99;
const int maximum_order_price = 9999999;


/* Trida pro reprezentaci objednavky.*/
class Order {
private:
    /* Obsahuje jmeno objednavky, a vektor tuplu, coz je vlastne samotna objednvka. Tuple je slozen z pointeru na Item a objekt tridy Money pro reprezentaci mnozstvi
     polozky v objednavce. Chci, aby pocet itemu v objednavce nemuselo byt cele cislo.*/
    string OrderName;
    
    
    //struktura sdruzujici polozku a mnozstvi polozky v objednavce
    struct item_in_order {
        observer_ptr<Item> item;
        Money amount;
        item_in_order(observer_ptr<Item> given_item,Money given_amount) : item(given_item), amount(given_amount){}
    };
    
    vector<item_in_order> items_in_order;


public:
    
    // prubezne aktualizovana promenna obsahujici celkovou hodnotu objevnavky
    Money order_sum;
    
    
    //u objednavky na zacatku staci jmeno, polozky se pridavaji postupne
    Order(const string& name);
    
    /* Nasledne je zde rozhrani objednavky, ktere umi mazat, pridavat polozky do objednavek, pripadne se secist jako objednavka.
     Komentare jsou u implementaci jednotlivych funkci. */
    const string& order_name();
    bool add_item_to_order(observer_ptr<Item> item);
    bool add_item_to_order(observer_ptr<Item> item, const Money& amount);
    void show_order();
    int order_size();
    void sum_order();
    
    //zjisti, zda danoy item predany pointerem jiz obsahuje
    bool contains_item(observer_ptr<Item> item);
    

    //vypis objednavky
    friend ostream& operator<<(ostream& os, const Order& order);
};
