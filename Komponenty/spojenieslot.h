#ifndef SPOJENIESLOT_H
#define SPOJENIESLOT_H

#include "Dokumenty/qrealvlastnost.h"
#include <functional>
#include "komponent.h"

namespace Komponenty {
class Komponent;

class SpojenieSlot {
public:
  SpojenieSlot(Komponent *Vlastnik,
               Komponent *Manipulator,
               Dokumenty::QrealVlastnost* nasobok,
               std::function<QPointF()> smer);

  bool JeVolny() const;

  void NastavSpojenie(Komponent* Spojenie);

  void ZrusSpojenie();

  bool Obsahuje(QPointF Bod) const;

  QDomElement Uloz(QDomDocument &doc);

  QPointF Smer() const;

  Komponent *Manipulator();

  Komponent *Vlastnik() const;

  Komponent *Spojenie() const;

  QPointF Bod() const;

  void setSmer(const std::function<QPointF ()> &smer);

private:
  Komponent* _spojenie = nullptr;
  Komponent *_komponent;
  Komponent *_manipulator;
  Dokumenty::QrealVlastnost* _nasobok;
  std::function<QPointF()> _smer;
};

using SpojenieSlotPtr = std::unique_ptr<SpojenieSlot>;
}

#endif // SPOJENIESLOT_H
