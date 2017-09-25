#include "tridiagonalnamatica.h"

using namespace Komponenty;

TridiagonalnaMatica::TridiagonalnaMatica(size_t m, QPointF a, QPointF b, QPointF c, QPointF d, QPointF tr, QPointF ll)
{
    _tr = tr;
    _ll = ll;
    _m = m;

    //alokovanie vektorov
    _a = std::make_unique<QPointF[]>(m);
    _b = std::make_unique<QPointF[]>(m);
    _c = std::make_unique<QPointF[]>(m);
    _d = std::make_unique<QPointF[]>(m);
    _rc = std::make_unique<QPointF[]>(m);
    _lc = std::make_unique<QPointF[]>(m);

    //inicializacia
    for(size_t i = 0; i < m; i++)
    {
        rc(i) = QPointF(0,0);
        lc(i) = QPointF(0,0);
        this->a(i) = a;
        this->b(i) = b;
        this->c(i) = c;
        this->d(i) = d;
    }

    rc(0) = tr;
    lc(_m - 1) = ll;
}

//riesenie tridiagonalnej matice v O(n)
void TridiagonalnaMatica::odstranSpodnuDiagonalu()
{
    for (size_t i = 1; i < _m; i++)
    {
        QPointF koef = a(i) / b(i - 1);
        a(i) -= koef * b(i - 1);
        if (i < _m - 1)
            b(i) -= koef * c(i - 1);
        d(i) -= koef * d(i - 1);
        rc(i) -= koef * rc(i - 1);
    }
}

void TridiagonalnaMatica::vynormujPoOdstraneniSpodnejDiagonaly()
{
    for (size_t i = 0; i < _m; i++)
    {
        QPointF koef = b(i);
        if(i > 1)
            lc(i) = lc(i) / koef;
        c(i) = c(i) / koef;
        if(i < _m - 2)
            rc(i) = rc(i) / koef;
        d(i) = d(i) / koef;
        b(i) = b(i) / koef;
    }
}

void TridiagonalnaMatica::odstranPravyStlpec()
{
    for (size_t i = 0; i < _m - 1; i++)
    {
        QPointF koef = rc(i);
        lc(i) -= lc(_m - 1) * koef;
        d(i) -= d(_m - 1) * koef;
        rc(i) -= rc(_m - 1) * koef;
    }
}

void TridiagonalnaMatica::odstranHornuDiagonalu()
{
    for (int i = static_cast<int>(_m) - 3; i >= 0; i--)
    {
        QPointF koef = -c(i) * b(i + 1);
        c(i) += koef * b(i + 1);
        lc(i) += koef * lc(i + 1);
        d(i) += koef * d(i + 1);
    }
}

void TridiagonalnaMatica::odstranLavyStlpec()
{
    for (size_t i = 1; i < _m; i++)
    {
        QPointF koef = lc(i);
        lc(i) -= lc(0) * koef;
        d(i) -= d(0) * koef;
    }
}

Pole TridiagonalnaMatica::Vyries()
{
    //matica musi mat nenulovu velkost
    if (_m == 0)
        return 0;

    //odstranenie spodnej diagonaly - po posledne 2 riadky bez konfliktov
    odstranSpodnuDiagonalu();

    //vynormovanie
    vynormujPoOdstraneniSpodnejDiagonaly();

    //odstranenie praveho stlpca
    //vytvori sa lavy
    odstranPravyStlpec();

    //odstranenie hornej diagonaly
    odstranHornuDiagonalu();

    //vynormovanie prveho riadku
    d(0) = d(0) / lc(0);
    lc(0) = lc(0) / lc(0);

    //odstranenie laveho stlpca
    odstranLavyStlpec();

    //vytvor riesenie
    auto riesenie = std::make_unique<QPointF[]>(_m);

    for (size_t i = 0; i < _m; i++)
        riesenie[i] = d(i);

    return riesenie;
}

QPointF &TridiagonalnaMatica::a(size_t i) { return _a[i]; }

QPointF &TridiagonalnaMatica::b(size_t i) { return _b[i]; }

QPointF &TridiagonalnaMatica::c(size_t i) { return _c[i]; }

QPointF &TridiagonalnaMatica::d(size_t i) { return _d[i]; }

QPointF &TridiagonalnaMatica::rc(size_t i)
{
    if(i < _m - 2)
        return  _rc[i];
    else if (i == _m - 2)
        return _c[i];
    else
        return _b[i];
}

QPointF &TridiagonalnaMatica::lc(size_t i)
{
    if(i > 1)
        return _lc[i];
    if(i == 0)
        return _b[0];
    else
        return _a[1];
}
