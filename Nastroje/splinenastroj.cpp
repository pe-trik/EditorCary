#include "splinenastroj.h"
#include "Komponenty/spline.h"

using namespace Nastroje;

SplineNastroj::SplineNastroj(Dokumenty::Dokument *dokument)
    : DvojbodovyNastroj(dokument) {}

void SplineNastroj::vytvorNovyKomponent(QPointF bod) {
  auto ciara = std::make_unique<Komponenty::Spline>();
  _komponent = ciara.get();
  for (auto &&m : _komponent->Manipulatory())
    dynamic_cast<Komponenty::Manipulator *>(m.get())->setBod(bod);
  _dokument->PridajKomponent(std::move(ciara));
}

NastrojPresenterPtr SplineNastroj::NastrojPresenter() const {
    return std::make_unique<SplinePresenter>();
}

Komponenty::Dvojbodovy *SplineNastroj::otestujTyp(Komponenty::Komponent *komponent)
{
    return dynamic_cast<Komponenty::Spline*>(komponent);
}
