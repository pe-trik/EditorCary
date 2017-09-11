#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include "Dokumenty/qrealvlastnost.h"
#include "komponent.h"
#include <Nastroje/nastroj.h>
#include "spojenie.h"
#include <QPainter>

namespace Komponenty {
class Manipulator : public Komponent {
    friend class Spojenie;
	Q_OBJECT
public:
  Manipulator(Dokumenty::QrealVlastnost *x, Dokumenty::QrealVlastnost *y, Komponent *vlastnik);
  void Vykresli(QPainter &painter) const;
  Nastroje::NastrojPtr Nastroj(Dokumenty::Dokument *dokument);

  QPointF getBod() const { return QPointF(_x->hodnota(), _y->hodnota()); }

  bool Obsahuje(QPointF bod) const;

  static const QPointF Polomer() {
      return QPointF(10, 10);
  }

  Komponenty::Komponent *Vlastnik() const;

public slots:
  void setBod(QPointF bod);

signals:
  void BodZmeneny(QPointF bod);

private:
  Dokumenty::QrealVlastnost *_x, *_y;
  Komponenty::Komponent *_vlastnik;
};
using ManipulatorPtr = std::unique_ptr<Manipulator>;
}

#endif // MANIPULATOR_H
