#include <cstdint>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

#include <can_interface_lib/can.h>
#include <mini_cheetah_actuator_lib/actuator.h>

#include <GLFW/glfw3.h>

namespace mca = ::mini_cheetah_actuator_lib;
namespace cil = ::can_interface_lib;
constexpr std::uint8_t MOTOR_ID{0x01};
constexpr const char *CAN_IF{"can0"};

static void glfw_error_callback(int error, const char *description) {
  std::cerr << "Glfw Error " << error << ": " << description << "\n";
}

struct Context {

  Context() {
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
      std::cerr << "OpenGL initialization failed.\n";
      exit(EXIT_FAILURE);
    }

    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window with graphics context
    window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example",
                              NULL, NULL);
    if (window == NULL) {
      std::cerr << "GLFW window creation failed.\n";
      exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
  }

  ~Context() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
  }

  void render() {
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  void startImguiFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  }

  bool closeRequested() { return glfwWindowShouldClose(window); }

  void pollEvents() { glfwPollEvents(); }

  GLFWwindow *window{};
  ImVec4 clear_color{0.45f, 0.55f, 0.60f, 1.00f};
};

int main(int, char **) {
  // Setup imgui
  Context context;
  // Setup actuator
  auto can = cil::makeCanInterface();
  if (!can) {
    std::cerr << "Could not create new can interface\n";
  }

  if (!can->connect(CAN_IF)) {
    std::cerr << "Failed to connect to " << CAN_IF << "\n";
    exit(EXIT_SUCCESS);
  }
  std::cout << "Can::connect() succeeded\n";

  mca::Actuator actuator{CAN_IF, mca::MotorId{MOTOR_ID}, *can};

  bool success = actuator.setPosition(
      mca::AngleDeg{0.0F}, mca::VelocityDegPerSecond{0.0F},
      mca::PositionFeedbackGain{0.0F}, mca::VelocityFeedbackGain{0.0F},
      mca::FeedForwardCurrentAmpere{0.0F});

  if (!success) {
    std::cerr << "set all parameters to zero failed.\n";
    exit(EXIT_SUCCESS);
  }
  std::cout << "Actuator::setPosition() to all zero succeeded\n";

  if (!actuator.enable()) {
    std::cerr << "Actuator::enable() (id=" << static_cast<int>(MOTOR_ID)
              << ") failed on " << CAN_IF << "\n";
    exit(EXIT_SUCCESS);
  }
  std::cout << "Actuator::enable() succeeded\n";

  // Main loop
  while (!context.closeRequested()) {
    context.pollEvents();
    context.startImguiFrame();

    static float position_deg{0.0F};
    static float velocity_deg{0.05F};
    static float position_feedback_gain{0.2F};
    static float velocity_feedback_gain{0.0F};
    static float feed_forward_current{0.0F};

    ImGui::Begin("MiniCheetahActuatorGui");
    ImGui::SliderFloat("Position °", &position_deg, -360.0f, 360.0f);
    ImGui::SliderFloat("Velocity °/s", &velocity_deg, 0.0f, 0.1f);
    ImGui::SliderFloat("Position Feedback Gain", &position_feedback_gain, 0.0f,
                       3.0f);
    ImGui::SliderFloat("Velocity Feedback Gain", &velocity_feedback_gain, 0.0f,
                       1.0f);
    ImGui::SliderFloat("Feed Forward Current", &feed_forward_current, 0.0f,
                       18.0f);

    static mca::Status status{mca::MotorId{0x00}, mca::AngleRad{0.0F},
                              mca::VelocityRadPerSecond{0.0F},
                              mca::CurrentAmpere{0.0F}};

    if (ImGui::Button("Send")) {
      actuator.setPosition(mca::AngleDeg{position_deg},
                           mca::VelocityDegPerSecond{velocity_deg},
                           mca::PositionFeedbackGain{position_feedback_gain},
                           mca::VelocityFeedbackGain{velocity_feedback_gain},
                           mca::FeedForwardCurrentAmpere{feed_forward_current});
      if (actuator.getStatus(status)) {
        std::cout << "received status\n";
      }
    }

    ImGui::Text("motor_id = %i", status.motor_id.get());
    ImGui::Text("position = %.5f", status.positon.get());
    ImGui::Text("velocity = %.5f", status.velocity.get());
    ImGui::Text("current  = %.5f", status.current.get());

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    context.render();
  }
  actuator.disable();
  can->disconnect();

  return EXIT_SUCCESS;
}