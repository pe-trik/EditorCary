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
            std::vector<SpojenieSlot*> cesta;
            if(sloty.at(0)->JeVolny() || poradieSlotuVSpojeni(sloty.at(0).get()) < 2)
                cesta = najdiCestu(sloty.at(0).get());
            else if(sloty.at(1)->JeVolny()|| poradieSlotuVSpojeni(sloty.at(1).get()) < 2)
                cesta = najdiCestu(sloty.at(1).get());

            if(cesta.size() > 2)
                vypocitajSpline(cesta);
        }
    }

    for(auto&& k : komponenty){
        if(auto spline = dynamic_cast<Spline*>(k.get()))
            spline->setPouzite(false);
    }
}

std::vector<SpojenieSlot *> SplineGroup::najdiCestu(SpojenieSlot *slot)
{
    std::vector<SpojenieSlot*> cesta;

    nastavSmer(slot);

    while (auto spline = dynamic_cast<Spline*>(slot->komponent())) {
        if(spline->pouzite()){
            //uzavrena cesta
            if(cesta.size() > 0 && cesta.at(0) == slot)
                cesta.push_back(slot);
            break;
        }
        else
        {
            cesta.push_back(slot);
            spline->setPouzite(true);

            slot = druhySlot(slot);

            if(auto spojenie = dynamic_cast<Spojenie*>(slot->spojenie())){
                auto sloty = spojenie->_spojenieZoznamVlastnost->hodnota();
                if(sloty.size() <= 1)
                {
                    cesta.push_back(slot);
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
                cesta.push_back(slot);
                nastavSmer(slot);
                break;
            }
        }
    }

    return cesta;
}

size_t SplineGroup::poradieSlotuVSpojeni(SpojenieSlot *slot)
{
    auto s = slot->spojenie();
    size_t pos = 0;
    if(auto spojenie = dynamic_cast<Spojenie*>(s))
    {
        for(auto& sl : spojenie->_spojenieZoznamVlastnost->hodnota())
        {
            if(slot == sl)
                return pos;
            pos++;
        }
    }
    return -1;
}

TridiagonalnaMatica SplineGroup::zostavMaticu(std::vector<QPointF> body)
{
    size_t n = body.size() - 1;

    TridiagonalnaMatica matica(n + 1,
                               QPointF(1,1),
                               QPointF(4,4),
                               QPointF(1,1),
                               QPointF(0,0),
                               QPointF(0,0),
                               QPointF(0,0));

    for (size_t i = 1; i < n; i++)
        matica.d(i) = 3.*(body[i + 1] - body[i - 1]);

        matica.b(0) = QPointF(2,2);
        matica.d(0) = 3 * (body[1] - body[0]);

        matica.b(n) = QPointF(2,2);
        matica.d(n) = 3. * (body[n] - body[n - 1]);


    return matica;
}

TridiagonalnaMatica SplineGroup::zostavUzavrenuMaticu(std::vector<QPointF>& body)
{
    body.erase(body.end() - 1);
    size_t n = body.size() - 1;

    TridiagonalnaMatica matica(n + 1,
                               QPointF(1,1),
                               QPointF(4,4),
                               QPointF(1,1),
                               QPointF(0,0),
                               QPointF(1,1),
                               QPointF(1,1));

    for (size_t i = 1; i < n; i++)
        matica.d(i) = 3.*(body[i + 1] - body[i - 1]);

    matica.d(0) = 3 * (body[1] - body[n]);

    matica.d(n) = 3. * (body[0] - body[n - 1]);

    return matica;
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

void SplineGroup::vypocitajSpline(std::vector<SpojenieSlot *> cesta)
{
    std::vector<QPointF> body;

    for(auto slot : cesta)
        body.push_back(slot->bod());

    if(body.at(0) == body.back())
        vykresliSpline(body, cesta, zostavUzavrenuMaticu(body).Vyries(), true);
    else
        vykresliSpline(body, cesta, zostavMaticu(body).Vyries(), false);
}

void SplineGroup::vykresliSpline(std::vector<QPointF> body, std::vector<SpojenieSlot *> cesta, Pole riesenie, bool uzavrena)
{
    if (riesenie != 0)
        {
            //prechadzame cez vsetky dvojice bodov a vykreslujeme medzi nimi polynomi
            for (size_t j = 0; j + !uzavrena < body.size(); j++)
            {
                size_t i = j % body.size();
                QVector<QPointF> krivka;
                krivka.append(body[i]);
                //koeficienty pre polynomi (pre Xovu aj Yovu os)
                auto k = koeficienty(body, riesenie, i);

                //zistime vzdialenost medzi bodmi
                qreal dl = qSqrt(qPow(body[(i + 1) % body.size()].x() - body[i].x(), 2) +
                        qPow(body[(i + 1) % body.size()].y() - body[i].y(), 2));
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
                krivka.append((QPointF)body[(i + 1) % body.size()]);
                if(auto spline = dynamic_cast<Spline*>(cesta.at(i)->komponent()))
                    spline->setKrivka(krivka);
            }
        }
}

Koeficienty SplineGroup::koeficienty(std::vector<QPointF> body, Pole& riesenie, size_t i) const
{
    QPointF a, b, c, d;

    a = body[i];

    b = riesenie[i];

    c = 3. * (body[(i + 1)% body.size()] - body[i]) - 2. * riesenie[i] - riesenie[(i + 1)% body.size()];

    d = 2. * (body[i] - body[(i + 1)% body.size()]) + riesenie[i] + riesenie[(i + 1)% body.size()];

    return std::make_tuple(a, b, c, d);
}

SpojenieSlot *SplineGroup::druhySlot(SpojenieSlot *slot)
{
    if(auto spline = dynamic_cast<Spline*>(slot->komponent()))
    {
        if(spline->SpojenieSloty().at(0).get() == slot)
            slot = spline->SpojenieSloty().at(1).get();
        else if(spline->SpojenieSloty().at(1).get() == slot)
            slot = spline->SpojenieSloty().at(0).get();
    }

    return slot;
}

