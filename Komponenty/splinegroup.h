#ifndef SPLINEGROUP_H
#define SPLINEGROUP_H

#include "spojenie.h"
#include "spline.h"
#include "spojenieslot.h"


namespace Komponenty {

typedef std::unique_ptr<qreal[]> Pole;
typedef std::tuple<QPointF, QPointF, QPointF, QPointF> Koeficienty;

class SplineGroup
{
public:
    SplineGroup(std::vector<Komponenty::KomponentPtr>& komponenty,
                std::vector<std::shared_ptr<Komponenty::Komponent>>& spojenia);
private:
    std::vector<Komponenty::Spline*> najdiCestu(Komponenty::SpojenieSlot* slot);
    void nastavSmer(Komponenty::SpojenieSlot *slot);
    void vypocitajSpline(std::vector<Komponenty::Spline*> cesta);
    void vykresliSpline(std::vector<QPointF> Body, std::vector<Komponenty::Spline*> cesta, Pole riesenieX, Pole riesenieY);
    Koeficienty koeficienty(std::vector<QPointF> Body, Pole &riesenieX, Pole &riesenieY, size_t i) const;
    Komponenty::SpojenieSlot* druhySlot(Komponenty::Spline* spline, Komponenty::SpojenieSlot* slot);
    std::vector<Komponenty::KomponentPtr>& _komponenty;
    std::vector<std::shared_ptr<Komponenty::Komponent>>& _spojenia;
};

class TridiagonalnaMatica
{
public:
    TridiagonalnaMatica(size_t m, qreal a = 0, qreal b = 0, qreal c = 0, qreal d = 0, qreal tr = 0, qreal ll = 0);
    Pole Vyries();
    qreal& a(size_t i) { return _a[i]; }
    qreal& b(size_t i) { return _b[i]; }
    qreal& c(size_t i) { return _c[i]; }
    qreal& d(size_t i) { return _d[i]; }
    qreal& rc(size_t i) { return _rc[i]; }
private:
    size_t _m;
    qreal _tr;
    qreal _ll;
    //pretoze matica je tvorena 3 diagonalami a "poslednym" stlpcom matice
    //staci mat 4 vektory velkosti m
    Pole _a ,
    _b,
    _c,
    _d,
    _rc,
    _lc;//pravy a lavy stlpec

};


}

#endif // SPLINEGROUP_H
