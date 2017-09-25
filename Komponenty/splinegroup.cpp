#include "splinegroup.h"
#include <algorithm>
#include "manipulator.h"
#include <QVector>
#include <iterator>
#include <QtMath>

using namespace Komponenty;

void SplineGroup::resetujSpliny()
{
    for(auto&& k : _komponenty){
        if(auto spline = dynamic_cast<Spline*>(k.get()))
            spline->ResetKrivka();
    }
}

void SplineGroup::najdiAVypocitajUseky()
{
    for(auto&& k : _komponenty){
        if(auto spline = dynamic_cast<Spline*>(k.get()))
        {
            if(!spline->Pouzite()){
                auto cesta = najdiCestu(spline);

                if(cesta.size() >= 2)
                    vypocitajSpline(cesta, true);
            }
        }
    }
}

void SplineGroup::vypocitajSamostatneSpliny(std::vector<Spline*>&& samostatne)
{
    for(auto&& spline : samostatne){
        std::vector<SpojenieSlot*> cesta;
        for(auto&& slot : spline->SpojenieSloty())
        {
            cesta.push_back(slot.get());
            nastavSmer(slot.get());
        }
        vypocitajSpline(cesta, false);
    }
}

void SplineGroup::resetujPouzitie()
{
    for(auto&& k : _komponenty){
        if(auto spline = dynamic_cast<Spline*>(k.get()))
            spline->setPouzite(false);
    }
}

SplineGroup::SplineGroup(std::vector<Komponenty::KomponentPtr> &komponenty, std::vector<KomponentPtr> &spojenia)
    : _komponenty(komponenty), _spojenia(spojenia)
{
    //resetuj vypocitane spliny
    resetujSpliny();

    //najdi spliny, ktore sa maju vypocitat samostatne
    auto samostatne = najdiSamostatne();

    //najdi hlavne cesty
    najdiAVypocitajUseky();

    vypocitajSamostatneSpliny(std::move(samostatne));

    resetujPouzitie();
}

std::vector<SpojenieSlot *> SplineGroup::najdiCestu(Spline *spline)
{
    auto slot = spline->SpojenieSloty().at(0).get();
    auto slot2 = druhyKomponentVSpojeni(spline->SpojenieSloty().at(0).get());

    //mohlo sa stat, ze spline nie je na zaciatku cesty
    //hladame teda zaciatok
    if(slot2 && dynamic_cast<Spline*>(slot2->Vlastnik()))
    {
        //zaciatok posuvame pokial sa nam v jednej iteracii nepodari predlzit cestu
        auto cesta2 = najdiCestu(slot2);
        auto cesta2n = cesta2;
        if(cesta2.size() > 1)
        {
            size_t i = 0; //ochrana proti zacykleniu
            do
            {
                for(auto s : cesta2n)
                {
                    if(auto sp = dynamic_cast<Spline*>(s->Vlastnik()))
                        sp->setPouzite(false);
                }
                auto c = cesta2n;
                cesta2n = najdiCestu(cesta2.back());
                cesta2 = c;
            }
            while(cesta2.size() > 1 && cesta2n != cesta2 && i++ < 1000);
            return cesta2;
        }
    }

    //ak je spline zaciatok cesty
    return najdiCestu(slot);
}

std::vector<SpojenieSlot *> SplineGroup::najdiCestu(SpojenieSlot *slot)
{
    std::vector<SpojenieSlot*> cesta;

    cesta.push_back(slot);

    while (auto spline = dynamic_cast<Spline*>(slot->Vlastnik())) {
        if(spline->Pouzite())
            break;
        else
        {
            spline->setPouzite(true);

            //druhy slot aktualneho useku
            slot = druhySlotKomponentu(slot);

            if(auto spojenie = dynamic_cast<Spojenie*>(slot->Spojenie())){
                auto sloty = spojenie->_spojenieZoznamVlastnost->Hodnota();
                if(sloty.size() <= 1)
                {
                    cesta.push_back(slot);
                    break;
                }
                else{
                    if(sloty.at(0)->Vlastnik() != spline)
                        slot = sloty.at(0);
                    else if(sloty.at(1)->Vlastnik() != spline)
                        slot = sloty.at(1);
                    cesta.push_back(slot);
                }
            }
            else
            {
                cesta.push_back(slot);
                break;
            }
        }
    }

    return cesta;
}

