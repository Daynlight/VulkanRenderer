# Vulkan Renderer
# Copyright 2026 Daynlight
# Licensed under the GNU General.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)



find_package(glfw3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(Vulkan REQUIRED)



project(vulkan-renderer LANGUAGES CXX)

set(src
  window.cpp
  renderer.cpp
)

add_library(vulkan-renderer STATIC ${src})
add_library(vulkan-renderer::vulkan-renderer ALIAS vulkan-renderer)

target_link_libraries(vulkan-renderer PUBLIC 
  Vulkan::Vulkan
  glfw
  fmt::fmt
  X11
  Xrandr
  Xcursor
  Xi
)

target_include_directories(
  vulkan-renderer
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>
)
