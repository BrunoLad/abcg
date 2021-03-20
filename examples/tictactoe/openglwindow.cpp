#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>

void OpenGLWindow::initializeGL() {
  auto windowSettings{getWindowSettings()};
  fmt::print("Initial window size: {}x{}\n", windowSettings.width,
             windowSettings.height);
}

void OpenGLWindow::paintGL() {
  // Set the clear color
  glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2],
               m_clearColor[3]);
  // Clear the color buffer
  glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::paintUI() {
  // Parent class will show fullscreen button and FPS meter
  abcg::OpenGLWindow::paintUI();

  // Our own ImGui widgets go below
  {
	bool tttActive = true;

    // Window begin
    ImGui::Begin("Tic-Tac-Toe", &tttActive, ImGuiWindowFlags_MenuBar);

	// Menu Bar
    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("Game")) {
        if (ImGui::MenuItem("Restart")) {
          /* Calls Restart game method. */
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    // Static text
    static char player{'X'};
    ImGui::Text("%c turn", player);

    // ImGui::Button("", ImVec2(50, 50));
    // ImGui::Button("", ImVec2(50, 50));
    // ImGui::Button("", ImVec2(50, 50));
    // ImGui::Button("", ImVec2(50, 50));
    // ImGui::Button("", ImVec2(50, 50));
    // ImGui::Button("", ImVec2(50, 50));
    // ImGui::Button("", ImVec2(50, 50));
    // ImGui::Button("", ImVec2(50, 50));

    if (ImGui::Button("##9", ImVec2(50, 50))) {
      fmt::print("Button pressed!\n");
      player = tttReturnPlayer(player);
    }

    // Restart game button
    if (ImGui::Button("Restart game", ImVec2(200, 50))) {
      fmt::print("Game Restarted\n");
    }

    // Window end
    ImGui::End();
  }
}

char OpenGLWindow::tttReturnPlayer(char c) { return c == 'X' ? 'O' : 'X'; }
