#ifndef SPLINENASTROJ_H
#define SPLINENASTROJ_H

#include "dvojbodovynastroj.h"

namespace Nastroje {
class SplineNastroj : public DvojbodovyNastroj {
public:
  SplineNastroj(Dokumenty::Dokument *dokument);
  void vytvorNovyKomponent(QPointF bod);
  virtual NastrojPresenterPtr NastrojPresenter() const;
protected:
  Komponenty::Dvojbodovy* otestujTyp(Komponenty::Komponent *komponent);
};
class SplinePresenter : public NastrojPresenter {
public:
  QString Nazov() const { return "Spline"; }
  QString Ikonka() const { return ":/ikonky/spline.svg"; }
  NastrojPtr Nastroj(Dokumenty::Dokument *dokument) const {
    return std::make_unique<SplineNastroj>(dokument);
  }
  Komponenty::KomponentPtr Komponent() const;
};
}

#endif // SPLINENASTROJ_H
