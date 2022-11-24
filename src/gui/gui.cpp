#include "gui.hpp"

#include "../resources.hpp"
#include "../../lib/imgui/imgui.h"
#include "../../lib/imgui/imgui_impl_glfw.h"
#include "../../lib/imgui/imgui_impl_opengl3.h"
#include "../../lib/imgui/imgui_stdlib.h"

ktp::gui::Log ktp::gui::log {};

void ktp::gui::clean() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ktp::gui::draw() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // ImGui::ShowDemoWindow();
  mainWindow();
  logWindow();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ktp::gui::init(GLFWwindow* window) {
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Control

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 430");
  log.add("Dear ImGui %s\n", ImGui::GetVersion());
}

void ktp::gui::logWindow() {
  constexpr auto title {"Log"};
  // For the demo: add a debug button _BEFORE_ the normal log window contents
  // We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
  // Most of the contents of the window will be added by the log.Draw() call.
  ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
  ImGui::Begin(title);
  ImGui::End();
  // Actually call in the regular Log helper (which will Begin() into the same window as we just did)
  log.draw(title);
}

void ktp::gui::mainWindow() {
  ImGui::Begin("keteMine");
  ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  if (ImGui::CollapsingHeader("Resources", ImGuiTreeNodeFlags_None)) {
    shaders();
    textures();
  }
  ImGui::End();
}

void ktp::gui::shaders() {
  if (ImGui::TreeNode("Shaders")) {
    static int selected {0};
    static std::string selected_name {Resources::shader_programs.cbegin()->first};

    // left
    ImGui::BeginChild("shaders list", ImVec2(150, 0), true);
    int index {0};
    for (auto key: Resources::shader_programs) {
      if (ImGui::Selectable(('#' + std::to_string(key.second.id) + ' ' + key.first).c_str(), selected == index))  {
        selected = index;
        selected_name = key.first;
      }
      ++index;
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // right
    ImGui::BeginGroup();
    ImGui::BeginChild("shader view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
    ImGui::Text("Shader program %s (id: %d)", selected_name.c_str(), Resources::shader_programs[selected_name].id);
    ImGui::Separator();
    if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
      if (ImGui::BeginTabItem("Vertex shader")) {
        ImGui::TextWrapped(Resources::shader_programs[selected_name].vertex.c_str());
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Fragment shader")) {
        ImGui::TextWrapped(Resources::shader_programs[selected_name].fragment.c_str());
        ImGui::EndTabItem();
      }
      bool geometry_shader_missing {Resources::shader_programs[selected_name].geometry == ""};
      ImGui::BeginDisabled(geometry_shader_missing);
      if (ImGui::BeginTabItem("Geometry shader")) {
        ImGui::TextWrapped(Resources::shader_programs[selected_name].geometry.c_str());
        ImGui::EndTabItem();
      }
      ImGui::EndDisabled();
      ImGui::EndTabBar();
    }
    ImGui::EndChild();
    if (ImGui::Button("Revert")) {}
    ImGui::SameLine();
    if (ImGui::Button("Save file")) {}
    ImGui::SameLine();
    if (ImGui::Button("Compile")) {}
    ImGui::EndGroup();

    ImGui::TreePop();
  }
}

void ktp::gui::textures() {
  if (ImGui::TreeNode("Textures")) {
    ImGui::TreePop();
  }
}
