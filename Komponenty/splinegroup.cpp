#include "splinegroup.h"
#include <algorithm>
#include "manipulator.h"
#include <QVector>
#include <QtMath>

using namespace Komponenty;

#include <stack>

SplineGroup::SplineGroup(std::vector<Komponenty::KomponentPtr> &komponenty, std::vector<std::shared_ptr<Komponenty::Komponent> > &spojenia)
    : _komponenty(komponenty), _spojenia(spojenia)
{
    for(auto&& k : komponenty){
        if(auto spline = dynamic_cast<Spline*>(k.get()))
        {
            auto&& sloty = spline->SpojenieSloty();
            std::vector<Spline*> cesta;
            if(sloty.at(0)->JeVolny())
                cesta = najdiCestu(sloty.at(0).get());
            else
                cesta = najdiCestu(sloty.at(0).get());

            if(cesta.size() > 1)
                vypocitajSpline(cesta);
        }
    }

    for(auto&& k : komponenty){
        if(auto spline = dynamic_cast<Spline*>(k.get()))
            spline->setPouzite(false);
    }
}

std::vector<Spline *> SplineGroup::najdiCestu(SpojenieSlot *slot)
{
    std::vector<Spline*> cesta;

    nastavSmer(slot);

    while (auto spline = dynamic_cast<Spline*>(slot->komponent())) {
        if(spline->pouzite()){
            //uzavrena cesta
            if(cesta.size() > 0 && cesta.at(0) == spline)
                cesta.push_back(spline);
            break;
        }
        else
        {
            cesta.push_back(spline);
            spline->setPouzite(true);

            slot = druhySlot(spline, slot);

            if(auto spojenie = dynamic_cast<Spojenie*>(slot->spojenie())){
                auto sloty = spojenie->_spojenieZoznamVlastnost->hodnota();
                if(sloty.size() <= 1)
                {
                    nastavSmer(slot);
                    break;
                }
                else{
                    if(sloty.at(0)->komponent() != spline)
                        slot = sloty.at(0);
                    else if(sloty.at(1)->komponent() != spline)
                        slot = sloty.at(1);
                }
            }
            else
            {
                nastavSmer(slot);
                break;
            }
        }
    }

    return cesta;
}

void SplineGroup::nastavSmer(SpojenieSlot *slot)
{
    if(auto spojenie = dynamic_cast<Spojenie*>(slot->spojenie()))
    {
        auto sloty = spojenie->_spojenieZoznamVlastnost->hodnota();
        if(sloty.size() >= 2 && sloty.at(0) != slot)
        {
            auto smer = sloty.at(0)->Smer();
            slot->setSmer( [smer](){return smer;});
        }
        else if(sloty.size() >= 2 && sloty.at(1) != slot)
        {
            auto smer = sloty.at(1)->Smer();
            slot->setSmer( [smer](){return smer;});
        }
        else
            slot->setSmer( [](){return QPointF(0,0);});
    }
}

void SplineGroup::vypocitajSpline(std::vector<Spline *> cesta)
{
    std::vector<SpojenieSlot*> sloty;

    SpojenieSlot* slot;
    if(cesta.at(0)->SpojenieSloty().at(0)->JeVolny())
        slot = cesta.at(0)->SpojenieSloty().at(0).get();
    else
        slot = cesta.at(0)->SpojenieSloty().at(1).get();

    sloty.push_back(slot);

    for(auto spline : cesta)
    {
        if(spline->SpojenieSloty().at(0)->bod() != sloty.back()->bod())
            sloty.push_back(spline->SpojenieSloty().at(0).get());
        else
            sloty.push_back(spline->SpojenieSloty().at(1).get());
    }

    size_t n = sloty.size() - 1;

    std::vector<QPointF> body;

    for(auto slot : sloty)
    {
        body.push_back(slot->bod());
    }

    //zostavime maticu pre Yovu suradnicu

    TridiagonalnaMatica matica(n + 1, 1, 4, 1, 0);

    for (size_t i = 1; i < n; i++)
        matica.d(i) = 3.*(body[i + 1].y() - body[i - 1].y());

    matica.b(0) = 2;
    matica.d(0) = 3 * (body[1].y() - body[0].y());

    matica.b(n) = 2;
    matica.d(n) = 3. * (body[n].y() - body[n - 1].y());

    auto GJRiesenieY = matica.Vyries();

    matica = TridiagonalnaMatica(n + 1, 1, 4, 1, 0);

    for (size_t i = 1; i < n; i++)
        matica.d(i) = 3.*(body[i + 1].x() - body[i - 1].x());

    matica.b(0) = 2;
    matica.d(0) = 3 * (body[1].x() - body[0].x());

    matica.b(n) = 2;
    matica.d(n) = 3. * (body[n].x() - body[n - 1].x());

    auto GJRiesenieX = matica.Vyries();

    vykresliSpline(body, cesta, std::move(GJRiesenieX), std::move(GJRiesenieY));
}

