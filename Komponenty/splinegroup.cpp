#include "splinegroup.h"

using namespace Komponenty;

SplineGroup::SplineGroup(std::vector<Komponenty::KomponentPtr> &komponenty, std::vector<std::shared_ptr<Komponenty::Komponent> > &spojenia)
    : _komponenty(komponenty), _spojenia(spojenia)
{
    for(auto&& s : spojenia)
    {
        if(auto spojenie = dynamic_cast<Spojenie*>(s.get()))
        {
            for(auto k : spojenie->_spojenieZoznamVlastnost->hodnota())
            {
                if(auto spline = dynamic_cast<Spline*>(k->komponent()))
                {
                    auto cesta = najdiCestu(k);
                }
            }
        }
    }
}

std::vector<Spline *> SplineGroup::najdiCestu(SpojenieSlot *slot)
{
    std::vector<Spline*> cesta;
    while (auto spline = dynamic_cast<Spline*>(slot->komponent())) {
        cesta.push_back(spline);
        spline->setPouzite(true);

        if(spline->SpojenieSloty().at(0).get() == slot)
            slot = spline->SpojenieSloty().at(1).get();
        else
            slot = spline->SpojenieSloty().at(0).get();

        auto spojenie = slot->spojenie();


    }
}
