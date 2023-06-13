cmake_minimum_required(VERSION 3.10)
project(ros_actionlib_benchmark)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  actionlib
  actionlib_msgs
  message_generation
  )

add_message_files(
  FILES
  RoundtripActionFeedback.msg
  RoundtripActionGoal.msg
  RoundtripAction.msg
  RoundtripActionResult.msg
  RoundtripFeedback.msg
  RoundtripGoal.msg
  RoundtripResult.msg
)

# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)


generate_messages(DEPENDENCIES actionlib_msgs)


catkin_package(
  INCLUDE_DIRS
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS
    roscpp
    actionlib_msgs
  DEPENDS 
)

add_executable(roundtrip_client src/cpp/roundtrip_client.cpp)
add_executable(roundtrip_server src/cpp/roundtrip_server.cpp)

target_link_libraries(roundtrip_client PUBLIC
  ${catkin_LIBRARIES}
)
target_link_libraries(roundtrip_server PUBLIC
  ${catkin_LIBRARIES}
)
