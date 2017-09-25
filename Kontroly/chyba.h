#ifndef CHYBA_H
#define CHYBA_H

#include "kontrola.h"
#include "Komponenty/komponent.h"
#include "QString"

namespace Kontroly {

class Kontrola;

class Chyba
{
public:
    Chyba(Kontroly::Kontrola* Kontrola, QString Sprava = "", Komponenty::Komponent* Komponent = nullptr);

    Kontroly::Kontrola *Kontrola() const;

    Komponenty::Komponent *Komponent() const;

    QString Sprava() const;

private:
    Kontroly::Kontrola* _kontrola;
    Komponenty::Komponent* _komponent;
    QString _msg;
};
}

#endif // CHYBA_H
