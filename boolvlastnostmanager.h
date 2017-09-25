#ifndef BOOLVLASTNOSTMANAGER_H
#define BOOLVLASTNOSTMANAGER_H

#include "Dokumenty/boolvlastnost.h"

using namespace Dokumenty;

class BoolVlastnostManager : public VlastnostManager
{
public:
    BoolVlastnostManager(BoolVlastnost *vlastnost);
    QWidget *Nastroj();
    QString Nazov() const;

private:
    BoolVlastnost *_vlastnost;
};

#endif // BOOLVLASTNOSTMANAGER_H
