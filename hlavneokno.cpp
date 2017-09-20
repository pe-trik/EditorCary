#include "hlavneokno.h"
#include "ui_hlavneokno.h"

#include "Nastroje/ciaranastroj.h"
#include "Nastroje/splinenastroj.h"
#include "Nastroje/volnaciaranastroj.h".h"
#include <QFileDialog>
#include <QPrinter>
#include <QSettings>
#include <QSvgGenerator>
#include <QTextStream>
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

    connect(ui->pracovnaPlocha, SIGNAL(VlastnostiZmenene(std::vector<Dokumenty::Vlastnost*>)),
            ui->vlastnostiObjektu, SLOT(NastavVlastnosti(std::vector<Dokumenty::Vlastnost*>)));

    connect(ui->vlastnostiObjektu, SIGNAL(VlastnostZmenena()), ui->pracovnaPlocha, SLOT(PrekresliAPrepocitajPlochu()));
    connect(ui->vlastnostiDokumentu, SIGNAL(VlastnostZmenena()), ui->pracovnaPlocha, SLOT(PrekresliAPrepocitajPlochu()));
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
    _nastroje.push_back(std::make_unique<Nastroje::VolnaCiaraPresenter>());

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

void HlavneOkno::on_actionExport_do_PDF_triggered()
{
    QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Export do PDF", QString(), "*.pdf");
    if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".pdf"); }

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QSize(ui->pracovnaPlocha->dokument()->sirka(),
                               ui->pracovnaPlocha->dokument()->vyska()),
                         QPrinter::Millimeter);
    printer.setOutputFileName(fileName);

    QPainter p;
    p.begin(&printer);
    ui->pracovnaPlocha->VykresliPlochu(p);
    p.end();
}

void HlavneOkno::on_actionExport_do_SVG_triggered()
{
    QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Export do SVG", QString(), "*.svg");
    if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".svg"); }

    QSvgGenerator generator;
    generator.setFileName(fileName);
    generator.setSize(QSize(ui->pracovnaPlocha->dokument()->sirka(),
                            ui->pracovnaPlocha->dokument()->vyska()));
    generator.setViewBox(QRect(0,0,
                               ui->pracovnaPlocha->dokument()->sirka(),
                               ui->pracovnaPlocha->dokument()->vyska()));
    generator.setTitle(tr("Editor čáry"));
    QPainter p;
    p.begin(&generator);
    ui->pracovnaPlocha->VykresliPlochu(p);
    p.end();
}

void HlavneOkno::on_actionUlo_i_ako_triggered()
{
    QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Uložiť ako", QString(), "*.xml");
    if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".xml"); }

    QFile outFile(fileName);
    if( !outFile.open( QIODevice::WriteOnly | QIODevice::Text ) )
    {
        qDebug( "Failed to open file for writing." );
        return;
    }

    QTextStream stream( &outFile );
    stream << ui->pracovnaPlocha->dokument()->Uloz().toString(1);

    outFile.close();
}
