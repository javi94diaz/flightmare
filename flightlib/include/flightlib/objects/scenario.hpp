#pragma once

#include <yaml-cpp/yaml.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <filesystem>
#include <flightlib/objects/static_item.hpp>
#include <flightlib/bridges/unity_bridge.hpp>

namespace flightlib {

class Scenario {
    public:
        Scenario(std::shared_ptr<UnityBridge>& unity_bridge_ptr )
        : unity_bridge_ptr_(unity_bridge_ptr){};
        ~Scenario(){};

        void printMap();
        bool exists(const std::string& id);

        std::shared_ptr<StaticItem> getObject (const std::string& id);

        bool addObject (const std::string& id, const std::string& prefab_id, 
            const Vector<3>& position, const Quaternion& quaternion);
        
        bool deleteObject(const std::string& id);
        bool setPosition (const std::string& id, const Vector<3>& position);
        bool setQuaternion (const std::string& id, const Quaternion& quaternion);
        int numItems();
        bool parseYaml (const std::string& filepath);

    private:
        // Map of items to render in Unity: pairs <id, pointer>
        std::map<std::string, std::shared_ptr<StaticItem> > hash_map;
        std::shared_ptr<UnityBridge> unity_bridge_ptr_;

        // List of items to render in Unity
        //std::map< std::shared_ptr<StaticItem> > getItemList();
        //std::vector< std::shared_ptr<StaticItem> > item_list;
        
};

}  // namespace flightlib
