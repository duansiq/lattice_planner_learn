// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from my_msgs:msg/Referenceline.idl
// generated code does not contain a copyright notice

#ifndef MY_MSGS__MSG__DETAIL__REFERENCELINE__STRUCT_HPP_
#define MY_MSGS__MSG__DETAIL__REFERENCELINE__STRUCT_HPP_

#include <rosidl_runtime_cpp/bounded_vector.hpp>
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"
// Member 'poses'
#include "geometry_msgs/msg/detail/pose_stamped__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__my_msgs__msg__Referenceline __attribute__((deprecated))
#else
# define DEPRECATED__my_msgs__msg__Referenceline __declspec(deprecated)
#endif

namespace my_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Referenceline_
{
  using Type = Referenceline_<ContainerAllocator>;

  explicit Referenceline_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    (void)_init;
  }

  explicit Referenceline_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _poses_type =
    std::vector<geometry_msgs::msg::PoseStamped_<ContainerAllocator>, typename ContainerAllocator::template rebind<geometry_msgs::msg::PoseStamped_<ContainerAllocator>>::other>;
  _poses_type poses;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__poses(
    const std::vector<geometry_msgs::msg::PoseStamped_<ContainerAllocator>, typename ContainerAllocator::template rebind<geometry_msgs::msg::PoseStamped_<ContainerAllocator>>::other> & _arg)
  {
    this->poses = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    my_msgs::msg::Referenceline_<ContainerAllocator> *;
  using ConstRawPtr =
    const my_msgs::msg::Referenceline_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<my_msgs::msg::Referenceline_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<my_msgs::msg::Referenceline_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      my_msgs::msg::Referenceline_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<my_msgs::msg::Referenceline_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      my_msgs::msg::Referenceline_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<my_msgs::msg::Referenceline_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<my_msgs::msg::Referenceline_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<my_msgs::msg::Referenceline_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__my_msgs__msg__Referenceline
    std::shared_ptr<my_msgs::msg::Referenceline_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__my_msgs__msg__Referenceline
    std::shared_ptr<my_msgs::msg::Referenceline_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Referenceline_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->poses != other.poses) {
      return false;
    }
    return true;
  }
  bool operator!=(const Referenceline_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Referenceline_

// alias to use template instance with default allocator
using Referenceline =
  my_msgs::msg::Referenceline_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace my_msgs

#endif  // MY_MSGS__MSG__DETAIL__REFERENCELINE__STRUCT_HPP_
