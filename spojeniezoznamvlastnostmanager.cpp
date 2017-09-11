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
    if (auto manipulator = dynamic_cast<Komponenty::Manipulator *>(i))
      list->addItem(manipulator->Vlastnik()->nazov());
  }

  layout->addWidget(new QPushButton("Zmazať"));

  widget->setLayout(layout);
  return widget;
}
