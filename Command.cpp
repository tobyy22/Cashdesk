//
//  Command.cpp
//  Pokladna
//
//  Created by Tobias Vavroch on 09/11/2021.
//

#include <iostream>

#include "Command.hpp"

/* Funkce, ktera tokenizuje input do vektoru, podle prvniho tokenu rozpozna prikaz a podle toho vytvori spravny prikaz,
 kteremu preda tokeny a pointer na kasu(parametry), na ktere bude prikaz provadet operace.
 Vytvoreny prikaz vraci.
 Funkce se vzdy porovnava prvni prikaz se vsemi moznymi zkratkami tohoto prikazu.*/
unique_ptr<Command> Command::correct_command(const string& input, CashDesk& kasa) {
    
    //tokenizace inputu
    vector<string> tokenized = tokenize_command(input);
    
    if(tokenized.empty()) {
        unique_ptr<Command> command_ptr (new Empty());
        return command_ptr;
    }
    if(std::count(show_acros.begin(), show_acros.end(), tokenized[0])) {
        unique_ptr<Command> command_ptr (new Show(kasa,tokenized));
        return command_ptr;
    }
    if(std::count(add_acros.begin(), add_acros.end(), tokenized[0])) {
        unique_ptr<Command> command_ptr (new Add(kasa,tokenized));
        return command_ptr;
    }
    if(std::count(remove_acros.begin(), remove_acros.end(), tokenized[0])) {
        unique_ptr<Command> command_ptr (new Remove(kasa,tokenized));
        return command_ptr;
    }
    if(std::count(process_acros.begin(), process_acros.end(), tokenized[0])) {
        unique_ptr<Command> command_ptr (new Process(kasa,tokenized));
        return command_ptr;
    }
    if(std::count(end_acros.begin(), end_acros.end(), tokenized[0])) {
        unique_ptr<End> command_ptr (new End());
        return command_ptr;
    }
    unique_ptr<Command> command_ptr (new Unknown());
    return command_ptr;
}

/*funkce parsujici string na tokeny
  postupne nacita vstupni string, pri space pridava token a zacina nacitat novy token */
vector<string> Command::tokenize_command(const string& command) {
    vector<string> tokenized;
    std::string token = "";
    for (auto ch: command) {
        if(isspace(ch)) {
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

//funkce overujici, zda je dec v decimalnim formatu
bool Command::is_decimal(const string& dec) {
    if(dec == "-") {
        return false;
    }
    bool is_dec = true;
    size_t i = 0;
    if(dec[0] == '-') {
        i = 1;
    }
    while (i < dec.size() && isdigit(dec[i])) {
        i++;
    }
    if(i == dec.size()) {
        return is_dec;
    }
    if(dec[i] == '.') {
        i++;
    }
    else {
        return false;
    }
    while (i < dec.size() && isdigit(dec[i])) {
        i++;
    }
    if(i == dec.size()) {
        return is_dec;
    }
    return false;
}













