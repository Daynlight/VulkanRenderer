// Vulkan Window
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#include "window.h"



// === Constructors
VR::Window::Window(const std::string& title) 
  : title(title) {
  if(DEBUG_LOGS) fmt::println(fg(fmt::color::purple), "[VR::Window::Window()] Constructor Called");
  initWindow();
};



VR::Window::~Window() {
  if(DEBUG_LOGS) fmt::println(fg(fmt::color::purple), "[VR::Window::~Window()] Destructor Called");
  destroyWindow();
};



// === Getters/Setters
bool VR::Window::isRunning() const {
  if(!window) return false;
  return is_running;
};



std::string VR::Window::getTitle() const {
  if(DEBUG_LOGS) fmt::println(fg(fmt::color::purple), "[VR::Window::getTitle] Called");
  return title;
};



void VR::Window::setTitle(const std::string& title) {
  if(DEBUG_LOGS) fmt::println(fg(fmt::color::purple), "[VR::Window::setTitle] Called");
  this->title = title;
  if(window) glfwSetWindowTitle(window, this->title.c_str());
};



// === Operations
void VR::Window::swapFrame() {
  if(!is_running) return;

  if(!window) { 
    if(DEBUG_LOGS) fmt::println(fg(fmt::color::red), "[VR::Window::swapFrame] Window didn't exists");
    is_running = false;
    return;
  };

  glfwPollEvents();
  is_running = !glfwWindowShouldClose(window);
  glfwGetWindowSize(window, &window_size[0], &window_size[1]);
};



// === Initializers
void VR::Window::initWindow() {
  if(DEBUG_LOGS) fmt::println(fg(fmt::color::purple), "[VR::Window::initWindow] Called");
  
  glfwInit(); 
  
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); 
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window = glfwCreateWindow(640, 480, title.c_str(), NULL, NULL);
  if(!window) {
    if(DEBUG_LOGS) fmt::println(fg(fmt::color::red), "[VR::Window::initWindow] Failed to create Window");
    is_running = false;
    return;
  };

  is_running = true;
};



void VR::Window::destroyWindow() {
  if(DEBUG_LOGS) fmt::println(fg(fmt::color::purple), "[VR::Window::destroyWindow] Called");
  
  if(!window) glfwDestroyWindow(window);
  glfwTerminate();

  is_running = false;
};
