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

  Komponenty::Komponent* VybranyKomponent() const{
      return _vybranyKomponent;
  }

private:
  bool _mysStlacena = false;
  Komponenty::Komponent *_vybranyKomponent = nullptr;
  QPointF _polohaMysi;
};

class KurzorPresenter : public NastrojPresenter {
  QString Nazov() const { return "Kurzor"; }
  QString Ikonka() const { return ":/ikonky/kursor.svg"; }
  NastrojPtr Nastroj(Dokumenty::Dokument *dokument) const {
    return std::make_unique<Kurzor>(dokument);
  }
  Komponenty::KomponentPtr Komponent() const;
};
}

#endif // KURZOR_H
