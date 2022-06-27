cmake_minimum_required(VERSION 3.5)
project(inertial_sense_ros)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(rosgraph_msgs REQUIRED)
# find_package(diagnostic_msgs REQUIRED)
find_package(rosidl_typesupport_cpp REQUIRED)
find_package(rosidl_typesupport_introspection_cpp REQUIRED)
find_package(rosidl_typesupport_c REQUIRED)
find_package(rosidl_typesupport_introspection_c REQUIRED)
find_package(rosidl_generator_cpp REQUIRED)
find_package(rosidl_generator_c REQUIRED)
# find_package(rosidl_default_generators REQUIRED)
# find_package(tf2 REQUIRED)
# find_package(tf2_ros REQUIRED)

find_package(Threads)

# SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -fms-extensions -Wl,--no-as-needed -DPLATFORM_IS_LINUX" )
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -fms-extensions -Wl,--no-as-needed -DPLATFORM_IS_LINUX")

# rosidl_generate_interfaces(
#   ${PROJECT_NAME}
#   msg/GTime.msg
#   msg/SatInfo.msg
#   msg/GPS.msg
#   msg/GPSInfo.msg
#   msg/PreIntIMU.msg
#   msg/RTKInfo.msg
#   msg/RTKRel.msg
#   msg/GlonassEphemeris.msg
#   msg/GNSSEphemeris.msg
#   msg/GNSSObservation.msg
#   msg/GNSSObsVec.msg
#   msg/INL2States.msg
#   srv/FirmwareUpdate.srv # Not sure if correct here
#   srv/RefLLAUpdate.srv
#   DEPENDENCIES std_msgs geometry_msgs
# )

# setup targets
include_directories(include
  # ${ament_INCLUDE_DIRS} # check that variables starting with ament_ are correct
  lib/inertial-sense-sdk/src #This line of CMakeList.txt stays in .external file to reference submodule
)

add_subdirectory(lib/inertial-sense-sdk)

add_library(inertial_sense_ros_lib 
        src/inertial_sense_ros.cpp
)

target_link_libraries(inertial_sense_ros_lib InertialSense pthread) # ${ament_LIBRARIES}
ament_target_dependencies(inertial_sense_ros_lib rclcpp rosgraph_msgs std_msgs geometry_msgs sensor_msgs)

add_executable(inertial_sense_node src/inertial_sense_node.cpp)
target_link_libraries(inertial_sense_node inertial_sense_ros_lib) # ${ament_LIBRARIES}
ament_target_dependencies(inertial_sense_node rclcpp rosgraph_msgs std_msgs geometry_msgs sensor_msgs)
# add_dependencies(inertial_sense_node inertial_sense_ros_lib)
# rosidl_target_interfaces(inertial_sense_ros_lib ${PROJECT_NAME} "rosidl_typesupport_cpp")

# export information to downstream packages
ament_export_dependencies(
  # rosidl_default_runtime 
  rclcpp 
  sensor_msgs 
  geometry_msgs
  rosgraph_msgs
)
ament_export_include_directories(include)
ament_export_libraries(inertial_sense_ros_lib)

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)

# install(DIRECTORY
#   config
#   DESTINATION share/${PROJECT_NAME}/
# )

install(TARGETS
  inertial_sense_ros_lib
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)

install(TARGETS
  inertial_sense_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()

# # install artifacts
# install(
#   DIRECTORY include/
#   DESTINATION include
# )
# install(
#   TARGETS inertial_sense_ros_lib inertial_sense_node
#   ARCHIVE DESTINATION lib
#   LIBRARY DESTINATION lib
#   RUNTIME DESTINATION bin
# )