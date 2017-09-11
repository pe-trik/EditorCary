#ifndef KUBICKYSPLINE_H
#define KUBICKYSPLINE_H

#include "ikomponent.h"
#include "Nastroje/dvojbodovy.h"
#include <QPointF>
#include <vector>
#include "Vlastnosti/pointvlastnost.h"

namespace Komponenty {
typedef std::unique_ptr<qreal[]> Pole;
typedef std::tuple<QPointF, QPointF, QPointF, QPointF> Koeficienty;
class KubickySpline : public IKomponent
{
public:
    KubickySpline(Dokumenty::Dokument *dokument);
    KubickySpline(Dokumenty::Dokument* dokument, QDomElement &e);
    bool Obsahuje(QPointF bod) const;
    void Vykresli(QPainter &p) const;
    QDomElement Uloz(QDomDocument &doc) const;
    std::unique_ptr<Nastroje::INastrojPresenter> Nastroj() const;
    //prepocita polynomi
    void Prepocitaj(bool o = true);
    //prepocita krivku; vysledok SplineInterpolacia.Krivka
    void prepocitajKrivku();
    QVector<QPointF> Krivka;
    std::vector<QPointF> Body;
    void kontrola();


    std::unique_ptr<PointVlastnost> Zaciatok;
    std::unique_ptr<PointVlastnost> Koniec;
    std::unique_ptr<PointVlastnost> SmerZaciatok;
    std::unique_ptr<PointVlastnost> SmerKoniec;
    std::unique_ptr<PointVlastnost> Zmena;
private:
    void obnovVlastnosti();
    void nastavVlastnosti();
    bool prepocitavam = false;
    Pole GJRiesenieX = 0;
    Pole GJRiesenieY = 0;
    //vypocita koeficienty iteho useku (pre Xovu aj Yovu os)
    Koeficienty koeficienty(size_t i) const;
    static size_t max_id;
    std::unique_ptr<DoubleVlastnost> nasobok;
    std::unique_ptr<DoubleVlastnost> nasobok2;
    bool validne = true;
};


class TridiagonalnaMatica
{
public:
    TridiagonalnaMatica(size_t m, qreal a = 0, qreal b = 0, qreal c = 0, qreal d = 0);
    Pole Vyries();
    qreal& a(size_t i) { return a_[i]; }
    qreal& b(size_t i) { return b_[i]; }
    qreal& c(size_t i) { return c_[i]; }
    qreal& d(size_t i) { return d_[i]; }
private:
    size_t m_;
    //pretoze matica je tvorena 3 diagonalami a "poslednym" stlpcom matice
    //staci mat 4 vektory velkosti m
    Pole a_ = 0,
    b_ = 0,
    c_ = 0,
    d_ = 0;

};
}


namespace Nastroje {
    template<>
    class ParametreNastroja<Dvojbodovy<Komponenty::KubickySpline>>
    {
    public:
        //nazov nastroja
        static const char* nazov;
        //cesta k ikonke
        static const char* ikonka;
    };
}

#endif // KUBICKYSPLINE_H
