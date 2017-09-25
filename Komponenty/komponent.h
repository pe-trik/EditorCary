#ifndef KOMPONENT_H
#define KOMPONENT_H

#include "Nastroje/nastroj.h"
#include "spojenieslot.h"
#include "Dokumenty/stringvlastnost.h"
#include "Dokumenty/qrealvlastnost.h"
#include <QPainter>
#include <memory>
#include <vector>
#include <QObject>

namespace Nastroje {
class Nastroj;
using NastrojPtr = std::unique_ptr<Nastroj>;
}

namespace Dokumenty {
class Dokument;
}

namespace Komponenty {
class Komponent;
using KomponentPtr = std::unique_ptr<Komponent>;

class SpojenieSlot;
using SpojenieSlotPtr = std::unique_ptr<SpojenieSlot>;

class Komponent : public QObject {
public:
    Komponent();

    virtual void Vykresli(QPainter &painter, QColor c = Qt::black, qreal sirka = 0) const = 0;

    //vrati nastroj, ktory dokaze pracovat s danym komponentom
    virtual Nastroje::NastrojPtr Nastroj(Dokumenty::Dokument *dokument) = 0;

    const std::vector<KomponentPtr> &Manipulatory() const;

    const std::vector<SpojenieSlotPtr> &SpojenieSloty() const;

    virtual bool Obsahuje(QPointF bod) const = 0;

    std::vector<Dokumenty::Vlastnost *> Vlastnosti() const;

    virtual QDomElement UlozKomponent(QDomDocument &doc) const = 0;

    virtual QString NazovTypu() const = 0;

    QString NazovKomponentu() const;

    virtual void ObnovKomponent(QDomElement& e);

    //zoznam bodov komponentu - default: body z manipulatorov
    //vyuziva sa pri kontrole
    virtual QVector<QPointF> BodyKomponentu() const;

protected:
    //zakresli okruhle konce na kraje, kde su spojenia - plynuly prechod
    void zakresliKonce(QPainter &painter, QColor c = Qt::black, qreal sirka = 0) const;
    bool bodObsahujeManipulator(QPointF bod) const;
    void obnovVlastnosti(QDomElement& e);
    QDomElement ulozVlastnosti(QDomDocument &doc) const;

    std::vector<KomponentPtr> _manipulatory;
    std::vector<SpojenieSlotPtr> _spojenieSloty;
    std::vector<Dokumenty::Vlastnost*> _vlastnosti;
    Dokumenty::StringVlastnostPtr _nazov;
    Dokumenty::QrealVlastnostPtr _sirkaCiary;

    static int id; //urcuje sa z neho unikatny nazov komponentu
};
}

#endif // KOMPONENT_H
