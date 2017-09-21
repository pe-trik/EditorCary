#include "qrealvlastnost.h"

#include "qrealvlastnostmanager.h"

using namespace Dokumenty;

QrealVlastnost::QrealVlastnost(QString nazov, qreal hodnota)
    : Vlastnost(nazov), _hodnota(hodnota) {}

VlastnostManagerPtr QrealVlastnost::NastrojVlastnosti() {
  return std::make_unique<QrealVlastnostManager>(this);
}

qreal QrealVlastnost::hodnota() const { return _hodnota; }

QDomElement QrealVlastnost::Uloz(QDomDocument &doc)
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

void QrealVlastnost::obnov(QDomElement &v)
{
    setHodnota(v.attribute("hodnota").toDouble());
}

qreal QrealVlastnost::max() const { return _max; }

void QrealVlastnost::setMax(qreal max) { _max = max; }

qreal QrealVlastnost::min() const { return _min; }

void QrealVlastnost::setMin(qreal min) { _min = min; }
