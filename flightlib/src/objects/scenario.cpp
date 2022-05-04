#include "flightlib/objects/scenario.hpp"

namespace flightlib {

    bool Scenario::exists(const std::string& id)
    {
        try
        {
            if ( hash_map[id] != nullptr){
                return true;
                std::cout << "Object found\n";
            }
            else
            {
                std::cout << "Null pointer\n";
                return false;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            std::cout << "Object not found\n";
            return false;

        }
    }


    // Adds an item to the list manually
    bool Scenario::addObject (const std::string& id, const std::string& prefab_id, 
            const Vector<3>& position, const Quaternion& quaternion)
    {
        if ( exists(id) )
        {
            // Create an auxiliar item as a shared_ptr
            std::shared_ptr<StaticItem> aux_item = std::make_shared<StaticItem>(id, prefab_id);
            aux_item->setPosition(position);
            aux_item->setQuaternion(quaternion);

            // Pushes the item into the list and updates the hash table
            item_list.push_back(aux_item);
            //hash_map[ id ] = numItems() - 1;
            hash_map[ id ] = aux_item;

            // Viendo el mapa
            std::cout << "**** Viendo el mapa:\n";
            for (const auto& n : hash_map) {
                std::cout << n.first << " = " << n.second << "\n";
            }

            return true;
        }
        else
        {
            return false;
        }
    }


    // Deletes an existing object from the item list
    bool Scenario::deleteObject(const std::string& id)
    {
        try
        {
            //int index = hash_map[id];

            // Removes the item from /*the list and its entry in*/ the hash table
            //getItemList().erase( getItemList().begin() + index );
            hash_map.erase(id);

            // Viendo el mapa
            std::cout << "**** Viendo el mapa:\n";
            for (const auto& n : hash_map) {
                std::cout << n.first << " = " << n.second << "\n";
            }

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
            int index = hash_map[id];
            getItemList()[index]->setPosition(position);
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
            int index = hash_map[id];
            getItemList()[index]->setQuaternion(quaternion);
            return true;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }
        
    }


    // Get number of Unity objects in the Scenario
    // int Scenario::numItems()
    // {
    //     return getItemList().size();
    // }


    // Recover object from list by string id
    // std::shared_ptr<StaticItem> Scenario::getObject (const std::string& id)
    // {
    //     int index = hash_map[id];
    //     return getItemList()[index];
    // }
    

    // Recover object from list by numeric index
    // std::shared_ptr<StaticItem> Scenario::getObject (const int& index)
    // {
    //     return getItemList()[index];
    // }


    // Private getter for item_list
    std::vector< std::shared_ptr<StaticItem> > Scenario::getItemList(){
        return Scenario::item_list;
    }


    // Reads YAML file indicated in filename
    bool Scenario::parseYaml(const std::string& filename)
    {
        // Returns true if parsing is ok
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
                
                // Current item position and orientation
                Vector<3> posit = {
                    node[key]["position"]["x"].as<float>(),
                    node[key]["position"]["y"].as<float>(),
                    node[key]["position"]["z"].as<float>()
                    };

                Quaternion quat = Quaternion(
                    node[key]["quaternion"]["w"].as<float>(),
                    node[key]["quaternion"]["x"].as<float>(),
                    node[key]["quaternion"]["y"].as<float>(),
                    node[key]["quaternion"]["z"].as<float>()
                    );              

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


                // Create an auxiliar item as a shared_ptr
                // taking the id and prefab_id from current key
                // std::shared_ptr<StaticItem> aux_item =
                // std::make_shared<StaticItem>(
                //     node[key]["id"].as<std::string>(),
                //     node[key]["prefab_id"].as<std::string>()
                // );
                
                // Sets position with a Vector3 and current key
                // aux_item->setPosition(
                //     {
                //     node[key]["position"]["x"].as<float>(),
                //     node[key]["position"]["y"].as<float>(),
                //     node[key]["position"]["z"].as<float>()
                //     }
                // );

                // Sets orientation with Quaternion and current key
                // aux_item->setQuaternion(
                //     Quaternion(
                //         node[key]["quaternion"]["w"].as<float>(),
                //         node[key]["quaternion"]["x"].as<float>(),
                //         node[key]["quaternion"]["y"].as<float>(),
                //         node[key]["quaternion"]["z"].as<float>()
                //     )
                // );
                
                // Saves the item into the list of the Scenario
                //item_list.push_back(aux_item);

                // Updating the hash table with id-index pair
                // /*hash_map[ node[key]["id"].as<std::string>() ] = index;*/
                //hash_map[ node[key]["id"].as<std::string>() ] = aux_item;
                

                // Ver el mapa como se va rellenando
                std::cout << "**** Viendo el mapa:\n";
                for (const auto& n : hash_map) {
                    std::cout << n.first << " = " << n.second << "\n";
                }
            }
        }
        catch(const std::exception& e)
        {
            // Returns false in case of error
            std::cerr << e.what() << '\n';
            ret = false;
        }

        return ret;
    }

}  // namespace flightlib