// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from my_msgs:msg/Referenceline.idl
// generated code does not contain a copyright notice

#ifndef MY_MSGS__MSG__DETAIL__REFERENCELINE__STRUCT_H_
#define MY_MSGS__MSG__DETAIL__REFERENCELINE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'poses'
#include "geometry_msgs/msg/detail/pose_stamped__struct.h"

// Struct defined in msg/Referenceline in the package my_msgs.
typedef struct my_msgs__msg__Referenceline
{
  std_msgs__msg__Header header;
  geometry_msgs__msg__PoseStamped__Sequence poses;
} my_msgs__msg__Referenceline;

// Struct for a sequence of my_msgs__msg__Referenceline.
typedef struct my_msgs__msg__Referenceline__Sequence
{
  my_msgs__msg__Referenceline * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} my_msgs__msg__Referenceline__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // MY_MSGS__MSG__DETAIL__REFERENCELINE__STRUCT_H_
