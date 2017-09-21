#include "komponent.h"

using namespace Komponenty;

int Komponent::id = 0;

Komponent::Komponent() {
    _nazov = std::make_unique<Dokumenty::StringVlastnost>("Názov", "komponent" + QString::number(id++));
}

std::vector<Dokumenty::Vlastnost *> Komponent::Vlastnosti() const
{
    return _vlastnosti;
}

QString Komponent::nazov() const
{
    return _nazov->hodnota();
}

void Komponent::Obnov(QDomElement &e)
{
    obnovVlastnosti(e);
}

void Komponent::obnovVlastnosti(QDomElement &e)
{
    for(auto& vlastnost : _vlastnosti)
        vlastnost->Obnov(e.childNodes());

    auto v = e.childNodes().at(0).toElement();
    while(!v.isNull())
    {
        if(v.nodeName() == "manipulatory"){
            size_t i = 0;
            for(auto& m : _manipulatory)
            {
                auto manipulator = v.childNodes().at(i++).toElement();
                if(!manipulator.isNull())
                    m->Obnov(manipulator);
            }
        }
        v = v.nextSibling().toElement();
    }
}

QDomElement Komponent::ulozVlastnosti(QDomDocument &doc) const
{
    auto e = doc.createElement("komponent");
    e.setAttribute("typ", Typ());

    for(auto& vlastnost : _vlastnosti)
        e.appendChild(vlastnost->Uloz(doc));

    auto manipulatory = doc.createElement("manipulatory");
    for(auto& m : _manipulatory)
        manipulatory.appendChild(m->Uloz(doc));
    e.appendChild(manipulatory);

    return e;
}