int SplineGroup::poradieSlotuVSpojeni(SpojenieSlot *slot)
{
    auto s = slot->Spojenie();
    size_t pos = 0;
    if(auto spojenie = dynamic_cast<Spojenie*>(s))
    {
        for(auto& sl : spojenie->_spojenieZoznamVlastnost->Hodnota())
        {
            if(slot == sl)
                return static_cast<int>(pos);
            pos++;
        }
    }
    return -1;
}

TridiagonalnaMatica SplineGroup::zostavMaticu(std::vector<QPointF> body, std::vector<SpojenieSlot*> sloty, bool kopirujSmer)
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

    if(!kopirujSmer)
        matica.d(0) = 3 * (body[1] - body[0]);
    else
        matica.d(0) = 3 * (sloty.at(0)->Smer());

    matica.b(n) = QPointF(2,2);

    if(!kopirujSmer)
        matica.d(n) = 3. * (body[n] - body[n - 1]);
    else
        matica.d(n) = - 3. * (sloty.back()->Smer());


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
    if(auto spojenie = dynamic_cast<Spojenie*>(slot->Spojenie()))
    {
        auto sloty = spojenie->_spojenieZoznamVlastnost->Hodnota();
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

void SplineGroup::vypocitajSpline(std::vector<SpojenieSlot *> cesta, bool nastavSmer)
{
    std::vector<QPointF> body;

    for(auto slot : cesta)
        body.push_back(slot->Bod());

    //zisti, ci je cesta uzavrena - tvori cyklus
    if(cesta.at(0) == cesta.back())
        vypocitajKrivkuSplinu(body, cesta, zostavUzavrenuMaticu(body).Vyries(), true, nastavSmer);
    else
        vypocitajKrivkuSplinu(body, cesta, zostavMaticu(body, cesta, !nastavSmer).Vyries(), false, nastavSmer);
}

void SplineGroup::vypocitajKrivkuSplinu(std::vector<QPointF> body, std::vector<SpojenieSlot *> cesta, Pole riesenie, bool uzavrena, bool nastavSmer)
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
            auto k = Koeficienty(body, riesenie, i);

            if(nastavSmer)
            {
                cesta.at(i)->setSmer([k](){
                    return -std::get<1>(k);
                });
                if(auto slot2 = druhyKomponentVSpojeni(cesta.at(i)))
                    slot2->setSmer([k](){
                        return std::get<1>(k);
                    });

                cesta.at((i + 1) % body.size())->setSmer([k](){
                    return -QPointF(std::get<1>(k).x() + 2 * std::get<2>(k).x()  + 3 * std::get<3>(k).x(),
                                    std::get<1>(k).y() + 2 * std::get<2>(k).y()  + 3 * std::get<3>(k).y());
                });
                if(auto slot2 = druhyKomponentVSpojeni(cesta.at((i + 1) % body.size())))
                    slot2->setSmer([k](){
                        return QPointF(std::get<1>(k).x() + 2 * std::get<2>(k).x()  + 3 * std::get<3>(k).x(),
                                       std::get<1>(k).y() + 2 * std::get<2>(k).y()  + 3 * std::get<3>(k).y());
                    });
            }

            //zistime vzdialenost medzi bodmi
            qreal dl = qSqrt(qPow(body[(i + 1) % body.size()].x() - body[i].x(), 2) +
                    qPow(body[(i + 1) % body.size()].y() - body[i].y(), 2));
            //v pripade, ze su body  vo vzdialenosti menej ako 100, pozadujeme aspon vzdialenost 300
            //casto byvaju pri takychto bodoch "uska" a tie su potom "polamane"
            dl = qMax(300., dl);
            //upravime dlzku kroku
            qreal kr = 5 / dl; //jeden krok bude mat priblizne 5 mm
            //polynom je zadany parametrom t na [0,1]
            qreal t = 0;
            do
            {
                QPointF b = QPointF(std::get<0>(k).x() + std::get<1>(k).x() * t + std::get<2>(k).x() * t * t + std::get<3>(k).x() * t * t * t,
                                    std::get<0>(k).y() + std::get<1>(k).y() * t + std::get<2>(k).y() * t * t + std::get<3>(k).y() * t * t * t);
                krivka.append(b);
                t += kr;
            }
            while (t <= 1);
            krivka.append((QPointF)body[(i + 1) % body.size()]);

            if(auto spline = dynamic_cast<Spline*>(cesta.at(i)->Vlastnik()))
                spline->setKrivka(krivka);
        }
    }
}

