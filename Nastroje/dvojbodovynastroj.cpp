#include "dvojbodovynastroj.h"
#include "Komponenty/manipulator.h"

using namespace Nastroje;

DvojbodovyNastroj::DvojbodovyNastroj(Dokumenty::Dokument *dokument) :Nastroj(dokument)
{

}

void Nastroje::DvojbodovyNastroj::MysStlacena(QPointF bod)
{
	_polohaMysi = bod;
	_mysStlacena = true;

	auto&& komponent = _dokument->Komponent(bod);

	if (komponent) {

		auto&& manipulator = std::find_if(komponent->Manipulatory().begin(),
			komponent->Manipulatory().end(),
			[bod](auto && m) { return m->Obsahuje(bod); });

		if (manipulator != komponent->Manipulatory().end())
			_manipulator = dynamic_cast<Komponenty::Manipulator*>((*manipulator).get());
		else
			_manipulator = dynamic_cast<Komponenty::Manipulator*>(komponent);;

        _komponent = otestujTyp(komponent);//dynamic_cast<Komponenty::Dvojbodovy*>(komponent);
	}
}

void Nastroje::DvojbodovyNastroj::MysPohyb(QPointF bod)
{
	if (_mysStlacena)
	{
		if (_manipulator)
			_manipulator->setBod(bod);
		else if(_komponent)
		{
			for (auto&& m : _komponent->Manipulatory())
			{
				auto manipulator = dynamic_cast<Komponenty::Manipulator*>(m.get());
				manipulator->setBod(manipulator->getBod() + bod - _polohaMysi);
			}
		}
	}
	_polohaMysi = bod;
}

void Nastroje::DvojbodovyNastroj::MysUvolnena(QPointF)
{
	_mysStlacena = false;
	_manipulator = nullptr;
	_komponent = nullptr;
}

void Nastroje::DvojbodovyNastroj::MysDvojklik(QPointF bod)
{
	vytvorNovyKomponent(bod);
	_mysStlacena = true;
	_manipulator = dynamic_cast<Komponenty::Manipulator*>(_komponent->Manipulatory().at(1).get());
}
