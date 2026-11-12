// Vulkan Window
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#include <GLFW/glfw3.h>

#include <fmt/base.h>
#include <fmt/color.h>

#include <array>
#include <string>

#include "config.h"



namespace VR {
class Window {
// ======================== //
// ========= Data ========= // 
// ======================== //
private:
  GLFWwindow* window = nullptr;
  bool is_running = false;
  std::array<int, 2> window_size = {};
  std::string title = "";


// ======================== //
// ====== Functions ======= // 
// ======================== //
// === Constructors
public:
  Window(const std::string& title = "");
  ~Window();

// === Getters/Setters
  bool isRunning() const;
  std::string getTitle() const;
  void setTitle(const std::string& title);

// === Operations
  void swapFrame(); 

// === Initializers
private:
  void initWindow();

// === Destroyers
private:
  void destroyWindow();

};
};

