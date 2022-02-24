//
//  Data.hpp
//  Pokladna
//
//  Created by Tobias Vavroch on 11/11/2021.
//



#pragma once
#include "lib/decimal.h"
#include "Item.hpp"
#include "Order.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>


/*
 Trida se stara o fyzicke ulozeni dat. Trida CashDesk obsahuje instanci teto tridy. Vzdy,
 kdyz pokladna nejakym zpusobem upravuje sva data, pouziva tuto tridu pro load/save/update dat.
 Data jsou ulozena v souborech ve formatu csv.
 */

class Data_Handler {
private:
    
    /*
     Const vektor files_to_check obsahuje jmena vsech souboru, ktere databaze pozaduje.
     Pokud nejaky neexistuje, databaze si ho vytvori.
     */
    const string path = "db/";
    const string items_file = path + "Items.txt";
    const string orders_file = path + "Orders.txt";
    const string items_in_orders_file = path + "Items_in_orders.txt";
    const vector<string> files_to_check = {items_file, orders_file,items_in_orders_file};
    
    /*
     Trida ma v pameti ulozene jmena vsech polozek a jmena vsech objednavek pro rychlejsi praci s daty.
     */
    vector<string> item_keys;
    vector<string> order_keys;
    
    
    //funkce na nacteni itemu, objednavek a itemu do objednavek. Kase je viditelna pouze funkce load_data, ktera tyto funkce sdruzuje
    void load_items(vector<Item> &items);
    void load_orders(vector<Order> &orders);
    void load_items_to_orders(vector<Item> &items, vector<Order> &orders);
    
    /*
     Pomocne funkce pro utot tridu, ktere umi smazat n tou radku v souboru a naparsovat radku v csv formatu.
     */
    void delete_nth_line(const string& filename, int n);
    vector<string> parse_csv_line(const string& line);
    
public:
    
    Data_Handler();
    
    /* Na zacatku programu pokladna preda vektory obsahujici polozky a objednavky teto tride, ktera se postara o nacteni dat do vektoru
     ze souboru.
     */
    void load_data(vector<Item> &items, vector<Order> &orders);
    
    void check_if_files_exists();
    
    /* Nasledne trida poskytuje pokladne interface pro ukladani dat.
       Pokud se v pokladne zmeni data, pokladna to da vedet tomuto api, ktere se postara o jejich fyzicke ulozeni.
     */
    void add_item(const string& item, const Money& price);
    void delete_item(const string& item);
    void add_order(const string& order_name);
    void add_item_to_order(const string& item, const string& order_name, const Money& m);
    void delete_order(const string& order_name);
    void delete_item_from_order(const string& item, const string& order_name);
    

};
