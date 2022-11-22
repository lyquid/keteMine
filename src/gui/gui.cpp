#include "gui.hpp"

#include "../../lib/imgui/imgui.h"
#include "../../lib/imgui/imgui_impl_glfw.h"
#include "../../lib/imgui/imgui_impl_opengl3.h"
#include <iostream>

void ktp::gui::clean() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ktp::gui::init(GLFWwindow* window) {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Control

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 430");
  std::cout << "Dear ImGui " << ImGui::GetVersion() << '\n';
}

void ktp::gui::layout() {
    // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::ShowDemoWindow();






  ImGui::Render();

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

//   ImGui::SetNextWindowSize(ImVec2(510, 528), ImGuiCond_FirstUseEver);
//   if (!ImGui::Begin("KeTerrain configuration")) {
//     ImGui::End();
//     return;
//   }
//   // ImGui::Text("Texture settings");
//   // ImGui::BeginDisabled(generating_texture);
//   //   size();
//   //   changeView();
//   //   ImGui::SameLine();
//   //   invertElevation();
//   //   ImGui::SameLine();
//   //   saveImage();
//   //   ImGui::SameLine();
//   //   defaults();
//   // ImGui::EndDisabled();
//   // if (size_changed) {
//   //   ImGui::Text("Generate a new texture to apply size changes!");
//   // }
//   // ImGui::Separator();
//   // ImGui::Text("Generation settings");
//   // ImGui::BeginDisabled(generating_texture);
//   //   tileable();
//   //   seed();
//   //   frequency();
//   //   gain();
//   //   lacunarity();
//   //   octaves();
//   //   ImGui::Separator();
//   //   // generation button
//   //   generateTexture();
//   //   ImGui::SameLine();
//   //   // randomize button
//   //   randomize();
//   //   ImGui::SameLine();
//   // ImGui::EndDisabled();
//   // ImGui::Separator();
//   // if (saving_image) {
//   //   ImGui::Text("Saving image...");
//   //   return;
//   // }
//   // if (generating_texture) {
//   //   ImGui::Text("Generating texture...");
//   // }
  // ImGui::End();
}
