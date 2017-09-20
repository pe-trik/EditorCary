#include "manipulator.h"

#include <Nastroje/kurzor.h>

using namespace Komponenty;

Manipulator::Manipulator(Dokumenty::QrealVlastnost *x,
                         Dokumenty::QrealVlastnost *y,
                         Komponenty::Komponent *vlastnik)
    : _x(x), _y(y), _vlastnik(vlastnik) {}

void Manipulator::Vykresli(QPainter &painter) const {
  auto center = QPointF(_x->hodnota(), _y->hodnota());

  painter.fillRect(QRectF(center - Polomer(), center + Polomer()), Qt::black);
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
    return ulozVlastnosti(doc);
}
