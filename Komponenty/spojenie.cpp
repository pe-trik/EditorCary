#include "spojenie.h"

using namespace Komponenty;

#include <QObject>
#include <algorithm>

Spojenie::Spojenie() {
  _x = std::make_unique<Dokumenty::QrealVlastnost>("X", 0);
  _y = std::make_unique<Dokumenty::QrealVlastnost>("Y", 0);

  _nazov->setHodnota("Spojenie" + QString::number(id - 1));
  _spojenieZoznamVlastnost =
      std::make_unique<Dokumenty::SpojenieZoznamVlastnost>("Komponenty", this);
  _vlastnosti = {_nazov.get(), _x.get(), _y.get(),
                 _spojenieZoznamVlastnost.get()};

  auto manipulator = std::make_unique<Manipulator>(_x.get(), _y.get(), this);
  _manipulator = manipulator.get();
  _manipulatory.push_back(std::move(manipulator));
  QObject::connect(_x.get(), &Dokumenty::QrealVlastnost::hodnotaZmenena,
                   [this](qreal) { this->obnovHodnoty(); });
  QObject::connect(_y.get(), &Dokumenty::QrealVlastnost::hodnotaZmenena,
                   [this](qreal) { this->obnovHodnoty(); });
}

void Spojenie::PridajKomponent(Komponent *manipulator) {
  auto manipulatory = _spojenieZoznamVlastnost->hodnota();
  auto it = std::find_if(manipulatory.begin(), manipulatory.end(),
                         [manipulator](auto m) { return m == manipulator; });
  if (it != manipulatory.end())
    return;
  manipulatory.push_back(manipulator);
  if (auto manipulatorSpojenia = dynamic_cast<Manipulator *>(_manipulator)) {
    if (auto manipulatorKomponentu = dynamic_cast<Manipulator *>(manipulator)) {
      manipulatorSpojenia->setBod(manipulatorKomponentu->getBod());
      connect(manipulatorKomponentu->_x,
              &Dokumenty::QrealVlastnost::hodnotaZmenena, manipulator,
              [manipulatorSpojenia, manipulatorKomponentu](qreal) {
                if (manipulatorKomponentu->getBod() !=
                    manipulatorSpojenia->getBod())
                  manipulatorSpojenia->setBod(manipulatorKomponentu->getBod());
              });
      connect(manipulatorKomponentu->_y,
              &Dokumenty::QrealVlastnost::hodnotaZmenena, manipulator,
              [manipulatorSpojenia, manipulatorKomponentu](qreal) {
                if (manipulatorKomponentu->getBod() !=
                    manipulatorSpojenia->getBod())
                  manipulatorSpojenia->setBod(manipulatorKomponentu->getBod());
              });
    }
  }
  _spojenieZoznamVlastnost->setHodnota(manipulatory);
}

void Spojenie::OdstranKomponent(Komponent *manipulator) {
  auto manipulatory = _spojenieZoznamVlastnost->hodnota();
  auto m = std::find(manipulatory.begin(), manipulatory.end(), manipulator);
  if (m != manipulatory.end()) {
    if (auto manipulatorKomponentu = dynamic_cast<Manipulator *>(manipulator)) {
      disconnect(manipulatorKomponentu->_x,
                 &Dokumenty::QrealVlastnost::hodnotaZmenena, manipulator,
                 nullptr);
      disconnect(manipulatorKomponentu->_y,
                 &Dokumenty::QrealVlastnost::hodnotaZmenena, manipulator,
                 nullptr);
    }
    manipulatory.erase(m);
  }
  _spojenieZoznamVlastnost->setHodnota(manipulatory);
}

void Komponenty::Spojenie::obnovHodnoty() {
  auto novaPozicia = QPointF(_x->hodnota(), _y->hodnota());
  for (auto &&manipulator : _spojenieZoznamVlastnost->hodnota()) {
    if (auto m = dynamic_cast<Manipulator *>(manipulator)) {
      if (m->getBod() != novaPozicia)
        m->setBod(novaPozicia);
    }
  }
}
