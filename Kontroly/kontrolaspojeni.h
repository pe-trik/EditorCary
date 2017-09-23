#ifndef KONTROLASPOJENI_H
#define KONTROLASPOJENI_H

#include "kontrola.h"

namespace Kontroly {
class KontrolaSpojeni : public Kontrola
{
public:
    KontrolaSpojeni(Dokumenty::Dokument* dokument);
    std::vector<Chyba> VykonajKontrolu();
};
}

#endif // KONTROLASPOJENI_H
