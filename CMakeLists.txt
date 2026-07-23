cmake_minimum_required(VERSION 3.16)
project(RobotAlgorithms)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  mujoco
  GIT_REPOSITORY https://github.com/google-deepmind/mujoco.git
  GIT_TAG        3.10.0 #most recent tag
)
FetchContent_MakeAvailable(mujoco)

find_package(glfw3 REQUIRED)

add_executable(main main.cpp)

target_link_libraries(main
    PRIVATE
        mujoco::mujoco
        glfw
)

target_compile_definitions(main PRIVATE
    SCENE_PATH="${CMAKE_SOURCE_DIR}/assets/drone_scene.xml"
)

target_compile_options(main PRIVATE -Wall -Wextra -Werror -Wconversion)
