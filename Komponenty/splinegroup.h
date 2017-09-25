#ifndef SPLINEGROUP_H
#define SPLINEGROUP_H

#include "spojenie.h"
#include "spline.h"
#include "spojenieslot.h"
#include "tridiagonalnamatica.h"


namespace Komponenty {

using Koeficienty = std::tuple<QPointF, QPointF, QPointF, QPointF>;

class SplineGroup
{
public:
    SplineGroup(std::vector<Komponenty::KomponentPtr>& komponenty,
                std::vector<Komponenty::KomponentPtr>& spojenia);

    static Koeficienty Koeficienty(std::vector<QPointF>& body, Pole& riesenie, size_t i);

private:
    //vyhlada cestu, ktora obsahuje dany spline
    std::vector<SpojenieSlot *> najdiCestu(Komponenty::Spline* spline);

    //vyhlada castu zacinajucu v slote
    std::vector<SpojenieSlot *> najdiCestu(Komponenty::SpojenieSlot* slot);

    //vrati poradie slotu v spojeni; -1 ak je volny
    int poradieSlotuVSpojeni(Komponenty::SpojenieSlot *slot);

    //zostavi tridiagonalnu maticu
    TridiagonalnaMatica zostavMaticu(std::vector<QPointF> body, std::vector<SpojenieSlot *> sloty, bool kopirujSmer);

    //zostavi tridiagonalnu maticu pre uzavrenu cestu
    TridiagonalnaMatica zostavUzavrenuMaticu(std::vector<QPointF> &body);

    //nastavi smer slotu podla spojenia
    void nastavSmer(Komponenty::SpojenieSlot *slot);

    //vypocita spline z cesty
    void vypocitajSpline(std::vector<SpojenieSlot *> cesta, bool nastavSmer);

    //vypocita a nastavi krivku splinu
    void vypocitajKrivkuSplinu(std::vector<QPointF> Body, std::vector<SpojenieSlot *> cesta, Pole riesenie, bool uzavrena, bool nastavSmer);

    //najde druhy slot komponentu(splinu)
    Komponenty::SpojenieSlot* druhySlotKomponentu(Komponenty::SpojenieSlot* slot);

    //najde prvy alebo druhy slot v spojeni lisiaci sa od argumentu
    Komponenty::SpojenieSlot* druhyKomponentVSpojeni(Komponenty::SpojenieSlot* slot);

    //najde spliny, ktore sa vypocitaju samostatne
    std::vector<Spline*> najdiSamostatne();

    void resetujSpliny();

    void najdiAVypocitajUseky();

    void vypocitajSamostatneSpliny(std::vector<Spline *> &&samostatne);

    void resetujPouzitie();

    std::vector<Komponenty::KomponentPtr>& _komponenty;
    std::vector<Komponenty::KomponentPtr>& _spojenia;
};

}

#endif // SPLINEGROUP_H