void SplineGroup::vykresliSpline(std::vector<QPointF> body, std::vector<Spline *> cesta, Pole riesenieX, Pole riesenieY)
{
    if (riesenieX != 0 && riesenieY != 0)
        {
            //prechadzame cez vsetky dvojice bodov a vykreslujeme medzi nimi polynomi
            for (size_t i = 0; (i + 1) < body.size(); i++)
            {
                QVector<QPointF> krivka;
                krivka.append(body[i]);
                //koeficienty pre polynomi (pre Xovu aj Yovu os)
                auto k = koeficienty(body, riesenieX, riesenieY, i);

                //zistime vzdialenost medzi bodmi
                qreal dl = qSqrt(qPow(body[i + 1].x() - body[i].x(), 2) +
                        qPow(body[i + 1].y() - body[i].y(), 2));
                //v pripade, ze su body  vo vzdialenosti menej ako 1, pozadujeme aspon vzdialenost 3
                //casto byvaju pri takychto bodoch "uska" a tie su potom "polamane"
                dl = qMax(3., dl);
                //upravime dlzku kroku
                //qreal kr = krok / dl;
                qreal kr = 0.01;
                //polynom je zadany parametrom t na [0,1]
                qreal t = 0;
                do
                {
                    krivka.append(QPointF(std::get<0>(k).x() + std::get<1>(k).x() * t + std::get<2>(k).x() * t * t + std::get<3>(k).x() * t * t * t,
                             std::get<0>(k).y() + std::get<1>(k).y() * t + std::get<2>(k).y() * t * t + std::get<3>(k).y() * t * t * t));
                    t += kr;
                }
                while (t <= 1);
                krivka.append((QPointF)body[i + 1]);
                cesta.at(i)->setKrivka(krivka);
            }
        }
}

Koeficienty SplineGroup::koeficienty(std::vector<QPointF> body, Pole& riesenieX, Pole& riesenieY, size_t i) const
{
    QPointF a, b, c, d;

    a = (body[i]);

    b = QPointF(riesenieX[i], riesenieY[i]);

    c = QPointF(3. * (body[i + 1].x() - body[i].x()) - 2. * riesenieX[i] - riesenieX[i + 1],
            3. * (body[i + 1].y() - body[i].y()) - 2. * riesenieY[i] - riesenieY[i + 1]);

    d = QPointF(2. * (body[i].x() - body[i + 1].x()) + riesenieX[i] + riesenieX[i + 1],
            2. * (body[i].y() - body[i + 1].y()) + riesenieY[i] + riesenieY[i + 1]);

    return std::make_tuple(a, b, c, d);
}

SpojenieSlot *SplineGroup::druhySlot(Spline *spline, SpojenieSlot *slot)
{
    if(spline->SpojenieSloty().at(0).get() == slot)
        slot = spline->SpojenieSloty().at(1).get();
    else
        slot = spline->SpojenieSloty().at(0).get();

    return slot;
}

TridiagonalnaMatica::TridiagonalnaMatica(size_t m, qreal a, qreal b, qreal c, qreal d, qreal tr, qreal ll)
{
    _tr = tr;
    _ll = ll;
    _m = m;

    //alokovanie vektorov
    _a = std::make_unique<qreal[]>(m);
    _b = std::make_unique<qreal[]>(m);
    _c = std::make_unique<qreal[]>(m);
    _d = std::make_unique<qreal[]>(m);
    _rc = std::make_unique<qreal[]>(m);
    _lc = std::make_unique<qreal[]>(m);

    //inicializacia
    for(size_t i = 0; i < m; i++)
    {
        rc(i) = 0;
        lc(i) = 0;
        this->a(i) = a;
        this->b(i) = b;
        this->c(i) = c;
        this->d(i) = d;
    }

    rc(0) = tr;
    lc(_m - 1) = ll;
}

//riesenie tridiagonalnej matice v O(n)
Pole TridiagonalnaMatica::Vyries()
{
    //matica musi mat nenulovu velkost
    if (_m == 0)
        return 0;

    auto riesenie = std::make_unique<double[]>(_m);

    //odstranenie spodnej diagonaly - po posledne 2 riadky bez konfliktov
    for (size_t i = 1; i < _m; i++)
    {
        double koef = a(i) / b(i - 1);
        a(i) -= koef * b(i - 1);
        if (i < _m - 1)
            b(i) -= koef * c(i - 1);
        d(i) -= koef * d(i - 1);
        rc(i) -= koef * rc(i - 1);
    }

    //vynormovanie
    for (size_t i = 0; i < _m; i++)
    {
        double koef = b(i);
        if(i > 1)
            lc(i) /= koef;
        c(i) /= koef;
        if(i < _m - 2)
            rc(i) /= koef;
        d(i) /= koef;
        b(i) /= koef;
    }

    //odstranenie praveho stlpca
    //vytvori sa lavy
    for (size_t i = 0; i < _m - 1; i++)
    {
        double koef = rc(i);
        lc(i) -= lc(_m - 1) * koef;
        d(i) -= d(_m - 1) * koef;
        rc(i) -= rc(_m - 1) * koef;
    }

    //odstranenie hornej diagonaly
    for (size_t i = 0; i < _m - 1; i++)
    {
        double koef = -c(i) * b(i + 1);
        c(i) = 0;
        lc(i) += koef * lc(i + 1);
        d(i) += koef * d(i + 1);
    }

    //vynormovanie prveho riadku
    d(0) /= lc(0);
    lc(0) /= lc(0);

    //odstranenie laveho stlpca
    for (size_t i = 1; i < _m; i++)
    {
        double koef = lc(i);
        lc(i) -= lc(0) * koef;
        d(i) -= d(0) * koef;
    }

    for (size_t i = 0; i < _m; i++)
        riesenie[i] = d(i);

    return riesenie;
}

qreal &TridiagonalnaMatica::a(size_t i) { return _a[i]; }

qreal &TridiagonalnaMatica::b(size_t i) { return _b[i]; }

qreal &TridiagonalnaMatica::c(size_t i) { return _c[i]; }

qreal &TridiagonalnaMatica::d(size_t i) { return _d[i]; }

qreal &TridiagonalnaMatica::rc(size_t i)
{
    if(i < _m - 2)
        return  _rc[i];
    else if (i == _m - 2)
        return _c[i];
    else if (i == _m - 1)
        return _b[i];
}

qreal &TridiagonalnaMatica::lc(size_t i)
{
    if(i > 1)
        return _lc[i];
    if(i == 0)
        return _b[0];
    else if(i == 1)
        return _a[1];
}
