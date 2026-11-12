# Vulkan Renderer
# Copyright 2026 Daynlight
# Licensed under the GNU General.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)



project(examples LANGUAGES CXX)

set(src
  main.cpp
)

add_executable(examples ${src})
target_link_libraries(examples PRIVATE
  vulkan-renderer::vulkan-renderer
)

