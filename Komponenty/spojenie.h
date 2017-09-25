#ifndef SPOJENIE_H
#define SPOJENIE_H

#include "Dokumenty/spojeniezoznamvlastnost.h"
#include "Dokumenty/qrealvlastnost.h"
#include "Nastroje/nastroj.h"
#include "komponent.h"

namespace Komponenty {
class Komponent;
class Spojenie : public Komponent {
    friend class SplineGroup;
public:
    Spojenie();

    void Vykresli(QPainter &painter, QColor c = Qt::black, qreal = 0) const;

    Nastroje::NastrojPtr Nastroj(Dokumenty::Dokument *dokument);

    bool Obsahuje(QPointF bod) const;

    void PridajKomponent(SpojenieSlot *slot);

    void OdstranKomponent(SpojenieSlot *slot);

    bool JePrazdne() const;

    std::vector<Komponenty::SpojenieSlot *> SlotySpojenia() const;

    QString NazovTypu() const;

    QDomElement UlozKomponent(QDomDocument &doc) const;

    void ObnovKomponent(QDomElement e, Dokumenty::Dokument *dokument);

private:
    void nastavUmiestneniaPripojenychManipulatorov();
    Komponent *_manipulator;
    Dokumenty::QrealVlastnostPtr _x;
    Dokumenty::QrealVlastnostPtr _y;
    Dokumenty::SpojenieZoznamVlastnostPtr _spojenieZoznamVlastnost;
};
}

#endif // SPOJENIE_H
