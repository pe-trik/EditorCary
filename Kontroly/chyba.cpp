#include "chyba.h"

using namespace Kontroly;

Chyba::Chyba(Kontroly::Kontrola *kontrola, QString msg, Komponenty::Komponent *komponent)
    : _kontrola(kontrola), _msg(msg), _komponent(komponent)
{

}

Kontroly::Kontrola *Chyba::Kontrola() const
{
    return _kontrola;
}

Komponenty::Komponent *Chyba::Komponent() const
{
    return _komponent;
}

QString Chyba::Sprava() const
{
    return _msg;
}
