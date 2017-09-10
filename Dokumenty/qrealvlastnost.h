#ifndef qrealVLASTNOST_H
#define qrealVLASTNOST_H

#include "vlastnost.h"

namespace Dokumenty {
class QrealVlastnost : public Vlastnost {
  Q_OBJECT
public:
  QrealVlastnost(QString nazov, qreal hodnota);
  VlastnostManagerPtr NastrojVlastnosti();
  qreal hodnota() const;

  qreal min() const;
  void setMin(qreal min);

  qreal max() const;
  void setMax(qreal max);

public slots:
  void setHodnota(qreal hodnota);

signals:
  void hodnotaZmenena(qreal novaHodnota);

private:
  qreal _hodnota;
  qreal _min = 0;
  qreal _max = 10000;
};
using QrealVlastnostPtr = std::unique_ptr<QrealVlastnost>;
}

#endif // qrealVLASTNOST_H
