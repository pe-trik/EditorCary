#ifndef TRIDIAGONALNAMATICA_H
#define TRIDIAGONALNAMATICA_H

#include <memory>
#include <QtMath>
#include <QPointF>

namespace Komponenty {

using Pole = std::unique_ptr<QPointF[]>;

class TridiagonalnaMatica
{
public:
    TridiagonalnaMatica(size_t m,
                        QPointF a = QPointF(),
                        QPointF b = QPointF(),
                        QPointF c = QPointF(),
                        QPointF d = QPointF(),
                        QPointF tr = QPointF(),
                        QPointF ll = QPointF());
    Pole Vyries();

    QPointF& a(size_t i);
    QPointF& b(size_t i);
    QPointF& c(size_t i);
    QPointF& d(size_t i);
    QPointF& rc(size_t i);
    QPointF& lc(size_t i);

private:
    size_t _m;
    QPointF _tr;
    QPointF _ll;
    //pretoze matica je tvorena 3 diagonalami a "poslednym" stlpcom matice
    //staci mat 4 vektory velkosti m
    Pole _a ,
    _b,
    _c,
    _d,
    _rc,
    _lc;//pravy a lavy stlpec

    void odstranSpodnuDiagonalu();
    void vynormujPoOdstraneniSpodnejDiagonaly();
    void odstranPravyStlpec();
    void odstranHornuDiagonalu();
    void odstranLavyStlpec();
};

inline QPointF operator *(QPointF a, QPointF b)
{
    return QPointF(a.x() * b.x(), a.y() * b.y());
}

inline QPointF operator /(QPointF a, QPointF b)
{
    return QPointF(a.x() / b.x(), a.y() / b.y());
}


}

#endif // TRIDIAGONALNAMATICA_H
