#include "kubickyspline.h"
#include "Nastroje/manipulator.h"
#include <qmath.h>
#include <QDebug>

using namespace Komponenty;

const char* Nastroje::ParametreNastroja<Nastroje::Dvojbodovy<Komponenty::KubickySpline>>::nazov = "Kubický spline";
const char* Nastroje::ParametreNastroja<Nastroje::Dvojbodovy<Komponenty::KubickySpline>>::ikonka = ":/ikonky/Nastroje/ikonky/spline.svg";

size_t KubickySpline::max_id = 0;

KubickySpline::KubickySpline(Dokumenty::Dokument *dokument)
{
    nastavVlastnosti();
    id->setHodnota("spline" + QString::number(++max_id));
    Dokument = dokument;
    PripojneBody.push_back(std::make_unique<Nastroje::Manipulator>(Zaciatok, SmerZaciatok, true, this, true));
    PripojneBody.push_back(std::make_unique<Nastroje::Manipulator>(Koniec, SmerKoniec, true, this, true));
}

KubickySpline::KubickySpline(Dokumenty::Dokument* dokument, QDomElement &e)
{
    nastavVlastnosti();
    id->setHodnota(e.attribute("id"));
    ++max_id;
    QString num;
    foreach(QChar c, id->hodnota()) {
        if (c.isDigit()) {
            num.append(c);
        }
    }
    bool ok;
    size_t ret = num.toULong(&ok);
    if (ok) {
        max_id = ret;
    }

    Zaciatok = QPointF(e.attribute("x1").toDouble(), e.attribute("y1").toDouble());
    Koniec = QPointF(e.attribute("x2").toDouble(), e.attribute("y2").toDouble());

    Dokument = dokument;
    auto m = e.firstChildElement();
    while (!m.isNull()) {
        if(m.attribute("poradie") == "0"){
            PripojneBody.push_back(std::make_unique<Nastroje::Manipulator>(m, Zaciatok, SmerZaciatok, true, this, true));
        }
        else if(m.attribute("poradie") == "1"){
            PripojneBody.push_back(std::make_unique<Nastroje::Manipulator>(m, Koniec, SmerKoniec, true, this, true));
        }
        m = m.nextSiblingElement();
    }
}

bool KubickySpline::Obsahuje(QPointF bod) const
{
    return Hranice.contains(bod);
}

void KubickySpline::Vykresli(QPainter &p) const
{    
    Qt::PenStyle style = viditelne->hodnota() ? Qt::SolidLine : Qt::DashLine;
    QColor farba = (validne || Dokument->Tlac->hodnota()) ? this->farba->hodnota() : Qt::red;
    p.setPen(QPen(farba, hrubkaCiary, style, Qt::FlatCap));

    if(!viditelne->hodnota() && Dokument->Tlac->hodnota())
        return;

    p.drawPolyline(Krivka);

    for(auto && pb : PripojneBody)
        pb->Vykresli(p);
}

QDomElement KubickySpline::Uloz(QDomDocument &doc) const
{
    auto e = doc.createElement("komponent");

    e.setAttribute("nazov", "spline");
    e.setAttribute("id", ID());

    e.setAttribute("x1", Zaciatok.x());
    e.setAttribute("y1", Zaciatok.y());
    e.setAttribute("x2", Koniec.x());
    e.setAttribute("y2", Koniec.y());

    size_t i = 0;
    for(auto && pb : PripojneBody){
        auto m = doc.createElement("manipulator");
        m.setAttribute("poradie", i++);
        m.setAttribute("id", pb->ID());
        for(auto && c : pb->Children()){
            auto ch = doc.createElement("spojenie");
            ch.setAttribute("idm", c->ID());
            ch.setAttribute("idk", c->Vlastnik->ID());
            m.appendChild(ch);
        }
        e.appendChild(m);
    }

    return e;
}

std::unique_ptr<Nastroje::INastrojPresenter> KubickySpline::Nastroj() const
{
    return std::unique_ptr<Nastroje::INastrojPresenter>(new Nastroje::NastrojPresenter<Nastroje::Dvojbodovy<KubickySpline>>());
}

