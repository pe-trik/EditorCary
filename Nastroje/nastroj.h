#ifndef NASTROJ_H
#define NASTROJ_H

#include <QObject>
#include <memory>

#include <Dokumenty/dokument.h>

namespace Dokumenty {
class Dokument;
}

namespace Nastroje {

class NastrojPresenter;

using NastrojPresenterPtr = std::unique_ptr<NastrojPresenter>;

class Nastroj : public QObject {
  Q_OBJECT
public:
  explicit Nastroj(Dokumenty::Dokument *dokument, QObject *parent = nullptr);
  virtual NastrojPresenterPtr NastrojPresenter() const = 0;
signals:

public slots:
  virtual void MysStlacena(QPointF) = 0;
  virtual void MysPohyb(QPointF) = 0;
  virtual void MysUvolnena(QPointF) = 0;
  virtual void MysDvojklik(QPointF) = 0;

protected:
  Dokumenty::Dokument *_dokument;
};

using NastrojPtr = std::unique_ptr<Nastroj>;

class NastrojPresenter {
public:
  virtual QString Nazov() const = 0;
  virtual QString Ikonka() const = 0;
  virtual NastrojPtr Nastroj(Dokumenty::Dokument *) const = 0;
  virtual ~NastrojPresenter() {}
};
}

#endif // NASTROJ_H
