#include "flightlib/objects/static_item.hpp"

namespace flightlib {

    // void StaticItem::parseYaml()
    // {
    //     std::cout << "Parsing YAML file" << std::endl;

    //     YAML::Node node;

    //     // Get current working directory (it is /home/javier/.ros)
    //     size_t tam = 50;
    //     std::string current_dir = getcwd(NULL, tam);
    //     std::cout << "Current directory: " << current_dir << std::endl;

    //     // Print list of files and folders under current directory
    //     for (const auto & file : std::filesystem::directory_iterator(current_dir))
    //     {
    //         std::cout << file.path() << std::endl;
    //     }

    //     std::cout<< "ALTERNATIVA" << std::endl;
        
    //     // Usando path absoluto (funciona)
    //     //const std::string filepath = "/home/javier/catkin_ws/src/flightmare/flightlib/src/objects/";
        
    //     // Usando path relativo (funciona tambien)
    //     const std::string filepath = "../catkin_ws/src/flightmare/flightlib/src/objects/";

    //     for (const auto & file : std::filesystem::directory_iterator(filepath))
    //     {
    //         std::cout << file.path() << std::endl;
    //     }
        
    //     std::string filename = "scenario.yaml";

    //     std::ifstream filestream(filepath + filename);
    //     node = YAML::Load(filestream);
    //     std::cout << "VER YAML: " << node["object_1"] << std::endl;
        
    //     YAML::Emitter emitter;
    //     emitter << node;
    //     std::cout<< "EMITTER Node:\n" << emitter.c_str() << std::endl;
        
    // }

}  // namespace flightlib