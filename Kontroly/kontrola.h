#ifndef KONTROLA_H
#define KONTROLA_H

#include <QObject>
#include "Dokumenty/dokument.h"
#include "chyba.h"


namespace Kontroly {
class Chyba;
class Kontrola : public QObject
{
    Q_OBJECT
public:
    Kontrola(Dokumenty::Dokument* dokument);
    bool Vysledok() const;

    QString Nazov() const;

    bool Povolena() const;
    void setPovolena(bool Povolena);


public slots:
    virtual std::vector<Chyba> VykonajKontrolu() = 0;

signals:
    void KontrolaZmenena();
    void KontrolaPovolenaZmena();

protected:
    void setVysledok(bool vysledok);
    QString _nazov;
    Dokumenty::Dokument* _dokument;
    bool _povolena = true;
private:
    bool _vysledok = true;
};
}

#endif // KONTROLA_H
