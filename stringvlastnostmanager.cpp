#include "stringvlastnostmanager.h"

#include <QLineEdit>

StringVlastnostManager::StringVlastnostManager(StringVlastnost *vlastnost) : _vlastnost(vlastnost)
{

}

QWidget *StringVlastnostManager::Nastroj()
{
    auto editor = new QLineEdit();
    editor->setText(_vlastnost->Hodnota());

    QObject::connect(editor, SIGNAL(textEdited(QString)), _vlastnost,
                     SLOT(setHodnota(QString)));
    QObject::connect(_vlastnost, SIGNAL(hodnotaZmenena(QString)), editor,
                     SLOT(setText(QString)));

    return editor;
}

QString StringVlastnostManager::Nazov() const { return _vlastnost->Nazov(); }
