#ifndef KONTROLADLZKY_H
#define KONTROLADLZKY_H

#include "kontrola.h"

namespace Kontroly {
class KontrolaDlzky : public Kontrola
{
public:
    KontrolaDlzky(Dokumenty::Dokument* dokument);
    std::vector<Chyba> VykonajKontrolu();
};
}

#endif // KONTROLADLZKY_H
