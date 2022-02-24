//
//  Command.hpp
//  Pokladna
//
//  Created by Tobias Vavroch on 09/11/2021.
//

#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include "CashDesk.hpp"
#include "Data.hpp"

#include "lib/decimal.h"

/*
 Rozhrani pro uivatelsky vstup. Command je abstraktni predek pro ostatni commandy.
 */

//zde jsou ulozene zkratky pro jednotlive prikazy
const vector<string> acros_item = {"item", "it", "items"};
const vector<string> acros_order = {"orders", "order", "or"};
const vector<string> acros_from = {"from", "fr"};
const vector<string> acros_process = {"process", "pro"};
const vector<string> acros_cash = {"cash", "ca"};

const vector<string> show_acros = {"show", "sh"};
const vector<string> end_acros = {"end"};
const vector<string> add_acros = {"add", "a"};
const vector<string> remove_acros = {"remove", "rem"};
const vector<string> process_acros = {"process", "pro"};

class Command {
private:
public:
    
    /* Mam celkem 2 konstruktory, jeden prazdny, ktery se pouziva v pripade neznamych a prazdneho commandu. Temto commandum nedava smysl cokoliv predavat.*/
    Command(){};
    
    /* Tento konstruktor prijima shared pointer na kasu, na ktere nasledne po naparsovani spravneho commandu zavola odpovidajici funkci.
       Druhy parametr je tokenizovany command. Trida se nasledne stara o rozpoznani parametru commandu a zavolani spravne funkce kasy.*/
    Command(CashDesk& given, const vector<string>& tokenized_command){
        tokenized = tokenized_command;
        kasa = &given;
    };
    virtual ~Command(){};
    
    /* Funkce, ktera vytvari spravny command. Naparsuje input od uzivatele pomoci nasledujici funkce a podle prvniho tokenu pozna, o ktery command se jedna.
     Nasledne vytvori jeho instanci a vrati ji.*/
    static unique_ptr<Command> correct_command(const string& input, CashDesk& kasa);
    
    /* Funkce dostane string a podle whitespaces ho naparsuje*/
    static vector<string> tokenize_command(const string& command);
    
    //funkce overi, zda dec je cislo v decimalnim formatu
    static bool is_decimal(const string& dec);

    //command si vytvori vlastni pointer na kasu, coz je zpusob, jak k ni pristupuje
    observer_ptr<CashDesk> kasa;
    
    //tokenized je naparsovany prikaz, ktery commandy analyzuji a podle toho volaji funkce kasy
    std::vector<std::string> tokenized;
    
    /* Vsechny commandy nasledne implementuji virtualni funkci process_command*/
    virtual void process_command() = 0;
};

//prazdny prikaz
class Empty : public Command {
public:
    Empty(){}
    virtual ~Empty(){}
    
    void process_command(){
        cout << "Empty command." << endl;
    };
};

//neznamy prikaz
class Unknown : public Command {
public:
    Unknown(){}
    virtual ~Unknown(){}
    
    void process_command(){
        cout << "Uknkown command." << endl;
    }
};

//prikaz pro zobrazeni informaci o pokladne
class Show : public Command {
private:
    //vypsani itemu a objednavek, implementace v Show.cpp
    void show_items();
    void show_orders();
    void show_help();
public:
    Show(CashDesk& kasa, const vector<string>& tokenized_command) : Command(kasa, tokenized_command){};
    virtual ~Show(){};
    
    void process_command();
};

//pridavani dat do pokladny
class Add : public Command {
private:
    //pridavani itemu, objednavek, itemu do objednavek, nastavovani hotovosti, cteni itemu ze vstupu
    void add_items();
    void add_order();
    void add_to();
    void add_cash();
    void read_order_items();

public:
    Add(CashDesk& kasa, const vector<string>& tokenized_command): Command(kasa, tokenized_command){};
    virtual ~Add(){};

    void process_command();

};

//odebirani dat z pokladny
class Remove : public Command {
private:
    void remove_item();
    void remove_order();
public:
    Remove(CashDesk& kasa, const vector<string>& tokenized_command): Command(kasa, tokenized_command){};
    virtual ~Remove(){};
    
    void process_command();
};


//procesovani objendnavek
class Process : public Command {
private:

public:
    Process(CashDesk& kasa, const vector<string>& tokenized_command): Command(kasa, tokenized_command){};
    virtual ~Process(){}
    //implementuje jedinou funkci, pro prehlednost implementovano jiz zde
    void process_command() {
        /*chci ukazat procesovanou objednavku, pokud neexistuje, nic procesovat nebudu
          kasa by sice objednavku odmitla, ale UI by se nechovalo idealne, lepsi hned odchytit*/
        if(!kasa->show_order(tokenized[1])) {
            return;
        }
        
        string amount;
        cout << "Paid: ";
        
        //pokusim se nacist placenou castku
        while (getline(cin, amount)) {
            //input si tokenizuji
            vector<string> test = Command::tokenize_command(amount);
            //pokud je prazdnu, je to zpusob jak procesovani objednavky zrusit
            if(amount.empty()) {
                cout << "Process cancelled." << endl;
                return;
            }
            //pokud mam vice nez jeden token(castku) nebo tento token neni v decimalnim formatu, odmitam a nacitam znovu
            if((test.size() > 1) | (!Command::is_decimal(test[0]))) {
                cout << "Invalid amount" << endl;
                continue;
            }
            //procesuji objednavku
            if(!kasa->process_order(tokenized[1], amount)) {
                cout << "Paid: ";
                continue;
            }
            return;
        }
    }
};

class End : public Command {
private:
public:
    End(){};
    virtual ~End(){};
    void process_command() {
        //ukonceni programu
        exit(0);
    };
};
