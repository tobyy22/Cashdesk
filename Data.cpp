//
//  Data.cpp
//  Pokladna
//
//  Created by Tobias Vavroch on 11/11/2021.
//


#include "Data.hpp"


//pri vytvoreni dat pouze zkontroluji, ze vektory jsou prazne a zkontroluji, zda existuji tabulky databaze
Data_Handler::Data_Handler() {
    item_keys.clear();
    order_keys.clear();
    check_if_files_exists();
}

//funkce sdruzujici nacteni vsech dat do kasy, viditelna pro kasu, kasa referenci predava sve kontejnery, kde chce mit data
void Data_Handler::load_data(vector<Item> &items, vector<Order> &orders) {
    load_items(items);
    load_orders(orders);
    load_items_to_orders(items, orders);
}

//vrati vektor naparsovany podle carek - hodi se na parsovani csv formatu
vector<string> Data_Handler::parse_csv_line(const string& line) {
    vector<string> tokenized;
    std::string token = "";
    for (auto ch: line) {
        if(ch == ',') {
            if(token.length() > 0) {
                tokenized.push_back(token);
                token = "";
            }
        }
        else {
            token += ch;
        }
    }
    if(token.length() > 0) {
        tokenized.push_back(token);
    }
    return tokenized;
}

//nacteni itemu do vektoru
void Data_Handler::load_items(vector<Item> &items) {
    
    //otevru si stream ze souboru kde jsou ulozene itemy
    ifstream items_stream(items_file);
    std::string line;
    
    //nactu si radek ze souboru
    while (getline (items_stream, line)) {
        if(line.empty()) {
            /*prazdne radky chci preskakovat, nemusi se jednat o chybu. konkretne prvni radka muze byt prazdna kvuli tomu, ze
             zapisuji pomoci parametru fstream::app*/
            continue;
        }
        //naparsuji csv radku
        vector<string> data = Data_Handler::parse_csv_line(line);
        //musi mit presne 2 hodnoty, jinak je s daty chyba
        if(data.size() == 2) {
            //do vektoru pridam nove vytvorene objekty
            items.push_back(Item(data[0], Money(data[1])));
            item_keys.push_back(data[0]);
        }
        else {
            std::cout << "Problem with data" << std::endl;
        }
    }
}

//totozne jako predchozi, akorat objednavky
void Data_Handler::load_orders(vector<Order> &orders) {
    ifstream orders_stream(orders_file);
    std::string line;
    while (getline (orders_stream, line)) {
        if(line.empty()) {
            continue;
        }
        vector<string> data = Data_Handler::parse_csv_line(line);
        if(data.size() == 1) {
            orders.push_back(Order(data[0]));
            order_keys.push_back(data[0]);
        }
        else {
            std::cout << "Problem with data" << std::endl;
        }
    }
}

//funkce nacita itemy do objednavek
void Data_Handler::load_items_to_orders(vector<Item> &items, vector<Order> &orders) {
    ifstream MyReadFile(items_in_orders_file);
    std::string line;
        
    while (getline (MyReadFile, line)) {
        if(line.empty()) {
            continue;
        }
        vector<string> data = Data_Handler::parse_csv_line(line);
        //musi mit 3 hodnoty, prvni jmeno objednavky, druhy item v objednavce, treti jmeno itemu v objednavce
        if(data.size() == 3) {
            //v datech najdu objednavku, do ktere chci polozku pridat
            for (size_t i = 0; i < orders.size(); ++i) {
                if(orders[i].order_name() == data[0]) {
                    //najdu samotnou polozku, at na ni mam pointer
                    for (size_t j = 0; j < items.size(); ++j) {
                        if(items[j].get_name() == data[1]) {
                            //do objednavky pridam polozku
                            orders[i].add_item_to_order(&items[j], Money(data[2]));
                            break;
                        }
                    }
                    break;
                }
            }
        }
        else {
            std::cout << "Problem with data" << std::endl;
        }
    }
}

//kontrola a pripadne vytvoreni tabulek(souboru) databaze
void Data_Handler::check_if_files_exists() {
    for (size_t i = 0; i < files_to_check.size(); ++i) {
        if(!std::filesystem::exists(files_to_check[i])) {
            std::ofstream outfile (files_to_check[i]);
            outfile.close();
        }
    }
}

//pridani itemu do databaze
void Data_Handler::add_item(const string& item, const Money& price) {
    
    //trigger na urovni databaze, ktery kontroluje, zda tam item jiz neni
    if(count(item_keys.begin(), item_keys.end(), item)) {
        cout << "Item exists!" << endl;
        return;
    }
    item_keys.push_back(item);
    
    //otevru si stream do items_file
    ofstream out;

    out = ofstream(items_file, fstream::app);
    
    //zapisu data ve spravnem formatu
    out << item << "," << price <<endl;
    out.close();
}

//smazani polozky z databaze
void Data_Handler::delete_item(const string& name) {
    //cyklem najdu item(dostanu jeho index, protoze v item_keys jsou ulozene na stejnem )
    for (size_t i = 0; i < item_keys.size(); ++i) {
        if(item_keys[i] != name) {
            continue;
        }
        /*
         poradi ve polozek vektoru item_keys a v tabulce si odpovidaji, mohu tedy smazat n-tou radku
         z tabulky. Prazdne radky me netrapi, ve funkci delete_nth_line je proste ignoruji.
         */
        delete_nth_line(items_file, (int)i);
        item_keys.erase(item_keys.begin() + (int)i);
        return;
    }
    cout << "Item does not exist in Database" << endl;
}

//pridani objenavky do databaze
void Data_Handler::add_order(const string& order_name) {
    if(count(order_keys.begin(), order_keys.end(), order_name)) {
        cout << "Order exists!" << endl;
        return;
    }
    ofstream out;
    out = ofstream(orders_file, fstream::app);
    out << order_name <<endl;
    out.close();
    order_keys.push_back(order_name);
}

