#ifndef KONTROLAZAKRIVENIA_H
#define KONTROLAZAKRIVENIA_H

#include "kontrola.h"

namespace Kontroly {
class KontrolaZakrivenia : public Kontrola
{
public:
    KontrolaZakrivenia(Dokumenty::Dokument* dokument);
    std::vector<Chyba> VykonajKontrolu();
private:
    qreal polomer(QPointF A, QPointF B, QPointF C) const;
    qreal vzdialenost(QPointF A, QPointF B) const;
};
}

#endif // KONTROLAZAKRIVENIA_H
