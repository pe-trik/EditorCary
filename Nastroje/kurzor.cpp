#include "kurzor.h"

using namespace Nastroje;

Kurzor::Kurzor(Dokumenty::Dokument *dokument) : Nastroj(dokument) {}

NastrojPresenterPtr Kurzor::NastrojPresenter() const {
    return std::make_unique<KurzorPresenter>();
}

void Kurzor::MysStlacena(QPointF bod) { 
    _polohaMysi = bod;
    _mysStlacena = true;
    _vybranyKomponent = nullptr;
    auto k = _dokument->NajdiKomponentPodBodom(bod);
    if (k) {
        for (auto& m : k->Manipulatory())
        {
            if (m->Obsahuje(bod))
                _vybranyKomponent = dynamic_cast<Komponenty::Manipulator*>( m.get());
        }
        if(!_vybranyKomponent)
            _vybranyKomponent = k;
    }
}

void Kurzor::MysPohyb(QPointF bod) {
    if (_vybranyKomponent)
    {
        //ak je vybrany komponent manipulator
        if(_vybranyKomponent->Manipulatory().size() == 0)
        {
            if(auto m = dynamic_cast<Komponenty::Manipulator*>(_vybranyKomponent))
                m->setBod(bod);
        }
        //ak je vybrany plnohodnotny komponent
        else
        {
            for (auto&& m : _vybranyKomponent->Manipulatory())
            {
                auto manipulator = dynamic_cast<Komponenty::Manipulator*>(m.get());
                manipulator->setBod(manipulator->Bod() + bod - _polohaMysi);
            }
        }
    }

    _polohaMysi = bod;
}

void Nastroje::Kurzor::MysUvolnena(QPointF) {
    _mysStlacena = false;
    _vybranyKomponent = nullptr;
}

Komponenty::Komponent *Kurzor::VybranyKomponent() const{
    return _vybranyKomponent;
}

NastrojPtr KurzorPresenter::Nastroj(Dokumenty::Dokument *dokument) const {
    return std::make_unique<Kurzor>(dokument);
}

Komponenty::KomponentPtr KurzorPresenter::Komponent() const
{
    return 0;
}
