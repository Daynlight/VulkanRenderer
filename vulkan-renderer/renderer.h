// Vulkan Window
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#pragma once
#define VK_MAX_PHYSICAL_DEVICE_NAME_SIZE  256U
#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif

#include <fmt/base.h>
#include <fmt/color.h>
#include <fmt/ranges.h>

#include <array>
#include <string>
#include <algorithm>

#include "window.h"
#include "config.h"



namespace VR {
class Renderer {
// ======================== //
// ========= Data ========= // 
// ======================== //
private:
  VR::Window& window;
  vk::raii::Context  context;
  vk::raii::Instance instance = nullptr;
  vk::raii::PhysicalDevice physicalDevice = nullptr;



// ======================== //
// ====== Functions ======= // 
// ======================== //
// === Constructors
public:
  Renderer(VR::Window& window);
  ~Renderer();

// === Initializers
private:
  void initVulkan();
  
  bool createInstance();

  void printDevicesInfo(const std::vector<std::pair<size_t, vk::raii::PhysicalDevice>>& physicalDevices);
  std::vector<vk::raii::PhysicalDevice> filtrSuitableDevices(const std::vector<vk::raii::PhysicalDevice>& physicalDevices);
  std::vector<std::pair<size_t, vk::raii::PhysicalDevice>> scoreDevices(const std::vector<vk::raii::PhysicalDevice>& physicalDevices);
  bool createDevice();
    
// === Destroyers
private:

};
};

