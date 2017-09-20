#ifndef CIARANASTROJ_H
#define CIARANASTROJ_H

#include "dvojbodovynastroj.h"

namespace Nastroje {
class CiaraNastroj : public DvojbodovyNastroj {
public:
  CiaraNastroj(Dokumenty::Dokument *dokument);
  void vytvorNovyKomponent(QPointF bod);
  NastrojPresenterPtr NastrojPresenter() const;
protected:
  Komponenty::Dvojbodovy* otestujTyp(Komponenty::Komponent *komponent);
};
class CiaraPresenter : public NastrojPresenter {
public:
  QString Nazov() const { return "Čiara"; }
  QString Ikonka() const { return ":/ikonky/ciara.svg"; }
  NastrojPtr Nastroj(Dokumenty::Dokument *dokument) const {
    return std::make_unique<CiaraNastroj>(dokument);
  }
};
}

#endif // CIARANASTROJ_H
