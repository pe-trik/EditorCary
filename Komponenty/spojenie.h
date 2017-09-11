#ifndef SPOJENIE_H
#define SPOJENIE_H

#include "Nastroje/nastroj.h"
#include "komponent.h"
#include "manipulator.h"
#include "Dokumenty\qrealvlastnost.h"
#include "Dokumenty/spojeniezoznamvlastnost.h"

namespace Komponenty {
class Komponent;
class Spojenie : public Komponent {
public:
  Spojenie();
  void Vykresli(QPainter &painter) const { _manipulator->Vykresli(painter); }

  Nastroje::NastrojPtr Nastroj(Dokumenty::Dokument *dokument) {
    return _manipulator->Nastroj(dokument);
  }

  bool Obsahuje(QPointF bod) const { return _manipulator->Obsahuje(bod); }

  void PridajKomponent(Komponent *komponent, Komponent *manipulator);

  void OdstranKomponent(Komponent *komponent, Komponent *manipulator);

private:
	void obnovHodnoty();
	Komponent* _manipulator;
	Dokumenty::QrealVlastnostPtr _x;
	Dokumenty::QrealVlastnostPtr _y;
  std::vector<Komponent *> _komponenty;
  std::vector<Komponent *> _manipulatoryKomponent;
  Dokumenty::SpojenieZoznamVlastnostPtr _spojenieZoznamVlastnost;
};
}

#endif // SPOJENIE_H
