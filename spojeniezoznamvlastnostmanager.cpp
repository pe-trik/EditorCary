#include "spojeniezoznamvlastnostmanager.h"

#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>

#include "Komponenty/manipulator.h"

SpojenieZoznamVlastnostManager::SpojenieZoznamVlastnostManager(
    Dokumenty::SpojenieZoznamVlastnost *vlastnost)
    : _vlastnost(vlastnost) {}

QWidget *SpojenieZoznamVlastnostManager::Nastroj() {
  auto widget = new QWidget();
  auto layout = new QVBoxLayout(widget);

  auto list = new QListWidget();
  layout->addWidget(list);

  for (auto &&i : _vlastnost->hodnota()) {
      list->addItem(i->komponent()->nazov());
  }

  auto btn = new QPushButton("Zmazať");
  btn->setVisible(false);

  layout->addWidget(btn);

  QObject::connect(list, &QListWidget::currentRowChanged, [btn](int index) {
    if (index > -1)
      btn->setVisible(true);
    else
      btn->setVisible(false);
  });

  QObject::connect(btn, &QPushButton::clicked, [list, this](auto) {
    if (list->currentRow() > -1) {
      if (auto spojenie =
              dynamic_cast<Dokumenty::SpojenieZoznamVlastnost *>(_vlastnost)) {
        if (auto s =
                dynamic_cast<Komponenty::Spojenie *>(spojenie->spojenie())) {
          s->OdstranKomponent(_vlastnost->hodnota().at(
              static_cast<size_t>(list->currentRow())));
          list->takeItem(list->currentRow());
        }
      }
    }
  });

  widget->setLayout(layout);
  return widget;
}
