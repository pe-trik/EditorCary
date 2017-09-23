#include "dokument.h"
#include "Komponenty/ciara.h"
#include "Komponenty/spojenieslot.h"
#include "Komponenty/spojenie.h"
#include "Komponenty/splinegroup.h"

#include <Nastroje/ciaranastroj.h>
#include <Nastroje/kurzor.h>
#include <Nastroje/prerusenienastroj.h>
#include <Nastroje/splinenastroj.h>
#include <Nastroje/volnaciaranastroj.h>

#include <Komponenty/prerusenie.h>

using namespace Dokumenty;

void Dokument::velkostDokumentuZmenena(qreal) { emit prekreslit(); }

Dokument::Dokument() {
    _sirka = std::make_unique<QrealVlastnost>("Šírka", 1189);
    _vyska = std::make_unique<QrealVlastnost>("Výška", 841);
    _okraj = std::make_unique<QrealVlastnost>("Okraj", 50);
    _nahlad = std::make_unique<BoolVlastnost>("Náhľad", false);
    _siet = std::make_unique<BoolVlastnost>("Sieť", true);

    connect(_sirka.get(), SIGNAL(hodnotaZmenena(qreal)), this,
            SLOT(velkostDokumentuZmenena(qreal)));
    connect(_vyska.get(), SIGNAL(hodnotaZmenena(qreal)), this,
            SLOT(velkostDokumentuZmenena(qreal)));
}

qreal Dokument::sirka() const { return _sirka->hodnota(); }

qreal Dokument::vyska() const { return _vyska->hodnota(); }

QDomDocument Dokument::Uloz()
{
    QDomDocument doc("draha");
    auto root = doc.createElement("draha");

    auto vlastnosti = doc.createElement("vlastnosti");
    for(auto& v : Vlastnosti())
        vlastnosti.appendChild(v->Uloz(doc));
    root.appendChild(vlastnosti);

    auto komponenty = doc.createElement("komponenty");
    for(auto& k : _komponenty)
        komponenty.appendChild(k->Uloz(doc));
    root.appendChild(komponenty);

    auto spojenia = doc.createElement("spojenia");
    for(auto& s : _spojenia)
        spojenia.appendChild(s->Uloz(doc));
    root.appendChild(spojenia);

    doc.appendChild(root);
    return doc;
}

void Dokument::Obnov(const QDomDocument &doc)
{
    _vybranyKomponent = nullptr;
    QDomNode blok = doc.childNodes().item(0).childNodes().at(0);
    while(!blok.isNull())
    {
        if(blok.nodeName() == "vlastnosti"){
            for(auto v : Vlastnosti())
                v->Obnov(blok.childNodes());
        }
        else if(blok.nodeName() == "komponenty"){
            obnovKomponenty(blok.childNodes());
        }
        else if(blok.nodeName() == "spojenia"){
            obnovSpojenia(blok.childNodes());
        }
        blok = blok.nextSibling();
    }
    Prepocitaj();
}

void Dokument::setSirka(qreal sirka) { _sirka->setHodnota(sirka); }

void Dokument::setVyska(qreal vyska) { _vyska->setHodnota(vyska); }

std::vector<Vlastnost *> Dokument::Vlastnosti() const {
    return {_sirka.get(), _vyska.get(), _okraj.get(), _nahlad.get(), _siet.get()};
}

void Dokument::Vykresli(QPainter &painter) {
    painter.fillRect(0, 0, _sirka->hodnota(), _vyska->hodnota(), Qt::white);

    if(!_nahlad->hodnota() && _siet->hodnota()){
        painter.setRenderHint(QPainter::Antialiasing);
        painter.fillRect(0, 0, _sirka->hodnota(), _vyska->hodnota(), Qt::lightGray);
        painter.fillRect(_okraj->hodnota(), _okraj->hodnota(),
                         _sirka->hodnota() - 2 * _okraj->hodnota(),
                         _vyska->hodnota() - 2 * _okraj->hodnota(), Qt::white);

        for(size_t i = 25; i < _sirka->hodnota(); i += 25)
        {
            if(i % 100 == 0)
                painter.setPen(QPen(QBrush(Qt::black), 1.5));
            else if(i % 50 == 0)
                painter.setPen(QPen(QBrush(Qt::black), 1));
            else
                painter.setPen(QPen(QBrush(Qt::black), 0.5));
            painter.drawLine(i,0,i,_vyska->hodnota());
        }

        for(size_t i = 25; i < _vyska->hodnota(); i += 25)
        {
            if(i % 100 == 0)
                painter.setPen(QPen(QBrush(Qt::black), 1.5));
            else if(i % 50 == 0)
                painter.setPen(QPen(QBrush(Qt::black), 1));
            else
                painter.setPen(QPen(QBrush(Qt::black), 0.5));
            painter.drawLine(0,i,_sirka->hodnota(),i);
        }
        painter.setRenderHint(QPainter::HighQualityAntialiasing, true);
    }

    for (auto &k : _komponenty)
    {
        if(_nahlad->hodnota() && dynamic_cast<Komponenty::Prerusenie*>(k.get())){
            if(auto p = dynamic_cast<Komponenty::Prerusenie*>(k.get()))
                p->Vykresli(painter, Qt::transparent);
        }
        else
        {
            k->Vykresli(painter);
            if(!_nahlad->hodnota())
            {
                for (auto &m : k->Manipulatory())
                    m->Vykresli(painter);
            }
        }
    }

    if(_vybranyKomponent)
    {
        _vybranyKomponent->Vykresli(painter, Qt::green);
        for (auto &m : _vybranyKomponent->Manipulatory())
            m->Vykresli(painter, Qt::green);
    }
}

