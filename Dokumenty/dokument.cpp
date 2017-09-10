#include "dokument.h"
#include "Komponenty/ciara.h"
#include "Komponenty/spojenieslot.h"
#include "Komponenty\spojenie.h"

using namespace Dokumenty;

void Dokument::velkostDokumentuZmenena(qreal) { emit prekreslit(); }

Dokument::Dokument() {
  _sirka = std::make_unique<QrealVlastnost>("Šírka", 1189);
  _vyska = std::make_unique<QrealVlastnost>("Výška", 841);
  connect(_sirka.get(), SIGNAL(hodnotaZmenena(qreal)), this,
          SLOT(velkostDokumentuZmenena(qreal)));
  connect(_vyska.get(), SIGNAL(hodnotaZmenena(qreal)), this,
          SLOT(velkostDokumentuZmenena(qreal)));

  auto ciara = std::make_unique<Komponenty::Ciara>();

  _komponenty.push_back(std::move(ciara));
}

qreal Dokument::sirka() const { return _sirka->hodnota(); }

qreal Dokument::vyska() const { return _vyska->hodnota(); }

void Dokument::setSirka(qreal sirka) { _sirka->setHodnota(sirka); }

void Dokument::setVyska(qreal vyska) { _vyska->setHodnota(vyska); }

std::vector<Vlastnost *> Dokument::Vlastnosti() const {
  return {_sirka.get(), _vyska.get()};
}

void Dokument::Vykresli(QPainter &painter) {
  painter.fillRect(0, 0, _sirka->hodnota(), _vyska->hodnota(), Qt::white);

  for (auto &k : _komponenty) {
    k->Vykresli(painter);
    for (auto &m : k->Manipulatory())
      m->Vykresli(painter);
  }
}

void Dokument::VytvorSpojenia(QPointF bod) {
  std::vector<Komponenty::SpojenieSlot *> sloty;

  for (auto &&k : _komponenty) {
    for (auto &&m : k->SpojenieSloty()) {
		if (m->Obsahuje(bod))
		{
			sloty.push_back(dynamic_cast<Komponenty::SpojenieSlot *>(m.get()));
			break; //maximalne jeden slot z jedneho komponentu
		}
    }
  }

  if (sloty.size() > 1) {

    auto spojenieIterator =
        std::find_if(_spojenia.begin(), _spojenia.end(),
                     [bod](auto &&s) { return s->Obsahuje(bod); });
    std::shared_ptr<Komponenty::Komponent> spojenie;
	if (spojenieIterator != _spojenia.end())
		spojenie = *spojenieIterator;
	else
	{
      spojenie = std::make_shared<Komponenty::Spojenie>();
      _spojenia.push_back(spojenie);
    }

    for (auto &&slot : sloty) {
      slot->NastavSpojenie(spojenie);
    }
  }
}

Komponenty::Komponent * Dokumenty::Dokument::Komponent(QPointF bod)
{
	auto&& s = std::find_if(_spojenia.begin(), _spojenia.end(), [bod](auto&& s) {return s->Obsahuje(bod); });
	if (s != _spojenia.end())
		return (*s).get();

	auto&& k = std::find_if(_komponenty.begin(), _komponenty.end(), [bod](auto&& k) {return k->Obsahuje(bod); });
	if (k != _komponenty.end())
	{
		auto&& m = std::find_if((*k)->Manipulatory().begin(), (*k)->Manipulatory().end(), [bod](auto&& m) {return m->Obsahuje(bod); });
		if (m != (*k)->Manipulatory().end())
			return (*m).get();

		return (*k).get();
	}

	return nullptr;
}
