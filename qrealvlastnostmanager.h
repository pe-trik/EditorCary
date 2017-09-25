#ifndef INTVLASTNOSTMANAGER_H
#define INTVLASTNOSTMANAGER_H

#include "Dokumenty/qrealvlastnost.h"

using namespace Dokumenty;

class QrealVlastnostManager : public VlastnostManager {
public:
  QrealVlastnostManager(QrealVlastnost *vlastnost);
  QWidget *Nastroj();
  QString Nazov() const;

private:
  QrealVlastnost *_vlastnost;
};

#endif // INTVLASTNOSTMANAGER_H
