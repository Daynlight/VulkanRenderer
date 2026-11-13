// Vulkan Window
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#include "renderer.h"



// === Constructors
VR::Renderer::Renderer(VR::Window& window)
  : window(window) {
  if(DEBUG_LOGS) fmt::println(fg(fmt::color::purple), "[VR::Renderer::Renderer()] Constructor Called");
  initVulkan();
};



VR::Renderer::~Renderer() {
  if(DEBUG_LOGS) fmt::println(fg(fmt::color::purple), "[VR::Renderer::~Renderer()] Destructor Called");
};



// === Initializers
void VR::Renderer::initVulkan() {
  if(DEBUG_LOGS) fmt::println(fg(fmt::color::purple), "[VR::Renderer::initVulkan] initVulkan Called");
  
  if(createInstance()){
    if(DEBUG_LOGS) fmt::println(fg(fmt::color::red), "[VR::Renderer::initVulkan] Failed to create instance");
    return;
  };

};



bool VR::Renderer::createInstance() {
  if(DEBUG_LOGS) fmt::println(fg(fmt::color::purple), "[VR::Renderer::createInstance] createInstance Called");
  
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Hello Triangle";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;
  
  uint32_t glfwExtensionCount = 0;
  auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  
  auto extensionProperties = context.enumerateInstanceExtensionProperties();
  for (uint32_t i = 0; i < glfwExtensionCount; ++i) {
    if (std::ranges::none_of(
      extensionProperties,
      [glfwExtension = glfwExtensions[i]](auto const& extensionProperty)
      { return strcmp(extensionProperty.extensionName, glfwExtension) == 0; }))
    {
      if(DEBUG_LOGS) fmt::println(fg(fmt::color::red), "[VR::Renderer::createInstance] Required GLFW extension not supported: {}", std::string(glfwExtensions[i]));
      return -1;
    }
  };
  
  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo         = &appInfo;
  createInfo.enabledExtensionCount    = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames  = glfwExtensions;
  
  instance = vk::raii::Instance(context, createInfo);
  if(instance == nullptr){
    if(DEBUG_LOGS) fmt::println(fg(fmt::color::red), "[VR::Renderer::createInstance] Failed to create instance");
    return -1;
  };

  return 0;
};



