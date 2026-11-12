# Vulkan Renderer
# Copyright 2026 Daynlight
# Licensed under the GNU General.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)



############## [Set Global Properties] #############
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Root directory of the project")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(VULKAN_RENDERER_BUILD_EXAMPLE "Build example" ON)
option(VULKAN_RENDERER_BUILD_TESTS "Build tests" ON)



############# [Set Build] #############
# [OPENGL/VULKAN/DIRECTX]
if(NOT DEFINED RENDERER)
  set(RENDERER "VULKAN")
endif()

# [UNIX/WIN32]
if(NOT DEFINED PLATFORM)
  if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(PLATFORM "WIN32")
  elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(PLATFORM "UNIX")
  else()
    message(FATAL_ERROR "Invalid PLATFORM specified: ${PLATFORM}.")
  endif()
endif()

message(STATUS, "Used Platform: ${PLATFORM}")
message(STATUS, "Used Renderer: ${RENDERER}")



############# [Set Dependencies] #############
find_package(GTest CONFIG REQUIRED)



############# [Build Project] #############
add_subdirectory(vulkan-renderer)

if(VULKAN_RENDERER_BUILD_EXAMPLE)
  add_subdirectory(examples)
endif()

# if(VULKAN_RENDERER_BUILD_TESTS)
#   enable_testing()
#   add_subdirectory(tests)
# endif()

