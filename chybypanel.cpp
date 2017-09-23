#include "chybypanel.h"

#include <QVBoxLayout>
#include <QHeaderView>

ChybyPanel::ChybyPanel(QWidget *parent) : QWidget(parent)
{
    _widget = new QTableWidget(this);
    auto layout = new QVBoxLayout(this);
    setLayout(layout);
    layout->addWidget(_widget);

    _widget->setColumnCount(3);
    QStringList nadpisy;
    nadpisy << "Kontrola" << "Komponent" << "Chyba";
    _widget->setHorizontalHeaderLabels(nadpisy);
    _widget->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(_widget, &QTableWidget::cellDoubleClicked,
            [this](int r, auto){
        if(r > -1 && r < _chyby.size())
            emit VybranyKomponent(_chyby.at(r).Komponent());
    });
}

void ChybyPanel::NastavChyby(std::vector<Kontroly::Chyba> chyby)
{
    _widget->clearContents();
    _widget->setRowCount(chyby.size());

    size_t i = 0;
    for(Kontroly::Chyba& ch : chyby){
        auto kontrola = new QTableWidgetItem(ch.Kontrola()->Nazov());

        auto komponent = new QTableWidgetItem();
        if(ch.Komponent())
            komponent->setText(ch.Komponent()->nazov());

        auto msg = new QTableWidgetItem(ch.Sprava());

        _widget->setItem(i, 0, kontrola);
        _widget->setItem(i, 1, komponent);
        _widget->setItem(i++, 2, msg);
    }

    _widget->horizontalHeader()->setStretchLastSection(true);
}