void KubickySpline::Prepocitaj(bool o)
{
       Body.clear();
       Body.push_back(Zaciatok);
       Body.push_back(Koniec);

       size_t n = Body.size() - 1;

       //zostavime maticu pre Yovu suradnicu

       TridiagonalnaMatica matica(n + 1, 1, 4, 1, 0);

       for (size_t i = 1; i < n; i++)
           matica.d(i) = 3.*(Body[i + 1].y() - Body[i - 1].y());

       matica.b(0) = 2;
       matica.d(0) = 3 * (Body[1].y() - Body[0].y());

       matica.b(n) = 2;
       matica.d(n) = 3. * (Body[n].y() - Body[n - 1].y());

           if(PripojneBody.front()->manipulator() != 0 && (!PripojneBody.front()->jeMaster() || PripojneBody.front()->PrisposobitelnaZmena)){
                   //PripojneBody.front()->manipulator()->Vlastnik->Prepocitaj(false);
               matica.b(0) = 1;
               matica.c(0) = 0;
               matica.d(0) = PripojneBody.front()->manipulator()->zmena().y() * nasobok->hodnota();
           }
           if(PripojneBody.back()->manipulator() != 0 && (!PripojneBody.back()->jeMaster() || PripojneBody.back()->PrisposobitelnaZmena)){
                   //PripojneBody.back()->manipulator()->Vlastnik->Prepocitaj(false);
               matica.b(n) = 1;
               matica.a(n) = 0;
               matica.d(n) = -PripojneBody.back()->manipulator()->zmena().y() * nasobok2->hodnota();
           }

       GJRiesenieY = matica.Vyries();

       //zostavime maticu pre Xovu suradnicu

       matica = TridiagonalnaMatica(n + 1, 1, 4, 1, 0);

       for (size_t i = 1; i < n; i++)
           matica.d(i) = 3.*(Body[i + 1].x() - Body[i - 1].x());

       matica.b(0) = 2;
       matica.d(0) = 3 * (Body[1].x() - Body[0].x());

       matica.b(n) = 2;
       matica.d(n) = 3. * (Body[n].x() - Body[n - 1].x());

       if(PripojneBody.front()->manipulator() != 0 && (!PripojneBody.front()->jeMaster() || PripojneBody.front()->PrisposobitelnaZmena)){
           matica.b(0) = 1;
           matica.c(0) = 0;
           matica.d(0) = PripojneBody.front()->manipulator()->zmena().x() * nasobok->hodnota();
       }
       if(PripojneBody.back()->manipulator() != 0 && (!PripojneBody.back()->jeMaster() || PripojneBody.back()->PrisposobitelnaZmena)){
           matica.b(n) = 1;
           matica.a(n) = 0;
           matica.d(n) = -PripojneBody.back()->manipulator()->zmena().x() * nasobok2->hodnota();
       }

       GJRiesenieX = matica.Vyries();

       SmerZaciatok = -QPointF(GJRiesenieX[0], GJRiesenieY[0]);
       SmerKoniec = QPointF(GJRiesenieX[n], GJRiesenieY[n]);

       prepocitajKrivku();

       if(Krivka.size() > 0) {
       qreal x1 = LONG_MAX, y1 = LONG_MAX, x2 = -LONG_MAX, y2 = -LONG_MAX;
       for(auto && b : Krivka){
           x1 = qMin(x1, b.x());
           y1 = qMin(y1, b.y());
           x2 = qMax(x2, b.x());
           y2 = qMax(y2, b.y());
       }

       Hranice = QRectF(x1, y1, x2-x1, y2-y1);}
       else
           Hranice = QRectF(Zaciatok, Koniec);
       prepocitavam = false;
       kontrola();
       obnovVlastnosti();
}

