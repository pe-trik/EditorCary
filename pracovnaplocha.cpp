#include "pracovnaplocha.h"

#include <QPainter>
#include <algorithm>

using namespace Dokumenty;

PracovnaPlocha::PracovnaPlocha(QWidget *parent) : QWidget(parent) {
    setMouseTracking(true); // zapne sledovanie mysi, aj ked nie su stlacene ziadne tlacitka
    _transformacia = QTransform();

    connect(&_timerVykresli, SIGNAL(timeout()), this,
            SLOT(timerVykresliTimeout()));
    _timerVykresli.setSingleShot(true);
    _timerVykresli.setInterval(1000 / 60); //60fps

    setFocusPolicy(Qt::WheelFocus);
}

// vykresli pracovnu plochu; tranformacia pre PracovnuPlochu
void PracovnaPlocha::VykresliPlochu(QPainter &painter,
                                    bool aplikujTranformaciu) {
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    if (_dokument != nullptr) {
        _sirka = _dokument->Sirka();
        _vyska = _dokument->Vyska();

        if (aplikujTranformaciu) {
            qreal pomer = static_cast<qreal>(width()) / static_cast<qreal>(height());
            painter.setWorldTransform(_transformacia);

            if ((_dokument->Sirka() / _dokument->Vyska()) < pomer)
                _sirka = _dokument->Vyska() * pomer;
            else
                _vyska = _dokument->Sirka() / pomer;
        }

        painter.setWindow(0, 0, static_cast<int>(_sirka), static_cast<int>(_vyska));

        _dokument->Vykresli(painter);
    }
}

void PracovnaPlocha::NastavDokument(Dokumenty::Dokument *dokument) {
    _dokument = dokument;
    connect(dokument, &Dokument::DokumentPrepocitany,
            [this, dokument](){
        emit DokumentZmeneny(dokument);
    });
    connect(dokument, SIGNAL(Prekresli()), this, SLOT(PrekresliAPrepocitajPlochu()));
    _zmeny.clear();
    _zmenaPozicia = 0;
    _zmeny.push_back(_dokument->UlozDokument());
    emit DokumentZmeneny(dokument);
}

QPointF PracovnaPlocha::PolohaMysi() {
    return _inverznaTransformacia.map(
                QPointF(_polohaMysi.x() / static_cast<qreal>(width()) * _sirka,
                        _polohaMysi.y() / static_cast<qreal>(height()) * _vyska));
}

void PracovnaPlocha::NastavNastroj(Nastroje::NastrojPtr nastroj) {
    _nastroj = std::move(nastroj);
    NastrojZmeneny(_nastroj.get());
}

void PracovnaPlocha::PrekresliAPrepocitajPlochu() {
    //aktivuje timer
    if (!_timerVykresli.isActive())
        _timerVykresli.start();
}

void PracovnaPlocha::timerVykresliTimeout() {
    _timerVykresli.stop();
    _dokument->VycistiSpojenia();
    _dokument->PrepocitajDokument();
    repaint();
}

void PracovnaPlocha::NastavVybranyKomponent(Komponenty::Komponent *k)
{
    _dokument->NastavVybranyKomponent(k);

    if(k){
        NastavNastroj(k->Nastroj(_dokument));
            emit VlastnostiZmenene(k->Vlastnosti());
    }
    else
        emit VlastnostiZmenene({});

    repaint();
}

void PracovnaPlocha::mouseMoveEvent(QMouseEvent *event) {
    if (_mysStlacena && (!_nastroj || !_nastroj->VybranyKomponent())) {
        auto delta = (event->localPos() - _polohaMysi) / _zoom;
        _transformacia.translate(delta.x() / static_cast<qreal>(width()) * _sirka,
                                 delta.y() / static_cast<qreal>(height()) * _vyska);
        _inverznaTransformacia = _transformacia.inverted();
    } else if (_nastroj)
        _nastroj->MysPohyb(PolohaMysi());

    _polohaMysi = event->localPos();

    if(_mysStlacena)
        PrekresliAPrepocitajPlochu();

    emit PolohaMysiZmenena(PolohaMysi());
}

