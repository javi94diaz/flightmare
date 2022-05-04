#include "flightlib/objects/scenario.hpp"

namespace flightlib {

    // Outputs the elements of hash_map
    void Scenario::printMap()
    {
        std::cout << "**** Printing map:\n";
        for (const auto& n : hash_map) {
            std::cout << n.first << " = " << n.second << "\n";
        }
    }


    // Checks if an item exists in the hash table by its id
    bool Scenario::exists(const std::string& id)
    {
        return hash_map.count(id) != 0;      
    }

    
    // Recover object from hash_map by its id
    std::shared_ptr<StaticItem> Scenario::getObject (const std::string& id)
    {
        if ( exists(id) )
        {
            return hash_map[id];
        }
        else
        {
            return nullptr;
        } 
    }


    // Adds an item to the list manually
    bool Scenario::addObject (const std::string& id, const std::string& prefab_id, 
        const Vector<3>& position, const Quaternion& quaternion)
    {
        if ( !exists(id) )
        {
            // Create an auxiliar item with a shared_ptr to StaticItem
            std::shared_ptr<StaticItem> aux_item = std::make_shared<StaticItem>(id, prefab_id);
            aux_item->setPosition(position);
            aux_item->setQuaternion(quaternion);

            // Pushes the item into the hash table
            hash_map[ id ] = aux_item;

            std::cout << "Adding...";;
            unity_bridge_ptr_->addStaticObject(aux_item);
            std::cout << " Added " << id << " to Unity succesfully" <<std::endl;
            
            printMap();
            
            return true;
        }
        else
        {
            std::cout << "Object " << id << " not added" <<std::endl;
            return false;
        }
    }

    // Deletes an existing object from the item list
    bool Scenario::deleteObject(const std::string& id)
    {
        try
        {
            // Removes the item from the hash table
            hash_map.erase(id);
            printMap();
            return true;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }
    }


    // Sets position of an existing item (named by id) in the list 
    bool Scenario::setPosition (const std::string& id, const Vector<3>& position)
    {
        try
        {
            hash_map[id]->setPosition(position);
            return true;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }
    }


    // Sets orientation of an existing item (named by id) in the list 
    bool Scenario::setQuaternion (const std::string& id, const Quaternion& quaternion)
    {
        try
        {
            hash_map[id]->setQuaternion(quaternion);
            return true;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }
        
    }


    // Get number of Unity objects in the Scenario
    int Scenario::numItems()
    {
        return hash_map.size();
        printMap();
    }


    // Reads YAML file indicated in filename, returning true if ok, false if not
    bool Scenario::parseYaml(
        const std::string& filename)
    {
        std::cout << "Parsing YAML file" << std::endl;
        
        try
        {
            // Loads the file into a node
            YAML::Node node;
            std::ifstream filestream(filename);
            node = YAML::Load(filestream);

            // Prints parsed YAML file with Emitter
            // YAML::Emitter emitter;
            // emitter << node;
            // std::cout<< "EMITTER Node:\n" << emitter.c_str() << std::endl;

            // Iterates over the keys to read the information below each node
            for ( YAML::const_iterator iter = node.begin(); iter != node.end() ; ++iter ) 
            {
                           
                // Current node key
                std::string key = iter->first.as<std::string>();
                
                //std::cout << "key: "<< key << std::endl;

                // Current item position and orientation
                Vector<3> posit = {
                    node[key]["position"]["x"].as<float>(),
                    node[key]["position"]["y"].as<float>(),
                    node[key]["position"]["z"].as<float>()
                    };

                //std::cout << "position created: "<< posit << std::endl;


                Quaternion quat = Quaternion(
                    node[key]["quaternion"]["w"].as<float>(),
                    node[key]["quaternion"]["x"].as<float>(),
                    node[key]["quaternion"]["y"].as<float>(),
                    node[key]["quaternion"]["z"].as<float>()
                    );              

                //std::cout << "quaternion created z: "<< node[key]["quaternion"]["z"].as<float>() << std::endl;

                // Adds item of current key to the list and the map
                if ( addObject(
                    node[key]["id"].as<std::string>(),
                    node[key]["prefab_id"].as<std::string>(),
                    posit,
                    quat
                    ) )
                {
                    std::cout << "Added ok" << std::endl;

                }
                else
                {
                    std::cout << "Not added" << std::endl; 
                }

                printMap();
                
            }
            return true;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }
    }

}  // namespace flightlib