//
//  Item.cpp
//  Pokladna
//
//  Created by Tobias Vavroch on 21/10/2021.
//
#include "Item.hpp"

using Money = dec::decimal<2>;

Item::Item(string name, Money price) {
    this->item_name = name;
    this->price = price;
}

const string& Item::get_name() {
    return this->item_name;
}

const Money& Item::get_price() {
    return price;
}

/*
 Item vypisuji tak, jak typicky vypadaji polozky na uctence – jmeno polozky, nekolik tecek, cena za kus, mnozstvi a celkova cena polozky pri danem mnozstvi
 pri maximalni delce jmena polozky chci, aby tam stale tecek bylo alespon 5
 
 Vypsani polozky na ostream. Vypise pouze jmeno polozky, oddelovac (tecky) a cenu polozky bez endl, aby za ni bylo mozne pridat i mnozstvi pri
 vypisovani objednavky
 */
ostream& operator<<(ostream& os, const Item& item)
{
    string offset(item_name_max_length + 5 - item.item_name.size(), '.');
    os << item.item_name << offset << item.price;
    return os;
}
