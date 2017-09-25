#include "dokument.h"
#include "Komponenty/ciara.h"
#include "Komponenty/spojenieslot.h"
#include "Komponenty/spojenie.h"
#include "Komponenty/splinegroup.h"

#include <Nastroje/ciaranastroj.h>
#include <Nastroje/kurzor.h>
#include <Nastroje/prerusenienastroj.h>
#include <Nastroje/splinenastroj.h>
#include <Nastroje/volnaciaranastroj.h>

#include <Komponenty/prerusenie.h>

using namespace Dokumenty;

void Dokument::velkostDokumentuZmenena(qreal) { emit Prekresli(); }

void Dokument::inicializujVlastnostiDokumentu()
{
    _sirka = std::make_unique<QrealVlastnost>("Šírka", 1189);
    _vyska = std::make_unique<QrealVlastnost>("Výška", 841);
    _okraj = std::make_unique<QrealVlastnost>("Okraj", 50);
    _minimalnyPolomerZatacky = std::make_unique<QrealVlastnost>("Minimálny polomer zatáčky", 50);
    _minimalnaDlzkaTrate = std::make_unique<QrealVlastnost>("Minimálna dĺžka trate", 1000);
    _maximalnaDlzkaTrate = std::make_unique<QrealVlastnost>("Maximálna dĺžka trate", 5000);
    _minimalnaVzdialenostKomponent = std::make_unique<QrealVlastnost>("Minimálna vzdialenosť komponent", 50);
    _zobrazMinimalnuVzdialenost = std::make_unique<BoolVlastnost>("Zobraz minimálnu vzdialenosť komponent", true);
    _nahlad = std::make_unique<BoolVlastnost>("Náhľad", false);
    _siet = std::make_unique<BoolVlastnost>("Sieť", true);
}

Dokument::Dokument() {
    inicializujVlastnostiDokumentu();

    connect(_sirka.get(), SIGNAL(hodnotaZmenena(qreal)), this,
            SLOT(velkostDokumentuZmenena(qreal)));
    connect(_vyska.get(), SIGNAL(hodnotaZmenena(qreal)), this,
            SLOT(velkostDokumentuZmenena(qreal)));
}

qreal Dokument::Sirka() const { return _sirka->Hodnota(); }

qreal Dokument::Vyska() const { return _vyska->Hodnota(); }

QDomDocument Dokument::UlozDokument() const
{
    QDomDocument xmlDokument("draha");
    auto root = xmlDokument.createElement("draha");

    auto vlastnosti = xmlDokument.createElement("vlastnosti");
    for(auto& vlastnost : VlastnostiDokumentu())
        vlastnosti.appendChild(vlastnost->UlozVlastnost(xmlDokument));
    root.appendChild(vlastnosti);

    auto komponenty = xmlDokument.createElement("komponenty");
    for(auto& komponent : _komponenty)
        komponenty.appendChild(komponent->UlozKomponent(xmlDokument));
    root.appendChild(komponenty);

    auto spojenia = xmlDokument.createElement("spojenia");
    for(auto& spojenie : _spojenia)
        spojenia.appendChild(spojenie->UlozKomponent(xmlDokument));
    root.appendChild(spojenia);

    xmlDokument.appendChild(root);
    return xmlDokument;
}

void Dokument::najdiBlokVlastnosti(QDomNode blok)
{
    while(!blok.isNull())
    {
        if(blok.nodeName() == "vlastnosti"){
            for(auto v : VlastnostiDokumentu())
                v->ObnovVlastnost(blok.childNodes());
        }
        else if(blok.nodeName() == "komponenty"){
            obnovKomponenty(blok.childNodes());
        }
        else if(blok.nodeName() == "spojenia"){
            obnovSpojenia(blok.childNodes());
        }
        blok = blok.nextSibling();
    }
}

void Dokument::najdiBlokKomponenty(QDomNode blok)
{
    while(!blok.isNull())
    {
        if(blok.nodeName() == "komponenty"){
            obnovKomponenty(blok.childNodes());
        }
        blok = blok.nextSibling();
    }
}

void Dokument::najdiBlokSpojenia(QDomNode blok)
{
    while(!blok.isNull())
    {
        if(blok.nodeName() == "spojenia"){
            obnovSpojenia(blok.childNodes());
        }
        blok = blok.nextSibling();
    }
}

void Dokument::ObnovDokument(const QDomDocument &doc)
{
    _vybranyKomponent = nullptr;

    QDomNode blok = doc.childNodes().item(0).childNodes().at(0);

    //nechceme prepocitat dokument po kazdom pridanom komponente
    _prepocitavanie = false;

    //spojenia sa mozu obnovit az po komponentoch
    najdiBlokVlastnosti(blok);
    najdiBlokKomponenty(blok);
    najdiBlokSpojenia(blok);

    _prepocitavanie = true;

    PrepocitajDokument();
}

void Dokument::setSirka(qreal sirka) { _sirka->setHodnota(sirka); }

