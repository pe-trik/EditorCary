#include "panelvlastnosti.h"

PanelVlastnosti::PanelVlastnosti(QWidget *parent) : QWidget(parent) {
  _layout = new QFormLayout(this);
  _layout->setMargin(0);
  this->setLayout(_layout);
}

void PanelVlastnosti::Reset() {
  QLayoutItem *item;
  while ((_layout->count() > 0) && (item = _layout->takeAt(0)))
    delete item;
}

void PanelVlastnosti::NastavVlastnosti(
    std::vector<Dokumenty::Vlastnost *> vlastnosti) {
  Reset();
  for (auto polozka : vlastnosti) {
    auto nastroj = polozka->NastrojVlastnosti();
    _layout->addRow(nastroj->Nazov(), nastroj->Nastroj());
    _nastroje.push_back(std::move(nastroj));
  }
}
