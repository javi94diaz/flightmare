#include "flightlib/objects/scenario.hpp"

namespace flightlib {

    // PONERTIPO Scenario::getItemList()
    // {
    //     return Scenario::item_list;
    // }

    // void Scenario::setItem(StaticItem item)
    // {
    //     Scenario::item = &item;
    //     std::cout << "setted" << std::endl;
    // }

    // Reads YAML file indicated in filename
    bool Scenario::parseYaml(std::string filename)
    {
        bool ret = true;
        std::cout << "Parsing YAML file" << std::endl;
        
        try
        {
            // Loads the file into a node
            YAML::Node node;
            std::ifstream filestream(filename);
            node = YAML::Load(filestream);

            // Iterates over the keys to read the information below each node
            for ( YAML::const_iterator iter = node.begin(); iter != node.end() ; ++iter ) 
            {
                // Current node key
                std::string key = iter->first.as<std::string>();
                
                // Create an auxiliar item as a shared_ptr
                // taking the id and prefab_id from current key
                std::shared_ptr<StaticItem> aux_item =
                std::make_shared<StaticItem>(
                    node[key]["id"].as<std::string>(),
                    node[key]["prefab_id"].as<std::string>()
                );
                
                // Sets position with a Vector3 and current key
                aux_item->setPosition(
                    {
                    node[key]["position"]["x"].as<float>(),
                    node[key]["position"]["y"].as<float>(),
                    node[key]["position"]["z"].as<float>()
                    }
                );

                // Sets orientation with Quaternion and current key
                aux_item->setQuaternion(
                    Quaternion(
                        node[key]["quaternion"]["w"].as<float>(),
                        node[key]["quaternion"]["x"].as<float>(),
                        node[key]["quaternion"]["y"].as<float>(),
                        node[key]["quaternion"]["z"].as<float>()
                    )
                );
                
                // Saves the item into the list of the Scenario
                item_list.push_back(aux_item);

            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            ret = false;
        }
        
        return ret;
    }

}  // namespace flightlib