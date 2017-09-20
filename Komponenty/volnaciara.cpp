#include "volnaciara.h"
#include "manipulator.h"
#include "tridiagonalnamatica.h"
#include "splinegroup.h"

#include <Nastroje/volnaciaranastroj.h>

using namespace Komponenty;
using namespace Dokumenty;

VolnaCiara::VolnaCiara()
{
    _x1 = std::make_unique<QrealVlastnost>("X1", 10);
    _y1 = std::make_unique<QrealVlastnost>("Y1", 10);
    _x2 = std::make_unique<QrealVlastnost>("X2", 200);
    _y2 = std::make_unique<QrealVlastnost>("Y2", 200);
    _vyhladzovanie = std::make_unique<QrealVlastnost>("Vyhladzovanie", 30);
    _nasobokZaciatok = std::make_unique<QrealVlastnost>("Násobok začiatok", 1);
    _nasobokZaciatok->setMax(100);
    _nasobokZaciatok->setMin(-100);
    _nasobokKoniec = std::make_unique<QrealVlastnost>("Násobok koniec", 1);
    _nasobokKoniec->setMax(100);
    _nasobokKoniec->setMin(-100);
    _vykresliAkoSpline = std::make_unique<BoolVlastnost>("Vykresli ako spline", true);
    connect(_vykresliAkoSpline.get(), SIGNAL(hodnotaZmenena(bool)),
            this, SLOT(vykresliAkoSpline(bool)));

    _vlastnosti = { _nazov.get(), _x1.get(), _y1.get(), _x2.get(), _y2.get(), _vyhladzovanie.get(), _nasobokZaciatok.get(), _nasobokKoniec.get(), _vykresliAkoSpline.get() };

    connect(_vyhladzovanie.get(), SIGNAL(hodnotaZmenena(qreal)),
            this, SLOT(vyhladzovanie(qreal)));

    auto manipulator = std::make_unique<Manipulator>(_x1.get(), _y1.get(), this);
    connect(manipulator.get(), SIGNAL(BodZmeneny(QPointF)), this, SLOT(manipulatorZmeneny(QPointF)));
    _spojenieSloty.push_back(
                std::make_unique<SpojenieSlot>(this, manipulator.get(), _nasobokZaciatok.get(), [this]() {
        if(_body.size() >= 2)
        {
            auto smer = (_body.front() - *(_body.begin() + 1));
            qreal v = smer.x()*smer.x()+smer.y()*smer.y();
            return 900 * smer / v;
        }
        else
            return QPointF(
                        (_x1->hodnota() - _x2->hodnota()),
                        (_y1->hodnota() - _y2->hodnota()));
    }));
    _manipulatory.push_back(std::move(manipulator));

    manipulator = std::make_unique<Manipulator>(_x2.get(), _y2.get(), this);
    connect(manipulator.get(), SIGNAL(BodZmeneny(QPointF)), this, SLOT(manipulator2Zmeneny(QPointF)));
    _spojenieSloty.push_back(
                std::make_unique<SpojenieSlot>(this, manipulator.get(), _nasobokKoniec.get(), [this]() {
        if(_body.size() >= 2)
        {
            auto smer = _body.back() - *(_body.end() - 2);
            qreal v = smer.x()*smer.x()+smer.y()*smer.y();
            return 900 * smer / v;
        }
        else
            return QPointF(
                        (_x2->hodnota() - _x1->hodnota()),
                        (_y2->hodnota() - _y1->hodnota()));
    }));
    _manipulatory.push_back(std::move(manipulator));
}

QVector<QPointF> &VolnaCiara::body()
{
    return _povodneBody;
}

void VolnaCiara::setBody(const QVector<QPointF> &body)
{
    _povodneBody = body;
    prepocitaj();
}

void VolnaCiara::vyhladzovanie(qreal)
{
    prepocitaj();
}

void VolnaCiara::manipulatorZmeneny(QPointF bod)
{
    if(!_ignorujZmenuManipulatora){
        _povodneBody.insert(_povodneBody.begin(), bod);
        prepocitaj();
    }
}

void VolnaCiara::manipulator2Zmeneny(QPointF bod)
{
    if(!_ignorujZmenuManipulatora){
        _povodneBody.push_back(bod);
        prepocitaj();
    }
}

void VolnaCiara::vykresliAkoSpline(bool spline)
{
    prepocitaj();
}

void VolnaCiara::prepocitaj()
{
    vyhlad();
    prepocitajSpline();
}

void VolnaCiara::vyhlad()
{
    if(_povodneBody.size() > 0)
    {
        _body.clear();
        _body.push_back(_povodneBody.front());

        for(auto b : _povodneBody)
        {
            if(((b.x() - _body.back().x())*(b.x() - _body.back().x())
                +(b.y() - _body.back().y())*(b.y() - _body.back().y()))
                    >= _vyhladzovanie->hodnota()*_vyhladzovanie->hodnota())
                _body.push_back(b);
        }

        if(_body.size() > 2)
        {
            _body.removeLast();
            _body.push_back(_povodneBody.back());
        }
    }
}

void VolnaCiara::prepocitajSpline()
{
    if(_vykresliAkoSpline->hodnota() && _body.size() > 2){

        size_t n = _body.size() - 1;
        TridiagonalnaMatica matica(n + 1,
                                   QPointF(1,1),
                                   QPointF(4,4),
                                   QPointF(1,1),
                                   QPointF(0,0),
                                   QPointF(0,0),
                                   QPointF(0,0));

        for (size_t i = 1; i < n; i++)
            matica.d(i) = 3.*(_body[i + 1] - _body[i - 1]);

        matica.d(0) = 3 * (_body[1] - _body[0]);

        matica.d(n) = 3. * (_body[n] - _body[n - 1]);

        auto riesenie = matica.Vyries();

        auto stdBody = _body.toStdVector();

        _body.clear();
        for (size_t i = 0; i + 1 < stdBody.size(); i++)
        {
            auto k = SplineGroup::koeficienty(stdBody, riesenie, i);
            qreal kr = 7. / qMax(_vyhladzovanie->hodnota(), 2.);

            qreal t = 0;
            do
            {
                QPointF b = QPointF(std::get<0>(k).x() + std::get<1>(k).x() * t + std::get<2>(k).x() * t * t + std::get<3>(k).x() * t * t * t,
                                    std::get<0>(k).y() + std::get<1>(k).y() * t + std::get<2>(k).y() * t * t + std::get<3>(k).y() * t * t * t);
                _body.push_back(b);
                t += kr;
            }
            while (t <= 1);
        }
    }
}

void VolnaCiara::Vykresli(QPainter &painter) const
{
    if(_body.size() > 0)
    {
        painter.setPen(QPen(Qt::black, 10,Qt::SolidLine));
        painter.drawPolyline(_body);
    }
}

Nastroje::NastrojPtr VolnaCiara::Nastroj(Dokumenty::Dokument *dokument)
{
    return std::make_unique<Nastroje::VolnaCiaraNastroj>(dokument);
}

bool VolnaCiara::Obsahuje(QPointF bod) const
{
    return std::any_of(_body.begin(), _body.end(), [bod](auto b){
        return ((b.x() - bod.x()) * (b.x() - bod.x())
                + (b.y() - bod.y()) * (b.y() - bod.y())) < 100;
    });
}
