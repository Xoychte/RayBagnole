cmake_minimum_required(VERSION 3.29)
project(RayBagnole C)

# Set C standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# First try to find installed Raylib
find_package(raylib QUIET)

if(NOT raylib_FOUND)
    message(STATUS "Raylib not found - downloading and building from source")

    # Include FetchContent for downloading dependencies
    include(FetchContent)

    # Raylib configuration options
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

    # Declare Raylib dependency
    FetchContent_Declare(
            raylib
            GIT_REPOSITORY https://github.com/raysan5/raylib.git
            GIT_TAG 5.5   # Specify a version/tag here
    )

    # Make Raylib available
    FetchContent_MakeAvailable(raylib)
else()
    message(STATUS "Found installed Raylib version ${raylib_VERSION}")
endif()

# Create executable
add_executable(${PROJECT_NAME} main.c
        car.c
        car.h
        vectUtils.c
        vectUtils.h)

# Link against Raylib (works for both installed and downloaded versions)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# OS-specific configurations
if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework OpenGL")
endif()

# Set additional compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

if(WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DGLFW_USE_HYBRID_HPG")
endif()