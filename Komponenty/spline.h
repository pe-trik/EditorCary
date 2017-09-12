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
  bool pouzite() const;
  void setPouzite(bool pouzite);

private:
  bool _pouzite = false;
};
}

#endif // SPLINE_H
