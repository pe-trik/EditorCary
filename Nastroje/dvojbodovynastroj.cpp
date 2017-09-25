#include "dvojbodovynastroj.h"
#include "Komponenty/manipulator.h"

using namespace Nastroje;

DvojbodovyNastroj::DvojbodovyNastroj(Dokumenty::Dokument *dokument) :Nastroj(dokument)
{

}

void Nastroje::DvojbodovyNastroj::MysStlacena(QPointF bod)
{
    _polohaMysi = bod;
    _mysStlacena = true;

    auto k = _dokument->NajdiKomponentPodBodom(bod);

    if(auto s = dynamic_cast<Komponenty::Spojenie*>(k))
    {
        if(k->Manipulatory().size() > 0)
            k = s->Manipulatory().at(0).get();
    }

    if(auto m = dynamic_cast<Komponenty::Manipulator*>(k))
    {
        _manipulator = m;
        k = m->Vlastnik();
    }

    if (auto komponent = otestujTyp(k)) {

        auto&& manipulator = std::find_if(komponent->Manipulatory().begin(),
                                          komponent->Manipulatory().end(),
                                          [bod](auto && m) { return m->Obsahuje(bod); });

        if (manipulator != komponent->Manipulatory().end())
            _manipulator = dynamic_cast<Komponenty::Manipulator*>((*manipulator).get());
        else
            _manipulator = dynamic_cast<Komponenty::Manipulator*>(komponent);;

        _komponent = otestujTyp(komponent);
    }
}

void Nastroje::DvojbodovyNastroj::MysPohyb(QPointF bod)
{
    if (_mysStlacena)
    {
        if (_manipulator)
            _manipulator->setBod(bod);

        else if(_komponent)
        {
            for (auto&& m : _komponent->Manipulatory())
            {
                auto manipulator = dynamic_cast<Komponenty::Manipulator*>(m.get());
                manipulator->setBod(manipulator->Bod() + bod - _polohaMysi);
            }
        }
    }
    _polohaMysi = bod;
}

void Nastroje::DvojbodovyNastroj::MysUvolnena(QPointF)
{
    _mysStlacena = false;
    _manipulator = nullptr;
    _komponent = nullptr;
}

void Nastroje::DvojbodovyNastroj::MysDvojklik(QPointF bod)
{
    vytvorNovyKomponent(bod);
    _mysStlacena = true;
    _manipulator = dynamic_cast<Komponenty::Manipulator*>(_komponent->Manipulatory().at(1).get());
}

Komponenty::Komponent *DvojbodovyNastroj::VybranyKomponent() const{
    if(_komponent)
        return _komponent;
    else
        return _manipulator;
}
