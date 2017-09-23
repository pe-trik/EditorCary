#ifndef CIARA_H
#define CIARA_H

#include "Dokumenty/qrealvlastnost.h"
#include "dvojbodovy.h"
#include "manipulator.h"

namespace Komponenty {
class Ciara : public Dvojbodovy {
public:
  Ciara();
  void Vykresli(QPainter &painter, QColor c = Qt::black, qreal sirka = 0) const;
  Nastroje::NastrojPtr Nastroj(Dokumenty::Dokument *dokument);
  bool Obsahuje(QPointF) const;
  QString Typ() const;
  QDomElement Uloz(QDomDocument &doc) const;
};
}

#endif // CIARA_H
