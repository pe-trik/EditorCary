#include "spojeniezoznamvlastnostmanager.h"

#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <algorithm>

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

    auto up = new QPushButton("Posuň hore");
    up->setVisible(false);

    auto down = new QPushButton("Posuň dole");
    down->setVisible(false);

    layout->addWidget(up);
    layout->addWidget(down);
    layout->addWidget(btn);

    QObject::connect(list, &QListWidget::currentRowChanged, [btn, up, down, this](int index) {
        if (index > -1)
            btn->setVisible(true);
        else
            btn->setVisible(false);

        if (index > 0)
            up->setVisible(true);
        else
            up->setVisible(false);

        if (index + 1 < _vlastnost->hodnota().size())
            down->setVisible(true);
        else
            down->setVisible(false);
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

    QObject::connect(up, &QPushButton::clicked, [list, this](auto) {
        std::vector<Komponenty::SpojenieSlot*> v = _vlastnost->hodnota();
        std::iter_swap(v.begin() + list->currentRow(), v.begin() + list->currentRow() - 1);
        _vlastnost->setHodnota(v);

        list->clear();
        for (auto &&i : _vlastnost->hodnota()) {
            list->addItem(i->komponent()->nazov());
        }
    });

    QObject::connect(down, &QPushButton::clicked, [list, this](auto) {
        std::vector<Komponenty::SpojenieSlot*> v = _vlastnost->hodnota();
        std::iter_swap(v.begin() + list->currentRow() + 1, v.begin() + list->currentRow());
        _vlastnost->setHodnota(v);

        list->clear();
        for (auto &&i : _vlastnost->hodnota()) {
            list->addItem(i->komponent()->nazov());
        }
    });

    widget->setLayout(layout);
    return widget;
}
