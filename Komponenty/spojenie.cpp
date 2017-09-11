#include "spojenie.h"

using namespace Komponenty;

#include <QObject>
#include <algorithm>

Spojenie::Spojenie() {
	_x = std::make_unique<Dokumenty::QrealVlastnost>("X", 0);
	_y = std::make_unique<Dokumenty::QrealVlastnost>("Y", 0);

    _nazov->setHodnota("Spojenie" + QString::number(id-1));
    _spojenieZoznamVlastnost = std::make_unique<Dokumenty::SpojenieZoznamVlastnost>("Komponenty", this, _manipulatoryKomponent);
    _vlastnosti = { _nazov.get(), _x.get(), _y.get(), _spojenieZoznamVlastnost.get() };

    auto manipulator = std::make_unique<Manipulator>(_x.get(), _y.get(), this);
	_manipulator = manipulator.get();
	_manipulatory.push_back(std::move(manipulator));
	QObject::connect(_x.get(), &Dokumenty::QrealVlastnost::hodnotaZmenena, [this](qreal) {this->obnovHodnoty(); });
	QObject::connect(_y.get(), &Dokumenty::QrealVlastnost::hodnotaZmenena, [this](qreal) {this->obnovHodnoty(); });
}

void Spojenie::PridajKomponent(Komponent *komponent, Komponent *manipulator) {
    auto it = std::find_if(_manipulatoryKomponent.begin(), _manipulatoryKomponent.end(), [manipulator](auto m){return m == manipulator;});
    if(it != _manipulatoryKomponent.end())
        return;
    _manipulatoryKomponent.push_back(manipulator);
	_komponenty.push_back(komponent);
    if (auto manipulatorSpojenia = dynamic_cast<Manipulator*>(_manipulator)) {
        if (auto manipulatorKomponentu = dynamic_cast<Manipulator*>(manipulator))
		{
            manipulatorSpojenia->setBod(manipulatorKomponentu->getBod());
            connect(manipulatorKomponentu->_x,
                    &Dokumenty::QrealVlastnost::hodnotaZmenena,
                    [manipulatorSpojenia, manipulatorKomponentu](qreal){
                if(manipulatorKomponentu->getBod() != manipulatorSpojenia->getBod())
                    manipulatorSpojenia->setBod(manipulatorKomponentu->getBod());
            });
            connect(manipulatorKomponentu->_y,
                    &Dokumenty::QrealVlastnost::hodnotaZmenena,
                    [manipulatorSpojenia, manipulatorKomponentu](qreal){
                if(manipulatorKomponentu->getBod() != manipulatorSpojenia->getBod())
                    manipulatorSpojenia->setBod(manipulatorKomponentu->getBod());
            });
		}
	}
    _spojenieZoznamVlastnost->setHodnota(_manipulatoryKomponent);
}

void Spojenie::OdstranKomponent(Komponent *komponent, Komponent *manipulator) {
	auto m = std::find(_manipulatoryKomponent.begin(), _manipulatoryKomponent.end(), komponent);
	if (m != _manipulatoryKomponent.end()) {
		//disconnect(*m, &Manipulator::BodZmeneny)
		_manipulatoryKomponent.erase(m);
	}

	auto k = std::find(_komponenty.begin(), _komponenty.end(), komponent);
	if (k != _komponenty.end())
		_komponenty.erase(k);
}

void Komponenty::Spojenie::obnovHodnoty()
{
    auto novaPozicia = QPointF(_x->hodnota(), _y->hodnota());
	for (auto&& manipulator : _manipulatoryKomponent)
	{
		if (auto m = dynamic_cast<Manipulator*>(manipulator))
        {
            if(m->getBod() != novaPozicia)
                m->setBod(novaPozicia);
        }
	}
}
