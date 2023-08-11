// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from my_msgs:msg/Referenceline.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "my_msgs/msg/detail/referenceline__rosidl_typesupport_introspection_c.h"
#include "my_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "my_msgs/msg/detail/referenceline__functions.h"
#include "my_msgs/msg/detail/referenceline__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `poses`
#include "geometry_msgs/msg/pose_stamped.h"
// Member `poses`
#include "geometry_msgs/msg/detail/pose_stamped__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void Referenceline__rosidl_typesupport_introspection_c__Referenceline_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  my_msgs__msg__Referenceline__init(message_memory);
}

void Referenceline__rosidl_typesupport_introspection_c__Referenceline_fini_function(void * message_memory)
{
  my_msgs__msg__Referenceline__fini(message_memory);
}

size_t Referenceline__rosidl_typesupport_introspection_c__size_function__PoseStamped__poses(
  const void * untyped_member)
{
  const geometry_msgs__msg__PoseStamped__Sequence * member =
    (const geometry_msgs__msg__PoseStamped__Sequence *)(untyped_member);
  return member->size;
}

const void * Referenceline__rosidl_typesupport_introspection_c__get_const_function__PoseStamped__poses(
  const void * untyped_member, size_t index)
{
  const geometry_msgs__msg__PoseStamped__Sequence * member =
    (const geometry_msgs__msg__PoseStamped__Sequence *)(untyped_member);
  return &member->data[index];
}

void * Referenceline__rosidl_typesupport_introspection_c__get_function__PoseStamped__poses(
  void * untyped_member, size_t index)
{
  geometry_msgs__msg__PoseStamped__Sequence * member =
    (geometry_msgs__msg__PoseStamped__Sequence *)(untyped_member);
  return &member->data[index];
}

bool Referenceline__rosidl_typesupport_introspection_c__resize_function__PoseStamped__poses(
  void * untyped_member, size_t size)
{
  geometry_msgs__msg__PoseStamped__Sequence * member =
    (geometry_msgs__msg__PoseStamped__Sequence *)(untyped_member);
  geometry_msgs__msg__PoseStamped__Sequence__fini(member);
  return geometry_msgs__msg__PoseStamped__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember Referenceline__rosidl_typesupport_introspection_c__Referenceline_message_member_array[2] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(my_msgs__msg__Referenceline, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "poses",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(my_msgs__msg__Referenceline, poses),  // bytes offset in struct
    NULL,  // default value
    Referenceline__rosidl_typesupport_introspection_c__size_function__PoseStamped__poses,  // size() function pointer
    Referenceline__rosidl_typesupport_introspection_c__get_const_function__PoseStamped__poses,  // get_const(index) function pointer
    Referenceline__rosidl_typesupport_introspection_c__get_function__PoseStamped__poses,  // get(index) function pointer
    Referenceline__rosidl_typesupport_introspection_c__resize_function__PoseStamped__poses  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers Referenceline__rosidl_typesupport_introspection_c__Referenceline_message_members = {
  "my_msgs__msg",  // message namespace
  "Referenceline",  // message name
  2,  // number of fields
  sizeof(my_msgs__msg__Referenceline),
  Referenceline__rosidl_typesupport_introspection_c__Referenceline_message_member_array,  // message members
  Referenceline__rosidl_typesupport_introspection_c__Referenceline_init_function,  // function to initialize message memory (memory has to be allocated)
  Referenceline__rosidl_typesupport_introspection_c__Referenceline_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t Referenceline__rosidl_typesupport_introspection_c__Referenceline_message_type_support_handle = {
  0,
  &Referenceline__rosidl_typesupport_introspection_c__Referenceline_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_my_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, my_msgs, msg, Referenceline)() {
  Referenceline__rosidl_typesupport_introspection_c__Referenceline_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  Referenceline__rosidl_typesupport_introspection_c__Referenceline_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, geometry_msgs, msg, PoseStamped)();
  if (!Referenceline__rosidl_typesupport_introspection_c__Referenceline_message_type_support_handle.typesupport_identifier) {
    Referenceline__rosidl_typesupport_introspection_c__Referenceline_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &Referenceline__rosidl_typesupport_introspection_c__Referenceline_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
