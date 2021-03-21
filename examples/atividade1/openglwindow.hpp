#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <random>

#include "abcg.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint m_vao{};
  GLuint m_vaoLine{};
  GLuint m_vboVertices{};
  GLuint m_vboLine{};
  GLuint m_vboColors{};
  GLuint m_vboLineColors{};
  GLuint m_program{};
  GLuint m_programLine{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  float m_angle_radian{0.0f};
  float m_step{-1.0f};

  std::random_device m_randomDevice;
  std::default_random_engine m_randomEngine;

  abcg::ElapsedTimer m_elapsedTimer;
  std::array<glm::vec2, 2> m_Points{};
  std::array<glm::vec2, 2> m_Lines{};

  void setupModelPoints();
  void setupModelLines();
  void paintLine();
  void paintPoints();
};

#endif