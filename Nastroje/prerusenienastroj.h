#ifndef PRERUSENIENASTROJ_H
#define PRERUSENIENASTROJ_H

#include "splinenastroj.h"

namespace Nastroje {
class PrerusenieNastroj : public SplineNastroj {
public:
    PrerusenieNastroj(Dokumenty::Dokument *dokument);

    virtual NastrojPresenterPtr NastrojPresenter() const;

protected:
    void vytvorNovyKomponent(QPointF bod);

    Komponenty::Dvojbodovy* otestujTyp(Komponenty::Komponent *komponent);
};

class PreruseniePresenter : public NastrojPresenter {
public:
    QString Nazov() const { return "Prerušenie"; }
    QString Ikonka() const { return ":/ikonky/prerusenie.svg"; }
    NastrojPtr Nastroj(Dokumenty::Dokument *dokument) const;
    Komponenty::KomponentPtr Komponent() const;
};
}

#endif // PRERUSENIENASTROJ_H
