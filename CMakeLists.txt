cmake_minimum_required(VERSION 2.8.3)
project(sk_marker)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  tf
  visualization_msgs
  rospy_tutorials
  message_generation
  std_msgs
  moveit_core
)

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend tag for "message_generation"
##   * add a build_depend and a run_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependency has been pulled in
##     but can be declared for certainty nonetheless:
##     * add a run_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
 add_message_files(
  FILES
  trajectory_msg.msg
 )

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
)

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES sk_marker
  CATKIN_DEPENDS message_runtime
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

#############
## Testing ##
#############

add_executable(skeleton_br src/skeleton_br.cpp)
target_link_libraries(skeleton_br ${catkin_LIBRARIES})

add_executable(data_saver src/data_saver.cpp)
target_link_libraries(data_saver ${catkin_LIBRARIES})

add_executable(data_loader src/data_loader.cpp)
target_link_libraries(data_loader ${catkin_LIBRARIES})

add_executable(trajectory src/trajectory.cpp)
add_dependencies(trajectory ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(trajectory ${catkin_LIBRARIES})

# Upper body
add_executable(head src/human_model/head.cpp)
target_link_libraries(head ${catkin_LIBRARIES})

add_executable(torso src/human_model/torso.cpp)
target_link_libraries(torso ${catkin_LIBRARIES})

add_executable(right_upper_arm src/human_model/right_upper_arm.cpp)
target_link_libraries(right_upper_arm ${catkin_LIBRARIES})

add_executable(right_fore_arm src/human_model/right_fore_arm.cpp)
target_link_libraries(right_fore_arm ${catkin_LIBRARIES})

add_executable(left_upper_arm src/human_model/left_upper_arm.cpp)
target_link_libraries(left_upper_arm ${catkin_LIBRARIES})

add_executable(left_fore_arm src/human_model/left_fore_arm.cpp)
target_link_libraries(left_fore_arm ${catkin_LIBRARIES})

add_executable(hands src/human_model/hands.cpp)
target_link_libraries(hands ${catkin_LIBRARIES})

# Lower body
add_executable(right_upper_leg src/human_model/right_upper_leg.cpp)
target_link_libraries(right_upper_leg ${catkin_LIBRARIES})

add_executable(right_fore_leg src/human_model/right_fore_leg.cpp)
target_link_libraries(right_fore_leg ${catkin_LIBRARIES})

add_executable(left_upper_leg src/human_model/left_upper_leg.cpp)
target_link_libraries(left_upper_leg ${catkin_LIBRARIES})

add_executable(left_fore_leg src/human_model/left_fore_leg.cpp)
target_link_libraries(left_fore_leg ${catkin_LIBRARIES})

add_executable(feet src/human_model/feet.cpp)
target_link_libraries(feet ${catkin_LIBRARIES})

