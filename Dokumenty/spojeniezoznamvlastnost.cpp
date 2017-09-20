#include "spojeniezoznamvlastnost.h"
#include "spojeniezoznamvlastnostmanager.h"

using namespace Dokumenty;

SpojenieZoznamVlastnost::SpojenieZoznamVlastnost(
    QString nazov, Komponenty::Komponent *spojenie)
    : Vlastnost(nazov), _spojenie(spojenie) {}

SpojenieZoznamVlastnost::SpojenieZoznamVlastnost(QString nazov, Komponenty::Komponent *spojenie,
    std::vector<Komponenty::SpojenieSlot *> hodnota)
    : Vlastnost(nazov), _spojenie(spojenie), _hodnota(hodnota) {}

VlastnostManagerPtr SpojenieZoznamVlastnost::NastrojVlastnosti() {
  return std::make_unique<SpojenieZoznamVlastnostManager>(this);
}

std::vector<Komponenty::SpojenieSlot *> SpojenieZoznamVlastnost::hodnota() const {
  return _hodnota;
}

void SpojenieZoznamVlastnost::setHodnota(
    std::vector<Komponenty::SpojenieSlot *> hodnota) {
  _hodnota = hodnota;
}

Komponenty::Komponent *SpojenieZoznamVlastnost::spojenie() { return _spojenie; }

QDomElement SpojenieZoznamVlastnost::Uloz(QDomDocument &doc)
{
    auto v = doc.createElement("vlastnost");
    v.setAttribute("nazov", _nazov);
    for(auto& slot : _hodnota)
        v.appendChild(slot->Uloz(doc));
    return v;
}
