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
# find_package(diagnostic_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)
# find_package(tf2 REQUIRED)
# find_package(tf2_ros REQUIRED)

find_package(Threads)

# SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -fms-extensions -Wl,--no-as-needed -DPLATFORM_IS_LINUX" )
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -fms-extensions -Wl,--no-as-needed -DPLATFORM_IS_LINUX")

rosidl_generate_interfaces(
  ${PROJECT_NAME}
  GTime.msg
  SatInfo.msg
  GPS.msg
  GPSInfo.msg
  PreIntIMU.msg
  RTKInfo.msg
  RTKRel.msg
  GlonassEphemeris.msg
  GNSSEphemeris.msg
  GNSSObservation.msg
  GNSSObsVec.msg
  INL2States.msg
  FirmwareUpdate.srv # Not sure if correct here
  refLLAUpdate.srv
  DEPENDENCIES std_msgs geometry_msgs
)


# generate_messages(
#   DEPENDENCIES
#   std_msgs
#   geometry_msgs
# )

ament_export_dependencies(rclcpp sensor_msgs geometry_msgs)
ament_export_libraries(inertial_sense_ros)
ament_package(
    INCLUDE_DIRS include
)

include_directories(include
  ${ament_INCLUDE_DIRS} # check that variables starting with ament_ are correct
  lib/inertial-sense-sdk/src #This line of CMakeList.txt stays in .external file to reference submodule
)

add_subdirectory(lib/inertial-sense-sdk)

add_library(inertial_sense_ros
        src/inertial_sense_ros.cpp
)
target_link_libraries(inertial_sense_ros InertialSense ${ament_LIBRARIES} pthread)
target_include_directories(inertial_sense_ros PUBLIC include lib/inertial-sense-sdk/src)
add_dependencies(inertial_sense_ros inertial_sense_ros_generate_messages_cpp)

add_executable(inertial_sense_node src/inertial_sense_node.cpp)
target_link_libraries(inertial_sense_node inertial_sense_ros ${ament_LIBRARIES})
