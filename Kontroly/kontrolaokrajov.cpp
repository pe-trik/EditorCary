#include "kontrolaokrajov.h"
#include "algorithm"

using namespace Kontroly;


KontrolaOkrajov::KontrolaOkrajov(Dokumenty::Dokument *dokument)
    :Kontrola(dokument)
{
    _nazov = "Kontrola okrajov";
}

std::vector<Chyba> KontrolaOkrajov::VykonajKontrolu()
{
    std::vector<Chyba> chyby;
    setVysledok(true);

    qreal okraj = _dokument->Okraj();
    QRectF vnutro(QPointF(okraj, okraj),
                  QPointF(_dokument->sirka() - okraj, _dokument->vyska() - okraj));

    if(_povolena){
        for(auto& k : _dokument->Komponenty())
        {
            auto body = k->BodyKomponentu();
            if(std::any_of(body.begin(), body.end(), [&vnutro](QPointF b){
                           return !vnutro.contains(b);
        }))
            {
                setVysledok(false);
                chyby.push_back(Chyba(this, "komponent prekračuje okraj", k));
            }
        }
    }

    return chyby;
}
