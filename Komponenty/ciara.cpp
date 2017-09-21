#include "ciara.h"

#include <Nastroje/ciaranastroj.h>

using namespace Komponenty;
using namespace Dokumenty;

Ciara::Ciara() : Dvojbodovy() {}

void Ciara::Vykresli(QPainter &painter, QColor c) const {
    painter.setPen(QPen(c, 10));
	painter.drawLine(
		QLineF(_x1->hodnota(), _y1->hodnota(), _x2->hodnota(), _y2->hodnota()));
}

Nastroje::NastrojPtr Ciara::Nastroj(Dokument *dokument) {
  return std::make_unique<Nastroje::CiaraNastroj>(dokument);
}

bool Komponenty::Ciara::Obsahuje(QPointF bod) const
{
	qreal x0 = bod.x(), x1 = _x1->hodnota(), x2 = _x2->hodnota(), y0 = bod.y(),
		y1 = _y1->hodnota(), y2 = _y2->hodnota();

	// https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line

	qreal dist = qAbs((y2 - y1) * x0 - (x2 - x1) * y0 + x2 * y1 - y2 * x1) /
		sqrt((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1));

    return dist <= 10
            && ((x1 <= x0 && x0 <= x2) || (x1 >= x0 && x0 >= x2))
            && ((y1 <= y0 && y0 <= y2) || (y1 >= y0 && y0 >= y2));
}

QString Ciara::Typ() const{
    return Nastroje::CiaraPresenter().Nazov();
}

QDomElement Ciara::Uloz(QDomDocument &doc) const
{
    return ulozVlastnosti(doc);
}


