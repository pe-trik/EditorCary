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

    void Vykresli(QPainter &painter, QColor c = Qt::black, qreal sirka = 0) const ;

    Nastroje::NastrojPtr Nastroj(Dokumenty::Dokument *dokument);

    bool Obsahuje(QPointF bod) const;

    QVector<QPointF>& Body();

    void setBody(const QVector<QPointF> &Body);

    //vyhladi, pripadne vypocita spline
    void Prepocitaj();

    QString NazovTypu() const;

    QDomElement UlozKomponent(QDomDocument &doc) const;

    void ObnovKomponent(QDomElement &e);

    QVector<QPointF> BodyKomponentu() const;

private slots:
    void vyhladzovanie(qreal);

    void manipulatorZmeneny(QPointF);

    void manipulator2Zmeneny(QPointF);

    void vykresliAkoSpline(bool);

private:
    //ignoruj body v mensej vzdialenosti ako _vyhladzovanie
    void vyhlad();

    void prepocitajSpline();

    void inicializujVlastnosti();

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
