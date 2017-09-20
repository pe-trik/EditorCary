#include "boolvlastnostmanager.h"

#include <QCheckBox>

BoolVlastnostManager::BoolVlastnostManager(BoolVlastnost *vlastnost) : _vlastnost(vlastnost)
{

}

QWidget *BoolVlastnostManager::Nastroj()
{
    auto editor = new QCheckBox();
    editor->setChecked(_vlastnost->hodnota());
    QObject::connect(editor, SIGNAL(clicked(bool)), _vlastnost,
                     SLOT(setHodnota(bool)));
    QObject::connect(_vlastnost, SIGNAL(hodnotaZmenena(bool)), editor,
                     SLOT(setChecked(bool)));
    return editor;
}
