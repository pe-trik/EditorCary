#ifndef SPOJENIESLOT_H
#define SPOJENIESLOT_H

#include "Dokumenty/qrealvlastnost.h"
#include "komponent.h"
#include <functional>

namespace Komponenty {
class Komponent;
class SpojenieSlot {
public:
  SpojenieSlot(Komponent *komponent, Komponent *manipulator,
               std::function<QPointF()> smer);

  bool JeVolny() const { return _spojenie.use_count() == 0; }

  void NastavSpojenie(std::shared_ptr<Komponent> spojenie);

  void ZrusSpojenie();

  bool Obsahuje(QPointF bod) const;

  QPointF Smer() const { return _smer(); }

private:
  std::weak_ptr<Komponent> _spojenie;
  Komponent *_komponent;
  Komponent *_manipulator;
  std::function<QPointF()> _smer;
};
using SpojenieSlotPtr = std::unique_ptr<SpojenieSlot>;
}

#endif // SPOJENIESLOT_H
