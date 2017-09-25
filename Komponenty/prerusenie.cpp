#include "prerusenie.h"

#include <Nastroje/prerusenienastroj.h>

using namespace Komponenty;

Prerusenie::Prerusenie()
{
    _nazov->setHodnota("prerusenie" + QString::number(id - 1));

    _velkost = std::make_unique<Dokumenty::QrealVlastnost>("Dĺžka krivky", 0);
    _vlastnosti.push_back(_velkost.get());

    connect(_x1.get(), SIGNAL(hodnotaZmenena(qreal)), this, SLOT(prepocitajDlzku()));
    connect(_x2.get(), SIGNAL(hodnotaZmenena(qreal)), this, SLOT(prepocitajDlzku()));
    connect(_y1.get(), SIGNAL(hodnotaZmenena(qreal)), this, SLOT(prepocitajDlzku()));
    connect(_y2.get(), SIGNAL(hodnotaZmenena(qreal)), this, SLOT(prepocitajDlzku()));
}

void Prerusenie::Vykresli(QPainter &painter, QColor c, qreal sirka) const
{
    if(sirka == 0)
        sirka = _sirkaCiary->Hodnota();

    painter.setPen(QPen(c, sirka, Qt::DashLine));

    if(_krivka.size() > 2)
        painter.drawPolyline(_krivka);
    else
        painter.drawLine(
                    QLineF(_x1->Hodnota(), _y1->Hodnota(), _x2->Hodnota(), _y2->Hodnota()));
}

Nastroje::NastrojPtr Prerusenie::Nastroj(Dokumenty::Dokument *dokument)
{
    return std::make_unique<Nastroje::PrerusenieNastroj>(dokument);
}

QString Prerusenie::NazovTypu() const
{
    return Nastroje::PreruseniePresenter().Nazov();
}

QVector<QPointF> Prerusenie::BodyKomponentu() const
{
    //prerusenie nema ziadnu dlzku
    return {};
}

void Prerusenie::prepocitajDlzku()
{
    /*auto p1 = _krivka.begin();
    auto p2 = p1 + 1;
    qreal v = 0;
    while(p2 < _krivka.end()){
        auto r = *p2 - *p1;
        v += sqrt(r.x() * r.x() + r.y() * r.y());
        ++p1;
        ++p2;
    }*/

    auto r = QPointF(_x1->Hodnota(), _y1->Hodnota()) - QPointF(_x2->Hodnota(), _y2->Hodnota());

    _velkost->setHodnota(sqrt(r.x() * r.x() + r.y() * r.y()));
}
