#ifndef STRINGVLASTNOST_H
#define STRINGVLASTNOST_H

#include "vlastnost.h"

namespace Dokumenty {
class StringVlastnost : public Vlastnost
{
    Q_OBJECT
public:
    StringVlastnost(QString nazov, QString Hodnota);

    VlastnostManagerPtr NastrojVlastnosti();

    QString Hodnota() const;

    QDomElement UlozVlastnost(QDomDocument &doc);

public slots:
    void setHodnota(QString Hodnota);

signals:
    void hodnotaZmenena(QString novaHodnota);

protected:
    void obnovVlastnost(QDomElement &v);

private:
    QString _hodnota;
};
using StringVlastnostPtr = std::unique_ptr<StringVlastnost>;
}

#endif // STRINGVLASTNOST_H
