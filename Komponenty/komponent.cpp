#include "komponent.h"
#include "manipulator.h"
#include "prerusenie.h"

using namespace Komponenty;

int Komponent::id = 0;

Komponent::Komponent() {
    _nazov = std::make_unique<Dokumenty::StringVlastnost>("Názov", "komponent" + QString::number(id++));
    _sirkaCiary = std::make_unique<Dokumenty::QrealVlastnost>("Šírka čiary", 15);
}

const std::vector<KomponentPtr> &Komponent::Manipulatory() const {
    return _manipulatory;
}

const std::vector<SpojenieSlotPtr> &Komponent::SpojenieSloty() const {
    return _spojenieSloty;
}

std::vector<Dokumenty::Vlastnost *> Komponent::Vlastnosti() const
{
    return _vlastnosti;
}

QString Komponent::NazovKomponentu() const
{
    return _nazov->Hodnota();
}

void Komponent::ObnovKomponent(QDomElement &e)
{
    obnovVlastnosti(e);
}

QVector<QPointF> Komponent::BodyKomponentu() const
{
    QVector<QPointF> body;

    for(auto&m : _manipulatory){
        if(auto man = dynamic_cast<Manipulator*>(m.get()))
            body.push_back(man->Bod());
    }

    return body;
}

void Komponent::zakresliKonce(QPainter &painter, QColor c, qreal sirka) const
{
    painter.setPen(Qt::NoPen);
    painter.setBrush(c);
    for(auto& slot : _spojenieSloty){
        if(!slot->JeVolny())
        {
            if(auto spojenie = dynamic_cast<Spojenie*>(slot->Spojenie()))
            {
                auto sloty = spojenie->SlotySpojenia();
                auto nieVsetkyPrerusenia = std::any_of(sloty.begin(), sloty.end(),
                                                    [](auto s){
                    return !dynamic_cast<Prerusenie*>(s->Vlastnik());
                });
                if(nieVsetkyPrerusenia)
                    painter.drawEllipse(slot->Bod(), sirka / 2, sirka / 2);
            }
        }
    }
}

bool Komponent::bodObsahujeManipulator(QPointF bod) const
{
    return std::any_of(_manipulatory.begin(), _manipulatory.end(), [bod](auto& m)
    {
        return m->Obsahuje(bod);
    });
}

void Komponent::obnovVlastnosti(QDomElement &e)
{
    for(auto& vlastnost : _vlastnosti)
        vlastnost->ObnovVlastnost(e.childNodes());

    auto v = e.childNodes().at(0).toElement();
    while(!v.isNull())
    {
        if(v.nodeName() == "manipulatory"){
            int i = 0;
            for(auto& m : _manipulatory)
            {
                auto manipulator = v.childNodes().at(i++).toElement();
                if(!manipulator.isNull())
                    m->ObnovKomponent(manipulator);
            }
        }
        v = v.nextSibling().toElement();
    }
}

QDomElement Komponent::ulozVlastnosti(QDomDocument &doc) const
{
    auto e = doc.createElement("komponent");
    e.setAttribute("typ", NazovTypu());

    for(auto& vlastnost : _vlastnosti)
        e.appendChild(vlastnost->UlozVlastnost(doc));

    auto manipulatory = doc.createElement("manipulatory");
    for(auto& m : _manipulatory)
        manipulatory.appendChild(m->UlozKomponent(doc));
    e.appendChild(manipulatory);

    return e;
}
