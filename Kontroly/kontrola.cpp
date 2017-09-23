#include "kontrola.h"

using namespace Kontroly;

Kontrola::Kontrola(Dokumenty::Dokument *dokument)
    : _dokument(dokument)
{
}

bool Kontrola::Vysledok() const
{
    return _vysledok;
}

QString Kontrola::Nazov() const
{
    return _nazov;
}

bool Kontrola::Povolena() const
{
    return _povolena;
}

void Kontrola::setPovolena(bool povolena)
{
    if(_povolena != povolena)
    {
        _povolena = povolena;
        emit KontrolaZmenena();
        emit KontrolaPovolenaZmena();
    }
}

void Kontrola::setVysledok(bool vysledok)
{
    if(_vysledok != vysledok)
    {
        _vysledok = vysledok;
        emit KontrolaZmenena();
    }
}
