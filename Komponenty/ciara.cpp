#include "ciara.h"

#include <Nastroje/ciaranastroj.h>

using namespace Komponenty;
using namespace Dokumenty;

Ciara::Ciara() : Dvojbodovy() {
    _nazov->setHodnota("ciara" + QString::number(id - 1));
}

void Ciara::Vykresli(QPainter &painter, QColor c, qreal sirka) const {
    if(sirka == 0)
        sirka = _sirkaCiary->Hodnota();

    painter.setPen(QPen(c, sirka, Qt::SolidLine, Qt::FlatCap));
    painter.drawLine(
                QLineF(_x1->Hodnota(), _y1->Hodnota(), _x2->Hodnota(), _y2->Hodnota()));

    zakresliKonce(painter, c, sirka);
}

Nastroje::NastrojPtr Ciara::Nastroj(Dokument *dokument) {
    return std::make_unique<Nastroje::CiaraNastroj>(dokument);
}

bool Komponenty::Ciara::Obsahuje(QPointF bod) const
{
    qreal x0 = bod.x(), x1 = _x1->Hodnota(), x2 = _x2->Hodnota(), y0 = bod.y(),
            y1 = _y1->Hodnota(), y2 = _y2->Hodnota();

    // https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line#Line_defined_by_two_points

    qreal dist = qAbs((y2 - y1) * x0 - (x2 - x1) * y0 + x2 * y1 - y2 * x1) /
            sqrt((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1));

    //ciara obsahuje bod, ak je v jej blízkosti alebo bod obsahuje niektorý z jej manipulátorov
    return (dist <= _sirkaCiary->Hodnota() / 2
            && ((x1 <= x0 && x0 <= x2) || (x1 >= x0 && x0 >= x2))
            && ((y1 <= y0 && y0 <= y2) || (y1 >= y0 && y0 >= y2)))
            || bodObsahujeManipulator(bod);
}

QString Ciara::NazovTypu() const{
    return Nastroje::CiaraPresenter().Nazov();
}

QDomElement Ciara::UlozKomponent(QDomDocument &doc) const
{
    return ulozVlastnosti(doc);
}


