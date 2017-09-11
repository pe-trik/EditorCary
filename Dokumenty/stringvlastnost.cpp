#include "stringvlastnost.h"
#include "stringvlastnostmanager.h"

using namespace Dokumenty;


StringVlastnost::StringVlastnost(QString nazov, QString hodnota) : Vlastnost(nazov), _hodnota(hodnota)
{

}

VlastnostManagerPtr StringVlastnost::NastrojVlastnosti()
{
    return std::make_unique<StringVlastnostManager>(this);
}

QString StringVlastnost::hodnota() const
{
    return _hodnota;
}

void StringVlastnost::setHodnota(QString hodnota)
{
    if(hodnota != _hodnota){
        _hodnota = hodnota;
        emit hodnotaZmenena(hodnota);
        emit VlastnostZmenena();
    }
}
