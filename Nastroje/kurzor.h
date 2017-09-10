#ifndef KURZOR_H
#define KURZOR_H

#include "Komponenty/manipulator.h"

namespace Nastroje {

class KurzorPresenter;

class Kurzor : public Nastroj {
  Q_OBJECT
public:
  Kurzor(Dokumenty::Dokument *dokument);
  NastrojPresenterPtr NastrojPresenter() const;
public slots:
  void MysStlacena(QPointF);
  void MysPohyb(QPointF);
  void MysUvolnena(QPointF);
  void MysDvojklik(QPointF) {}

private:
  bool _mysStlacena = false;
  Komponenty::Manipulator *_vybranyKomponent = nullptr;
};

class KurzorPresenter : public NastrojPresenter {
  QString Nazov() const { return "Kurzor"; }
  QString Ikonka() const { return ":/ikonky/kursor.svg"; }
  NastrojPtr Nastroj(Dokumenty::Dokument *dokument) const {
    return std::make_unique<Kurzor>(dokument);
  }
};
}

#endif // KURZOR_H
