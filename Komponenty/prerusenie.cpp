#include "prerusenie.h"

using namespace Komponenty;

Prerusenie::Prerusenie()
{
    _velkost = std::make_unique<Dokumenty::QrealVlastnost>("Dĺžka krivky", 0);
    _vlastnosti.push_back(_velkost.get());
    connect(_x1.get(), SIGNAL(hodnotaZmenena(qreal)), this, SLOT(prepocitajDlzku()));
    connect(_x2.get(), SIGNAL(hodnotaZmenena(qreal)), this, SLOT(prepocitajDlzku()));
    connect(_y1.get(), SIGNAL(hodnotaZmenena(qreal)), this, SLOT(prepocitajDlzku()));
    connect(_y2.get(), SIGNAL(hodnotaZmenena(qreal)), this, SLOT(prepocitajDlzku()));
}

void Prerusenie::Vykresli(QPainter &painter, QColor c) const
{
    painter.setPen(QPen(c, 10, Qt::DashLine));
    if(_krivka.size() > 2)
        painter.drawPolyline(_krivka);
    else
        painter.drawLine(
                    QLineF(_x1->hodnota(), _y1->hodnota(), _x2->hodnota(), _y2->hodnota()));
}

void Prerusenie::prepocitajDlzku()
{
    auto p1 = _krivka.begin();
    auto p2 = p1 + 1;
    qreal v = 0;
    while(p2 < _krivka.end()){
        auto r = *p2 - *p1;
        v += sqrt(r.x() * r.x() + r.y() * r.y());
        ++p1;
        ++p2;
    }
    _velkost->setHodnota(v);
}
