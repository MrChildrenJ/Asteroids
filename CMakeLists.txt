cmake_minimum_required(VERSION 3.8)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set CMAKE_PREFIX_PATH to help find SFML on macOS with Homebrew
if(APPLE)
    set(CMAKE_PREFIX_PATH "/opt/homebrew;/usr/local" ${CMAKE_PREFIX_PATH})
endif()

find_package(SFML 2.6 COMPONENTS graphics system window REQUIRED)

project(asteroids LANGUAGES CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

file(GLOB SRC_FILES ${CMAKE_SOURCE_DIR}/*)

add_executable(asteroids ${SRC_FILES})
set(CMAKE_BUILD_TYPE Debug)

TARGET_LINK_LIBRARIES(asteroids sfml-graphics sfml-system sfml-window)
target_compile_features(asteroids PRIVATE cxx_std_20)

add_compile_options(
  -Wall
  -Wextra
  -Wpedantic
  -fsanitize=address
  -fsanitize=undefined
  -fsanitize-address-use-after-scope
  -fsanitize-address-use-after-return=runtime
  -fno-omit-frame-pointer
)

add_link_options(
  -Wall
  -Wextra
  -Wpedantic
  -fsanitize=address
  -fsanitize=undefined
  -fsanitize-address-use-after-scope
  -fsanitize-address-use-after-return=runtime
  -fno-omit-frame-pointer
)

# Set runtime directory to bin directory
set_target_properties(asteroids PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Copy images folder to the output directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/images DESTINATION ${CMAKE_BINARY_DIR})
# Copy font to the output directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/IosevkaNerdFont-Bold.ttf DESTINATION ${CMAKE_BINARY_DIR})
