#ifndef STRINGVLASTNOSTMANAGER_H
#define STRINGVLASTNOSTMANAGER_H

#include "Dokumenty/stringvlastnost.h"

using namespace Dokumenty;

class StringVlastnostManager : public VlastnostManager
{
public:
    StringVlastnostManager(StringVlastnost *vlastnost);
    QWidget *Nastroj();
    QString Nazov() const;

  private:
    StringVlastnost *_vlastnost;
};

#endif // STRINGVLASTNOSTMANAGER_H
