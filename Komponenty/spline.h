#ifndef SPLINE_H
#define SPLINE_H

#include "dvojbodovy.h"

namespace Komponenty {
class Spline : public Dvojbodovy {
public:
  Spline();
  void Vykresli(QPainter &painter) const;
  Nastroje::NastrojPtr Nastroj(Dokumenty::Dokument *dokument);
  bool Obsahuje(QPointF) const;
};
}

#endif // SPLINE_H
