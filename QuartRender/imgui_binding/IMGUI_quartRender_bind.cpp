#include "../QuartRender.h"
#include "../src/vendor/imgui/imgui.h"
#include "../src/vendor/imgui/imgui_impl_glfw.h"
#include "../src/vendor/imgui/imgui_impl_opengl3.h"

#include "../quartRenderDLLImplUtils.h"

//TODO: implement per window imgui? I dont know



int IMGUIFunc(ShowDemoWindow)(RendererHandle renderer, ErrorLogHandle errorLog)
{
    CATCH_LOG_RETURN_GL(
        ImGui::ShowDemoWindow(); ,
        GET_ERROR_LOG(errorLog)
    );
    return SUCCESS_TERMINATE_CODE_GL;
}

int IMGUIFunc(End)(RendererHandle renderer, ErrorLogHandle errorLog)
{
    CATCH_LOG_RETURN_GL(ImGui::End(); ,
        GET_ERROR_LOG(errorLog));
    return SUCCESS_TERMINATE_CODE_GL;
}

int IMGUIFunc(Begin)(RendererHandle renderer, ErrorLogHandle errorLog, const char* name, bool* p_open, ImGuiWindowFlags flags, bool* retval)
{
    CATCH_LOG_RETURN_GL(*retval = ImGui::Begin(name, p_open, flags);,
        GET_ERROR_LOG(errorLog))
    return SUCCESS_TERMINATE_CODE_GL;
}