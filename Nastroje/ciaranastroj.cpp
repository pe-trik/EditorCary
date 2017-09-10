#include "ciaranastroj.h"
#include "Komponenty/ciara.h"

using namespace Nastroje;

CiaraNastroj::CiaraNastroj(Dokumenty::Dokument *dokument)
    : DvojbodovyNastroj(dokument) {}

void CiaraNastroj::vytvorNovyKomponent(QPointF bod) {
  auto ciara = std::make_unique<Komponenty::Ciara>();
  _komponent = ciara.get();
  for (auto &&m : _komponent->Manipulatory())
    dynamic_cast<Komponenty::Manipulator *>(m.get())->setBod(bod);
  _dokument->Komponenty().push_back(std::move(ciara));
}

NastrojPresenterPtr CiaraNastroj::NastrojPresenter() const {
  return std::make_unique<CiaraPresenter>();
}
