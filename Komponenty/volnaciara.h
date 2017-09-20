#ifndef VOLNACIARA_H
#define VOLNACIARA_H

#include "komponent.h"
#include "Dokumenty/qrealvlastnost.h"
#include "Dokumenty/boolvlastnost.h"

namespace Komponenty {
class VolnaCiara : public Komponent
{
    Q_OBJECT
public:
    VolnaCiara();
    void Vykresli(QPainter &painter) const ;
    Nastroje::NastrojPtr Nastroj(Dokumenty::Dokument *dokument);
    bool Obsahuje(QPointF bod) const;
    QVector<QPointF>& body();
    void setBody(const QVector<QPointF> &body);
    void prepocitaj();
    QString Typ() const;
    QDomElement Uloz(QDomDocument &doc) const;

private slots:
    void vyhladzovanie(qreal);
    void manipulatorZmeneny(QPointF);
    void manipulator2Zmeneny(QPointF);
    void vykresliAkoSpline(bool);

private:
    void vyhlad();
    void prepocitajSpline();
    bool _ignorujZmenuManipulatora = false;
    QVector<QPointF> _body;
    QVector<QPointF> _povodneBody;
    Dokumenty::QrealVlastnostPtr _vyhladzovanie;
    Dokumenty::QrealVlastnostPtr _x1;
    Dokumenty::QrealVlastnostPtr _x2;
    Dokumenty::QrealVlastnostPtr _y1;
    Dokumenty::QrealVlastnostPtr _y2;
    Dokumenty::QrealVlastnostPtr _nasobokZaciatok;
    Dokumenty::QrealVlastnostPtr _nasobokKoniec;
    Dokumenty::BoolVlastnostPtr _vykresliAkoSpline;
};
}

#endif // VOLNACIARA_H
