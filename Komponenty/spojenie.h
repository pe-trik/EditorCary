#ifndef SPOJENIE_H
#define SPOJENIE_H

#include "Dokumenty/spojeniezoznamvlastnost.h"
#include "Dokumenty\qrealvlastnost.h"
#include "Nastroje/nastroj.h"
#include "komponent.h"
#include "manipulator.h"

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

  void PridajKomponent(Komponent *manipulator);

  void OdstranKomponent(Komponent *manipulator);

private:
  void obnovHodnoty();
  Komponent *_manipulator;
  Dokumenty::QrealVlastnostPtr _x;
  Dokumenty::QrealVlastnostPtr _y;
  Dokumenty::SpojenieZoznamVlastnostPtr _spojenieZoznamVlastnost;
};
}

#endif // SPOJENIE_H
