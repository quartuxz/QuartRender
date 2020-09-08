#include "OnScreenRendererIMGUI.h"
#include <stdexcept>

#include "../input/InputManager.h"

#include "../utilsGL.h"


void OnScreenRendererIMGUI::m_startIMGUIFrame()
{

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

OnScreenRendererIMGUI::OnScreenRendererIMGUI(unsigned int sizex, unsigned int sizey):
	IWindowedRenderer(sizex,sizey)
{

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();

    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 460");


    m_startIMGUIFrame();
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

    InputManager::getInputManagerForThread()->m_setIMGUIWantsKeyboard(m_io->WantCaptureKeyboard);

    glfwPollEvents();

    m_startIMGUIFrame();
    

}

OnScreenRendererIMGUI::~OnScreenRendererIMGUI()
{
    // imgui Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
