#include "hlavneokno.h"
#include "ui_hlavneokno.h"

#include "Nastroje/ciaranastroj.h"
#include "Nastroje/splinenastroj.h"
#include "Nastroje/volnaciaranastroj.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPrinter>
#include <QSettings>
#include <QSvgGenerator>
#include <QTextStream>
#include <algorithm>

HlavneOkno::HlavneOkno(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::HlavneOkno)
{
    //nastavi ikonku okna
    setWindowIcon(QIcon(":/ikonky/volnaciara.svg"));

    ui->setupUi(this);

    obnovRozlozenieOkna();

    nacitajNastroje();

    NovyDokument();

    prepojeniePanelov();
}

//obnovi rozlozenie okna
void HlavneOkno::obnovRozlozenieOkna()
{
    QSettings settings("MFF UK", "EditorCary");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
}

void HlavneOkno::prepojeniePanelov()
{
    connect(ui->pracovnaPlocha, &PracovnaPlocha::PolohaMysiZmenena,
            [this](auto b){
        ui->statusBar->showMessage("X: " + QString::number(b.x())
                                   + " Y: " + QString::number(b.y()));
    });

    connect(ui->pracovnaPlocha, SIGNAL(NastrojZmeneny(Nastroje::Nastroj *)), this,
            SLOT(nastavNastrojToolBar(Nastroje::Nastroj *)));

    connect(ui->pracovnaPlocha, SIGNAL(VlastnostiZmenene(std::vector<Dokumenty::Vlastnost*>)),
            ui->vlastnostiObjektu, SLOT(NastavVlastnosti(std::vector<Dokumenty::Vlastnost*>)));

    connect(ui->vlastnostiObjektu, SIGNAL(VlastnostZmenena()), ui->pracovnaPlocha, SLOT(PrekresliAPrepocitajPlochu()));
    connect(ui->vlastnostiDokumentu, SIGNAL(VlastnostZmenena()), ui->pracovnaPlocha, SLOT(PrekresliAPrepocitajPlochu()));

    connect(ui->kontroly, &KontrolyPanel::KontrolyZmenene,
            [this](){
        ui->chyby->NastavChyby(ui->kontroly->VykonajKontroly());
    });

    connect(ui->chyby, &ChybyPanel::VybranyKomponent,
            [this](auto k){
        ui->pracovnaPlocha->NastavVybranyKomponent(k);
    });
}

HlavneOkno::~HlavneOkno() { delete ui; }

void HlavneOkno::NovyDokument() {
    _dokument = std::make_unique<Dokumenty::Dokument>();
    ui->pracovnaPlocha->NastavDokument(_dokument.get());
    ui->vlastnostiDokumentu->NastavVlastnosti(_dokument->VlastnostiDokumentu());

    ui->kontroly->NastavDokument(_dokument.get());

    connect(ui->pracovnaPlocha, &PracovnaPlocha::DokumentZmeneny,
            [this](auto d){
        ui->kontroly->NastavDokument(d);
        ui->chyby->NastavChyby(ui->kontroly->VykonajKontroly());
    });
}

void HlavneOkno::closeEvent(QCloseEvent *event) {
    //ulozi rozlozenie okna
    QSettings settings("MFF UK", "EditorCary");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    QMainWindow::closeEvent(event);
}

//vyberie odpovedajuci nastroj v paneli nastrojov
void HlavneOkno::nastavNastrojToolBar(Nastroje::Nastroj *nastroj) {
    for (auto btn : _nastrojeTlacitka) {
        if (nastroj && btn->toolTip() == nastroj->NastrojPresenter()->Nazov())
            btn->setChecked(true);
        else
            btn->setChecked(false);
    }
}

//informuje pracovnu plochu o zmene nastroja
void HlavneOkno::nastavNastroj(Nastroje::NastrojPtr nastroj) {
    ui->pracovnaPlocha->NastavNastroj(std::move(nastroj));
}

