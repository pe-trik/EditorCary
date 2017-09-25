#include "qrealvlastnost.h"

#include "qrealvlastnostmanager.h"

using namespace Dokumenty;

QrealVlastnost::QrealVlastnost(QString nazov, qreal hodnota)
    : Vlastnost(nazov), _hodnota(hodnota) {}

VlastnostManagerPtr QrealVlastnost::NastrojVlastnosti() {
    return std::make_unique<QrealVlastnostManager>(this);
}

qreal QrealVlastnost::Hodnota() const { return _hodnota; }

QDomElement QrealVlastnost::UlozVlastnost(QDomDocument &doc)
{
    auto v = doc.createElement("vlastnost");
    v.setAttribute("nazov", _nazov);
    v.setAttribute("hodnota", _hodnota);
    return v;
}

void QrealVlastnost::setHodnota(qreal hodnota) {
    if (_hodnota != hodnota) {
        _hodnota = hodnota;
        emit hodnotaZmenena(hodnota);
        emit VlastnostZmenena();
    }
}

void QrealVlastnost::obnovVlastnost(QDomElement &v)
{
    setHodnota(v.attribute("hodnota").toDouble());
}

qreal QrealVlastnost::Max() const { return _max; }

void QrealVlastnost::setMax(qreal max) { _max = max; }

qreal QrealVlastnost::Min() const { return _min; }

void QrealVlastnost::setMin(qreal min) { _min = min; }
