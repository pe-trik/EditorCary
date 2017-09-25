#ifndef CHYBYPANEL_H
#define CHYBYPANEL_H

#include <QWidget>
#include <QTableWidget>

#include <Kontroly/chyba.h>

class ChybyPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ChybyPanel(QWidget *parent = nullptr);

signals:
    void VybranyKomponent(Komponenty::Komponent* komponent);

public slots:
    void NastavChyby(std::vector<Kontroly::Chyba> chyby);

private:
    QTableWidget* _widget;
    std::vector<Kontroly::Chyba> _chyby;
};

#endif // CHYBYPANEL_H
