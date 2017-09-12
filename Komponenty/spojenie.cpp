#include "spojenie.h"

using namespace Komponenty;

#include <QObject>
#include <algorithm>
#include "manipulator.h"

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

void Spojenie::PridajKomponent(SpojenieSlot *slot) {
        auto manipulatorSpojenia = dynamic_cast<Manipulator*>(_manipulator);
        auto sloty = _spojenieZoznamVlastnost->hodnota();
        auto it = std::find_if(sloty.begin(), sloty.end(),
                               [slot](auto s) {
            return s == slot || slot->komponent() == s->komponent();
        });
        if (it != sloty.end())
            return;

        sloty.push_back(slot);

        auto manipulator = dynamic_cast<Manipulator*>(slot->manipulator());

        if(sloty.size() > 1)
            manipulator->setBod(manipulatorSpojenia->getBod());
        else
            manipulatorSpojenia->setBod(manipulator->getBod());

        connect(manipulator->_x,
                &Dokumenty::QrealVlastnost::hodnotaZmenena, slot->manipulator(),
                [manipulatorSpojenia, manipulator](qreal) {
            if (manipulator->getBod() !=
                    manipulatorSpojenia->getBod())
                manipulatorSpojenia->setBod(manipulator->getBod());
        });

        connect(manipulator->_y,
                &Dokumenty::QrealVlastnost::hodnotaZmenena, slot->manipulator(),
                [manipulatorSpojenia, manipulator](qreal) {
            if (manipulator->getBod() !=
                    manipulatorSpojenia->getBod())
                manipulatorSpojenia->setBod(manipulator->getBod());
        });

        _spojenieZoznamVlastnost->setHodnota(sloty);
}

void Spojenie::OdstranKomponent(SpojenieSlot *slot) {
    auto sloty = _spojenieZoznamVlastnost->hodnota();
    auto m = std::find(sloty.begin(), sloty.end(), slot);
    auto manipulator = dynamic_cast<Manipulator*>(slot->manipulator());

    if (m != sloty.end()) {
        disconnect(manipulator->_x,
                   &Dokumenty::QrealVlastnost::hodnotaZmenena, slot->manipulator(),
                   nullptr);
        disconnect(manipulator->_y,
                   &Dokumenty::QrealVlastnost::hodnotaZmenena, slot->manipulator(),
                   nullptr);
    }

    manipulator->setBod(manipulator->getBod() + 2 * Manipulator::Polomer());
    slot->ZrusSpojenie();
    sloty.erase(m);
    _spojenieZoznamVlastnost->setHodnota(sloty);
}

void Komponenty::Spojenie::obnovHodnoty() {
    auto novaPozicia = QPointF(_x->hodnota(), _y->hodnota());
    for (auto &&slot : _spojenieZoznamVlastnost->hodnota()) {
        auto manipulator = dynamic_cast<Manipulator*>(slot->manipulator());
            if (manipulator->getBod() != novaPozicia)
                manipulator->setBod(novaPozicia);
    }
}
