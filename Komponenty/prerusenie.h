#ifndef PRERUSENIE_H
#define PRERUSENIE_H

#include "spline.h"


namespace Komponenty {
class Prerusenie : public Spline
{
    Q_OBJECT
public:
    Prerusenie();
    void Vykresli(QPainter &painter, QColor c = Qt::black) const;

    QVector<QPointF> BodyKomponentu() const;
private slots:
    void prepocitajDlzku();
private:
    Dokumenty::QrealVlastnostPtr _velkost;
};
}

#endif // PRERUSENIE_H
