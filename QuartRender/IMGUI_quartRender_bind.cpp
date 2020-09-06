#include "QuartRender.h"
#include "src/vendor/imgui/imgui.h"
#include "src/vendor/imgui/imgui_impl_glfw.h"
#include "src/vendor/imgui/imgui_impl_opengl3.h"

#include "quartRenderDLLImplUtils.h"

int IMGUIFunc(showDemoWindow)(RendererHandle renderer, ErrorLogHandle errorLog)
{
    CATCH_LOG_RETURN_GL(
        ImGui::ShowDemoWindow(); ,
        GET_ERROR_LOG(errorLog)
    );
    return SUCCESS_TERMINATE_CODE_GL;
}