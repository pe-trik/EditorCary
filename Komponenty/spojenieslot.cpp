#include "spojenieslot.h"
#include "spojenie.h"
#include "manipulator.h"
using namespace Komponenty;

void SpojenieSlot::NastavSpojenie(Komponent *spojenie) {
    if(JeVolny())
    {
        _spojenie = spojenie;
        if (auto s = dynamic_cast<Spojenie *>(spojenie))
            s->PridajKomponent(this);
    }
}

SpojenieSlot::SpojenieSlot(Komponent *komponent, Komponent *manipulator, Dokumenty::QrealVlastnost *nasobok,
                           std::function<QPointF()> smer)
    : _komponent(komponent), _manipulator(manipulator), _smer(smer), _nasobok(nasobok) {}

void SpojenieSlot::ZrusSpojenie() {
    auto sp = _spojenie;
    _spojenie = nullptr;
    if (auto s = dynamic_cast<Spojenie *>(sp))
        s->OdstranKomponent(this);
}

bool SpojenieSlot::Obsahuje(QPointF bod) const {
    return _manipulator->Obsahuje(bod);
}

QDomElement SpojenieSlot::Uloz(QDomDocument &doc)
{
    auto v = doc.createElement("spojenieSlot");
    v.setAttribute("komponent", _komponent->nazov());
    v.setAttribute("manipulator", _manipulator->nazov());
    return v;
}

Komponent *SpojenieSlot::manipulator()
{
    return _manipulator;
}

Komponent *SpojenieSlot::komponent() const
{
    return _komponent;
}

Komponent *SpojenieSlot::spojenie() const
{
    return _spojenie;
}

QPointF SpojenieSlot::bod() const
{
    return dynamic_cast<Manipulator*>(_manipulator)->getBod();
}

void SpojenieSlot::setSmer(const std::function<QPointF ()> &smer)
{
    _smer = smer;
}
