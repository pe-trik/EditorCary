#ifndef CIARA_H
#define CIARA_H

#include "Dokumenty/qrealvlastnost.h"
#include "dvojbodovy.h"
#include "manipulator.h"

namespace Komponenty {
class Ciara : public Dvojbodovy {
public:
  Ciara();
  void Vykresli(QPainter &painter) const;
  Nastroje::NastrojPtr Nastroj(Dokumenty::Dokument *dokument);
  bool Obsahuje(QPointF) const;
};
}

#endif // CIARA_H
