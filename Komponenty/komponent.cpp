#include "komponent.h"

using namespace Komponenty;

int Komponent::id = 0;

Komponent::Komponent() {
    _nazov = std::make_unique<Dokumenty::StringVlastnost>("Názov", "komponent" + QString::number(id++));
}

std::vector<Dokumenty::Vlastnost *> Komponent::Vlastnosti() const
{
    return _vlastnosti;
}

QString Komponent::nazov() const
{
    return _nazov->hodnota();
}
