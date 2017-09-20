#ifndef VOLNACIARANASTROJ_H
#define VOLNACIARANASTROJ_H

#include "nastroj.h"
#include "Komponenty/volnaciara.h"
#include "Komponenty/manipulator.h"

namespace Nastroje {
class VolnaCiaraNastroj : public Nastroj
{
public:
    VolnaCiaraNastroj(Dokumenty::Dokument *dokument);

    NastrojPresenterPtr NastrojPresenter() const;

    void MysStlacena(QPointF);
    void MysPohyb(QPointF);
    void MysUvolnena(QPointF);
    void MysDvojklik(QPointF);

protected:
    Komponenty::VolnaCiara *_komponent = nullptr;
    Komponenty::Manipulator *_manipulator = nullptr;
    bool _mysStlacena = false;
    bool _novy = false;
    QPointF _polohaMysi;
};
class VolnaCiaraPresenter : public NastrojPresenter {
public:
  QString Nazov() const { return "Voľná čiara"; }
  QString Ikonka() const { return ":/ikonky/volnaciara.svg"; }
  NastrojPtr Nastroj(Dokumenty::Dokument *dokument) const {
    return std::make_unique<VolnaCiaraNastroj>(dokument);
  }
};
}

#endif // VOLNACIARANASTROJ_H
