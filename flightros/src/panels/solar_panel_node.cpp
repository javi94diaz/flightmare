#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <ros/ros.h>

#include <unistd.h>

// flightlib
#include "flightlib/bridges/unity_bridge.hpp"
#include "flightlib/common/quad_state.hpp"
#include "flightlib/common/types.hpp"
#include "flightlib/objects/quadrotor.hpp"

#include "flightlib/bridges/unity_message_types.hpp"
#include "flightlib/objects/static_gate.hpp"
#include "flightlib/objects/static_item.hpp"
#include "flightlib/objects/scenario.hpp"
#include "flightlib/sensors/rgb_camera.hpp"

// flightros
#include "flightros/pilot/flight_pilot.hpp"

// trajectory
#include <polynomial_trajectories/minimum_snap_trajectories.h>
#include <polynomial_trajectories/polynomial_trajectories_common.h>
#include <polynomial_trajectories/polynomial_trajectory.h>
#include <polynomial_trajectories/polynomial_trajectory_settings.h>
#include <quadrotor_common/trajectory_point.h>

int main(int argc, char *argv[]) {

    // Initialize ROS
    ros::init(argc, argv, "panels_example");
    ros::NodeHandle nh("");
    ros::NodeHandle pnh("~");
    ros::Rate(50.0);

    // Flightmare(Unity3D)
    std::shared_ptr<UnityBridge> unity_bridge_ptr = UnityBridge::getInstance();
    SceneID scene_id{UnityScene::CUSTOM};
    bool unity_ready{false};

    // Instantiate Unity quadrotor
    std::shared_ptr<Quadrotor> quad_ptr = std::make_shared<Quadrotor>();
    QuadState quad_state;
    quad_state.setZero();
    quad_ptr->reset(quad_state);
    // define quadsize scale (for unity visualization only)
    Vector<3> quad_size(0.5, 0.5, 0.5);
    quad_ptr->setSize(quad_size);

    // Initialize Unity Scenario
    std::shared_ptr<Scenario> scenario_1 = std::make_shared<Scenario>(unity_bridge_ptr);
    
    std::cout << "\nNUMITEMS ANTES DEL YAML: " << scenario_1->numItems() << std::endl;

    // Reading YAML file containing some items
    bool yaml_success = scenario_1->parseYaml(
        "../catkin_ws/src/flightmare/flightros/src/resources/scenario.yaml");

    std::cout << "\nNUMITEMS DESPUES del YAML:" << scenario_1->numItems() << std::endl;
    scenario_1->printMap();

    
    // Prefab names in Unity
    std::string prefab_id_panel = "SolarPanel";
    std::string prefab_id_panel_mod = "SolarPanelModif";
    std::string prefab_id_light = "DirectionalLight";
    std::string prefab_id_terrain = "Terrain";
    std::string prefab_id_cam = "HDCamera";

    // Initialize Unity objects manually with flightlib methods

    // std::string object_id = "panel1";
    // std::shared_ptr<StaticItem> panel_1 = 
    //     std::make_shared<StaticItem>(object_id, prefab_id_panel_mod);
    // panel_1->setPosition(Eigen::Vector3f(-5, 0, 0));
    // panel_1->setQuaternion(
    //     Quaternion(std::cos(1 * M_PI_4), 0.0, 0.0, std::sin(1 * M_PI_4)));



    // Adding a panel manually into the map
    bool addSuccess = scenario_1->addObject( 
        "panel_manual",
        prefab_id_panel, 
        Eigen::Vector3f(10, 0, 1),
        Quaternion(std::cos(1 * M_PI_4), 0.0, 0.0, std::sin(1 * M_PI_4))
        );

    std::cout << "NO EXISTE: " << scenario_1->exists("patata") << std::endl;
    std::cout << "SI EXISTE: " << scenario_1->exists("panel_manual") << std::endl;

    std::cout << "\nNUMITEMS DESPUES del YAML Y panel_manual: " << scenario_1->numItems() << std::endl;
    scenario_1->printMap();


    // Define path of quadrotor through gates
    std::vector<Eigen::Vector3d> way_points;
    way_points.push_back(Eigen::Vector3d(0, 10, 2.5));
    way_points.push_back(Eigen::Vector3d(5, 0, 2.5));
    way_points.push_back(Eigen::Vector3d(0, -10, 2.5));
    way_points.push_back(Eigen::Vector3d(-5, 0, 2.5));

    std::size_t num_waypoints = way_points.size();
    Eigen::VectorXd segment_times(num_waypoints);
    segment_times << 10.0, 10.0, 10.0, 10.0;
    Eigen::VectorXd minimization_weights(5);
    minimization_weights << 0.0, 1.0, 1.0, 1.0, 1.0;

    polynomial_trajectories::PolynomialTrajectorySettings trajectory_settings =
        polynomial_trajectories::PolynomialTrajectorySettings(
        way_points, minimization_weights, 7, 4);

    polynomial_trajectories::PolynomialTrajectory trajectory =
        polynomial_trajectories::minimum_snap_trajectories::
        generateMinimumSnapRingTrajectory(segment_times, trajectory_settings,
                                            20.0, 20.0, 6.0); 

    // Start racing
    ros::Time t0 = ros::Time::now();
    unsigned int microseconds = 1000000;
    int posPanel = -3;
    float orienPanel = -0.7071;

    // Add to unity
    std::cout << "Adding quadrotor to Unity" << std::endl;
    unity_bridge_ptr->addQuadrotor(quad_ptr);


    // Connect to unity
    unity_ready = unity_bridge_ptr->connectUnity(scene_id);

    // Main loop
    FrameID frame_id = 0;
    while (ros::ok() && unity_ready) {
        
        // Getting next pose of quadrotor from trajectory
        quadrotor_common::TrajectoryPoint desired_pose =
            polynomial_trajectories::getPointFromTrajectory(
                trajectory, 
                ros::Duration((ros::Time::now() - t0))
            );
        
        // Setting the pose to the quadrotor
        quad_state.x[QS::POSX] = (Scalar)desired_pose.position.x();
        quad_state.x[QS::POSY] = (Scalar)desired_pose.position.y();
        quad_state.x[QS::POSZ] = (Scalar)desired_pose.position.z();
        quad_state.x[QS::ATTW] = (Scalar)desired_pose.orientation.w();
        quad_state.x[QS::ATTX] = (Scalar)desired_pose.orientation.x();
        quad_state.x[QS::ATTY] = (Scalar)desired_pose.orientation.y();
        quad_state.x[QS::ATTZ] = (Scalar)desired_pose.orientation.z();

        quad_ptr->setState(quad_state);

        // Move the panel to next position and wait 1 second
        // posPanel = -posPanel;
        // scenario_1->setPosition("unity_panel_2", Eigen::Vector3f(posPanel, 0, 0));
        // scenario_1->setQuaternion("unity_panel_2", Quaternion(0.7071, 0.0, 0.0, -orienPanel));
        // //panel_1->setPosition(Eigen::Vector3f(posPanel, 0, 0));
        // usleep(microseconds);

        unity_bridge_ptr->getRender(frame_id);
        unity_bridge_ptr->handleOutput();

        //
        frame_id += 1;

        // if (frame_id == 5)
        // {
        //     std::cout << "BORRANDO PANEL DE UNITY (true/false): " << scenario_1->deleteObject("unity_panel_2") << std::endl;
        //     break;
        // }

    }
    std::cout << "Finished";
}