#ifndef BOOLVLASTNOST_H
#define BOOLVLASTNOST_H

#include "vlastnost.h"

namespace Dokumenty {
class BoolVlastnost : public Vlastnost
{
    Q_OBJECT
public:
    BoolVlastnost(QString nazov, bool Hodnota);

    VlastnostManagerPtr NastrojVlastnosti();

    bool Hodnota() const;

    QDomElement UlozVlastnost(QDomDocument &doc);

public slots:
    void setHodnota(bool Hodnota);

signals:
    void hodnotaZmenena(bool novaHodnota);

protected:
    void obnovVlastnost(QDomElement &v);

private:
    bool _hodnota;
};
using BoolVlastnostPtr = std::unique_ptr<BoolVlastnost>;
}

#endif // BOOLVLASTNOST_H
