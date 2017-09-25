#ifndef DOKUMENT_H
#define DOKUMENT_H

#include "Komponenty/komponent.h"
#include "qrealvlastnost.h"
#include "boolvlastnost.h"
#include <QDomDocument>
#include <QObject>
#include <QPainter>
#include <vector>

namespace Komponenty {
class Komponent;

using KomponentPtr = std::unique_ptr<Komponent>;
}

namespace Nastroje {
class NastrojPresenter;

using NastrojPresenterPtr = std::unique_ptr<NastrojPresenter>;

class Nastroj;
}

namespace Dokumenty {
class Dokument : public QObject {
    Q_OBJECT

private slots:
    void velkostDokumentuZmenena(qreal);

signals:
    void Prekresli(); //poziada pracovnu plochu o prekreslenie
    void DokumentPrepocitany(); //informuje pracovnu plochu o zmene dokumentu

public:
    Dokument();

    qreal Sirka() const;
    qreal Vyska() const;

    QDomDocument UlozDokument() const;

    void ObnovDokument(const QDomDocument &doc);

    void setSirka(qreal Sirka);

    void setVyska(qreal Vyska);

    std::vector<Vlastnost *> VlastnostiDokumentu() const;

    void Vykresli(QPainter &painter);

    //vyhlada volne sloty v okoli daneho bodu a vytvori Spojenie
    void VytvorSpojenia(QPointF bod);

    Komponenty::Komponent* NajdiKomponentPodBodom(QPointF bod);

    const std::vector<Komponenty::KomponentPtr> &Komponenty() const;
    const std::vector<Komponenty::KomponentPtr> &Spojenia() const;

    void PridajKomponent(Komponenty::KomponentPtr komponent);

    Komponenty::Komponent *VybranyKomponent() const;

    void NastavVybranyKomponent(Komponenty::Komponent* k);

    //vymaze spojenia s poctom slotov mensim ako 2
    void VycistiSpojenia();

    void PrepocitajDokument();

    static std::vector<Nastroje::NastrojPresenterPtr> DostupneNastroje();

    void ZmazVybranyKomponent();

    qreal Okraj() const;

    qreal MinimalnyPolomer() const;

    qreal MinimalnaDlzkaTrate() const;

    qreal MaximalnaDlzkaTrate() const;

    //std::vector<Komponenty::Komponent *> Spojenia() const;

    QString CestaSubor() const;
    void setCestaSubor(const QString &CestaSubor);

private:
    void obnovKomponenty(QDomNodeList komponenty);
    void obnovSpojenia(QDomNodeList Spojenia);
    void vykresliMriezkuAOkraj(QPainter &painter);
    void najdiBlokVlastnosti(QDomNode blok);
    void najdiBlokKomponenty(QDomNode blok);
    void najdiBlokSpojenia(QDomNode blok);
    void inicializujVlastnostiDokumentu();

    QrealVlastnostPtr _sirka;
    QrealVlastnostPtr _vyska;
    QrealVlastnostPtr _okraj;
    QrealVlastnostPtr _minimalnyPolomerZatacky;
    QrealVlastnostPtr _minimalnaDlzkaTrate;
    QrealVlastnostPtr _maximalnaDlzkaTrate;
    QrealVlastnostPtr _minimalnaVzdialenostKomponent;
    BoolVlastnostPtr _zobrazMinimalnuVzdialenost;
    BoolVlastnostPtr _nahlad;
    BoolVlastnostPtr _siet;

    std::vector<Komponenty::KomponentPtr> _komponenty;
    std::vector<Komponenty::KomponentPtr> _spojenia;

    Komponenty::Komponent* _vybranyKomponent = nullptr;
    bool _prepocitavanie = true;
    QString _cestaSubor = "";
};
}

#endif // DOKUMENT_H
