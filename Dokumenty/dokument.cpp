#include "dokument.h"
#include "Komponenty/ciara.h"
#include "Komponenty/spojenieslot.h"
#include "Komponenty/spojenie.h"
#include "Komponenty/splinegroup.h"

using namespace Dokumenty;

void Dokument::velkostDokumentuZmenena(qreal) { emit prekreslit(); }

Dokument::Dokument() {
    _sirka = std::make_unique<QrealVlastnost>("Šírka", 1189);
    _vyska = std::make_unique<QrealVlastnost>("Výška", 841);
    connect(_sirka.get(), SIGNAL(hodnotaZmenena(qreal)), this,
            SLOT(velkostDokumentuZmenena(qreal)));
    connect(_vyska.get(), SIGNAL(hodnotaZmenena(qreal)), this,
            SLOT(velkostDokumentuZmenena(qreal)));
}

qreal Dokument::sirka() const { return _sirka->hodnota(); }

qreal Dokument::vyska() const { return _vyska->hodnota(); }

void Dokument::setSirka(qreal sirka) { _sirka->setHodnota(sirka); }

void Dokument::setVyska(qreal vyska) { _vyska->setHodnota(vyska); }

std::vector<Vlastnost *> Dokument::Vlastnosti() const {
    return {_sirka.get(), _vyska.get()};
}

void Dokument::Vykresli(QPainter &painter) {
    painter.fillRect(0, 0, _sirka->hodnota(), _vyska->hodnota(), Qt::white);

    for (auto &k : _komponenty) {
        k->Vykresli(painter);
        for (auto &m : k->Manipulatory())
            m->Vykresli(painter);
    }

    if(_vybranyKomponent)
    {
        qreal x1 = _sirka->hodnota();
        qreal x2 = 0;
        qreal y1 = _vyska->hodnota();
        qreal y2 = 0;
        for(auto&& m: _vybranyKomponent->Manipulatory())
        {
            if(auto man = dynamic_cast<Komponenty::Manipulator*>(m.get()))
            {
                auto b = man->getBod();
                x1 = qMin(x1, b.x());
                x2 = qMax(x2, b.x());
                y1 = qMin(y1, b.y());
                y2 = qMax(y2, b.y());
            }
        }
        painter.setPen(QPen(QBrush(Qt::red),1));
        painter.drawRect(QRectF(QPointF(x1,y1) - Komponenty::Manipulator::Polomer(),
                                QPointF(x2,y2) + Komponenty::Manipulator::Polomer()));
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
        std::shared_ptr<Komponenty::Komponent> spojenie;
        if (spojenieIterator != _spojenia.end())
            spojenie = *spojenieIterator;
        else
        {
            spojenie = std::make_shared<Komponenty::Spojenie>();
            _spojenia.push_back(spojenie);
        }

        for (auto &&slot : sloty) {
            slot->NastavSpojenie(spojenie.get());
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
    std::remove_if(_spojenia.begin(), _spojenia.end(),
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
            return false;
        }
    });
}

void Dokument::Prepocitaj()
{
    Komponenty::SplineGroup g(_komponenty, _spojenia);
}
