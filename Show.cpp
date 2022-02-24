//
//  Show.cpp
//  Pokladna
//
//  Created by Tobias Vavroch on 20/12/2021.
//

#include "Command.hpp"

//procesovani prikazu show, rozpoznava se items, orders, cash, podle toho se volaji funkce, hledaji se zkratky a akronymy
void Show::process_command() {
    if(find(acros_item.begin(), acros_item.end(), tokenized[1]) != acros_item.end()) {
        show_items();
        return;
    }
    else if(find(acros_order.begin(), acros_order.end(), tokenized[1]) != acros_order.end()) {
        show_orders();
        return;
    }
    else if(find(acros_cash.begin(), acros_cash.end(), tokenized[1]) != acros_cash.end()) {
        kasa->show_cash();
    }
    else {
        cout << "Uknown command" << endl;
    }
}

/*show items, pokud jsou nespecifikovane itemy, ukazou se vsechny, jinak se projdou tokeny a kasa ukaze vsechny itemy
ktere v sobe obsahuji dany string */
void Show::show_items() {
    if(tokenized.size() > 2) {
        for (size_t i = 2; i < tokenized.size() ; ++i) {
            kasa->find_item(tokenized[i]);
        }
    }
    else {
        kasa->show_all_items();
    }
}

//show orders, bud se opet ukazou vsechny a nebo uzivatel zada ty, ktere ho zajimaji
void Show::show_orders() {
    if(tokenized.size() > 2) {
        for (size_t i = 2; i < tokenized.size() ; ++i) {
            kasa->find_order(tokenized[i]);
        }
    }
    else {
        kasa->show_all_orders();
    }
}


