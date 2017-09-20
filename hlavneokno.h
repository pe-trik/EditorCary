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


  void on_actionExport_do_PDF_triggered();

  void on_actionExport_do_SVG_triggered();

  void on_actionUlo_i_ako_triggered();

private:
	void nastavNastroj(Nastroje::NastrojPtr nastroj);
  void nacitajNastroje();
  Ui::HlavneOkno *ui;
  std::unique_ptr<Dokumenty::Dokument> _dokument;
  std::vector<Nastroje::NastrojPresenterPtr> _nastroje;
  std::vector<QToolButton*> _nastrojeTlacitka;
};

#endif // HLAVNEOKNO_H