//naplni panel nastrojov
void HlavneOkno::nacitajNastroje() {
    _nastroje = ui->pracovnaPlocha->Dokument()->DostupneNastroje();

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

void HlavneOkno::exportDoPDF()
{
    QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Export do PDF", QString(), "*.pdf");
    if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".pdf"); }

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QSize(ui->pracovnaPlocha->Dokument()->Sirka(),
                               ui->pracovnaPlocha->Dokument()->Vyska()),
                         QPrinter::Millimeter);
    printer.setOutputFileName(fileName);

    QPainter p;
    p.begin(&printer);
    ui->pracovnaPlocha->VykresliPlochu(p);
    p.end();
}

void HlavneOkno::exportDoSVG()
{
    QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Export do SVG", QString(), "*.svg");
    if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".svg"); }

    QSvgGenerator generator;
    generator.setFileName(fileName);
    generator.setSize(QSize(ui->pracovnaPlocha->Dokument()->Sirka(),
                            ui->pracovnaPlocha->Dokument()->Vyska()));
    generator.setViewBox(QRect(0,0,
                               ui->pracovnaPlocha->Dokument()->Sirka(),
                               ui->pracovnaPlocha->Dokument()->Vyska()));
    generator.setTitle(tr("Editor čáry"));

    QPainter p;
    p.begin(&generator);
    ui->pracovnaPlocha->VykresliPlochu(p);
    p.end();
}

void HlavneOkno::ulozitAko()
{
    QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Uložiť ako", QString(), "*.xml");
    if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".xml"); }

    ulozit(fileName);
}

void HlavneOkno::otvorit()
{
    QString fileName = QFileDialog::getOpenFileName((QWidget* )0, "Otvoriť", QString(), "*.xml");
    if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".xml"); }

    QFile file(fileName);
    if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        QMessageBox::critical(this, "Chyba", "Pri otváraní súboru došlo k chybe!");
        return;
    }

    QDomDocument doc;

    if (!doc.setContent(&file)) {
        file.close();
        QMessageBox::critical(this, "Chyba", "Pri otváraní súboru došlo k chybe!");
        return;
    }

    ui->pracovnaPlocha->Dokument()->setCestaSubor(fileName);

    try
    {
        ui->pracovnaPlocha->Dokument()->ObnovDokument(doc);
    }
    catch(std::exception e){
        QMessageBox::critical(this, "Chyba", "Pri otváraní súboru došlo k chybe!");
    }
    ui->pracovnaPlocha->PrekresliAPrepocitajPlochu();
}

void HlavneOkno::novaDraha()
{
    NovyDokument();
}

void HlavneOkno::ulozit()
{
    if(ui->pracovnaPlocha->Dokument()->CestaSubor() == "")
        ulozitAko();
    else
        ulozit(ui->pracovnaPlocha->Dokument()->CestaSubor());
}

void HlavneOkno::ulozit(QString path)
{
    QFile outFile(path);
    if( !outFile.open( QIODevice::WriteOnly | QIODevice::Text ) )
    {
        QMessageBox::critical(this, "Chyba", "Pri ukladaní do súboru došlo k chybe!");
        return;
    }

    ui->pracovnaPlocha->Dokument()->setCestaSubor(path);
    QTextStream stream( &outFile );
    stream.setCodec("UTF-8");
    stream << ui->pracovnaPlocha->Dokument()->UlozDokument().toString(1);

    outFile.close();
}

void HlavneOkno::on_actionNov_dr_ha_triggered()
{
    novaDraha();
}

void HlavneOkno::on_actionOtvori_existuj_cu_dr_hu_triggered()
{
    otvorit();
}

void HlavneOkno::on_actionUlo_i_triggered()
{
    ulozit();
}

void HlavneOkno::on_actionUlo_i_ako_triggered()
{
    ulozitAko();
}

void HlavneOkno::on_actionUkon_i_triggered()
{
    close();
}
