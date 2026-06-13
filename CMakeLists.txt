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