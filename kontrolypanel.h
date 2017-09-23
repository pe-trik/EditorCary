#ifndef KONTROLYPANEL_H
#define KONTROLYPANEL_H

#include <QWidget>
#include "Kontroly/kontrola.h"

class KontrolyPanel : public QWidget
{
    Q_OBJECT
public:
    explicit KontrolyPanel(QWidget *parent = nullptr);
    void NastavDokument(Dokumenty::Dokument* dokument);
    std::vector<Kontroly::Chyba> VykonajKontroly();

signals:
    void KontrolyZmenene();
public slots:
private:
    Dokumenty::Dokument* _dokument;
    std::vector<std::unique_ptr<Kontroly::Kontrola>> _kontroly;
};

#endif // KONTROLYPANEL_H
