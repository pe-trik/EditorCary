#ifndef PANELVLASTNOSTI_H
#define PANELVLASTNOSTI_H

#include <Dokumenty/vlastnost.h>
#include <QFormLayout>
#include <QWidget>
#include <vector>

class PanelVlastnosti : public QWidget {
  Q_OBJECT
public:
  explicit PanelVlastnosti(QWidget *parent = nullptr);
  void Reset();
  void NastavVlastnosti(std::vector<Dokumenty::Vlastnost *> vlastnosti);

signals:

public slots:

private:
  QFormLayout *_layout;
  std::vector<VlastnostManagerPtr> _nastroje;
};

#endif // PANELVLASTNOSTI_H
