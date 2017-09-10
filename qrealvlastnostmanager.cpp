#include "qrealvlastnostmanager.h"

#include <QDoubleSpinBox>

QrealVlastnostManager::QrealVlastnostManager(QrealVlastnost *vlastnost)
    : _vlastnost(vlastnost) {}

QWidget *QrealVlastnostManager::Nastroj() {
  auto editor = new QDoubleSpinBox();
  editor->setRange(_vlastnost->min(), _vlastnost->max());
  editor->setValue(_vlastnost->hodnota());
  QObject::connect(editor, SIGNAL(valueChanged(qreal)), _vlastnost,
                   SLOT(setHodnota(qreal)));
  QObject::connect(_vlastnost, SIGNAL(hodnotaZmenena(qreal)), editor,
                   SLOT(setValue(qreal)));
  return editor;
}
