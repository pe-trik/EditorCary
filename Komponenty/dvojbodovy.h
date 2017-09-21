#ifndef DVOJBODOVY_H
#define DVOJBODOVY_H

#include "Dokumenty/qrealvlastnost.h"
#include "komponent.h"

namespace Komponenty {
class Dvojbodovy : public Komponent {
public:
  Dvojbodovy();

protected:
  Dokumenty::QrealVlastnostPtr _x1;
  Dokumenty::QrealVlastnostPtr _y1;
  Dokumenty::QrealVlastnostPtr _x2;
  Dokumenty::QrealVlastnostPtr _y2;
  Dokumenty::QrealVlastnostPtr _sirka;
  Dokumenty::QrealVlastnostPtr _vyska;
  Dokumenty::QrealVlastnostPtr _nasobokZaciatok;
  Dokumenty::QrealVlastnostPtr _nasobokKoniec;
};
}
#endif // DVOJBODOVY_H
