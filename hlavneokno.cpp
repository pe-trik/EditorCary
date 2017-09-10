#include "hlavneokno.h"
#include "ui_hlavneokno.h"

#include "Nastroje/ciaranastroj.h"
#include "Nastroje/splinenastroj.h"
#include <QSettings>
#include <algorithm>

HlavneOkno::HlavneOkno(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::HlavneOkno) {
  ui->setupUi(this);
  QSettings settings("MFF UK", "EditorCary");
  restoreGeometry(settings.value("geometry").toByteArray());
  restoreState(settings.value("windowState").toByteArray());

  nacitajNastroje();

  NovyDokument();

  connect(ui->pracovnaPlocha, SIGNAL(NastrojZmeneny(Nastroje::Nastroj *)), this,
          SLOT(nastavNastrojToolBar(Nastroje::Nastroj *)));
}

HlavneOkno::~HlavneOkno() { delete ui; }

void HlavneOkno::NovyDokument() {
  _dokument = std::make_unique<Dokumenty::Dokument>();
  ui->pracovnaPlocha->NastavDokument(_dokument.get());
  ui->vlastnostiDokumentu->NastavVlastnosti(_dokument->Vlastnosti());
}

void HlavneOkno::closeEvent(QCloseEvent *event) {
  QSettings settings("MFF UK", "EditorCary");
  settings.setValue("geometry", saveGeometry());
  settings.setValue("windowState", saveState());
  QMainWindow::closeEvent(event);
}

void HlavneOkno::nastavNastrojToolBar(Nastroje::Nastroj *nastroj) {
  for (auto btn : _nastrojeTlacitka) {
    if (nastroj && btn->toolTip() == nastroj->NastrojPresenter()->Nazov())
      btn->setChecked(true);
    else
      btn->setChecked(false);
  }
}

void HlavneOkno::nastavNastroj(Nastroje::NastrojPtr nastroj) {
  ui->pracovnaPlocha->NastavNastroj(std::move(nastroj));
}

void HlavneOkno::nacitajNastroje() {
  _nastroje = std::vector<Nastroje::NastrojPresenterPtr>();
  _nastroje.push_back(std::make_unique<Nastroje::KurzorPresenter>());
  _nastroje.push_back(std::make_unique<Nastroje::CiaraPresenter>());
  _nastroje.push_back(std::make_unique<Nastroje::SplinePresenter>());

  for (auto &&nastroj : _nastroje) {
    auto btn = new QToolButton();
    btn->setIcon(QIcon(nastroj->Ikonka()));
    btn->setToolTip(nastroj->Nazov());
    btn->setCheckable(true);
    connect(btn, &QToolButton::clicked, [btn, this, &nastroj](bool c) {
      if (!c)
        this->nastavNastroj(nullptr);
      else {
        std::for_each(this->_nastrojeTlacitka.begin(),
                      this->_nastrojeTlacitka.end(),
                      [](auto b) { b->setChecked(false); });
        btn->setChecked(true);
        this->nastavNastroj(nastroj->Nastroj(this->_dokument.get()));
      }
    });

    _nastrojeTlacitka.push_back(btn);

    ui->toolBar->addWidget(btn);
  }
}
