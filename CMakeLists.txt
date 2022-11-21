cmake_minimum_required(VERSION 3.12)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project(
  keteMine
  VERSION 0.1.0
  DESCRIPTION "Minecraft clone"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Let's nicely support folders in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  # Testing only available if this is the main app
  # Note this needs to be done in the main CMakeLists
  # since it calls enable_testing, which must be in the
  # main CMakeLists.
  include(CTest)
	enable_testing()

  # Docs only available if this is the main app
  # find_package(Doxygen)
  # if(Doxygen_FOUND)
  #   add_subdirectory(docs)
  # else()
  #   message(STATUS "Doxygen not found, not building docs")
  # endif()
endif()

if(DEFINED CMAKE_TOOLCHAIN_FILE)
	message(STATUS "Using vcpkg toolchain file")
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  # Find the packages needed
  find_package(GLEW REQUIRED)
  find_package(glfw3 CONFIG REQUIRED)
  find_package(glm CONFIG REQUIRED)
else()
	message(STATUS "Using CMake find modules")
  find_package(GLEW REQUIRED)
  find_package(glfw3 CONFIG REQUIRED)
  find_package(glm CONFIG REQUIRED)
endif()

add_subdirectory(lib/imgui)
add_subdirectory(src)
add_subdirectory(src/gui)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

file(COPY ./resources/ DESTINATION ./src/resources)
