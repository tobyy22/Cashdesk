//
//  Identificator.hpp
//  Pokladna
//
//  Created by Tobias Vavroch on 16/12/2021.
//

#include <string>
#include <stdint.h>

/*
 Trida obstaravajici vytvareni id pro objednavky, u kterych nebylo specifikovano jmeno.
 Diky omezenim pokladny staci typ int.
 Nutne rozmyslet potencialni problem, kdy tato trida vygeneruje nekolik id, nasledne se program ukonci a pri opetovnem spusteni zacne opet od nuly
 -> bude tedy generovat jiz pouzita id.
 V soucasne chvili si to resi pokladna sama, ktera si rekne o nove id, zkusi objednavku pridat, zjisti, ze nemuze, protoze jiz existuje, rekne si tedy o nove id
 a ceka dokud neuspeje.
 */

class ID {
private:
    int id = 0;
public:
    ID(){};
    std::string get_unique() {
        id = id + 1;
        return std::to_string(id);
    }
};
