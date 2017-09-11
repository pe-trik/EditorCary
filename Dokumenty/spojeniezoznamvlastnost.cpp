#include "spojeniezoznamvlastnost.h"
#include "spojeniezoznamvlastnostmanager.h"

using namespace Dokumenty;

SpojenieZoznamVlastnost::SpojenieZoznamVlastnost(
    QString nazov, Komponenty::Komponent *spojenie)
    : Vlastnost(nazov), _spojenie(spojenie) {}

SpojenieZoznamVlastnost::SpojenieZoznamVlastnost(
    QString nazov, Komponenty::Komponent *spojenie,
    std::vector<Komponenty::Komponent *> hodnota)
    : Vlastnost(nazov), _spojenie(spojenie), _hodnota(hodnota) {}

VlastnostManagerPtr SpojenieZoznamVlastnost::NastrojVlastnosti() {
  return std::make_unique<SpojenieZoznamVlastnostManager>(this);
}

std::vector<Komponenty::Komponent *> SpojenieZoznamVlastnost::hodnota() const {
  return _hodnota;
}

void SpojenieZoznamVlastnost::setHodnota(
    std::vector<Komponenty::Komponent *> hodnota) {
  _hodnota = hodnota;
}

Komponenty::Komponent *SpojenieZoznamVlastnost::spojenie() { return _spojenie; }
