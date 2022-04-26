#pragma once

#include "flightlib/objects/static_object.hpp"
#include "flightlib/json/json.hpp"
#include <yaml-cpp/yaml.h>
#include <fstream>

namespace flightlib {
class StaticItem : public StaticObject {
  public:
    StaticItem(const std::string& id, const std::string& prefab_id = "SolarPanel")
      : StaticObject(id, prefab_id) {}
    ~StaticItem() {}

    void parseJson (/*std::string filename*/);

    void parseYaml (/*std::string filename*/);
};

}  // namespace flightlib
