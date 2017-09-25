#ifndef SPLINENASTROJ_H
#define SPLINENASTROJ_H

#include "dvojbodovynastroj.h"

namespace Nastroje {
class SplineNastroj : public DvojbodovyNastroj {
public:
    SplineNastroj(Dokumenty::Dokument *dokument);

    NastrojPresenterPtr NastrojPresenter() const;

protected:
    void vytvorNovyKomponent(QPointF bod);
    Komponenty::Dvojbodovy* otestujTyp(Komponenty::Komponent *komponent);
};
class SplinePresenter : public NastrojPresenter {
public:
    QString Nazov() const { return "Spline"; }
    QString Ikonka() const { return ":/ikonky/spline.svg"; }
    NastrojPtr Nastroj(Dokumenty::Dokument *dokument) const;
    Komponenty::KomponentPtr Komponent() const;
};
}

#endif // SPLINENASTROJ_H
