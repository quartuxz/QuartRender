#pragma once
#include "src/core/ErrorLog.h"
#include "src/core/utilsGL.h"
#include "src/core/renderers/RendererThreadManager.h"

#define GET_RENDERER_MULT(rendererHandle) static_cast<RendererThreadManager*>(rendererHandle)
#define GET_ERROR_LOG(errorLogHandle) static_cast<ErrorLog*>(errorLogHandle)