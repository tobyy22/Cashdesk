//
//  CashDesk.hpp
//  Pokladna
//
//  Created by Tobias Vavroch on 21/10/2021.
//



#include <stdio.h>
#include <vector>

#include "Data.hpp"
#include "Identificator.hpp"



/*
 Omezeni, ktera by mela byt v naproste vetsine pripadu neomezujici, na vetsi pocty neni aplikace stavena (bylo by treba uz vyuzivat databazi. Omezeni maji 2 vyznamy - jednak
 diky nim neprecete pocet penez v kase a zaroven se jedna o smysluplne omezeni vzhledem k "databazi", ktera by vetsi data podporovala jen velice stezi.
 */
    
const int max_number_of_items = 1000;
const int max_number_of_orders = 999;
const int max_item_price = 999999;
const int max_number_of_all_items_in_all_orders = 1000;
const int max_cash_in_cashdesk = 1000000000;


/* Trida simuluje klasickou pokladnu. Obsahuje data o polozkach, zamestnancich, lze pridavat/mazat/upravovat data, vytvaret objednavky
 procesovat objednavky, evidovat hotovost.
 */

class CashDesk {
private:
    
//  Trida, ktera prideluje unikatni ID objednavkam, u kterych uzivatel nechtel vymyslet jmeno.
    ID orders_id;
//  Kasa ma svuj Data_Handler, ktery se stara o fyzicke ukladani dat
    Data_Handler data;
    
//  Data ulozena v kase - polozky, objednavky, hotovost
    vector<Item> items;
    vector<Order> orders;
    Money cash;
    
    /*kasa si pamatuje posledni objednavku, se kterou bylo pracovano
      poskytuje tim moznost pracovat s posledni objednavkou, se kterou bylo pracovano
      pokusil jsem se ti implementovat co nejvice intuitivne
     */
    observer_ptr<Order> active_order;
    
    
    /* Funkce, ktere naleznou, danou polozku nbo objednavku v kase.
     V pripade neuspechu vraci nullptr, coz typicky pouzivam pro overeni, zda objednavka v kase existuje nebo ne.
     */
    observer_ptr<Order> get_order_pointer_by_name(const string& order_name, bool state_success, bool state_fail);
    observer_ptr<Item> get_item_pointer_by_name(const string& name, bool state_success, bool state_fail);
    
    /* par statickych metod tridy
        "nezavisle" na jednotlivych instancich tridy
     */
    static bool positive_price_ok(string amount, int n);
    static bool negative_price_ok(string amount, int n);
    static bool price_ok(string amount);
    static bool item_order_name_ok(const string& item_name);
    
    int number_of_all_items_in_all_orders();
    
public:
    /*
     Rozhrani tridy vypada tak, ze vetsina funkci prijima stringove parametry, ktere si pak sama prevadi to pozadovane podoby.
     Program funguje tak, ze se naparsuje command, ktery nasledne zavola spravnou funkci pokladny. Pokud tedy uzivatel zada command,
     ktery ma pridat polozku do pokladny, kasa mu poskytne interface, ktere udaje o polozce pozaduje a sama si pak objekt polozky vytvori.
     Diky tomu by byla snazsi integrace rozhrani pokladny s jinym uzivatelskym interface.

     Muze se stat, ze nektere funkce aktualni interface ani nepouziva, nicmene davalo mi smysl je tam ponechat pro pripadne rozsirovani interface a funkcnosti programu.
     */
    CashDesk();
    
    
    /*
     Funkce na pridavani dat do pokladny.
     */
    void add_item(const string& item_name, const string& item_price);
    bool add_order(const string& order_name);
    bool add_order();
    void add_to_order(const string& item_name, const string& amount);
    void add_to_order(const string& order_name, const string& item_name, const string& amount);
    
    /*
     Odebirani dat z pokladny. Chybi funkce pro odebrani polozek z objednavky.
     To resim v ramci predchozich funkci, ktere umi pridat i zaporne mnozstvi, jelikoz jsem stejne chtel podporovat unarni minus v ramci pokladny.
     */
    void remove_order(const string& order_name);
    void remove_item(const string& name);
    
    /* ruzne dalsi funkce CashDesk, podorobneji popsany tam, kde jsou implementovany
     jejich funkce by mela byt zrejma z jejich jmen, pripadne slozitosti v implementaci popsany v implementaci*/
    void find_order(const string& order_name);
    void find_item(const string& item);
    void set_cash(const string& cash);
    bool show_order(const string& order_name);
    bool process_order(const string& order_name, const string& paid);
    void show_all_items();
    void show_all_orders();
    void show_cash();
    bool active_order_ready();
    bool show_active_order();
    
    



    

    
    
    

    
};
