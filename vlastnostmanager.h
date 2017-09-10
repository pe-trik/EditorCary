#ifndef VLASTNOSTMANAGER_H
#define VLASTNOSTMANAGER_H

#include <QWidget>
#include <memory>

class VlastnostManager {
public:
  VlastnostManager();
  virtual QWidget *Nastroj() = 0;
  virtual QString Nazov() const = 0;
};

using VlastnostManagerPtr = std::unique_ptr<VlastnostManager>;

#endif // VLASTNOSTMANAGER_H
