#include "vlastnost.h"

using namespace Dokumenty;

Vlastnost::Vlastnost(QString nazov, QObject *parent)
    : QObject(parent), _nazov(nazov) {}

void Vlastnost::Obnov(QDomNodeList vlastnosti)
{
    QDomNode v  = vlastnosti.at(0);
    while(!v.isNull())
    {
        QDomElement e = v.toElement();
        if(!e.isNull())
        {
            auto n = e.attribute("nazov");
            if(n == _nazov)
                obnov(e);
        }
        v = v.nextSibling();
    }
}
