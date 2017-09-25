#ifndef HLAVNEOKNO_H
#define HLAVNEOKNO_H

#include "Dokumenty/dokument.h"
#include "Nastroje/kurzor.h"
#include <QMainWindow>
#include <QToolButton>
#include <memory>

namespace Ui {
class HlavneOkno;
}

class HlavneOkno : public QMainWindow {
  Q_OBJECT

public:
  explicit HlavneOkno(QWidget *parent = 0);
  ~HlavneOkno();
  void NovyDokument();

protected:
  void closeEvent(QCloseEvent *);

  private slots:
  void nastavNastrojToolBar(Nastroje::Nastroj*);


  void exportDoPDF();

  void exportDoSVG();

  void ulozitAko();

  void otvorit();

  void novaDraha();

  void ulozit();

  void on_actionNov_dr_ha_triggered();

  void on_actionOtvori_existuj_cu_dr_hu_triggered();

  void on_actionUlo_i_triggered();

  void on_actionUlo_i_ako_triggered();

  void on_actionUkon_i_triggered();

private:
  void ulozit(QString path);
  void nastavNastroj(Nastroje::NastrojPtr nastroj);
  void nacitajNastroje();
  void obnovRozlozenieOkna();
  void prepojeniePanelov();
  Ui::HlavneOkno *ui;
  std::unique_ptr<Dokumenty::Dokument> _dokument;
  std::vector<Nastroje::NastrojPresenterPtr> _nastroje;
  std::vector<QToolButton*> _nastrojeTlacitka;
};

#endif // HLAVNEOKNO_H
