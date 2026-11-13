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

  if(createDevice()) {
    if(DEBUG_LOGS) fmt::println(fg(fmt::color::red), "[VR::Renderer::initVulkan] Failed to create device");
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



void VR::Renderer::printDevicesInfo(const std::vector<std::pair<size_t, vk::raii::PhysicalDevice>>& physicalDevices){
  if(!DEBUG_LOGS) return; 
  fmt::println(fg(fmt::color::blue), "=================================");
  for (auto physicalDevice : physicalDevices) {
    auto deviceProperties = physicalDevice.second.getProperties();

    fmt::println(fg(fmt::color::blue), "Score: {}", physicalDevice.first);
    fmt::println(fg(fmt::color::blue), "Name: {}", fmt::join(deviceProperties.deviceName, ""));
    fmt::println(fg(fmt::color::blue), "Api Version: {}", deviceProperties.apiVersion);
    fmt::println(fg(fmt::color::blue), "Driver Version: {}", deviceProperties.driverVersion);
    fmt::println(fg(fmt::color::blue), "Device ID: {}", deviceProperties.deviceID);
    
    std::string type = "";
    switch(deviceProperties.deviceType) {
      case vk::PhysicalDeviceType::eOther:
        type = "Other";
        break;
      case vk::PhysicalDeviceType::eIntegratedGpu:
        type = "Integrated";
        break;
      case vk::PhysicalDeviceType::eDiscreteGpu:
        type = "Discrete";
        break;
      case vk::PhysicalDeviceType::eVirtualGpu:
        type = "Virtual";
        break;
      case vk::PhysicalDeviceType::eCpu:
        type = "CPU";
        break;
      default:
        break;
    };

    fmt::println(fg(fmt::color::blue), "Device Type: {}", type);
    fmt::println(fg(fmt::color::blue), "=================================");
  };
  if(physicalDevices.empty()) fmt::println(fg(fmt::color::blue), "=================================");
};



std::vector<vk::raii::PhysicalDevice> VR::Renderer::filtrSuitableDevices(const std::vector<vk::raii::PhysicalDevice>& physicalDevices) {
  if(DEBUG_LOGS) fmt::println(fg(fmt::color::purple), "[VR::Renderer::filtrSuitableDevices] filtrSuitableDevices Called");
  
  std::vector<vk::raii::PhysicalDevice> filtred_devices = {};
  filtred_devices.reserve(physicalDevices.size());
  
  for (auto physicalDevice : physicalDevices) {
    auto deviceFeatures = physicalDevice.getFeatures();
    
    std::vector<const char*> requiredDeviceExtension = {vk::KHRSwapchainExtensionName};
   
    bool suportsGeometryShader      = deviceFeatures.geometryShader;
    bool supportsVulkan1_3          = physicalDevice.getProperties().apiVersion >= vk::ApiVersion13;
    auto queueFamilies              = physicalDevice.getQueueFamilyProperties();
    bool supportsGraphics           = std::ranges::any_of( queueFamilies, []( auto const & qfp ) { return !!( qfp.queueFlags & vk::QueueFlagBits::eGraphics ); } ); 
    auto availableDeviceExtensions  = physicalDevice.enumerateDeviceExtensionProperties();
    auto features                   = physicalDevice.template getFeatures2<vk::PhysicalDeviceFeatures2,
      vk::PhysicalDeviceVulkan11Features,
      vk::PhysicalDeviceVulkan13Features,
      vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();
    bool supportsAllRequiredExtensions =
    std::ranges::all_of(requiredDeviceExtension,
      [&availableDeviceExtensions]( auto const & requiredDeviceExtension ) {
        return std::ranges::any_of( availableDeviceExtensions,
          [requiredDeviceExtension]( auto const & availableDeviceExtension )
          { return strcmp( availableDeviceExtension.extensionName, requiredDeviceExtension ) == 0; } );
        } 
      );

    bool is_suitable = (suportsGeometryShader && supportsVulkan1_3 && supportsGraphics && supportsAllRequiredExtensions);
    if (is_suitable) filtred_devices.emplace_back(physicalDevice);
  };

  filtred_devices.shrink_to_fit();
  return filtred_devices;
};



std::vector<std::pair<size_t, vk::raii::PhysicalDevice>> VR::Renderer::scoreDevices(const std::vector<vk::raii::PhysicalDevice>& physicalDevices) {
  std::vector<std::pair<size_t, vk::raii::PhysicalDevice>> scored_devices = {};
  scored_devices.reserve(physicalDevices.size());
 
  for (auto physicalDevice : physicalDevices) {
    auto deviceProperties = physicalDevice.getProperties();
    
    size_t score = 0;
    
    switch(deviceProperties.deviceType) {
      case vk::PhysicalDeviceType::eOther:
        score += 100;
        break;
      case vk::PhysicalDeviceType::eIntegratedGpu:
        score += 500;
        break;
      case vk::PhysicalDeviceType::eDiscreteGpu:
        score += 1000;
        break;
      case vk::PhysicalDeviceType::eVirtualGpu:
        score += 150;
        break;
      case vk::PhysicalDeviceType::eCpu:
        score += 300;
        break;
      default:
        break;
    };

    scored_devices.emplace_back(score, physicalDevice);
  };
 
  std::sort(scored_devices.begin(), scored_devices.end(), [](auto &left, auto &right) {
    return left.first > right.first;
  });

  return scored_devices;
};



bool VR::Renderer::createDevice() {
  if(DEBUG_LOGS) fmt::println(fg(fmt::color::purple), "[VR::Renderer::createDevice] createDevice Called");
  
  auto physicalDevices = instance.enumeratePhysicalDevices();
  if (physicalDevices.empty()) {
    if(DEBUG_LOGS) fmt::println(fg(fmt::color::red), "[VR::Renderer::createDevice] failed to find GPUs with Vulkan support!");
    return -1;
  };

  auto filtred_devices = filtrSuitableDevices(physicalDevices); 
  if(filtred_devices.empty()){
    if(DEBUG_LOGS) fmt::println(fg(fmt::color::red), "[VR::Renderer::createDevice] No suitable Device");
    return -1;
  };

  auto scored_devices = scoreDevices(filtred_devices);
  printDevicesInfo(scored_devices);
  this->physicalDevice = scored_devices[0].second;
  if(DEBUG_LOGS) fmt::println(fg(fmt::color::lime), "[VR::Renderer::createDevice] Chosen device: {}", fmt::join(this->physicalDevice.getProperties().deviceName, ""));

  return 0;
};






















