#include "manipulator.h"

#include <Nastroje/kurzor.h>

using namespace Komponenty;

Manipulator::Manipulator(Dokumenty::QrealVlastnost *x,
                         Dokumenty::QrealVlastnost *y,
                         Komponenty::Komponent *vlastnik)
    : _x(x), _y(y), _vlastnik(vlastnik) {}

void Manipulator::Vykresli(QPainter &painter, QColor c) const {
  auto center = QPointF(_x->hodnota(), _y->hodnota());

  painter.fillRect(QRectF(center - Polomer(), center + Polomer()), c);
}

Nastroje::NastrojPtr Manipulator::Nastroj(Dokumenty::Dokument *dokument) {
  return _vlastnik->Nastroj(dokument);
}

bool Manipulator::Obsahuje(QPointF bod) const {
  return qAbs(_x->hodnota() - bod.x()) <= Polomer().x() &&
         qAbs(_y->hodnota() - bod.y()) <= Polomer().y();
}

void Komponenty::Manipulator::setBod(QPointF bod) {
	if (_x->hodnota() != bod.x() || _y->hodnota() != bod.y()) {
		_x->setHodnota(bod.x());
		_y->setHodnota(bod.y());
		emit BodZmeneny(bod);
	}
}

Komponenty::Komponent *Manipulator::Vlastnik() const
{
    return _vlastnik;
}

QString Manipulator::Typ() const{
    return "Manipulátor";
}

QDomElement Manipulator::Uloz(QDomDocument &doc) const
{
    auto e = doc.createElement("manipulator");
    e.setAttribute("nazov", nazov());
    e.setAttribute("x", _x->hodnota());
    e.setAttribute("y", _y->hodnota());
    return e;
}

void Manipulator::Obnov(QDomElement &e)
{
    _nazov->setHodnota(e.attribute("nazov"));
    _x->setHodnota(e.attribute("x").toDouble());
    _y->setHodnota(e.attribute("y").toDouble());
}

QVector<QPointF> Manipulator::BodyKomponentu() const
{
    return {getBod()};
}
