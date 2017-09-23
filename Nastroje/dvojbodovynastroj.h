#ifndef DVOJBODOVYNASTROJ_H
#define DVOJBODOVYNASTROJ_H

#include "nastroj.h"
#include "Komponenty\dvojbodovy.h"
#include "Komponenty\manipulator.h"

namespace Nastroje {
class DvojbodovyNastroj : public Nastroj
{
public:
    DvojbodovyNastroj(Dokumenty::Dokument *dokument);


    void MysStlacena(QPointF);
    void MysPohyb(QPointF);
    void MysUvolnena(QPointF);
    void MysDvojklik(QPointF);

    Komponenty::Komponent* VybranyKomponent() const{
        if(_komponent)
            return _komponent;
        else
            return _manipulator;
    }

protected:
    virtual Komponenty::Dvojbodovy* otestujTyp(Komponenty::Komponent* komponent) = 0;
    virtual void vytvorNovyKomponent(QPointF bod) = 0;
    Komponenty::Dvojbodovy *_komponent = nullptr;
    Komponenty::Manipulator *_manipulator = nullptr;
    bool _mysStlacena = false;
    QPointF _polohaMysi;
};
}

#endif // DVOJBODOVYNASTROJ_H
