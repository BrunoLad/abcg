#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#include "abcg.hpp"

void OpenGLWindow::initializeGL() {
  // Create shader program
  m_program = createProgramFromFile(getAssetsPath() + "objects.vert",
                                    getAssetsPath() + "objects.frag");

  m_programLine = createProgramFromFile(getAssetsPath() + "objects.vert",
                                        getAssetsPath() + "objects.frag");
  // Clear window
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

#if !defined(__EMSCRIPTEN__)
  glEnable(GL_PROGRAM_POINT_SIZE);
#endif
}

void OpenGLWindow::paintGL() {
  paintPoints();
  if (m_elapsedTimer.elapsed() >= 1000 / 1000.0) {
    m_elapsedTimer.restart();
    paintLine();
  }
}

void OpenGLWindow::paintPoints() {
  // Create OpenGL buffers for array of points m_Points
  setupModelPoints();

  // Set the viewport
  glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  if (m_step >= 1) {
    m_step = -1.0f;
    m_angle_radian = 0.0f;
    glClear(GL_COLOR_BUFFER_BIT);
  }

  // First helix of a dna molecule
  m_Points[0].y = m_step;
  m_angle_radian = m_step * 2000 * (M_PI / 180.0f);
  m_Points[0].x = std::sin(m_angle_radian / 4.0f) / 4.0f;

  // Second helix of a dna molecule
  m_Points[1].y = m_step;
  m_Points[1].x = std::cos(m_angle_radian / 4.0f + M_PI_2) / 4.0f;

  m_step += 0.001f;

  // Start using VAO
  glUseProgram(m_program);
  // Start using buffers created in createBuffers()
  glBindVertexArray(m_vao);

  // Draw both points
  glDrawArrays(GL_POINTS, 0, 2);

  // End using VAO
  glBindVertexArray(0);
  // End using the shader program
  glUseProgram(0);
}

void OpenGLWindow::paintLine() {
  setupModelLines();

  // Set the viewport
  glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  // Draw Lines
  m_Lines[0].x = m_Points[0].x;
  m_Lines[0].y = m_Points[0].y;
  m_Lines[1].x = m_Points[1].x;
  m_Lines[1].y = m_Points[1].y;

  // Start using VAO
  glUseProgram(m_programLine);
  // Start using buffers created in createBuffers()
  glBindVertexArray(m_vaoLine);

  glDrawArrays(GL_LINES, 0, 2);

  // End using VAO
  glBindVertexArray(0);
  // End using the shader program
  glUseProgram(0);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  {
    ImGui::SetNextWindowPos(ImVec2(5, 81));
    ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoDecoration);

    if (ImGui::Button("Clear window", ImVec2(150, 30))) {
      glClear(GL_COLOR_BUFFER_BIT);
    }

    ImGui::End();
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
  glDeleteProgram(m_program);
  glDeleteBuffers(1, &m_vboVertices);
  glDeleteBuffers(1, &m_vboColors);
  glDeleteVertexArrays(1, &m_vao);
  glDeleteProgram(m_programLine);
  glDeleteBuffers(1, &m_vboLine);
  glDeleteBuffers(1, &m_vboLineColors);
  glDeleteVertexArrays(1, &m_vaoLine);
}

void OpenGLWindow::setupModelPoints() {
  // Release previous VBO and VAO
  glDeleteBuffers(1, &m_vboVertices);
  glDeleteBuffers(1, &m_vboColors);
  glDeleteVertexArrays(1, &m_vao);

  std::array<glm::vec3, 2> colors{glm::vec3{1, 1, 1}, glm::vec3{1, 1, 1}};

  // Generate a new VBO and get the associated ID
  glGenBuffers(1, &m_vboVertices);
  // Bind VBO in order to use it
  glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
  // Upload data to VBO
  glBufferData(GL_ARRAY_BUFFER, m_Points.size() * sizeof(glm::vec2),
               m_Points.data(), GL_STATIC_DRAW);

  // Generate VBO of colors
  glGenBuffers(1, &m_vboColors);
  glBindBuffer(GL_ARRAY_BUFFER, m_vboColors);
  glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3),
               colors.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Unbinding the VBO is allowed (data can be released now)
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};
  GLint colorAttribute{glGetAttribLocation(m_program, "inColor")};

  // Create VAO
  glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  glBindVertexArray(m_vao);

  glEnableVertexAttribArray(positionAttribute);
  glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
  glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glEnableVertexAttribArray(colorAttribute);
  glBindBuffer(GL_ARRAY_BUFFER, m_vboColors);
  glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  glBindVertexArray(0);
}

void OpenGLWindow::setupModelLines() {
  // LINE DRAW

  // Release previous VBO and VAO
  glDeleteBuffers(1, &m_vboLine);
  glDeleteBuffers(1, &m_vboLineColors);
  glDeleteVertexArrays(1, &m_vaoLine);

  glm::vec3 color{1, 0, 0};

  // Generate a new VBO and get the associated ID
  glGenBuffers(1, &m_vboLine);
  // Bind VBO in order to use it
  glBindBuffer(GL_ARRAY_BUFFER, m_vboLine);

  // Upload data to VBO
  glBufferData(GL_ARRAY_BUFFER, m_Lines.size() * sizeof(glm::vec2),
               m_Lines.data(), GL_STATIC_DRAW);
  // Generate VBO of colors
  glGenBuffers(1, &m_vboLineColors);
  glBindBuffer(GL_ARRAY_BUFFER, m_vboLineColors);
  glBufferData(GL_ARRAY_BUFFER, sizeof(color), &color, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Unbinding the VBO is allowed (data can be released now)
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{glGetAttribLocation(m_programLine, "inPosition")};
  GLint colorAttribute{glGetAttribLocation(m_programLine, "inColor")};

  // Create VAO
  glGenVertexArrays(1, &m_vaoLine);

  // Bind vertex attributes to current VAO
  glBindVertexArray(m_vaoLine);

  glEnableVertexAttribArray(positionAttribute);
  glBindBuffer(GL_ARRAY_BUFFER, m_vboLine);
  glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glEnableVertexAttribArray(colorAttribute);
  glBindBuffer(GL_ARRAY_BUFFER, m_vboLineColors);
  glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  glBindVertexArray(0);
}