void Dokument::setVyska(qreal vyska) { _vyska->setHodnota(vyska); }

std::vector<Vlastnost *> Dokument::VlastnostiDokumentu() const {
    return {_sirka.get(), _vyska.get(), _okraj.get(), _nahlad.get(), _siet.get(),
                _minimalnyPolomerZatacky.get(), _minimalnaDlzkaTrate.get(), _maximalnaDlzkaTrate.get(),
                _minimalnaVzdialenostKomponent.get(), _zobrazMinimalnuVzdialenost.get()
    };
}

void Dokument::vykresliMriezkuAOkraj(QPainter &painter)
{
    //mriezku vykreslujeme nizsou kvalizou antialiasingu - vyssia rychlost
    painter.setRenderHint(QPainter::Antialiasing);

    //okraj
    painter.fillRect(0, 0, _sirka->Hodnota(), _vyska->Hodnota(), QColor(255,0,0,150));
    painter.fillRect(_okraj->Hodnota(), _okraj->Hodnota(),
                     _sirka->Hodnota() - 2 * _okraj->Hodnota(),
                     _vyska->Hodnota() - 2 * _okraj->Hodnota(), Qt::white);

    for(int i = 50; i < _sirka->Hodnota(); i += 50)
    {
        if(i % 100 == 0)
            painter.setPen(QPen(QBrush(Qt::gray), 1.5));
        else
            painter.setPen(QPen(QBrush(Qt::lightGray), 1));
        painter.drawLine(i,0,i,_vyska->Hodnota());
    }

    for(int i = 50; i < _vyska->Hodnota(); i += 50)
    {
        if(i % 100 == 0)
            painter.setPen(QPen(QBrush(Qt::gray), 1.5));
        else
            painter.setPen(QPen(QBrush(Qt::lightGray), 1));
        painter.drawLine(0,i,_sirka->Hodnota(),i);
    }

    painter.setRenderHint(QPainter::HighQualityAntialiasing);
}

