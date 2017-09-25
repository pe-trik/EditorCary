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
    //nazov vlastnosti je povinna polozka
    explicit Vlastnost(QString nazov, QObject *parent = nullptr);

    virtual VlastnostManagerPtr NastrojVlastnosti() = 0;

    QString Nazov() const;

    virtual QDomElement UlozVlastnost(QDomDocument &doc) = 0;

    void ObnovVlastnost(QDomNodeList vlastnosti);

signals:
    void VlastnostZmenena();

public slots:

protected:
    //fcia volana ObnovVlastnost(), arg je element odpovedajuci vlastnosti s odpovedajucim nazvom vlastnosti
    virtual void obnovVlastnost(QDomElement& v) = 0;
    QString _nazov = "";
};

using VlastnostPtr = std::unique_ptr<Vlastnost>;
}

#endif // VLASTNOST_H
