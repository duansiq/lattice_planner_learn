#include <chrono> // 时间相关
#include <functional>
#include <memory> //指针
#include <string>
#include "test.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "nav_msgs/msg/path.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"
#include "manager.hpp"
#include "pathpoint.hpp"
#include "visualization_msgs/msg/marker_array.hpp"

using namespace std::chrono_literals;
using namespace std;


/* This example creates a subclass of Node and uses std::bind() to register a* member function as a callback from the timer. */

class MinimalPublisher : public rclcpp::Node
{
public:
  MinimalPublisher()
      : Node("planner"), count_(0)
  {
    publisher_latpath = this->create_publisher<nav_msgs::msg::Path>("latpath", 10);
    publisher_referenceline_left = this->create_publisher<nav_msgs::msg::Path>("referenceline_left", 10);
    publisher_referenceline_right = this->create_publisher<nav_msgs::msg::Path>("referenceline_right", 10);
    publisher_test = this->create_publisher<std_msgs::msg::String>("test_topic", 10);
    publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
    timer_ = this->create_wall_timer(100ms, std::bind(&MinimalPublisher::timer_callback, this));
  }

private:
  void timer_callback()
  {
    auto start_time = std::chrono::system_clock::now();
    // manageplanner.test();
    manageplanner.planner();
    manageplanner.writetxt();

    auto referenceline_points_right = manageplanner.get_referenceline_points();
    auto rpointsright = nav_msgs::msg::Path();
    for (auto referencepoint : referenceline_points_right)
    {
      geometry_msgs::msg::PoseStamped rpoint;
      rpoint.header.frame_id = "/map";
      rpoint.pose.position.x = referencepoint.x;
      rpoint.pose.position.y = referencepoint.y;
      rpoint.pose.position.z = 0.;
      rpointsright.poses.emplace_back(rpoint);
    }
    rpointsright.header.frame_id = "/map";

    auto rpointsleft = nav_msgs::msg::Path();
    for (auto referencepoint : referenceline_points_right)
    {
      geometry_msgs::msg::PoseStamped rpoint;
      rpoint.header.frame_id = "/map";
      rpoint.pose.position.x = referencepoint.x;
      rpoint.pose.position.y = referencepoint.y+3.5;
      rpoint.pose.position.z = 0.;
      rpointsleft.poses.emplace_back(rpoint);
    }
    rpointsleft.header.frame_id = "/map";

    auto latpath_gcs = manageplanner.get_latpath();
    auto latpathposes = nav_msgs::msg::Path();
    for (auto latpathpoint : latpath_gcs)
    {
      geometry_msgs::msg::PoseStamped point;
      point.header.frame_id = "map";
      point.pose.position.x = latpathpoint.x;
      point.pose.position.y = latpathpoint.y;
      point.pose.position.z = 0.;
      latpathposes.poses.emplace_back(point);
    }
    latpathposes.header.frame_id = "map";

    auto message = std_msgs::msg::String();
    message.data = "Hello, world! " + std::to_string(count_++);
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
    publisher_->publish(message);
    auto messagetest = std_msgs::msg::String();
    messagetest.data = "publisher , test " + std::to_string(count_++);
    publisher_test->publish(messagetest);
    publisher_referenceline_right->publish(rpointsright);
    publisher_referenceline_left->publish(rpointsleft);
    publisher_latpath->publish(latpathposes);

    auto endtime=std::chrono::system_clock::now();
    std::chrono::duration<double> diff = endtime - start_time;
    cout << "cal time: " << diff.count() << endl;
  }
  rclcpp::TimerBase::SharedPtr timer_;                                             // 定时器
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;                  // 发布者
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_test;              // 发布者
  rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr publisher_referenceline_right; // 发布者
  rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr publisher_referenceline_left;  // 发布者
  rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr publisher_latpath;             // 发布者

  size_t count_;
  // Test test_;
  Manage_Planner manageplanner;
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);                           // 节点初始化
  rclcpp::spin(std::make_shared<MinimalPublisher>()); // 处理回调函数、定时器
  rclcpp::shutdown();
  return 0;
}