#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include "Dokumenty/qrealvlastnost.h"
#include "komponent.h"
#include <Nastroje/nastroj.h>
#include <QPainter>

namespace Komponenty {
class Manipulator : public Komponent {
	Q_OBJECT
public:
  Manipulator(Dokumenty::QrealVlastnost *x, Dokumenty::QrealVlastnost *y);
  void Vykresli(QPainter &painter) const;
  Nastroje::NastrojPtr Nastroj(Dokumenty::Dokument *dokument);

  QPointF getBod() const { return QPointF(_x->hodnota(), _y->hodnota()); }

  bool Obsahuje(QPointF bod) const;

  const QPointF Polomer = QPointF(7.5, 7.5);


  public slots:
  void setBod(QPointF bod);

  signals:
  void BodZmeneny(QPointF bod);

private:
  Dokumenty::QrealVlastnost *_x, *_y;
};
using ManipulatorPtr = std::unique_ptr<Manipulator>;
}

#endif // MANIPULATOR_H
