#ifndef KOMPONENT_H
#define KOMPONENT_H

#include "Nastroje/nastroj.h"
#include "spojenieslot.h"
#include "Dokumenty/stringvlastnost.h"
#include <QPainter>
#include <memory>
#include <vector>
#include <QObject>

namespace Nastroje {
class Nastroj;
using NastrojPtr = std::unique_ptr<Nastroj>;
}

namespace Dokumenty {
class Dokument;
}

namespace Komponenty {
class Komponent;
using KomponentPtr = std::unique_ptr<Komponent>;
class SpojenieSlot;
using SpojenieSlotPtr = std::unique_ptr<SpojenieSlot>;
class Komponent : public QObject {
public:
  Komponent();
  virtual void Vykresli(QPainter &painter) const = 0;
  virtual Nastroje::NastrojPtr Nastroj(Dokumenty::Dokument *dokument) = 0;
  const std::vector<KomponentPtr> &Manipulatory() const {
    return _manipulatory;
  }
  const std::vector<SpojenieSlotPtr> &SpojenieSloty() const {
    return _spojenieSloty;
  }
  virtual bool Obsahuje(QPointF bod) const = 0;

  std::vector<Dokumenty::Vlastnost *> Vlastnosti() const;

  QString nazov() const;

protected:
  std::vector<KomponentPtr> _manipulatory;
  std::vector<SpojenieSlotPtr> _spojenieSloty;
  std::vector<Dokumenty::Vlastnost*> _vlastnosti;
  Dokumenty::StringVlastnostPtr _nazov;
  static int id;
};
}

#endif // KOMPONENT_H
