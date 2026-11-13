// Vulkan Window
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#include <vulkan-renderer/window.h>
#include <vulkan-renderer/renderer.h>



int main() {
  VR::Window window("Vulkan Window");
  VR::Renderer renderer(window);

  while(window.isRunning()) {
    window.swapFrame();
  };

  return 0;
};

