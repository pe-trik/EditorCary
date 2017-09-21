#ifndef BOOLVLASTNOST_H
#define BOOLVLASTNOST_H

#include "vlastnost.h"

namespace Dokumenty {
class BoolVlastnost : public Vlastnost
{
    Q_OBJECT
public:
    BoolVlastnost(QString nazov, bool hodnota);
    VlastnostManagerPtr NastrojVlastnosti();
    bool hodnota() const;
    QDomElement Uloz(QDomDocument &doc);

  public slots:
    void setHodnota(bool hodnota);

  signals:
    void hodnotaZmenena(bool novaHodnota);

protected:
    void obnov(QDomElement &v);

  private:
    bool _hodnota;
};
using BoolVlastnostPtr = std::unique_ptr<BoolVlastnost>;
}

#endif // BOOLVLASTNOST_H
