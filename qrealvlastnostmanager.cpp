#include "qrealvlastnostmanager.h"

#include <QDoubleSpinBox>
#include <QObject>

QrealVlastnostManager::QrealVlastnostManager(QrealVlastnost *vlastnost)
    : _vlastnost(vlastnost) {}

QWidget *QrealVlastnostManager::Nastroj() {
  auto editor = new QDoubleSpinBox();
  editor->setRange(_vlastnost->min(), _vlastnost->max());
  editor->setValue(_vlastnost->hodnota());

  QObject::connect(editor,
                   static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          [this, editor](qreal h){if(editor->hasFocus())_vlastnost->setHodnota(h);}
  );

  QObject::connect(_vlastnost, SIGNAL(hodnotaZmenena(qreal)), editor,
                   SLOT(setValue(qreal)));
  return editor;
}
