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

  void NastavDokument(Dokumenty::Dokument *Dokument);

  QPointF PolohaMysi();

  void NastavNastroj(Nastroje::NastrojPtr nastroj);

  Dokumenty::Dokument *Dokument() const;
  void setDokument(Dokumenty::Dokument *Dokument);

signals:
  void PolohaMysiZmenena(QPointF);
  void NastrojZmeneny(Nastroje::Nastroj* nastroj);
  void VlastnostiZmenene(const std::vector<Dokumenty::Vlastnost*>& vlastnosti);
  void DokumentZmeneny(Dokumenty::Dokument*);

public slots:
  void PrekresliAPrepocitajPlochu();
  void timerVykresliTimeout();
  void NastavVybranyKomponent(Komponenty::Komponent* k);

protected:
  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);
  void wheelEvent(QWheelEvent *event);
  void mouseDoubleClickEvent(QMouseEvent * event);
  void keyPressEvent(QKeyEvent * event);

private:
  void paintEvent(QPaintEvent *event);

  qreal _zoom = 1;
  QPointF _posunutie;
  QTransform _transformacia; 
  QTransform _inverznaTransformacia;
  qreal _sirka, _vyska;
  bool _mysStlacena = false, _dvojklik = false;
  QPointF _polohaMysi;
  Dokumenty::Dokument *_dokument = nullptr;
  Nastroje::NastrojPtr _nastroj;
  QTimer _timerVykresli;
  QVector<QDomDocument> _zmeny;
  size_t _zmenaPozicia = 0;
};

#endif // PRACOVNAPLOCHA_H