void PracovnaPlocha::mousePressEvent(QMouseEvent *) {
    _mysStlacena = true;

    auto b = PolohaMysi();

    if (_nastroj)
        _nastroj->MysStlacena(b);

    if (_nastroj == nullptr || !_nastroj->VybranyKomponent()) {
        if (auto k = _dokument->NajdiKomponentPodBodom(b))
        {
            _nastroj = k->Nastroj(_dokument);
            _nastroj->MysStlacena(b);
        }
    }



    NastrojZmeneny(_nastroj.get());
    if (_dokument->VybranyKomponent() != nullptr)
        emit VlastnostiZmenene(_dokument->VybranyKomponent()->Vlastnosti());
    else
        emit VlastnostiZmenene({});

    PrekresliAPrepocitajPlochu();
}

void PracovnaPlocha::mouseReleaseEvent(QMouseEvent *) {
    _dokument->VytvorSpojenia(PolohaMysi());

    if (_nastroj)
        _nastroj->MysUvolnena(PolohaMysi());

    if (_dokument->VybranyKomponent() != nullptr)
        emit VlastnostiZmenene(_dokument->VybranyKomponent()->Vlastnosti());
    else
        emit VlastnostiZmenene({});

    if(_dvojklik && _nastroj)
    {
        PrekresliAPrepocitajPlochu();
        auto b = _zmeny.begin() + _zmenaPozicia + 1;
        if(b < _zmeny.end())
            _zmeny.erase(b, _zmeny.end());
        _zmeny.push_back(_dokument->UlozDokument());
        _zmenaPozicia++;
    }

    _mysStlacena = false;
}

void PracovnaPlocha::wheelEvent(QWheelEvent *event) {
    if (event->delta() > 0) {
        _zoom *= 1.2;
        _transformacia.scale(1.2, 1.2);
    } else {
        _zoom /= 1.2;
        _transformacia.scale(1 / 1.2, 1 / 1.2);
    }
    _inverznaTransformacia = _transformacia.inverted();

    PrekresliAPrepocitajPlochu();
}

void PracovnaPlocha::mouseDoubleClickEvent(QMouseEvent *) {
    _dvojklik = true;
    if (_nastroj)
        _nastroj->MysDvojklik(PolohaMysi());
    _dokument->VytvorSpojenia(PolohaMysi());
    if (_dokument->VybranyKomponent() != nullptr)
        emit VlastnostiZmenene(_dokument->VybranyKomponent()->Vlastnosti());
    else
    {
        _nastroj.reset();
        NastrojZmeneny(nullptr);
    }

    PrekresliAPrepocitajPlochu();
}

void PracovnaPlocha::keyPressEvent(QKeyEvent *event)
{
    if((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier){
        if(event->key() == Qt::Key_Z){
            if(_zmenaPozicia > 0)
            {
                --_zmenaPozicia;
                _dokument->ObnovDokument(_zmeny.at(static_cast<int>(_zmenaPozicia)));
                repaint();
            }
        }
        if(event->key() == Qt::Key_Y){
            if(_zmenaPozicia + 1 < _zmeny.size())
            {
                ++_zmenaPozicia;
                _dokument->ObnovDokument(_zmeny.at(static_cast<int>(_zmenaPozicia)));
                repaint();
            }
        }
    }

    if(event->key() == Qt::Key_Delete){
        _dokument->ZmazVybranyKomponent();
        PrekresliAPrepocitajPlochu();
        auto b = _zmeny.begin() + _zmenaPozicia + 1;
        if(b < _zmeny.end())
            _zmeny.erase(b, _zmeny.end());
        _zmeny.push_back(_dokument->UlozDokument());
        _zmenaPozicia++;
    }
}

void PracovnaPlocha::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    VykresliPlochu(painter, true);
    if (_mysStlacena)
        painter.fillRect(QRectF(PolohaMysi(), QSizeF(10, 10)), Qt::red);
}

Dokumenty::Dokument *PracovnaPlocha::Dokument() const
{
    return _dokument;
}

void PracovnaPlocha::setDokument(Dokumenty::Dokument *dokument)
{
    _dokument = dokument;
    emit DokumentZmeneny(dokument);
}
