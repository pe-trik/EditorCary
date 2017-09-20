#ifndef BOOLVLASTNOSTMANAGER_H
#define BOOLVLASTNOSTMANAGER_H

#include "Dokumenty/boolvlastnost.h"

using namespace Dokumenty;

class BoolVlastnostManager : public VlastnostManager
{
public:
    BoolVlastnostManager(BoolVlastnost *vlastnost);
    QWidget *Nastroj();
    QString Nazov() const { return _vlastnost->Nazov(); }

  private:
    BoolVlastnost *_vlastnost;
};

#endif // BOOLVLASTNOSTMANAGER_H
