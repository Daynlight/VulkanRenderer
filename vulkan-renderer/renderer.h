// Vulkan Window
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#pragma once
#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif

#include <fmt/base.h>
#include <fmt/color.h>

#include <array>
#include <string>

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

// === Destroyers
private:

};
};

