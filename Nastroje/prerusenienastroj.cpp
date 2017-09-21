#include "prerusenienastroj.h"

#include <Komponenty/prerusenie.h>

using namespace Nastroje;

PrerusenieNastroj::PrerusenieNastroj(Dokumenty::Dokument *dokument)
    : SplineNastroj(dokument) {}

void PrerusenieNastroj::vytvorNovyKomponent(QPointF bod) {
  auto ciara = std::make_unique<Komponenty::Prerusenie>();
  _komponent = ciara.get();
  for (auto &&m : _komponent->Manipulatory())
    dynamic_cast<Komponenty::Manipulator *>(m.get())->setBod(bod);
  _dokument->PridajKomponent(std::move(ciara));
}

NastrojPresenterPtr PrerusenieNastroj::NastrojPresenter() const {
    return std::make_unique<PreruseniePresenter>();
}

Komponenty::Dvojbodovy *PrerusenieNastroj::otestujTyp(Komponenty::Komponent *komponent)
{
    return dynamic_cast<Komponenty::Prerusenie*>(komponent);
}

Komponenty::KomponentPtr PreruseniePresenter::Komponent() const
{
    return std::make_unique<Komponenty::Prerusenie>();
}
