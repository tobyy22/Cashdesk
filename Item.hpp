//
//  Item.hpp
//  Pokladna
//
//  Created by Tobias Vavroch on 21/10/2021.
//
#pragma once


#include <stdio.h>
#include <string>
#include "lib/decimal.h"

template<typename T>
using observer_ptr = T*;

using Money = dec::decimal<2>;


/*
 Maximalni delka jmena polozky, vyuziva se dale pri vypisu polozky.
 */
const int item_name_max_length = 25;

/*
 Trida se stara o reprezentaci polozek v kase.
 */

using namespace std;

class Item {
private:
    /*
     pamatuji si jmeno polozky, coz je zaroven jeji UNIKATNI identifikator,
     kasa muze obsahovat pouze jednu polozku s danym jmenem
     */
    string item_name;
    
    //dale si pamatuji jeji cenu
    Money price;
    
public:
    //polozka musi dostat jmeno a cenu
    Item(string name, Money price);
    
    /*jednoduche funkce, ktere pouze vraci jmeno polozky, jeji cenu, pripadne polozku v prehlednem formatu vypisuji
     podrobnejsi komentare u implementaci funkci
     */
    const string& getName();
    const Money& getPrice();
    void ShowItem();
    void ShowItem(Money m);
};
