#pragma once

#include "flightlib/objects/static_object.hpp"

namespace flightlib {
class StaticPanel : public StaticObject {
 public:
  StaticPanel(const std::string& id, const std::string& prefab_id = "SolarPanel")
    : StaticObject(id, prefab_id) {}
  ~StaticPanel() {}
};

}  // namespace flightlib
