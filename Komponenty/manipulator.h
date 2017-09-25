#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include "Dokumenty/qrealvlastnost.h"
#include "komponent.h"
#include <Nastroje/nastroj.h>
#include "spojenie.h"
#include <QPainter>

namespace Komponenty {
class Manipulator : public Komponent {
    friend class Spojenie;
    Q_OBJECT
public:
    Manipulator(Dokumenty::QrealVlastnost *x, Dokumenty::QrealVlastnost *y, Komponent *vlastnik);

    void Vykresli(QPainter &painter, QColor c = Qt::black, qreal sirka = 0) const;

    Nastroje::NastrojPtr Nastroj(Dokumenty::Dokument *dokument);

    QPointF Bod() const;

    bool Obsahuje(QPointF bod) const;

    static const QPointF Polomer();

    Komponenty::Komponent *Vlastnik() const;

    QString NazovTypu() const;

    QDomElement UlozKomponent(QDomDocument &doc) const;

    void ObnovKomponent(QDomElement &e);

    QVector<QPointF> BodyKomponentu() const;

public slots:
    void setBod(QPointF bod);

signals:
    void BodZmeneny(QPointF bod);

private:
    Dokumenty::QrealVlastnost *_x, *_y;
    Komponenty::Komponent *_vlastnik; //komponent, ktoremu manipulator patri
};
using ManipulatorPtr = std::unique_ptr<Manipulator>;
}

#endif // MANIPULATOR_H
