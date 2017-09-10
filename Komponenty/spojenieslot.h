#ifndef SPOJENIESLOT_H
#define SPOJENIESLOT_H

#include "komponent.h"

namespace Komponenty {
class Komponent;
class SpojenieSlot {
public:
  SpojenieSlot(Komponent *komponent, Komponent *manipulator);

  bool JeVolny() const { return _spojenie.use_count() == 0; }

  void NastavSpojenie(std::shared_ptr<Komponent> spojenie);

  void ZrusSpojenie();

  bool Obsahuje(QPointF bod) const;

private:
  std::weak_ptr<Komponent> _spojenie;
  Komponent *_komponent;
  Komponent *_manipulator;
};
using SpojenieSlotPtr = std::unique_ptr<SpojenieSlot>;
}

#endif // SPOJENIESLOT_H
