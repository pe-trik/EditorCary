#include "kontrolaspojeni.h"

#include <Komponenty/spline.h>
#include <Komponenty/spojenie.h>
#include <Komponenty/spojenieslot.h>

using namespace Kontroly;

KontrolaSpojeni::KontrolaSpojeni(Dokumenty::Dokument *dokument)
    :Kontrola(dokument)
{
    _nazov = "Kontrola spojení";
}

std::vector<Chyba> KontrolaSpojeni::VykonajKontrolu()
{
    setVysledok(true);
    std::vector<Chyba> chyby;

    if(_povolena){
        for(auto& s : _dokument->spojenia()){
            if(auto spojenie = dynamic_cast<Komponenty::Spojenie*>(s)){
                int ps = 0;
                for(auto k : spojenie->SlotySpojenia()){
                    if(dynamic_cast<Komponenty::Spline*>(k->komponent()))
                        ++ps;
                }
                if(static_cast<int>(spojenie->SlotySpojenia().size()) - ps > 1)
                {
                    chyby.push_back(Chyba(this, "spojenie obsahuje viaceré komponenty s neupraviteľným smerom", spojenie));
                    setVysledok(false);
                }
            }
        }
    }

    return chyby;
}
