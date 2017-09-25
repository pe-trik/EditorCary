#include "manipulator.h"

#include <Nastroje/kurzor.h>

using namespace Komponenty;

Manipulator::Manipulator(Dokumenty::QrealVlastnost *x,
                         Dokumenty::QrealVlastnost *y,
                         Komponenty::Komponent *vlastnik)
    : _x(x), _y(y), _vlastnik(vlastnik) {
    _nazov->setHodnota("manipulator" + QString::number(id - 1));
}

void Manipulator::Vykresli(QPainter &painter, QColor c, qreal) const
{
    auto center = QPointF(_x->Hodnota(), _y->Hodnota());

    painter.fillRect(QRectF(center - Polomer(), center + Polomer()), c);
}

Nastroje::NastrojPtr Manipulator::Nastroj(Dokumenty::Dokument *dokument) {
    return _vlastnik->Nastroj(dokument);
}

QPointF Manipulator::Bod() const { return QPointF(_x->Hodnota(), _y->Hodnota()); }

bool Manipulator::Obsahuje(QPointF bod) const {
    return qAbs(_x->Hodnota() - bod.x()) <= Polomer().x() &&
            qAbs(_y->Hodnota() - bod.y()) <= Polomer().y();
}

const QPointF Manipulator::Polomer() {
    return QPointF(10, 10);
}

void Komponenty::Manipulator::setBod(QPointF bod) {
    if (_x->Hodnota() != bod.x() || _y->Hodnota() != bod.y()) {
        _x->setHodnota(bod.x());
        _y->setHodnota(bod.y());
        emit BodZmeneny(bod);
    }
}

Komponenty::Komponent *Manipulator::Vlastnik() const
{
    return _vlastnik;
}

QString Manipulator::NazovTypu() const{
    return "Manipulátor";
}

QDomElement Manipulator::UlozKomponent(QDomDocument &doc) const
{
    auto e = doc.createElement("manipulator");
    e.setAttribute("nazov", NazovKomponentu());
    e.setAttribute("x", _x->Hodnota());
    e.setAttribute("y", _y->Hodnota());
    return e;
}

void Manipulator::ObnovKomponent(QDomElement &e)
{
    _nazov->setHodnota(e.attribute("nazov"));
    _x->setHodnota(e.attribute("x").toDouble());
    _y->setHodnota(e.attribute("y").toDouble());
}

QVector<QPointF> Manipulator::BodyKomponentu() const
{
    return {Bod()};
}
