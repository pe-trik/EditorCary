#ifndef PRACOVNAPLOCHA_H
#define PRACOVNAPLOCHA_H

#include <QWidget>
#include <QTimer>
#include "Dokumenty/dokument.h"
#include "Komponenty/komponent.h"
#include "Nastroje/nastroj.h"
#include <QMouseEvent>

class PracovnaPlocha : public QWidget {
  Q_OBJECT
public:
  explicit PracovnaPlocha(QWidget *parent = nullptr);

  void VykresliPlochu(QPainter &painter, bool aplikujTranformaciu = false);

  void NastavDokument(Dokumenty::Dokument *dokument);

  QPointF PolohaMysi();

  void NastavNastroj(Nastroje::NastrojPtr nastroj);

signals:
  void NastrojZmeneny(Nastroje::Nastroj* nastroj);
  void VlastnostiZmenene(const std::vector<Dokumenty::Vlastnost*>& vlastnosti);

public slots:
  void PrekresliAPrepocitajPlochu();
  void timerVykresliTimeout();

protected:
  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);
  void mouseDoubleClickEvent(QMouseEvent * event);

private:
  void paintEvent(QPaintEvent *event);

  qreal _zoom = 1;
  QPointF _posunutie;
  QTransform _transformacia; 
  QTransform _inverznaTransformacia;
  qreal _sirka, _vyska;
  bool _mysStlacena = false;
  QPointF _polohaMysi;
  Dokumenty::Dokument *_dokument = nullptr;
  Nastroje::NastrojPtr _nastroj;
  QTimer _timerVykresli;
};

#endif // PRACOVNAPLOCHA_H
