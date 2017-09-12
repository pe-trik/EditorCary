#include "spojenieslot.h"
#include "spojenie.h"
using namespace Komponenty;

void SpojenieSlot::NastavSpojenie(Komponent *spojenie) {
    if(JeVolny())
    {
        _spojenie = spojenie;
        if (auto s = dynamic_cast<Spojenie *>(spojenie))
            s->PridajKomponent(this);
    }
}

SpojenieSlot::SpojenieSlot(Komponent *komponent, Komponent *manipulator,
                           std::function<QPointF()> smer)
    : _komponent(komponent), _manipulator(manipulator), _smer(smer) {}

void SpojenieSlot::ZrusSpojenie() {
    _spojenie = nullptr;
}

bool SpojenieSlot::Obsahuje(QPointF bod) const {
    return _manipulator->Obsahuje(bod);
}

Komponent *SpojenieSlot::manipulator()
{
    return _manipulator;
}

Komponent *SpojenieSlot::komponent() const
{
    return _komponent;
}
