#ifndef PRERUSENIE_H
#define PRERUSENIE_H

#include "spline.h"


namespace Komponenty {
class Prerusenie : public Spline
{
    Q_OBJECT
public:
    Prerusenie();
    void Vykresli(QPainter &painter, QColor c = Qt::black, qreal sirka = 0) const;
    Nastroje::NastrojPtr Nastroj(Dokumenty::Dokument *dokument);

    QString Typ() const;

    QVector<QPointF> BodyKomponentu() const;
private slots:
    void prepocitajDlzku();
private:
    Dokumenty::QrealVlastnostPtr _velkost;
};
}

#endif // PRERUSENIE_H
