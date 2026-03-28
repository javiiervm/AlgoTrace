cmake_minimum_required(VERSION 3.14)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
project(AlgoTrace CXX)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(raylib URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz)
FetchContent_MakeAvailable(raylib)
FetchContent_Declare(raylib-cpp URL https://github.com/RobLoach/raylib-cpp/archive/refs/tags/v5.0.2.tar.gz)
FetchContent_MakeAvailable(raylib-cpp)

# Search all .cpp files inside src/ and lib/
# CONFIGURE_DEPENDS forces CMake to update automatically when new files are added
file(GLOB_RECURSE MY_FONTS CONFIGURE_DEPENDS 
    "src/*.cpp" 
    "lib/*.cpp"
)

add_executable(AlgoTrace ${MY_FONTS})

target_include_directories(AlgoTrace PRIVATE 
    include
    ${raylib-cpp_SOURCE_DIR}/include
)
target_link_libraries(AlgoTrace PRIVATE raylib)