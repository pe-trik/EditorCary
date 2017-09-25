#ifndef SPOJENIEZOZNAMVLASTNOST_H
#define SPOJENIEZOZNAMVLASTNOST_H

#include "vlastnost.h"
#include "Komponenty/spojenieslot.h"

namespace Dokumenty {
//udrziava zoznam slotov spojenia
class SpojenieZoznamVlastnost : public Vlastnost {
    Q_OBJECT
public:
    SpojenieZoznamVlastnost(QString nazov, Komponenty::Komponent *Spojenie);

    VlastnostManagerPtr NastrojVlastnosti();

    std::vector<Komponenty::SpojenieSlot *> Hodnota() const;

    Komponenty::Komponent *Spojenie();

    QDomElement UlozVlastnost(QDomDocument &doc);

    void ObnovVlastnost(QDomNodeList l, Dokumenty::Dokument* dokument);

public slots:
    void setHodnota(std::vector<Komponenty::SpojenieSlot *> Hodnota);

signals:
    void hodnotaZmenena(std::vector<Komponenty::SpojenieSlot *> &novaHodnota);


protected:
    void obnovVlastnost(QDomElement &);

private:
    std::vector<Komponenty::SpojenieSlot *> _hodnota;
    Komponenty::Komponent *_spojenie;
};
using SpojenieZoznamVlastnostPtr = std::unique_ptr<SpojenieZoznamVlastnost>;
}

#endif // SPOJENIEZOZNAMVLASTNOST_H
