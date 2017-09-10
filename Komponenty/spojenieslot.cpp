#include "spojenieslot.h"
#include "spojenie.h"
using namespace Komponenty;

void SpojenieSlot::NastavSpojenie(std::shared_ptr<Komponent> spojenie) {
  _spojenie = spojenie;
  if (auto s = dynamic_cast<Spojenie *>(spojenie.get()))
    s->PridajKomponent(_komponent, _manipulator);
}

SpojenieSlot::SpojenieSlot(Komponent *komponent, Komponent *manipulator)
    : _komponent(komponent), _manipulator(manipulator) {}

void SpojenieSlot::ZrusSpojenie() {
  if (auto spojenie = _spojenie.lock()) {
    if (auto s = dynamic_cast<Spojenie *>(spojenie.get()))
      s->OdstranKomponent(_komponent, _manipulator);
  }
  _spojenie.reset();
}

bool SpojenieSlot::Obsahuje(QPointF bod) const {
  return _manipulator->Obsahuje(bod);
}
