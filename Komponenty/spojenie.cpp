#include "spojenie.h"

using namespace Komponenty;

#include <QObject>

Spojenie::Spojenie() {
	_x = std::make_unique<Dokumenty::QrealVlastnost>("X", 0);
	_y = std::make_unique<Dokumenty::QrealVlastnost>("Y", 0);

	auto manipulator = std::make_unique<Manipulator>(_x.get(), _y.get());
	_manipulator = manipulator.get();
	_manipulatory.push_back(std::move(manipulator));
	QObject::connect(_x.get(), &Dokumenty::QrealVlastnost::hodnotaZmenena, [this](qreal) {this->obnovHodnoty(); });
	QObject::connect(_y.get(), &Dokumenty::QrealVlastnost::hodnotaZmenena, [this](qreal) {this->obnovHodnoty(); });
}

void Spojenie::PridajKomponent(Komponent *komponent, Komponent *manipulator) {
	_manipulatoryKomponent.push_back(manipulator);
	_komponenty.push_back(komponent);
	if (auto m = dynamic_cast<Manipulator*>(_manipulator)) {
		if (auto nm = dynamic_cast<Manipulator*>(manipulator))
		{
			m->setBod(nm->getBod());
			connect(nm, &Manipulator::BodZmeneny, [this, m](auto b) {if (m->getBod() != b) {
				m->setBod(b);
				obnovHodnoty();
			}});
		}
	}
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
	for (auto&& manipulator : _manipulatoryKomponent)
	{
		if (auto m = dynamic_cast<Manipulator*>(manipulator))
			m->setBod(QPointF(_x->hodnota(), _y->hodnota()));
	}
}
