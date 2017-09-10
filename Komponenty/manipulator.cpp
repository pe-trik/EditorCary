#include "manipulator.h"

#include <Nastroje/kurzor.h>

using namespace Komponenty;

Manipulator::Manipulator(Dokumenty::QrealVlastnost *x,
                         Dokumenty::QrealVlastnost *y)
    : _x(x), _y(y) {}

void Manipulator::Vykresli(QPainter &painter) const {
  auto center = QPointF(_x->hodnota(), _y->hodnota());

  painter.fillRect(QRectF(center - Polomer, center + Polomer), Qt::black);
}

Nastroje::NastrojPtr Manipulator::Nastroj(Dokumenty::Dokument *dokument) {
  return std::make_unique<Nastroje::Kurzor>(dokument);
}

bool Manipulator::Obsahuje(QPointF bod) const {
  return qAbs(_x->hodnota() - bod.x()) <= Polomer.x() &&
         qAbs(_y->hodnota() - bod.y()) <= Polomer.y();
}

void Komponenty::Manipulator::setBod(QPointF bod) {
	if (_x->hodnota() != bod.x() || _y->hodnota() != bod.y()) {
		_x->setHodnota(bod.x());
		_y->setHodnota(bod.y());
		emit BodZmeneny(bod);
	}
}
