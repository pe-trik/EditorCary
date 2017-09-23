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
  void prekreslit();
  void DokumentPrepocitany();

public:
  Dokument();
  qreal sirka() const;
  qreal vyska() const;
  QDomDocument Uloz();
  void Obnov(const QDomDocument &doc);

  void setSirka(qreal sirka);

  void setVyska(qreal vyska);

  std::vector<Vlastnost *> Vlastnosti() const;

  void Vykresli(QPainter &painter);

  void VytvorSpojenia(QPointF bod);

  Komponenty::Komponent* Komponent(QPointF bod);

  const std::vector<Komponenty::KomponentPtr> &Komponenty() const { return _komponenty; }

  void PridajKomponent(Komponenty::KomponentPtr komponent);

  Komponenty::Komponent *vybranyKomponent() const;

  void NastavVybranyKomponent(Komponenty::Komponent* k);

  void VycistiSpojenia();

  void Prepocitaj();

  static std::vector<Nastroje::NastrojPresenterPtr> DostupneNastroje();

  std::vector<Komponenty::Komponent*> Komponenty();

  void ZmazVybranyKomponent();

  qreal Okraj() const{
      return _okraj->hodnota();
  }

  qreal MinimalnyPolomer() const{
      return _minimalnyPolomerZatacky->hodnota();
  }

  qreal minimalnaDlzkaTrate() const;

  qreal maximalnaDlzkaTrate() const;

  std::vector<Komponenty::Komponent *> spojenia() const;

private:
  void obnovKomponenty(QDomNodeList komponenty);
  void obnovSpojenia(QDomNodeList spojenia);
  QrealVlastnostPtr _sirka;
  QrealVlastnostPtr _vyska;
  QrealVlastnostPtr _okraj;
  QrealVlastnostPtr _minimalnyPolomerZatacky;
  QrealVlastnostPtr _minimalnaDlzkaTrate;
  QrealVlastnostPtr _maximalnaDlzkaTrate;
  BoolVlastnostPtr _nahlad;
  BoolVlastnostPtr _siet;
  std::vector<Komponenty::KomponentPtr> _komponenty;
  std::vector<std::unique_ptr<Komponenty::Komponent>> _spojenia;
  Komponenty::Komponent* _vybranyKomponent = nullptr;
  bool _prepocitavanie = true;
};
}

#endif // DOKUMENT_H
