#ifndef STRINGVLASTNOST_H
#define STRINGVLASTNOST_H

#include "vlastnost.h"

namespace Dokumenty {
class StringVlastnost : public Vlastnost
{
    Q_OBJECT
public:
    StringVlastnost(QString nazov, QString hodnota);
    VlastnostManagerPtr NastrojVlastnosti();
    QString hodnota() const;
    QDomElement Uloz(QDomDocument &doc);

  public slots:
    void setHodnota(QString hodnota);

  signals:
    void hodnotaZmenena(QString novaHodnota);

  private:
    QString _hodnota;
};
using StringVlastnostPtr = std::unique_ptr<StringVlastnost>;
}

#endif // STRINGVLASTNOST_H