void Dokument::VytvorSpojenia(QPointF bod) {
    std::vector<Komponenty::SpojenieSlot *> sloty;

    for (auto &&k : _komponenty) {
        for (auto &&m : k->SpojenieSloty()) {
            if (m->Obsahuje(bod))
            {
                sloty.push_back(dynamic_cast<Komponenty::SpojenieSlot *>(m.get()));
                break; //maximalne jeden slot z jedneho komponentu
            }
        }
    }

    if (sloty.size() > 1) {

        auto spojenieIterator =
                std::find_if(_spojenia.begin(), _spojenia.end(),
                             [bod](auto &&s) { return s->Obsahuje(bod); });
        Komponenty::Komponent* spojenie = nullptr;
        if (spojenieIterator != _spojenia.end())
            spojenie = (*spojenieIterator).get();
        else
        {
            _spojenia.push_back(std::make_unique<Komponenty::Spojenie>());
            spojenie = _spojenia.back().get();
        }

        for (auto &&slot : sloty) {
            slot->NastavSpojenie(spojenie);
        }
    }
}

Komponenty::Komponent * Dokumenty::Dokument::Komponent(QPointF bod)
{
    auto&& s = std::find_if(_spojenia.rbegin(), _spojenia.rend(), [bod](auto&& s) {return s->Obsahuje(bod); });
    if (s != _spojenia.rend())
    {
        _vybranyKomponent = (*s).get();
        return (*s).get();
    }

    auto&& k = std::find_if(_komponenty.rbegin(), _komponenty.rend(), [bod](auto&& k) {return k->Obsahuje(bod); });
    if (k != _komponenty.rend())
    {
        auto&& m = std::find_if((*k)->Manipulatory().rbegin(), (*k)->Manipulatory().rend(), [bod](auto&& m) {return m->Obsahuje(bod); });
        if (m != (*k)->Manipulatory().rend())
        {
            _vybranyKomponent = (*k).get();
            return (*m).get();
        }
        else
        {
            _vybranyKomponent = (*k).get();
            return (*k).get();
        }
    }

    _vybranyKomponent = nullptr;
    return nullptr;
}

void Dokument::PridajKomponent(Komponenty::KomponentPtr komponent)
{
    _vybranyKomponent = komponent.get();
    _komponenty.push_back(std::move(komponent));
}

Komponenty::Komponent *Dokument::vybranyKomponent() const
{
    return _vybranyKomponent;
}

void Dokument::VycistiSpojenia()
{
    auto s = std::find_if(_spojenia.begin(), _spojenia.end(),
                          [this](auto& s) {
        if(auto spojenie = dynamic_cast<Komponenty::Spojenie*>(s.get()))
        {
            if(spojenie->JePrazdne()) {
                if(spojenie->SpojenieSloty().size() == 1)
                    spojenie->SpojenieSloty().at(0)->ZrusSpojenie();
                if(_vybranyKomponent == spojenie)
                    _vybranyKomponent = nullptr;
                return true;
            }
        }
        return false;
    });
    if(s != _spojenia.end())
        _spojenia.erase(s);
}

void Dokument::Prepocitaj()
{
    if(_prepocitavanie)
    {
        Komponenty::SplineGroup g(_komponenty, _spojenia);
        emit DokumentPrepocitany();
    }
}

std::vector<Nastroje::NastrojPresenterPtr> Dokument::DostupneNastroje()
{
    auto nastroje = std::vector<Nastroje::NastrojPresenterPtr>();
    nastroje.push_back(std::make_unique<Nastroje::KurzorPresenter>());
    nastroje.push_back(std::make_unique<Nastroje::CiaraPresenter>());
    nastroje.push_back(std::make_unique<Nastroje::SplinePresenter>());
    nastroje.push_back(std::make_unique<Nastroje::VolnaCiaraPresenter>());
    nastroje.push_back(std::make_unique<Nastroje::PreruseniePresenter>());
    return nastroje;
}

std::vector<Komponenty::Komponent *> Dokument::Komponenty()
{
    std::vector<Komponenty::Komponent *> v;
    for(auto& e : _komponenty)
        v.push_back(e.get());
    return v;
}

void Dokument::ZmazVybranyKomponent()
{
    if(_vybranyKomponent){
        if(auto m = dynamic_cast<Komponenty::Manipulator*>(_vybranyKomponent))
            _vybranyKomponent = m->Vlastnik();
        for(auto& s : _vybranyKomponent->SpojenieSloty())
            s->ZrusSpojenie();
        VycistiSpojenia();
        auto k = std::find_if(_komponenty.begin(), _komponenty.end(),
                              [this](auto& k){
            return k.get() == _vybranyKomponent;
        });
        if(k != _komponenty.end())
            _komponenty.erase(k);
        _vybranyKomponent = nullptr;
        Prepocitaj();
    }
}

void Dokument::obnovKomponenty(QDomNodeList komponenty)
{
    _prepocitavanie = false;
    _komponenty.clear();
    auto dostupneNastroje = DostupneNastroje();
    QDomElement e = komponenty.at(0).toElement();
    while(!e.isNull())
    {
        for(auto& n : dostupneNastroje){
            if(n->Nazov() == e.attribute("typ")){
                auto k = n->Komponent();
                k->Obnov(e);
                _komponenty.push_back(std::move(k));
            }
        }
        e = e.nextSibling().toElement();
    }
    _prepocitavanie = true;
    Prepocitaj();
}

void Dokument::obnovSpojenia(QDomNodeList spojenia)
{
    _spojenia.clear();
    QDomElement e = spojenia.at(0).toElement();
    while(!e.isNull())
    {
        auto spojenie = std::make_unique<Komponenty::Spojenie>();
        spojenie->Obnov(e, this);
        e = e.nextSibling().toElement();
        _spojenia.push_back(std::move(spojenie));
    }
}
