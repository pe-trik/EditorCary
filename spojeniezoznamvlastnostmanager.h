#ifndef SPOJENIEZOZNAMVLASTNOSTMANAGER_H
#define SPOJENIEZOZNAMVLASTNOSTMANAGER_H

#include "vlastnostmanager.h"
#include "Dokumenty/spojeniezoznamvlastnost.h"

class SpojenieZoznamVlastnostManager : public VlastnostManager {
public:
  SpojenieZoznamVlastnostManager(Dokumenty::SpojenieZoznamVlastnost *vlastnost);
  QWidget *Nastroj();
  QString Nazov() const { return _vlastnost->Nazov(); }

private:
  Dokumenty::SpojenieZoznamVlastnost *_vlastnost;
};

#endif // SPOJENIEZOZNAMVLASTNOSTMANAGER_H
