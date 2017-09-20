#include "volnaciaranastroj.h"

#include <Komponenty/volnaciara.h>

using namespace Nastroje;

VolnaCiaraNastroj::VolnaCiaraNastroj(Dokumenty::Dokument *dokument) :Nastroj(dokument)
{

}

NastrojPresenterPtr VolnaCiaraNastroj::NastrojPresenter() const {
    return std::make_unique<VolnaCiaraPresenter>();
}

void Nastroje::VolnaCiaraNastroj::MysStlacena(QPointF bod)
{
    _polohaMysi = bod;
    _mysStlacena = true;

    auto&& komponent = _dokument->Komponent(bod);

    if (komponent) {

        auto&& manipulator = std::find_if(komponent->Manipulatory().begin(),
            komponent->Manipulatory().end(),
            [bod](auto && m) { return m->Obsahuje(bod); });

        if (manipulator != komponent->Manipulatory().end())
            _manipulator = dynamic_cast<Komponenty::Manipulator*>((*manipulator).get());
        else
            _manipulator = dynamic_cast<Komponenty::Manipulator*>(komponent);

        _komponent = dynamic_cast<Komponenty::VolnaCiara*>(komponent);
    }
}

void Nastroje::VolnaCiaraNastroj::MysPohyb(QPointF bod)
{
    if (_mysStlacena)
    {
        if(_manipulator && _komponent && _novy)
        {
            _komponent->body().push_back(bod);
            _komponent->prepocitaj();
            _manipulator->setBod(bod);
        }
        else if(_manipulator)
            _manipulator->setBod(bod);
        else if(_komponent)
        {
            auto& body = _komponent->body();
            for(auto& b : body)
                b += bod - _polohaMysi;
            for(auto& m : _komponent->Manipulatory())
            {
                if(auto manipulator = dynamic_cast<Komponenty::Manipulator*>(m.get()))
                    manipulator->setBod(manipulator->getBod() + bod - _polohaMysi);
            }
            _komponent->prepocitaj();
        }
    }
    _polohaMysi = bod;
}

void Nastroje::VolnaCiaraNastroj::MysUvolnena(QPointF)
{
    _mysStlacena = false;
    _manipulator = nullptr;
    _komponent = nullptr;
    _novy = false;
}

void Nastroje::VolnaCiaraNastroj::MysDvojklik(QPointF bod)
{
    _novy = true;
    auto ciara = std::make_unique<Komponenty::VolnaCiara>();
    _komponent = ciara.get();
    _dokument->PridajKomponent(std::move(ciara));
    for (auto &&m : _komponent->Manipulatory())
      dynamic_cast<Komponenty::Manipulator *>(m.get())->setBod(bod);
    _manipulator =
            dynamic_cast<Komponenty::Manipulator *>(_komponent->Manipulatory().at(1).get());
    _mysStlacena = true;
}
