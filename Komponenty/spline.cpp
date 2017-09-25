#include "spline.h"

#include <Nastroje/splinenastroj.h>

using namespace Komponenty;
using namespace Dokumenty;

Spline::Spline() : Dvojbodovy() {
    _nazov->setHodnota("spline" + QString::number(id - 1));
}

void Spline::Vykresli(QPainter &painter, QColor c, qreal sirka) const {
    if(sirka == 0)
        sirka = _sirkaCiary->Hodnota();

    painter.setPen(QPen(c, sirka, Qt::SolidLine, Qt::FlatCap));

    //ak nie je krivka vypocitana (ma len dva body), vykresli spline ako ciaru
    if(_krivka.size() > 2)
        painter.drawPolyline(_krivka);
    else
        painter.drawLine(
                    QLineF(_x1->Hodnota(), _y1->Hodnota(), _x2->Hodnota(), _y2->Hodnota()));


    zakresliKonce(painter, c, sirka);
}

Nastroje::NastrojPtr Spline::Nastroj(Dokument *dokument) {
    return std::make_unique<Nastroje::SplineNastroj>(dokument);
}

bool Spline::Obsahuje(QPointF bod) const {
    //treba osetrit pripad, ked spline nie je vypocitany a je to ciara urcena dvoma bodmi
    if(_krivka.size() < 3){
        qreal x0 = bod.x(), x1 = _x1->Hodnota(), x2 = _x2->Hodnota(), y0 = bod.y(),
                y1 = _y1->Hodnota(), y2 = _y2->Hodnota();

        // https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line

        qreal dist = qAbs((y2 - y1) * x0 - (x2 - x1) * y0 + x2 * y1 - y2 * x1) /
                sqrt((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1));

        return dist <= 10
                && ((x1 <= x0 && x0 <= x2) || (x1 >= x0 && x0 >= x2))
                && ((y1 <= y0 && y0 <= y2) || (y1 >= y0 && y0 >= y2));
    }
    //spline je vypocitany
    else
    {
        return std::any_of(_krivka.begin(), _krivka.end(), [bod](auto b){
            return ((b.x() - bod.x()) * (b.x() - bod.x())
                    + (b.y() - bod.y()) * (b.y() - bod.y())) < 100;
        });
    }
}

bool Spline::Pouzite() const
{
    return _pouzite;
}

void Spline::setPouzite(bool pouzite)
{
    _pouzite = pouzite;
}

QVector<QPointF> Spline::Krivka() const
{
    return _krivka;
}

void Spline::ResetKrivka()
{
    _krivka.clear();
    _krivka.push_back(QPointF(_x1->Hodnota(), _y1->Hodnota()));
    _krivka.push_back(QPointF(_x2->Hodnota(), _y2->Hodnota()));

    _spojenieSloty.at(0)->setSmer([this]() {
        return QPointF(
                    -(_x1->Hodnota() - _x2->Hodnota()),
                    -(_y1->Hodnota() - _y2->Hodnota()));
    });

    _spojenieSloty.at(1)->setSmer([this]() {
        return QPointF(
                    -(_x2->Hodnota() - _x1->Hodnota()),
                    -(_y2->Hodnota() - _y1->Hodnota()));
    });
}

void Spline::setKrivka(const QVector<QPointF> &krivka)
{
    _krivka = krivka;
}

QString Spline::NazovTypu() const{
    return Nastroje::SplinePresenter().Nazov();
}

QDomElement Spline::UlozKomponent(QDomDocument &doc) const
{
    return ulozVlastnosti(doc);
}

QVector<QPointF> Spline::BodyKomponentu() const
{
    return _krivka;
}
