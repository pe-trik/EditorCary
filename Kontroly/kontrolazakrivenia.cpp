#include "kontrolazakrivenia.h"
#include "algorithm"
#include <QtMath>

using namespace Kontroly;


KontrolaZakrivenia::KontrolaZakrivenia(Dokumenty::Dokument *dokument)
    :Kontrola(dokument)
{
    _nazov = "Kontrola zakrivenia";
}

std::vector<Chyba> KontrolaZakrivenia::VykonajKontrolu()
{
    std::vector<Chyba> chyby;
    setVysledok(true);

    qreal mpolomer = _dokument->MinimalnyPolomer();

    if(_povolena){
        for(auto& k : _dokument->Komponenty())
        {
            auto body = k->BodyKomponentu();
            if(body.size() > 7)
            {
                for(size_t i = 4; i < body.size() - 2; i++){
                   if(polomer(body.at(i - 2), body.at(i - 1), body.at(i)) < mpolomer)
                   {
                       setVysledok(false);
                       chyby.push_back(Chyba(this, "komponent má polomer zakrivenia menší ako " + QString::number(mpolomer) + " mm", k));
                       break;
                   }
                }
            }
        }
    }

    return chyby;
}

qreal KontrolaZakrivenia::polomer(QPointF A, QPointF B, QPointF C) const
{
    //http://mathworld.wolfram.com/SSSTheorem.html
    auto a = vzdialenost(B, C);
    auto b = vzdialenost(A, C);
    auto c = vzdialenost(A, B);

    if(!((a + b) > c && (a + c) > b && (b + c) > a))
        return INT_MAX;

    auto s = (a + b + c) / 2.;
    auto K = qSqrt(s*(s-a)*(s-b)*(s-c));

    return a*b*c/(4.*K);
}

qreal KontrolaZakrivenia::vzdialenost(QPointF A, QPointF B) const
{
    auto x = A - B;
    return qSqrt(x.x()*x.x() + x.y()*x.y());
}
