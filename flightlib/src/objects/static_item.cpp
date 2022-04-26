#include "flightlib/objects/static_item.hpp"

namespace flightlib {

    void StaticItem::parseJson ()
    {
        // Leer un fichero json
        std::ifstream myfile("src/panels/scenario.json");
        nlohmann::json j;
        try{
            myfile >> j;
        }
        catch(std::exception exc){
        std::cout << "ERROR al pasar el fichero al objeto json.";
        }
        std::cout << j << std::endl;
    }

    void StaticItem::parseYaml()
    {
        std::cout << "Parsing YAML file" << std::endl;

        const std::string config_path = "scenario.yaml";

        std::ifstream config_file(config_path);

        YAML::Node config_node = YAML::Load(config_file);

        std::cout << config_node << std::endl;
    }

}  // namespace flightlib