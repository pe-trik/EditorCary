#include "spojenieslot.h"
#include "spojenie.h"
#include "manipulator.h"
using namespace Komponenty;

void SpojenieSlot::NastavSpojenie(Komponent *spojenie) {
    if(JeVolny())
    {
        _spojenie = spojenie;
        if (auto s = dynamic_cast<Komponenty::Spojenie *>(spojenie))
            s->PridajKomponent(this);
    }
}

SpojenieSlot::SpojenieSlot(Komponent *komponent, Komponent *manipulator, Dokumenty::QrealVlastnost *nasobok,
                           std::function<QPointF()> smer)
    : _komponent(komponent), _manipulator(manipulator), _smer(smer), _nasobok(nasobok) {}

bool SpojenieSlot::JeVolny() const { return _spojenie == nullptr; }

void SpojenieSlot::ZrusSpojenie() {
    auto sp = _spojenie; //kopirujeme - inak by sa vytvoril cyklus
    _spojenie = nullptr;
    if (auto s = dynamic_cast<Komponenty::Spojenie *>(sp))
        s->OdstranKomponent(this);
}

bool SpojenieSlot::Obsahuje(QPointF bod) const {
    return _manipulator->Obsahuje(bod);
}

QDomElement SpojenieSlot::Uloz(QDomDocument &doc)
{
    auto v = doc.createElement("spojenieSlot");
    v.setAttribute("komponent", _komponent->NazovKomponentu());
    v.setAttribute("manipulator", _manipulator->NazovKomponentu());
    return v;
}

QPointF SpojenieSlot::Smer() const { return _nasobok->Hodnota() * _smer(); }

Komponent *SpojenieSlot::Manipulator()
{
    return _manipulator;
}

Komponent *SpojenieSlot::Vlastnik() const
{
    return _komponent;
}

Komponent *SpojenieSlot::Spojenie() const
{
    return _spojenie;
}

QPointF SpojenieSlot::Bod() const
{
    return dynamic_cast<Komponenty::Manipulator*>(_manipulator)->Bod();
}

void SpojenieSlot::setSmer(const std::function<QPointF ()> &smer)
{
    _smer = smer;
}
