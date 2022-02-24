//
//  Item.cpp
//  Pokladna
//
//  Created by Tobias Vavroch on 21/10/2021.
//
#include <iostream>
#include "Item.hpp"

using Money = dec::decimal<2>;

Item::Item(string name, Money price) {
    this->item_name = name;
    this->price = price;
}

const string& Item::getName() {
    return this->item_name;
}

const Money& Item::getPrice() {
    return price;
}

/*item vypisuji tak, jak typicky vypadaji polozky na uctence – jmeno polozky, nekolik tecek, cena za kus, mnozstvi a celkova cena polozky pri danem mnozstvi
 pri maximalni delce jmena polozky chci, aby tam stale tecek bylo alespon 5
 */
void Item::ShowItem(Money amount) {
    string offset(item_name_max_length + 5 - item_name.size(), '.');
    std::cout << item_name << offset << price << " x " << amount << ' ' << amount*price << std:: endl;
}

//zobrazeni polozky bez mnozstvi a celkove ceny – tak jak typicky vypadaji v restauracnim menu
void Item::ShowItem() {
    string offset(item_name_max_length + 5 - item_name.size(), '.');
    std::cout << item_name << offset << price << std:: endl;
}
