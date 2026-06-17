cmake_minimum_required(VERSION 4.0)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "Minimum policy version for all subprojects")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3 # It's best to lock onto a specific stable version
)
FetchContent_MakeAvailable(json)

add_library(common INTERFACE)
target_compile_features(common INTERFACE cxx_std_17)
target_compile_options(common INTERFACE
  $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>: -Wall>
  $<$<CXX_COMPILER_ID:MSVC>: /W4>
)

add_executable(Tappa01 Tappa01/tappa01.cpp)
target_link_libraries(Tappa01 PRIVATE SFML::Graphics)

add_executable(Tappa02 Tappa02/tappa02.cpp)
target_link_libraries(Tappa02 PRIVATE SFML::Graphics)

add_executable(Tappa03 Tappa03/tappa03.cpp)
target_link_libraries(Tappa03 PRIVATE SFML::Graphics)
target_link_libraries(Tappa03 PRIVATE nlohmann_json::nlohmann_json)

add_executable(Tappa04 Tappa04/tappa04.cpp)
target_link_libraries(Tappa04 PRIVATE SFML::Graphics)
target_link_libraries(Tappa04 PRIVATE nlohmann_json::nlohmann_json)

add_executable(Tappa05 Tappa05/tappa05.cpp)
target_link_libraries(Tappa05 PRIVATE SFML::Graphics)
target_link_libraries(Tappa05 PRIVATE nlohmann_json::nlohmann_json)

add_executable(Tappa06 Tappa06/tappa06.cpp)
target_link_libraries(Tappa06 PRIVATE SFML::Graphics)
target_link_libraries(Tappa06 PRIVATE nlohmann_json::nlohmann_json)

add_executable(Tappa07 Tappa07/enemies.cpp Tappa07/environment.cpp Tappa07/player.cpp Tappa07/projectiles.cpp Tappa07/room.cpp Tappa07/state.cpp Tappa07/tappa07-main.cpp)
target_link_libraries(Tappa07 PRIVATE SFML::Graphics)
target_link_libraries(Tappa07 PRIVATE nlohmann_json::nlohmann_json)

add_executable(Tappa08 Tappa08/enemies.cpp Tappa08/environment.cpp Tappa08/player.cpp Tappa08/projectiles.cpp Tappa08/room.cpp Tappa08/state.cpp Tappa08/tappa08-main.cpp)
target_link_libraries(Tappa08 PRIVATE SFML::Graphics)
target_link_libraries(Tappa08 PRIVATE nlohmann_json::nlohmann_json)

add_executable(Tappa09 Tappa09/enemies.cpp Tappa09/environment.cpp Tappa09/player.cpp Tappa09/projectiles.cpp Tappa09/room.cpp Tappa09/state.cpp Tappa09/tappa09-main.cpp)
target_link_libraries(Tappa09 PRIVATE SFML::Graphics)
target_link_libraries(Tappa09 PRIVATE nlohmann_json::nlohmann_json)