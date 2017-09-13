#ifndef SPLINE_H
#define SPLINE_H

#include "dvojbodovy.h"
#include <QVector>

namespace Komponenty {
class Spline : public Dvojbodovy {
public:
  Spline();
  void Vykresli(QPainter &painter) const;
  Nastroje::NastrojPtr Nastroj(Dokumenty::Dokument *dokument);
  bool Obsahuje(QPointF) const;
  bool pouzite() const;
  void setPouzite(bool pouzite);

  QVector<QPointF> krivka() const;
  void setKrivka(const QVector<QPointF> &krivka);

private:
  bool _pouzite = false;
  QVector<QPointF> _krivka;
};
}

#endif // SPLINE_H
