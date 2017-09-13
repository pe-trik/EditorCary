#ifndef SPOJENIESLOT_H
#define SPOJENIESLOT_H

#include "Dokumenty/qrealvlastnost.h"
#include <functional>
#include "komponent.h"

namespace Komponenty {
class Komponent;
class SpojenieSlot {
public:
  SpojenieSlot(Komponent *komponent, Komponent *manipulator,
               std::function<QPointF()> smer);

  bool JeVolny() const { return _spojenie == nullptr; }

  void NastavSpojenie(Komponent* spojenie);

  void ZrusSpojenie();

  bool Obsahuje(QPointF bod) const;

  QPointF Smer() const { return _smer(); }

  Komponent *manipulator();

  Komponent *komponent() const;

  Komponent *spojenie() const;

  QPointF bod() const;

  void setSmer(const std::function<QPointF ()> &smer);

private:
  Komponent* _spojenie = nullptr;
  Komponent *_komponent;
  Komponent *_manipulator;
  std::function<QPointF()> _smer;
};
using SpojenieSlotPtr = std::unique_ptr<SpojenieSlot>;
}

#endif // SPOJENIESLOT_H
