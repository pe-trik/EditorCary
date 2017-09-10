#include "dvojbodovy.h"
#include "manipulator.h"
#include "spojenieslot.h"
#include <QPen>

using namespace Komponenty;
using namespace Dokumenty;

Dvojbodovy::Dvojbodovy() {
  _x1 = std::make_unique<QrealVlastnost>("X1", 10);
  _y1 = std::make_unique<QrealVlastnost>("Y1", 10);
  _x2 = std::make_unique<QrealVlastnost>("X2", 200);
  _y2 = std::make_unique<QrealVlastnost>("Y2", 200);

  auto manipulator = std::make_unique<Manipulator>(_x1.get(), _y1.get());
  _spojenieSloty.push_back(
      std::make_unique<SpojenieSlot>(this, manipulator.get()));
  _manipulatory.push_back(std::move(manipulator));

  manipulator = std::make_unique<Manipulator>(_x2.get(), _y2.get());
  _spojenieSloty.push_back(
      std::make_unique<SpojenieSlot>(this, manipulator.get()));
  _manipulatory.push_back(std::move(manipulator));
}
