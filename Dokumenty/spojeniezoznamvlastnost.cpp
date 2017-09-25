#include "spojeniezoznamvlastnost.h"
#include "spojeniezoznamvlastnostmanager.h"

using namespace Dokumenty;

SpojenieZoznamVlastnost::SpojenieZoznamVlastnost(
        QString nazov, Komponenty::Komponent *spojenie)
    : Vlastnost(nazov), _spojenie(spojenie) {}

VlastnostManagerPtr SpojenieZoznamVlastnost::NastrojVlastnosti() {
    return std::make_unique<SpojenieZoznamVlastnostManager>(this);
}

std::vector<Komponenty::SpojenieSlot *> SpojenieZoznamVlastnost::Hodnota() const {
    return _hodnota;
}

void SpojenieZoznamVlastnost::setHodnota(
        std::vector<Komponenty::SpojenieSlot *> hodnota) {
    if(_hodnota != hodnota)
    {
        _hodnota = hodnota;
        emit hodnotaZmenena(hodnota);
        emit VlastnostZmenena();
    }
}

void SpojenieZoznamVlastnost::obnovVlastnost(QDomElement &)
{

}

Komponenty::Komponent *SpojenieZoznamVlastnost::Spojenie() { return _spojenie; }

QDomElement SpojenieZoznamVlastnost::UlozVlastnost(QDomDocument &doc)
{
    auto v = doc.createElement("vlastnost");
    v.setAttribute("nazov", _nazov);
    for(auto& slot : _hodnota)
        v.appendChild(slot->Uloz(doc));
    return v;
}

void SpojenieZoznamVlastnost::ObnovVlastnost(QDomNodeList l, Dokument *dokument)
{
    auto slot = l.at(0).toElement();
    while(!slot.isNull()){
        for(auto& e : dokument->Komponenty())
        {
            if(e->NazovKomponentu() == slot.attribute("komponent")){
                for(auto& s : e->SpojenieSloty()){
                    if(s->Manipulator()->NazovKomponentu() == slot.attribute("manipulator"))
                        s->NastavSpojenie(_spojenie);
                }
            }
        }
        slot = slot.nextSiblingElement();
    }
}
