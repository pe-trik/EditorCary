#ifndef KONTROLAOKRAJOV_H
#define KONTROLAOKRAJOV_H

#include "kontrola.h"

namespace Kontroly {
class KontrolaOkrajov : public Kontrola
{
public:
    KontrolaOkrajov(Dokumenty::Dokument* dokument);
    std::vector<Chyba> VykonajKontrolu();
};
}

#endif // KONTROLAOKRAJOV_H
