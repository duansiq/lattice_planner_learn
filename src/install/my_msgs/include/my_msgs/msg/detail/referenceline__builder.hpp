// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from my_msgs:msg/Referenceline.idl
// generated code does not contain a copyright notice

#ifndef MY_MSGS__MSG__DETAIL__REFERENCELINE__BUILDER_HPP_
#define MY_MSGS__MSG__DETAIL__REFERENCELINE__BUILDER_HPP_

#include "my_msgs/msg/detail/referenceline__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace my_msgs
{

namespace msg
{

namespace builder
{

class Init_Referenceline_poses
{
public:
  explicit Init_Referenceline_poses(::my_msgs::msg::Referenceline & msg)
  : msg_(msg)
  {}
  ::my_msgs::msg::Referenceline poses(::my_msgs::msg::Referenceline::_poses_type arg)
  {
    msg_.poses = std::move(arg);
    return std::move(msg_);
  }

private:
  ::my_msgs::msg::Referenceline msg_;
};

class Init_Referenceline_header
{
public:
  Init_Referenceline_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Referenceline_poses header(::my_msgs::msg::Referenceline::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_Referenceline_poses(msg_);
  }

private:
  ::my_msgs::msg::Referenceline msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::my_msgs::msg::Referenceline>()
{
  return my_msgs::msg::builder::Init_Referenceline_header();
}

}  // namespace my_msgs

#endif  // MY_MSGS__MSG__DETAIL__REFERENCELINE__BUILDER_HPP_
