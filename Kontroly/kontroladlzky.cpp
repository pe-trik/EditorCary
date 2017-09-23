#include "kontroladlzky.h"

using namespace Kontroly;

KontrolaDlzky::KontrolaDlzky(Dokumenty::Dokument *dokument)
    :Kontrola(dokument)
{
    _nazov = "Kontrola dĺžky";
}

std::vector<Chyba> KontrolaDlzky::VykonajKontrolu()
{
    setVysledok(true);

    qreal dlzka = 0;

    if(_povolena){
        for(auto& k : _dokument->Komponenty())
        {
            auto body = k->BodyKomponentu();
            for(size_t i = 1; i < body.size(); i++){
                auto r = body.at(i) - body.at(i - 1);
                dlzka += sqrt(r.x()*r.x() + r.y()*r.y());
            }
        }

        if(dlzka > _dokument->maximalnaDlzkaTrate() || dlzka < _dokument->minimalnaDlzkaTrate())
            return {Chyba(this,"Trať má dĺžku " + QString::number(dlzka) + " mm")};

    }

    return {};
}
