#include "kurzor.h"

using namespace Nastroje;

Kurzor::Kurzor(Dokumenty::Dokument *dokument) : Nastroj(dokument) {}

NastrojPresenterPtr Kurzor::NastrojPresenter() const {
  return std::make_unique<KurzorPresenter>();
}

void Kurzor::MysStlacena(QPointF bod) { 
	_mysStlacena = true;
	_vybranyKomponent = nullptr;
	auto k = _dokument->Komponent(bod);
	if (k) {
		for (auto& m : k->Manipulatory())
		{
			if (m->Obsahuje(bod))
				_vybranyKomponent = dynamic_cast<Komponenty::Manipulator*>( m.get());
		}
		if(!_vybranyKomponent)
			_vybranyKomponent = dynamic_cast<Komponenty::Manipulator*>(k);
	}
}

void Kurzor::MysPohyb(QPointF bod) {
	if (_vybranyKomponent)
		_vybranyKomponent->setBod(bod);
}

inline void Nastroje::Kurzor::MysUvolnena(QPointF) {
	_mysStlacena = false;
	_vybranyKomponent = nullptr;
}
