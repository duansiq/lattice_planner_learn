// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from my_msgs:msg/Referenceline.idl
// generated code does not contain a copyright notice

#ifndef MY_MSGS__MSG__DETAIL__REFERENCELINE__TRAITS_HPP_
#define MY_MSGS__MSG__DETAIL__REFERENCELINE__TRAITS_HPP_

#include "my_msgs/msg/detail/referenceline__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<my_msgs::msg::Referenceline>()
{
  return "my_msgs::msg::Referenceline";
}

template<>
inline const char * name<my_msgs::msg::Referenceline>()
{
  return "my_msgs/msg/Referenceline";
}

template<>
struct has_fixed_size<my_msgs::msg::Referenceline>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<my_msgs::msg::Referenceline>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<my_msgs::msg::Referenceline>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // MY_MSGS__MSG__DETAIL__REFERENCELINE__TRAITS_HPP_
