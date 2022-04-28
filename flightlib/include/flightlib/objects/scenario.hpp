#pragma once

#include <yaml-cpp/yaml.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <filesystem>
#include <flightlib/objects/static_item.hpp>

namespace flightlib {

class Scenario {
    public:
        Scenario() {}
        ~Scenario(){}

        bool parseYaml (std::string filepath);

        // Lista de objetos de Unity donde guardamos la info leida del YAML
        // Es un vector de punteros_intelig a StaticItem
        // me falta hacerla privada y hacer bien sus metodos get y set

    private:
        std::vector< std::shared_ptr<StaticItem> >& getItemList();
        std::vector< std::shared_ptr<StaticItem> > item_list;

        
};

}  // namespace flightlib
