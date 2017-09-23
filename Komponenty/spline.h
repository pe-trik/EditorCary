#ifndef SPLINE_H
#define SPLINE_H

#include "dvojbodovy.h"
#include <QVector>

namespace Komponenty {
class Spline : public Dvojbodovy {
public:
  Spline();
  void Vykresli(QPainter &painter, QColor c = Qt::black, qreal sirka = 0) const;
  Nastroje::NastrojPtr Nastroj(Dokumenty::Dokument *dokument);
  bool Obsahuje(QPointF) const;

  bool pouzite() const;
  void setPouzite(bool pouzite);

  QVector<QPointF> krivka() const;
  void resetKrivka();
  void setKrivka(const QVector<QPointF> &krivka);
  QString Typ() const;
  QDomElement Uloz(QDomDocument &doc) const;

  QVector<QPointF> BodyKomponentu() const;

protected:
  bool _pouzite = false;
  QVector<QPointF> _krivka;
};
}

#endif // SPLINE_H
