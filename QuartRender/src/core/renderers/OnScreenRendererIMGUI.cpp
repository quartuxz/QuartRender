#include "OnScreenRendererIMGUI.h"
#include <stdexcept>

#include "../../vendor/imgui/imgui.h"
#include "../../vendor/imgui/imgui_impl_glfw.h"
#include "../../vendor/imgui/imgui_impl_opengl3.h"


#include "../InputManager.h"

#include "../utilsGL.h"


OnScreenRendererIMGUI::OnScreenRendererIMGUI(unsigned int sizex, unsigned int sizey):
	IWindowedRenderer(sizex,sizey)
{

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    [[maybe_unused]]
    ImGuiIO& io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 460");


    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

}

void OnScreenRendererIMGUI::display()
{
    ImGui::Render();
    int newvsx = 0, newvsy = 0;
    glfwGetFramebufferSize(m_window, &newvsx, &newvsy);
    THROW_ERRORS_GL_FAST(glViewport(0, 0, newvsx, newvsy));
    IRenderer::display();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(m_window);


    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();
}

OnScreenRendererIMGUI::~OnScreenRendererIMGUI()
{
    // imgui Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
