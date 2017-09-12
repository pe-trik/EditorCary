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
  _timerVykresli.setInterval(1000 / 60); // 30fps
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

  PrekresliAPrepocitajPlochu();
}

void PracovnaPlocha::mousePressEvent(QMouseEvent *event) {
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

void PracovnaPlocha::mouseReleaseEvent(QMouseEvent *event) {
  _dokument->VytvorSpojenia(PolohaMysi());

  _mysStlacena = false;
  if (_nastroj)
    _nastroj->MysUvolnena(PolohaMysi());

  if (_dokument->vybranyKomponent() != nullptr)
    emit VlastnostiZmenene(_dokument->vybranyKomponent()->Vlastnosti());
  else
    emit VlastnostiZmenene({});

  PrekresliAPrepocitajPlochu();
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

void PracovnaPlocha::mouseDoubleClickEvent(QMouseEvent *event) {
  if (_nastroj)
    _nastroj->MysDvojklik(PolohaMysi());
  _dokument->VytvorSpojenia(PolohaMysi());
  if (_dokument->vybranyKomponent() != nullptr)
    emit VlastnostiZmenene(_dokument->vybranyKomponent()->Vlastnosti());

  PrekresliAPrepocitajPlochu();
}

void PracovnaPlocha::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  VykresliPlochu(painter, true);
  if (_mysStlacena)
    painter.fillRect(QRectF(PolohaMysi(), QSizeF(10, 10)), Qt::red);
}
