#ifndef SPLINE_H
#define SPLINE_H

#include "dvojbodovy.h"
#include <QVector>

namespace Komponenty {
class Spline : public Dvojbodovy {
public:
    Spline();

    void Vykresli(QPainter &painter, QColor c = Qt::black, qreal sirka = 0) const;

    Nastroje::NastrojPtr Nastroj(Dokumenty::Dokument *dokument);

    bool Obsahuje(QPointF) const;

    bool Pouzite() const;

    void setPouzite(bool Pouzite);

    QVector<QPointF> Krivka() const;

    //fciu vola SplineGroup - spline bude rovna ciara urcena dvoma bodmi
    void ResetKrivka();

    //fciu vola SplineGroup - nastavi vypocitanu krivku
    void setKrivka(const QVector<QPointF> &Krivka);

    QString NazovTypu() const;

    QDomElement UlozKomponent(QDomDocument &doc) const;

    QVector<QPointF> BodyKomponentu() const;

protected:
    bool _pouzite = false;
    QVector<QPointF> _krivka;
};
}

#endif // SPLINE_H
