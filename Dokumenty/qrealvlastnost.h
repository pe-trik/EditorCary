#ifndef qrealVLASTNOST_H
#define qrealVLASTNOST_H

#include "vlastnost.h"

namespace Dokumenty {
class QrealVlastnost : public Vlastnost {
    Q_OBJECT
public:
    QrealVlastnost(QString nazov, qreal Hodnota);
    VlastnostManagerPtr NastrojVlastnosti();
    qreal Hodnota() const;

    QDomElement UlozVlastnost(QDomDocument &doc);

    qreal Min() const;
    void setMin(qreal Min);

    qreal Max() const;
    void setMax(qreal Max);

public slots:
    void setHodnota(qreal Hodnota);

signals:
    void hodnotaZmenena(qreal novaHodnota);


protected:
    void obnovVlastnost(QDomElement &v);

private:
    qreal _hodnota;
    qreal _min = -10000;
    qreal _max = 10000;
};
using QrealVlastnostPtr = std::unique_ptr<QrealVlastnost>;
}

#endif // qrealVLASTNOST_H