void KubickySpline::prepocitajKrivku()
{
    //nova krivka
    Krivka.clear();
    //iba ak mame riesenie
    if (GJRiesenieX != 0 && GJRiesenieY != 0)
    {
        Krivka.append(Body[0]);
        //prechadzame cez vsetky dvojice bodov a vykreslujeme medzi nimi polynomi
        for (size_t i = 0; (i + 1) < Body.size(); i++)
        {
            //koeficienty pre polynomi (pre Xovu aj Yovu os)
            auto k = koeficienty(i);

            //zistime vzdialenost medzi bodmi
            qreal dl = qSqrt(qPow(Body[i + 1].x() - Body[i].x(), 2) +
                    qPow(Body[i + 1].y() - Body[i].y(), 2));
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
                Krivka.append(QPointF(std::get<0>(k).x() + std::get<1>(k).x() * t + std::get<2>(k).x() * t * t + std::get<3>(k).x() * t * t * t,
                         std::get<0>(k).y() + std::get<1>(k).y() * t + std::get<2>(k).y() * t * t + std::get<3>(k).y() * t * t * t));
                t += kr;
            }
            while (t <= 1);
            Krivka.append((QPointF)Body[i + 1]);
        }
    }
}

void KubickySpline::kontrola()
{
    qreal min = 100;
    validne = true;
    for (size_t i = 0; (i + 1) < Body.size(); i++)
    {
        //koeficienty pre polynomi (pre Xovu aj Yovu os)
        auto k = koeficienty(i);
        for(qreal t = 0.; t <= 1; t += 0.0009765625){
            auto prva = QPointF(3*t*t*std::get<0>(k).x() + 2*t*std::get<1>(k).x()+std::get<2>(k).x(),
                                3*t*t*std::get<0>(k).y() + 2*t*std::get<1>(k).y()+std::get<2>(k).y());
            auto druha = QPointF(6*t*std::get<0>(k).x() + 2*std::get<1>(k).x(),
                                6*t*std::get<0>(k).y() + 2*std::get<1>(k).y());
            qreal polomer = pow(prva.x()*prva.x() + prva.y()*prva.y(), 3./2.) /
                    abs(prva.x()*druha.y()-prva.y()*druha.x());
            qreal polomer2 = pow(prva.x()*prva.x() + prva.y()*prva.y(), 3./2.) /
                    abs(-prva.x()*druha.y()+prva.y()*druha.x());
            polomer = qMax(polomer, polomer2);
            min = qMin(polomer, min);
            if(polomer < 50){
                validne = false;
                return;
            }
        }
    }
    qDebug() << min;
}

void KubickySpline::obnovVlastnosti()
{
    return;
    x1->setHodnota(Zaciatok.x());
    y1->setHodnota(Zaciatok.y());
    x2->setHodnota(Koniec.x());
    y2->setHodnota(Koniec.y());

    dx->setHodnota((Koniec - Zaciatok).x());
    dy->setHodnota((Koniec - Zaciatok).y());
}

