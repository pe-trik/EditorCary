#ifndef SPOJENIE_H
#define SPOJENIE_H

#include "Dokumenty/spojeniezoznamvlastnost.h"
#include "Dokumenty/qrealvlastnost.h"
#include "Nastroje/nastroj.h"
#include "komponent.h"

namespace Komponenty {
class Komponent;
class Spojenie : public Komponent {
    friend class SplineGroup;
public:
  Spojenie();
  void Vykresli(QPainter &painter) const { _manipulator->Vykresli(painter); }

  Nastroje::NastrojPtr Nastroj(Dokumenty::Dokument *dokument) {
    return _manipulator->Nastroj(dokument);
  }

  bool Obsahuje(QPointF bod) const { return _manipulator->Obsahuje(bod); }

  void PridajKomponent(SpojenieSlot *slot);

  void OdstranKomponent(SpojenieSlot *slot);

  bool JePrazdne() const {
      return _spojenieZoznamVlastnost->hodnota().size() <= 1;
  }

  QString Typ() const{
      return "Spojenie";
  }
  QDomElement Uloz(QDomDocument &doc) const;

private:
  void obnovHodnoty();
  Komponent *_manipulator;
  Dokumenty::QrealVlastnostPtr _x;
  Dokumenty::QrealVlastnostPtr _y;
  Dokumenty::SpojenieZoznamVlastnostPtr _spojenieZoznamVlastnost;
};
}

#endif // SPOJENIE_H
