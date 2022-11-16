#include "inertial_sense_ros.h"

int main(int argc, char**argv)
{
  rclcpp::init(argc, argv);
  InertialSenseROS thing;
  auto node = std::make_shared<InertialSenseROS>();
  while (rclcpp::ok()) {
    rclcpp::spin_some(node);
    node->update();
  }
  rclcpp::shutdown();

  return 0;
}