void KubickySpline::nastavVlastnosti()
{
    id = std::make_unique<StringVlastnost>("ID", "", [](QString){});
    x1 = std::make_unique<DoubleVlastnost>("X1", 0, [&](double v){Zaciatok = QPointF(v, Zaciatok.y()); PripojneBody[0]->Pohyb(Zaciatok); Dokument->Prepocitaj();});
    x2 = std::make_unique<DoubleVlastnost>("X2", 0, [&](double v){Koniec = QPointF(v, Koniec.y()); PripojneBody[1]->Pohyb(Koniec); Dokument->Prepocitaj();});
    y1 = std::make_unique<DoubleVlastnost>("Y1", 0, [&](double v){Zaciatok = QPointF(Zaciatok.x(), v); PripojneBody[0]->Pohyb(Zaciatok); Dokument->Prepocitaj();});
    y2 = std::make_unique<DoubleVlastnost>("Y2", 0, [&](double v){Koniec = QPointF(Koniec.x(), v); PripojneBody[1]->Pohyb(Koniec); Dokument->Prepocitaj();});
    nasobok = std::make_unique<DoubleVlastnost>("Násobok zmeny začiatok", 1, [&](double v){Dokument->Prepocitaj(); Dokument->Prekresli();});
    nasobok2 = std::make_unique<DoubleVlastnost>("Násobok zmeny koniec", 1, [&](double v){Dokument->Prepocitaj(); Dokument->Prekresli();});

    dx = std::make_unique<DoubleVlastnost>("dX", 0, [&](double v){Koniec = Zaciatok + QPointF(v,dy->hodnota()); PripojneBody[1]->Pohyb(Koniec); Dokument->Prepocitaj();});
    dy = std::make_unique<DoubleVlastnost>("dY", 0, [&](double v){Koniec = Zaciatok + QPointF(dx->hodnota(),v); PripojneBody[1]->Pohyb(Koniec); Dokument->Prepocitaj();});

    farba = std::make_unique<ColorVlastnost>("Farba", 0, [&](auto){Dokument->Prekresli();});
    viditelne = std::make_unique<BoolVlastnost>("Viditeľné", true, [&](auto){Dokument->Prekresli();});
    Vlastnosti.push_back(id.get());
    Vlastnosti.push_back(x1.get());
    Vlastnosti.push_back(y1.get());
    Vlastnosti.push_back(x2.get());
    Vlastnosti.push_back(y2.get());
    Vlastnosti.push_back(dx.get());
    Vlastnosti.push_back(dy.get());
    Vlastnosti.push_back(farba.get());
    Vlastnosti.push_back(viditelne.get());
    Vlastnosti.push_back(nasobok.get());
    Vlastnosti.push_back(nasobok2.get());
}


Koeficienty KubickySpline::koeficienty(size_t i) const
{
    //vypocet koeficientov
    //pre podrobnosti vid dokumentacia str. 4
    QPointF a, b, c, d;
    a = (Body[i]);

    b = QPointF(GJRiesenieX[i], GJRiesenieY[i]);

    c = QPointF(3. * (Body[i + 1].x() - Body[i].x()) - 2. * GJRiesenieX[i] - GJRiesenieX[i + 1],
            3. * (Body[i + 1].y() - Body[i].y()) - 2. * GJRiesenieY[i] - GJRiesenieY[i + 1]);

    d = QPointF(2. * (Body[i].x() - Body[i + 1].x()) + GJRiesenieX[i] + GJRiesenieX[i + 1],
            2. * (Body[i].y() - Body[i + 1].y()) + GJRiesenieY[i] + GJRiesenieY[i + 1]);
    return std::make_tuple(a, b, c, d);
}

TridiagonalnaMatica::TridiagonalnaMatica(size_t m, qreal a, qreal b, qreal c, qreal d)
{
    m_ = m;

    //alokovanie vektorov
    a_ = std::make_unique<qreal[]>(m);
    b_ = std::make_unique<qreal[]>(m);
    c_ = std::make_unique<qreal[]>(m);
    d_ = std::make_unique<qreal[]>(m);

    //inicializacia
    for(size_t i = 0; i < m; i++)
    {
        a_[i] = a;
        b_[i] = b;
        c_[i] = c;
        d_[i] = d;
    }
}

Pole TridiagonalnaMatica::Vyries()
{
    //riesenie tridiagonalnej matice v O(n)
    //pre blizsie informacie k algoritmu pozri dokumentaciu str. 5
    if(m_ == 0)
        return 0;

    c(0) /= b(0);
    d(0) /= b(0);

    auto riesenie = std::make_unique<qreal[]>(m_);

    //dopredna substitucia
    for(size_t i = 1; i < m_ - 1; i++)
        c(i) /= b(i) - a(i)*c(i-1);

    for(size_t i = 1; i < m_; i++)
        d(i) = (d(i) - a(i)*d(i-1))/(b(i) - a(i)*c(i-1));

    //spatna substitucia
    riesenie[m_ - 1] = d(m_ - 1);

    for(size_t i = 2; i <= m_; i++)
        riesenie[m_ - i] = d(m_ - i) - c(m_ - i) * riesenie[m_ - i + 1];

    return riesenie;
}
