#ifndef CIARANASTROJ_H
#define CIARANASTROJ_H

#include "dvojbodovynastroj.h"

namespace Nastroje {
class CiaraNastroj : public DvojbodovyNastroj {
public:
    CiaraNastroj(Dokumenty::Dokument *dokument);

    NastrojPresenterPtr NastrojPresenter() const;

protected:
    void vytvorNovyKomponent(QPointF bod);
    Komponenty::Dvojbodovy* otestujTyp(Komponenty::Komponent *komponent);
};

class CiaraPresenter : public NastrojPresenter {
public:
    QString Nazov() const { return "Čiara"; }
    QString Ikonka() const { return ":/ikonky/ciara.svg"; }
    NastrojPtr Nastroj(Dokumenty::Dokument *dokument) const;
    Komponenty::KomponentPtr Komponent() const;
};
}

#endif // CIARANASTROJ_H
