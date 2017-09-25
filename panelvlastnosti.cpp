#include "panelvlastnosti.h"

PanelVlastnosti::PanelVlastnosti(QWidget *parent) : QWidget(parent) {
  _layout = new QFormLayout(this);
  _layout->setMargin(0);
  this->setLayout(_layout);
}

void PanelVlastnosti::Reset() {
  while ((_layout->count() > 0))
    _layout->removeRow(0);

  _nastroje.clear();
}

void PanelVlastnosti::NastavVlastnosti(
    std::vector<Dokumenty::Vlastnost *> vlastnosti) {
  Reset(); //vymaz zobrazene vlastnosti

  for (auto polozka : vlastnosti) {
    auto vlastnost = polozka->NastrojVlastnosti();

    auto nastroj = vlastnost->Nastroj();

    nastroj->setParent(this);

    connect(polozka, &Dokumenty::Vlastnost::VlastnostZmenena,
            [this]()
    {
        emit VlastnostZmenena();
    });

    _layout->addRow(vlastnost->Nazov(), nastroj);
    _nastroje.push_back(std::move(vlastnost));
  }
}
