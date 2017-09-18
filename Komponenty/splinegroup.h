#ifndef SPLINEGROUP_H
#define SPLINEGROUP_H

#include "spojenie.h"
#include "spline.h"
#include "spojenieslot.h"
#include "tridiagonalnamatica.h"


namespace Komponenty {

typedef std::tuple<QPointF, QPointF, QPointF, QPointF> Koeficienty;

class SplineGroup
{
public:
    SplineGroup(std::vector<Komponenty::KomponentPtr>& komponenty,
                std::vector<std::shared_ptr<Komponenty::Komponent>>& spojenia);
private:
    std::vector<SpojenieSlot *> najdiCestu(Komponenty::Spline* spline);
    std::vector<SpojenieSlot *> najdiCestu(Komponenty::SpojenieSlot* slot);
    int poradieSlotuVSpojeni(Komponenty::SpojenieSlot *slot);
    TridiagonalnaMatica zostavMaticu(std::vector<QPointF> body, std::vector<SpojenieSlot *> sloty, bool kopirujSmer);
    TridiagonalnaMatica zostavUzavrenuMaticu(std::vector<QPointF> &body);
    void nastavSmer(Komponenty::SpojenieSlot *slot);
    void vypocitajSpline(std::vector<SpojenieSlot *> cesta, bool nastavSmer);
    void vykresliSpline(std::vector<QPointF> Body, std::vector<SpojenieSlot *> cesta, Pole riesenie, bool uzavrena, bool nastavSmer);
    Koeficienty koeficienty(std::vector<QPointF> Body, Pole &riesenie, size_t i) const;
    Komponenty::SpojenieSlot* druhySlot(Komponenty::SpojenieSlot* slot);
    Komponenty::SpojenieSlot* druhyKomponentVSpojeni(Komponenty::SpojenieSlot* slot);
    std::vector<Spline*> najdiSamostatne();
    std::vector<Komponenty::KomponentPtr>& _komponenty;
    std::vector<std::shared_ptr<Komponenty::Komponent>>& _spojenia;
};

}

#endif // SPLINEGROUP_H
