#pragma once

#include "flightlib/objects/static_object.hpp"
#include "flightlib/json/json.hpp"
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <unistd.h>
#include <filesystem>

namespace flightlib {

class StaticItem : public StaticObject {
  public:
    StaticItem(const std::string& id, const std::string& prefab_id = "SolarPanel")
      : StaticObject(id, prefab_id) {}
    ~StaticItem() {}
};

}  // namespace flightlib