void Dokument::Vykresli(QPainter &painter) {

    //vykresli mriezku a okraj, alebo vybieli pozadie
    if(!_nahlad->Hodnota() && _siet->Hodnota())
        vykresliMriezkuAOkraj(painter);
    else
        painter.fillRect(0, 0, _sirka->Hodnota(), _vyska->Hodnota(), Qt::white);


    //vykresli okolie ciar
    if(!_nahlad->Hodnota() && _zobrazMinimalnuVzdialenost->Hodnota())
    {
        for (auto &k : _komponenty)
            k->Vykresli(painter, QColor(255,153,153,255), _minimalnaVzdialenostKomponent->Hodnota());
    }

    //vykresli komponenty
    for (auto &k : _komponenty)
    {
        if(_nahlad->Hodnota() && dynamic_cast<Komponenty::Prerusenie*>(k.get())){
            if(auto p = dynamic_cast<Komponenty::Prerusenie*>(k.get()))
                p->Vykresli(painter, Qt::transparent);
        }
        else
        {
            k->Vykresli(painter);
            if(!_nahlad->Hodnota())
            {
                for (auto &m : k->Manipulatory())
                    m->Vykresli(painter);
            }
        }
    }

    //zvyrazni vybrany komponent
    if(_vybranyKomponent && !_nahlad->Hodnota())
    {
        _vybranyKomponent->Vykresli(painter, QColor(102, 0, 204, 255));
        for (auto &m : _vybranyKomponent->Manipulatory())
            m->Vykresli(painter, QColor(102, 0, 204, 255));
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

    //ak nasiel viac slotov, vytvori spojenie
    if (sloty.size() > 1) {

        //zisti, ci sa uz na danom mieste nenachadza spojenie
        auto spojenieIterator =
                std::find_if(_spojenia.begin(), _spojenia.end(),
                             [bod](auto &&s) { return s->Obsahuje(bod); });

        Komponenty::Komponent* spojenie = nullptr;

        if (spojenieIterator != _spojenia.end())
            spojenie = (*spojenieIterator).get();

        else
        {
            _spojenia.push_back(std::make_unique<Komponenty::Spojenie>());
            spojenie = _spojenia.back().get();
        }

        for (auto &&slot : sloty) {
            slot->NastavSpojenie(spojenie);
        }
    }
}

Komponenty::Komponent * Dokumenty::Dokument::NajdiKomponentPodBodom(QPointF bod)
{
    auto&& s = std::find_if(_spojenia.rbegin(), _spojenia.rend(), [bod](auto&& s) {return s->Obsahuje(bod); });
    if (s != _spojenia.rend())
    {
        _vybranyKomponent = (*s).get();
        return (*s).get();
    }

    auto&& k = std::find_if(_komponenty.rbegin(), _komponenty.rend(), [bod](auto&& k) {return k->Obsahuje(bod); });
    if (k != _komponenty.rend())
    {
        auto&& m = std::find_if((*k)->Manipulatory().rbegin(), (*k)->Manipulatory().rend(), [bod](auto&& m) {return m->Obsahuje(bod); });
        if (m != (*k)->Manipulatory().rend())
        {
            _vybranyKomponent = (*k).get();
            return (*m).get();
        }
        else
        {
            _vybranyKomponent = (*k).get();
            return (*k).get();
        }
    }

    _vybranyKomponent = nullptr;
    return nullptr;
}

const std::vector<Komponenty::KomponentPtr> &Dokument::Komponenty() const { return _komponenty; }

const std::vector<Komponenty::KomponentPtr> &Dokument::Spojenia() const
{
    return _spojenia;
}

void Dokument::PridajKomponent(Komponenty::KomponentPtr komponent)
{
    _vybranyKomponent = komponent.get();
    _komponenty.push_back(std::move(komponent));
}

Komponenty::Komponent *Dokument::VybranyKomponent() const
{
    return _vybranyKomponent;
}

void Dokument::NastavVybranyKomponent(Komponenty::Komponent *k)
{
    _vybranyKomponent = k;
}

void Dokument::VycistiSpojenia()
{
    auto s = std::find_if(_spojenia.begin(), _spojenia.end(),
                          [this](auto& s) {
        if(auto spojenie = dynamic_cast<Komponenty::Spojenie*>(s.get()))
        {
            if(spojenie->JePrazdne()) {
                if(spojenie->SpojenieSloty().size() == 1)
                    spojenie->SpojenieSloty().at(0)->ZrusSpojenie();
                if(_vybranyKomponent == spojenie)
                    _vybranyKomponent = nullptr;
                return true;
            }
        }
        return false;
    });

    if(s != _spojenia.end())
        _spojenia.erase(s);
}

void Dokument::PrepocitajDokument()
{
    if(_prepocitavanie)
    {
        Komponenty::SplineGroup g(_komponenty, _spojenia);
        emit DokumentPrepocitany();
    }
}

std::vector<Nastroje::NastrojPresenterPtr> Dokument::DostupneNastroje()
{
    auto nastroje = std::vector<Nastroje::NastrojPresenterPtr>();
    nastroje.push_back(std::make_unique<Nastroje::KurzorPresenter>());
    nastroje.push_back(std::make_unique<Nastroje::CiaraPresenter>());
    nastroje.push_back(std::make_unique<Nastroje::SplinePresenter>());
    nastroje.push_back(std::make_unique<Nastroje::VolnaCiaraPresenter>());
    nastroje.push_back(std::make_unique<Nastroje::PreruseniePresenter>());
    return nastroje;
}

void Dokument::ZmazVybranyKomponent()
{
    if(_vybranyKomponent){
        if(auto m = dynamic_cast<Komponenty::Manipulator*>(_vybranyKomponent))
            _vybranyKomponent = m->Vlastnik();

        for(auto& s : _vybranyKomponent->SpojenieSloty())
            s->ZrusSpojenie();

        VycistiSpojenia();

        auto k = std::find_if(_komponenty.begin(), _komponenty.end(),
                              [this](auto& k){
            return k.get() == _vybranyKomponent;
        });

        if(k != _komponenty.end())
            _komponenty.erase(k);

        _vybranyKomponent = nullptr;

        PrepocitajDokument();
    }
}

qreal Dokument::Okraj() const{
    return _okraj->Hodnota();
}

qreal Dokument::MinimalnyPolomer() const{
    return _minimalnyPolomerZatacky->Hodnota();
}

void Dokument::obnovKomponenty(QDomNodeList komponenty)
{
    _komponenty.clear();

    //poskytne zoznam typov komponentov
    auto dostupneNastroje = DostupneNastroje();

    QDomElement e = komponenty.at(0).toElement();

    while(!e.isNull())
    {
        for(auto& n : dostupneNastroje){
            if(n->Nazov() == e.attribute("typ")){
                auto k = n->Komponent();
                k->ObnovKomponent(e);
                _komponenty.push_back(std::move(k));
            }
        }
        e = e.nextSibling().toElement();
    }
}

void Dokument::obnovSpojenia(QDomNodeList spojenia)
{
    _spojenia.clear();

    QDomElement e = spojenia.at(0).toElement();

    while(!e.isNull())
    {
        auto spojenie = std::make_unique<Komponenty::Spojenie>();
        spojenie->ObnovKomponent(e, this);
        e = e.nextSibling().toElement();
        _spojenia.push_back(std::move(spojenie));
    }
}

QString Dokument::CestaSubor() const
{
    return _cestaSubor;
}

void Dokument::setCestaSubor(const QString &cestaSubor)
{
    _cestaSubor = cestaSubor;
}

qreal Dokument::MaximalnaDlzkaTrate() const
{
    return _maximalnaDlzkaTrate->Hodnota();
}

qreal Dokument::MinimalnaDlzkaTrate() const
{
    return _minimalnaDlzkaTrate->Hodnota();
}
