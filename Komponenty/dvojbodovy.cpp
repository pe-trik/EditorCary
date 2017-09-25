#include "dvojbodovy.h"
#include "manipulator.h"
#include "spojenieslot.h"

using namespace Komponenty;
using namespace Dokumenty;

void Dvojbodovy::inicializujVlastnosti()
{
    _x1 = std::make_unique<QrealVlastnost>("X1", 0);
    _y1 = std::make_unique<QrealVlastnost>("Y1", 0);
    _x2 = std::make_unique<QrealVlastnost>("X2", 0);
    _y2 = std::make_unique<QrealVlastnost>("Y2", 0);
    _sirka = std::make_unique<QrealVlastnost>("Šírka", 0);
    _vyska = std::make_unique<QrealVlastnost>("Výška", 0);
    _nasobokZaciatok = std::make_unique<QrealVlastnost>("Násobok začiatok", 1);
    _nasobokZaciatok->setMax(100);
    _nasobokZaciatok->setMin(-100);
    _nasobokKoniec = std::make_unique<QrealVlastnost>("Násobok koniec", 1);
    _nasobokKoniec->setMax(100);
    _nasobokKoniec->setMin(-100);

    _vlastnosti = { _nazov.get(), _sirkaCiary.get(), _x1.get(), _y1.get(), _x2.get(), _y2.get(), _sirka.get(), _vyska.get(), _nasobokZaciatok.get(), _nasobokKoniec.get()};
}

void Dvojbodovy::inicializujManipulatoryASloty()
{
    auto manipulator = std::make_unique<Manipulator>(_x1.get(), _y1.get(), this);
    _spojenieSloty.push_back(
                std::make_unique<SpojenieSlot>(this, manipulator.get(), _nasobokZaciatok.get(), [this]() {
        return QPointF(
                    (_x1->Hodnota() - _x2->Hodnota()),
                    (_y1->Hodnota() - _y2->Hodnota()));
    }));
    connect(manipulator.get(), &Manipulator::BodZmeneny,
            [this](auto){
        _sirka->setHodnota(_x2->Hodnota() - _x1->Hodnota());
        _vyska->setHodnota(_y2->Hodnota() - _y1->Hodnota());
    });
    _manipulatory.push_back(std::move(manipulator));

    manipulator = std::make_unique<Manipulator>(_x2.get(), _y2.get(), this);
    _spojenieSloty.push_back(
                std::make_unique<SpojenieSlot>(this, manipulator.get(), _nasobokKoniec.get(), [this]() {
        return QPointF(
                    (_x2->Hodnota() - _x1->Hodnota()),
                    (_y2->Hodnota() - _y1->Hodnota()));
    }));
    connect(manipulator.get(), &Manipulator::BodZmeneny,
            [this](auto){
        _sirka->setHodnota(_x2->Hodnota() - _x1->Hodnota());
        _vyska->setHodnota(_y2->Hodnota() - _y1->Hodnota());
    });
    _manipulatory.push_back(std::move(manipulator));
}

Dvojbodovy::Dvojbodovy() {

    inicializujVlastnosti();

    inicializujManipulatoryASloty();

    connect(_sirka.get(), &QrealVlastnost::hodnotaZmenena,
            [this](qreal sirka){
        _x2->setHodnota(_x1->Hodnota() + sirka);
    });

    connect(_vyska.get(), &QrealVlastnost::hodnotaZmenena,
            [this](qreal vyska){
        _y2->setHodnota(_y1->Hodnota() + vyska);
    });
}