//pridani polozky do objednavky v ramci databaze vcetne specifikace mnozstvi
void Data_Handler::add_item_to_order(const string& item, const string& order_name, const Money& m) {
    ifstream MyReadFile(items_in_orders_file);
    string line;
    int j = 0;
    
    /*
     postupne prochazim soubor a hledam, zda je jiz item v objednavce obsazen
     data nemam ulozene i v operacni pameti, musim je najit v souboru narozdil od predchozich
    */
    while (getline (MyReadFile, line)) {
        
        //prazdne radky preskakuji
        if(line.empty()) {
            continue;
        }
        
        //naparsuji radku
        vector<string> data = Data_Handler::parse_csv_line(line);
        
        //pokud neobsahuje 3 hodnoty, je s daty neco spatne
        if(data.size() != 3) {
            cout << "Problem with data" << endl;
            return;
        }
        
        //pokud objednavka i item sedi, nasel jsem spravnou radku
        if(data[0] == order_name && data[1] == item) {
            
            //spocitam nove mnozstvi polozky v objednavce
            Money m1 = Money(data[2]) + m;
            
            //smazu odpovidajici radku
            delete_nth_line(items_in_orders_file, j);
            
            //pokud je nova hodnota > 0, zaznamenam ji do databaze
            if(m1 > 0) {
                ofstream items(items_in_orders_file, fstream::app);
                items << order_name << "," << item << "," << m1 <<endl;
                items.close();
            }
            return;
        }
        j++;
    }
    
    //jinak jsem udaj v databazi nenasel, proste jej zaznamenam na novy radek
    ofstream items(items_in_orders_file, fstream::app);
    items << order_name << "," << item << "," << m << endl;
    items.close();

}

//smazani objednavky
void Data_Handler::delete_order(const string& order_name) {
    
    //nejrpve musim smazat vsechny itemy z objednavky, zkousim vsechny, ty co tam nejsou jsou ignorovany
    for (size_t i = 0; i < item_keys.size(); ++i) {
        delete_item_from_order(item_keys[i], order_name);
    }
    
    //nasledne najdu index objednavky
    for (size_t i = 0; i < order_keys.size(); ++i) {
        if(order_keys[i] != order_name) {
            continue;
        }
        //objednavku smazu z databaze
        delete_nth_line(orders_file, (int)i);
        order_keys.erase(order_keys.begin() + (int)i);
    }
}


//funkce na smazani itemu z objednavky nehlede na mnozstvi
void Data_Handler::delete_item_from_order(const string& item,const string& order_name) {
    ifstream MyReadFile(items_in_orders_file);
    std::string line;
    int j = 0;
    while (getline (MyReadFile, line)) {
        if(line.empty()) {
            continue;
        }
        vector<string> data = Data_Handler::parse_csv_line(line);
        if(data.size() != 3) {
            cout << "Problem with data" << endl;
            return;
        }
        
        //az najdu radku s itemem a objednavkou, proste ji smazu
        if(data[0] == order_name && data[1] == item) {
            delete_nth_line(items_in_orders_file, j);
            return;
        }
        j++;
    }
}

/*pomocna funkce, ktera dostane filename a int n, ktere rikaji, ze v souboru pojmenovanem filename mam
 smazat n-tou neprazdnou radku
 */
void Data_Handler::delete_nth_line(const string& filename, int n) {
    
    //musim to resit tak, ze cely soubor krome n-te neprazdne radky prepisuji do noveho, ktery pak prejmenuji
    ifstream MyReadFile(filename);
    std::ofstream temp(path + "Temp.txt");
    std::string line;
    
    size_t j = 0;
        
    while (getline (MyReadFile, line)) {
        
        //prazdne radky me nezajimaji
        if(line.empty()) {
            continue;
        }
        
        //pokudj nejsem na n-te radce, prepisuji ji do noveho souboru
        if((int)j != n) {
            temp << line << '\n';
        }
        j++;
    }
    //streamy uzavru, smazu stary soubor, novy prejmenuji
    MyReadFile.close();
    temp.close();
    remove(filename.c_str());
    rename((path + "Temp.txt").c_str(), filename.c_str());
}

//vrati soucasne datum a cas ve formatu string
string Data_Handler::get_time_stamp() {

    time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    std::stringstream ss;
    ss << ctime(&now);
    string time = ss.str();
    time.erase(remove_if(time.begin(), time.end(), ::isspace), time.end());
    return time;
}

void Data_Handler::save_order(Order &order_to_save) {
    
    //nejprve vytvorim jmeno objednavky - potrebuji rozlisit pripady, kdy objednavka ma/nema jmeno
    string order_name = "";
    
    if(order_to_save.order_name().empty()) {
        order_name = "NoName";
    }
    else {
        order_name = order_to_save.order_name();
    }
    
    //nasledne si reknu o datum a cas
    string time = get_time_stamp();

    string save_name = path_to_orders +  order_name + "-" + time + ".txt";
    
    /*
     tady resim extremni pripad, ktery nejspis ani nemuze nastat ale pro jistotu
     get_time_stamp vraci konkretni datum, hodinu i vterinu
     teoreticky by se mohlo stat, ze by uzivatel zprocesoval 2 objednavky stejneho jmena ve stejnou vterinu
     v takovem pripade budu cekat pomoci while, az se vterina preklopi a nasledne se objednavka smaze
     */
    while(std::filesystem::exists(save_name)) {
        time = get_time_stamp();
        save_name = path_to_orders + order_name + "-" + time + ".txt";
    }
    
    std::ofstream new_file (save_name);
    
    new_file << order_to_save << endl;
    
}



