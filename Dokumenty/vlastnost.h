#ifndef VLASTNOST_H
#define VLASTNOST_H

#include "vlastnostmanager.h"
#include <QDomDocument>
#include <QObject>
#include <memory>

namespace Dokumenty {
class Vlastnost : public QObject {
  Q_OBJECT
public:
  explicit Vlastnost(QString nazov, QObject *parent = nullptr);
  virtual VlastnostManagerPtr NastrojVlastnosti() = 0;
  QString Nazov() const { return _nazov; }
  virtual QDomElement Uloz(QDomDocument &doc) = 0;
signals:
    void VlastnostZmenena();
public slots:
protected:
  QString _nazov = "";
};

using VlastnostPtr = std::unique_ptr<Vlastnost>;
}

#endif // VLASTNOST_H
