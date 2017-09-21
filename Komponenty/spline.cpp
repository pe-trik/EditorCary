#include "spline.h"

#include <Nastroje/splinenastroj.h>

using namespace Komponenty;
using namespace Dokumenty;

Spline::Spline() : Dvojbodovy() {}

void Spline::Vykresli(QPainter &painter, QColor c) const {
    painter.setPen(QPen(c, 10));
    if(_krivka.size() > 2)
        painter.drawPolyline(_krivka);
    else
        painter.drawLine(
                    QLineF(_x1->hodnota(), _y1->hodnota(), _x2->hodnota(), _y2->hodnota()));
}

Nastroje::NastrojPtr Spline::Nastroj(Dokument *dokument) {
    return std::make_unique<Nastroje::SplineNastroj>(dokument);
}

bool Spline::Obsahuje(QPointF bod) const {
    if(_krivka.size() < 3){
    qreal x0 = bod.x(), x1 = _x1->hodnota(), x2 = _x2->hodnota(), y0 = bod.y(),
            y1 = _y1->hodnota(), y2 = _y2->hodnota();

    // https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line

    qreal dist = qAbs((y2 - y1) * x0 - (x2 - x1) * y0 + x2 * y1 - y2 * x1) /
            sqrt((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1));

    return dist <= 10
            && ((x1 <= x0 && x0 <= x2) || (x1 >= x0 && x0 >= x2))
            && ((y1 <= y0 && y0 <= y2) || (y1 >= y0 && y0 >= y2));
    }
    else
    {
        return std::any_of(_krivka.begin(), _krivka.end(), [bod](auto b){
            return ((b.x() - bod.x()) * (b.x() - bod.x())
                    + (b.y() - bod.y()) * (b.y() - bod.y())) < 100;
        });
    }
}

bool Spline::pouzite() const
{
    return _pouzite;
}

void Spline::setPouzite(bool pouzite)
{
    _pouzite = pouzite;
}

QVector<QPointF> Spline::krivka() const
{
    return _krivka;
}

void Spline::resetKrivka()
{
    _krivka.clear();
    _krivka.push_back(QPointF(_x1->hodnota(), _y1->hodnota()));
    _krivka.push_back(QPointF(_x2->hodnota(), _y2->hodnota()));

    _spojenieSloty.at(0)->setSmer([this]() {
        return QPointF(
                    -(_x1->hodnota() - _x2->hodnota()),
                    -(_y1->hodnota() - _y2->hodnota()));
    });

    _spojenieSloty.at(1)->setSmer([this]() {
        return QPointF(
                    -(_x2->hodnota() - _x1->hodnota()),
                    -(_y2->hodnota() - _y1->hodnota()));
    });
}

void Spline::setKrivka(const QVector<QPointF> &krivka)
{
    _krivka = krivka;
}

QString Spline::Typ() const{
    return Nastroje::SplinePresenter().Nazov();
}

QDomElement Spline::Uloz(QDomDocument &doc) const
{
    return ulozVlastnosti(doc);
}
