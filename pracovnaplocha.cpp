#include "pracovnaplocha.h"

#include <QPainter>
#include <algorithm>

using namespace Dokumenty;

PracovnaPlocha::PracovnaPlocha(QWidget *parent) : QWidget(parent) {
    setMouseTracking(
                true); // zapne sledovanie mysi, aj ked nie su stlacene ziadne tlacitka
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
        _sirka = _dokument->sirka();
        _vyska = _dokument->vyska();

        if (aplikujTranformaciu) {
            qreal pomer = static_cast<qreal>(width()) / static_cast<qreal>(height());
            painter.setWorldTransform(_transformacia);

            if ((_dokument->sirka() / _dokument->vyska()) < pomer)
                _sirka = _dokument->vyska() * pomer;
            else
                _vyska = _dokument->sirka() / pomer;
        }

        painter.setWindow(0, 0, static_cast<int>(_sirka), static_cast<int>(_vyska));

        _dokument->Vykresli(painter);
    }
}

void PracovnaPlocha::NastavDokument(Dokument *dokument) {
    _dokument = dokument;
    connect(dokument, SIGNAL(prekreslit()), this, SLOT(PrekresliAPrepocitajPlochu()));
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
    if (!_timerVykresli.isActive())
        _timerVykresli.start();
}

void PracovnaPlocha::timerVykresliTimeout() {
    _dokument->VycistiSpojenia();
    _dokument->Prepocitaj();
    repaint();
    _timerVykresli.stop();
}

void PracovnaPlocha::mouseMoveEvent(QMouseEvent *event) {
    if (_mysStlacena && !_nastroj) {
        auto delta = (event->localPos() - _polohaMysi) / _zoom;
        _transformacia.translate(delta.x() / static_cast<qreal>(width()) * _sirka,
                                 delta.y() / static_cast<qreal>(height()) * _vyska);
        _inverznaTransformacia = _transformacia.inverted();
    } else if (_nastroj)
        _nastroj->MysPohyb(PolohaMysi());

    _polohaMysi = event->localPos();

    if(_mysStlacena && _nastroj)
        PrekresliAPrepocitajPlochu();
}

void PracovnaPlocha::mousePressEvent(QMouseEvent *) {
    _mysStlacena = true;

    auto b = PolohaMysi();

    if (_nastroj == nullptr) {
        if (auto k = _dokument->Komponent(b))
            _nastroj = k->Nastroj(_dokument);
    }

    if (_nastroj)
        _nastroj->MysStlacena(b);

    NastrojZmeneny(_nastroj.get());
    if (_dokument->vybranyKomponent() != nullptr)
        emit VlastnostiZmenene(_dokument->vybranyKomponent()->Vlastnosti());
    else
        emit VlastnostiZmenene({});

    PrekresliAPrepocitajPlochu();
}

void PracovnaPlocha::mouseReleaseEvent(QMouseEvent *) {
    _dokument->VytvorSpojenia(PolohaMysi());

    if (_nastroj)
        _nastroj->MysUvolnena(PolohaMysi());

    if (_dokument->vybranyKomponent() != nullptr)
        emit VlastnostiZmenene(_dokument->vybranyKomponent()->Vlastnosti());
    else
        emit VlastnostiZmenene({});

    if(_mysStlacena && _nastroj)
    {
        PrekresliAPrepocitajPlochu();
        auto b = _zmeny.begin() + _zmenaPozicia + 1;
        if(b < _zmeny.end())
            _zmeny.erase(b, _zmeny.end());
        _zmeny.push_back(_dokument->Uloz());
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
    if (_nastroj)
        _nastroj->MysDvojklik(PolohaMysi());
    _dokument->VytvorSpojenia(PolohaMysi());
    if (_dokument->vybranyKomponent() != nullptr)
        emit VlastnostiZmenene(_dokument->vybranyKomponent()->Vlastnosti());

    PrekresliAPrepocitajPlochu();
}

void PracovnaPlocha::keyPressEvent(QKeyEvent *event)
{
    if((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier){
        if(event->key() == Qt::Key_Z){
            if(_zmenaPozicia > 0)
            {
                --_zmenaPozicia;
                _dokument->Obnov(_zmeny.at(_zmenaPozicia));
                repaint();
            }
        }
        else if(event->key() == Qt::Key_Y){
            if(_zmenaPozicia + 1 < _zmeny.size())
            {
                ++_zmenaPozicia;
                _dokument->Obnov(_zmeny.at(_zmenaPozicia));
                repaint();
            }
        }
    }
}

void PracovnaPlocha::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    VykresliPlochu(painter, true);
    if (_mysStlacena)
        painter.fillRect(QRectF(PolohaMysi(), QSizeF(10, 10)), Qt::red);
}

Dokumenty::Dokument *PracovnaPlocha::dokument() const
{
    return _dokument;
}

void PracovnaPlocha::setDokument(Dokumenty::Dokument *dokument)
{
    _dokument = dokument;
}
