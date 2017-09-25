#include "vlastnost.h"

using namespace Dokumenty;

Vlastnost::Vlastnost(QString nazov, QObject *parent)
    : QObject(parent), _nazov(nazov) {}

QString Vlastnost::Nazov() const { return _nazov; }

void Vlastnost::ObnovVlastnost(QDomNodeList vlastnosti)
{
    QDomNode v  = vlastnosti.at(0);
    while(!v.isNull())
    {
        QDomElement e = v.toElement();
        //vyhlada element s odpovedajucim nazvom a zavola metodu obnovVlastnost
        if(!e.isNull())
        {
            auto n = e.attribute("nazov");
            if(n == _nazov)
                obnovVlastnost(e);
        }
        v = v.nextSibling();
    }
}
