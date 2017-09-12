#ifndef SPLINEGROUP_H
#define SPLINEGROUP_H

#include "spojenie.h"
#include "spline.h"
#include "spojenieslot.h"


namespace Komponenty {
class SplineGroup
{
public:
    SplineGroup(std::vector<Komponenty::KomponentPtr>& komponenty,
                std::vector<std::shared_ptr<Komponenty::Komponent>>& spojenia);
private:
    std::vector<Komponenty::Spline*> najdiCestu(Komponenty::SpojenieSlot* slot);
    std::vector<Komponenty::KomponentPtr>& _komponenty;
    std::vector<std::shared_ptr<Komponenty::Komponent>>& _spojenia;
};
}

#endif // SPLINEGROUP_H
