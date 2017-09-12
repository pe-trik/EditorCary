#ifndef DOKUMENT_H
#define DOKUMENT_H

#include "Komponenty/komponent.h"
#include "qrealvlastnost.h"
#include <QObject>
#include <QPainter>
#include <vector>

namespace Komponenty {
class Komponent;

using KomponentPtr = std::unique_ptr<Komponent>;
}

namespace Dokumenty {
class Dokument : public QObject {
  Q_OBJECT
private slots:
  void velkostDokumentuZmenena(qreal);
signals:
  void prekreslit();

public:
  Dokument();
  qreal sirka() const;
  qreal vyska() const;

  void setSirka(qreal sirka);

  void setVyska(qreal vyska);

  std::vector<Vlastnost *> Vlastnosti() const;

  void Vykresli(QPainter &painter);

  void VytvorSpojenia(QPointF bod);

  Komponenty::Komponent* Komponent(QPointF bod);

  const std::vector<Komponenty::KomponentPtr> &Komponenty() const { return _komponenty; }

  void PridajKomponent(Komponenty::KomponentPtr komponent);

  Komponenty::Komponent *vybranyKomponent() const;

  void VycistiSpojenia();

private:
  QrealVlastnostPtr _sirka;
  QrealVlastnostPtr _vyska;
  std::vector<Komponenty::KomponentPtr> _komponenty;
  std::vector<std::shared_ptr<Komponenty::Komponent>> _spojenia;
  Komponenty::Komponent* _vybranyKomponent = nullptr;
};
}

#endif // DOKUMENT_H
