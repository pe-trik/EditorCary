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
            for(int i = 1, total = body.size(); i < total; ++i){
                auto r = body.at(i) - body.at(i - 1);
                dlzka += sqrt(r.x()*r.x() + r.y()*r.y());
            }
        }

        if(dlzka > _dokument->MaximalnaDlzkaTrate() || dlzka < _dokument->MinimalnaDlzkaTrate())
        {
            setVysledok(false);
            return { Chyba(this,"trať má dĺžku " + QString::number(dlzka) + " mm") };
        }

    }

    return {};
}
