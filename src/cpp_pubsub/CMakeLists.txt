cmake_minimum_required(VERSION 3.5)
project(cpp_pubsub)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies 添加依赖
find_package (ament_cmake_auto REQUIRED) 
ament_auto_find_build_dependencies ()



ament_auto_add_executable(planner 
  src/publisher.cpp
  src/test.cpp
  src/manager.cpp
  src/referenceline.cpp
  src/cartesian_frenet_converter.cpp
  src/Trajectory1dGenerator.cpp
  src/path_matcher.cpp
  src/liner_interpolation.cpp
  src/obstacle.cpp
  src/trajectory_evaluator.cpp
  src/constraint_checker1d.cpp
  src/collisionchecker.cpp
  src/end_condition_sampler.cpp
  src/feasible_region.cpp
  src/path_time_graph.cpp


)

ament_auto_add_library (cpp_pubsub SHARED 
    src/test.cpp
    include/test.hpp
    
    include/manager.hpp
    src/manager.cpp

    include/referenceline.hpp
    src/referenceline.cpp

    include/cartesian_frenet_converter.hpp
    src/cartesian_frenet_converter.cpp

    include/Trajectory1dGenerator.hpp
    src/Trajectory1dGenerator.cpp

    include/path_matcher.hpp
    src/path_matcher.cpp

    include/liner_interpolation.hpp
    src/liner_interpolation.cpp

    include/obstacle.hpp
    src/obstacle.cpp

    include/trajectory_evaluator.hpp
    src/trajectory_evaluator.cpp

    include/constraint_checker1d.hpp
    src/constraint_checker1d.cpp

    include/collisionchecker.hpp
    src/collisionchecker.cpp

    include/end_condition_sampler.hpp
    src/end_condition_sampler.cpp

    include/feasible_region.hpp
    src/feasible_region.cpp

    include/path_time_graph.hpp
    src/path_time_graph.cpp

)
ament_auto_package ()




# find_package(ament_cmake REQUIRED)
# find_package(rclcpp REQUIRED)
# find_package(std_msgs REQUIRED)
# # uncomment the following section in order to fill in
# # further dependencies manually.
# # find_package(<dependency> REQUIRED)
# # 添加可执行文件
# add_library( my_lib SHARD
# include/test.h
# )
# ament_target_dependencies(my_lib
# PUBLIC
# rclcpp
# )
# add_executable(talker src/publisher.cpp)
# add_executable(listener src/subscriber.cpp)
# ament_target_dependencies(talker  rclcpp std_msgs)
# ament_target_dependencies(listener rclcpp std_msgs)


# install(TARGETS
#   talker
#   listener
#   DESTINATION
#   lib/${PROJECT_NAME})

# if(BUILD_TESTING)
#   find_package(ament_lint_auto REQUIRED)
#   # the following line skips the linter which checks for copyrights
#   # uncomment the line when a copyright and license is not present in all source files
#   #set(ament_cmake_copyright_FOUND TRUE)
#   # the following line skips cpplint (only works in a git repo)
#   # uncomment the line when this package is not in a git repo
#   #set(ament_cmake_cpplint_FOUND TRUE)
#   ament_lint_auto_find_test_dependencies()
# endif()

# ament_package()
