#include "spojenie.h"

using namespace Komponenty;

#include <QObject>
#include <algorithm>
#include "manipulator.h"
#include <Nastroje/kurzor.h>

Spojenie::Spojenie() {
    _nazov->setHodnota("spojenie" + QString::number(id - 1));

    _x = std::make_unique<Dokumenty::QrealVlastnost>("X", 0);
    _y = std::make_unique<Dokumenty::QrealVlastnost>("Y", 0);

    _spojenieZoznamVlastnost =
            std::make_unique<Dokumenty::SpojenieZoznamVlastnost>("Komponenty", this);
    _vlastnosti = {_nazov.get(), _x.get(), _y.get(),
                   _spojenieZoznamVlastnost.get()};

    auto manipulator = std::make_unique<Manipulator>(_x.get(), _y.get(), this);
    _manipulator = manipulator.get();
    _manipulatory.push_back(std::move(manipulator));

    QObject::connect(_x.get(), &Dokumenty::QrealVlastnost::hodnotaZmenena,
                     [this](qreal) { this->nastavUmiestneniaPripojenychManipulatorov(); });
    QObject::connect(_y.get(), &Dokumenty::QrealVlastnost::hodnotaZmenena,
                     [this](qreal) { this->nastavUmiestneniaPripojenychManipulatorov(); });
}

void Spojenie::Vykresli(QPainter &painter, QColor c, qreal) const { _manipulator->Vykresli(painter, c); }

Nastroje::NastrojPtr Spojenie::Nastroj(Dokumenty::Dokument *dokument) {
    //vrati nastroj prveho komponentu
    if(_spojenieZoznamVlastnost->Hodnota().size() > 0)
        return _spojenieZoznamVlastnost->Hodnota().at(0)->Vlastnik()->Nastroj(dokument);
    else
        return std::make_unique<Nastroje::Kurzor>(dokument);
}

bool Spojenie::Obsahuje(QPointF bod) const { return _manipulator->Obsahuje(bod); }

void Spojenie::PridajKomponent(SpojenieSlot *slot) {
    auto manipulatorSpojenia = dynamic_cast<Manipulator*>(_manipulator);

    auto sloty = _spojenieZoznamVlastnost->Hodnota();

    //zisti, ci sa uz slot nenachadza v spojeni
    auto it = std::find_if(sloty.begin(), sloty.end(),
                           [slot](auto s) {
        return s == slot || slot->Vlastnik() == s->Vlastnik();
    });
    if (it != sloty.end())
        return;

    sloty.push_back(slot);

    auto manipulator = dynamic_cast<Manipulator*>(slot->Manipulator());

    //zjednot pozicie manipulatorov
    if(sloty.size() > 1)
        manipulator->setBod(manipulatorSpojenia->Bod());
    else
        manipulatorSpojenia->setBod(manipulator->Bod());

    //synchronizacia poloh spojenia a komponentu
    connect(manipulator->_x,
            &Dokumenty::QrealVlastnost::hodnotaZmenena, slot->Manipulator(),
            [manipulatorSpojenia, manipulator](qreal) {
        if (manipulator->Bod() !=
                manipulatorSpojenia->Bod())
            manipulatorSpojenia->setBod(manipulator->Bod());
    });

    //synchronizacia poloh spojenia a komponentu
    connect(manipulator->_y,
            &Dokumenty::QrealVlastnost::hodnotaZmenena, slot->Manipulator(),
            [manipulatorSpojenia, manipulator](qreal) {
        if (manipulator->Bod() !=
                manipulatorSpojenia->Bod())
            manipulatorSpojenia->setBod(manipulator->Bod());
    });

    //nastav zoznam slotov do vlastnosti
    _spojenieZoznamVlastnost->setHodnota(sloty);
}

void Spojenie::OdstranKomponent(SpojenieSlot *slot) {
    auto sloty = _spojenieZoznamVlastnost->Hodnota();
    auto m = std::find(sloty.begin(), sloty.end(), slot);
    auto manipulator = dynamic_cast<Manipulator*>(slot->Manipulator());

    if (m != sloty.end()) {

        //zrusenie synchronizacie polohy spojenia a komponentu
        disconnect(manipulator->_x,
                   &Dokumenty::QrealVlastnost::hodnotaZmenena, slot->Manipulator(),
                   nullptr);
        disconnect(manipulator->_y,
                   &Dokumenty::QrealVlastnost::hodnotaZmenena, slot->Manipulator(),
                   nullptr);

        //posunutie odpajaneho manipulatora
        if(sloty.size() == 2)
            manipulator->setBod(manipulator->Bod() + 3 * Manipulator::Polomer());
        else
            manipulator->setBod(manipulator->Bod() + 2 * Manipulator::Polomer());

        slot->ZrusSpojenie();

        sloty.erase(m);

        //Spojenie nedava zmysel iba s jednym zapojenym komponentom
        if(sloty.size() == 1)
        {
            _spojenieZoznamVlastnost->setHodnota(sloty);
            OdstranKomponent(sloty.at(0));
            _spojenieZoznamVlastnost->setHodnota({});
        }
        else
            _spojenieZoznamVlastnost->setHodnota(sloty);
    }
}

//spojenie sa povazuje za prazdne, ak obsahuje najviac jeden komponent
bool Spojenie::JePrazdne() const {
    return _spojenieZoznamVlastnost->Hodnota().size() <= 1;
}

std::vector<SpojenieSlot *> Spojenie::SlotySpojenia() const{
    return _spojenieZoznamVlastnost->Hodnota();
}

QString Spojenie::NazovTypu() const{
    return "Spojenie";
}

QDomElement Spojenie::UlozKomponent(QDomDocument &doc) const
{
    return ulozVlastnosti(doc);
}

void Spojenie::ObnovKomponent(QDomElement e, Dokumenty::Dokument *dokument)
{
    e = e.childNodes().at(0).toElement();
    while(!e.isNull())
    {
        if(e.attribute("nazov") == "Komponenty")
            _spojenieZoznamVlastnost->ObnovVlastnost(e.childNodes(), dokument);
        e = e.nextSiblingElement();
    }
}

//prejde vsetky pripojene manipulatory a nastavi im nove umiestnenie
void Komponenty::Spojenie::nastavUmiestneniaPripojenychManipulatorov() {
    auto novaPozicia = QPointF(_x->Hodnota(), _y->Hodnota());
    for (auto &&slot : _spojenieZoznamVlastnost->Hodnota()) {
        auto manipulator = dynamic_cast<Manipulator*>(slot->Manipulator());
        if (manipulator->Bod() != novaPozicia)
            manipulator->setBod(novaPozicia);
    }
}