Koeficienty SplineGroup::Koeficienty(std::vector<QPointF>& body, Pole& riesenie, size_t i)
{
    QPointF a, b, c, d;

    a = body[i];

    b = riesenie[i];

    c = 3. * (body[(i + 1)% body.size()] - body[i]) - 2. * riesenie[i] - riesenie[(i + 1)% body.size()];

    d = 2. * (body[i] - body[(i + 1)% body.size()]) + riesenie[i] + riesenie[(i + 1)% body.size()];

    return std::make_tuple(a, b, c, d);
}

SpojenieSlot *SplineGroup::druhySlotKomponentu(SpojenieSlot *slot)
{
    if(auto spline = dynamic_cast<Spline*>(slot->Vlastnik()))
    {
        if(spline->SpojenieSloty().at(0).get() == slot)
            slot = spline->SpojenieSloty().at(1).get();
        else if(spline->SpojenieSloty().at(1).get() == slot)
            slot = spline->SpojenieSloty().at(0).get();
    };

    return slot;
}

SpojenieSlot *SplineGroup::druhyKomponentVSpojeni(SpojenieSlot *slot)
{
    if(auto spojenie = dynamic_cast<Spojenie*>(slot->Spojenie()))
    {
        auto sloty = spojenie->_spojenieZoznamVlastnost->Hodnota();
        if(sloty.size() > 1)
        {
            if(slot == sloty.at(0))
                return sloty.at(1);
            else if(slot == sloty.at(1))
                return sloty.at(0);
        }
    }

    return nullptr;
}

std::vector<Spline *> SplineGroup::najdiSamostatne()
{
    std::vector<Spline*> samostatne;

    //najdi spliny susediace s komponentom, ktory nie je spline
    //alebo ich poradie v spojeni je vacsie ak 2
    for(auto&& k : _komponenty){
        if(auto spline = dynamic_cast<Spline*>(k.get()))
        {
            auto slot1 = druhyKomponentVSpojeni(spline->SpojenieSloty().at(0).get());
            auto slot2 = druhyKomponentVSpojeni(spline->SpojenieSloty().at(1).get());

            if((!(spline->SpojenieSloty().at(0)->JeVolny() && spline->SpojenieSloty().at(1)->JeVolny()))
                    &&
                    (
                        (poradieSlotuVSpojeni(spline->SpojenieSloty().at(0).get()) > 1)
                        || (slot1 && !dynamic_cast<Spline*>(slot1->Vlastnik()))
                        || (poradieSlotuVSpojeni(spline->SpojenieSloty().at(1).get()) > 1)
                        || (slot2 && !dynamic_cast<Spline*>(slot2->Vlastnik()))
                        ))
            {
                samostatne.push_back(spline);
                spline->setPouzite(true);
            }
        }
    }

    return samostatne;
}

