#pragma once
#include <cstdint>
#include "src/core/input/inputStructs.h"


#ifdef QUARTRENDER_EXPORTS
#define QUARTRENDER_API __declspec(dllexport)
#else
#define QUARTRENDER_API __declspec(dllimport)
#endif


#define quartRenderFunc(name) quartRender_##name
#define IMGUIFunc(name) IMGUI_##name

typedef void* ErrorLogHandle;
typedef void* RendererHandle;



struct testStruct {
	int testID;
	std::uint16_t data16;
	std::uint8_t data8;
};

extern "C" {

	//creation
	QUARTRENDER_API int quartRenderFunc(initQuartRender)();
	QUARTRENDER_API ErrorLogHandle quartRenderFunc(createLogger)();
	QUARTRENDER_API RendererHandle quartRenderFunc(createRenderer)(ErrorLogHandle errorLog, unsigned int startx, unsigned int starty, unsigned int rendererType);

	//usage
	QUARTRENDER_API int quartRenderFunc(renderImage)(RendererHandle renderer, ErrorLogHandle errorLog);
	QUARTRENDER_API int quartRenderFunc(getAndAllowClose)(RendererHandle renderer, ErrorLogHandle errorLog, bool *val);

		//deprecated
	QUARTRENDER_API void quartRenderFunc(getRenderImage)(RendererHandle renderer, const std::uint8_t** imgbuf, unsigned int* sizex, unsigned int* sizey);

	//TODO: implement this
	QUARTRENDER_API void quartRenderFunc(getLogString)(ErrorLogHandle errorLog, char* str, unsigned int *len);

	//destruction
	QUARTRENDER_API int quartRenderFunc(destroyRenderer)(RendererHandle renderer);
	QUARTRENDER_API int quartRenderFunc(destroyLogger)(ErrorLogHandle errorLog);
		//call this when the application stops using quartRender
	QUARTRENDER_API int quartRenderFunc(exitQuartRender)();

	//util
	QUARTRENDER_API void quartRenderFunc(getGLVersion)(char*);
	
	//input
	QUARTRENDER_API int quartRenderFunc(getAndPopLastKeyboardInput)(RendererHandle renderer, ErrorLogHandle errorLog, KeyboardInput *keyboardInput);
	QUARTRENDER_API int quartRenderFunc(getAndPopLastMouseButtonInput)(RendererHandle renderer, ErrorLogHandle errorLog, MouseButtonInput *mouseButtonInput);
	QUARTRENDER_API int quartRenderFunc(getAndPopLastScrollInput)(RendererHandle renderer, ErrorLogHandle errorLog, ScrollInput* scrollInput);
	QUARTRENDER_API int quartRenderFunc(getCurrentCursorPosition)(RendererHandle renderer, ErrorLogHandle errorLog, CursorPosition *cursorPosition);

	//IMGUI
	QUARTRENDER_API int IMGUIFunc(showDemoWindow)(RendererHandle renderer, ErrorLogHandle errorLog);


	//tests (USE std::cout/clog/cerr but only for testing) never use these functions in final product
		//run after initQuartRender but before anything else
	QUARTRENDER_API const char* quartRenderFunc(runTests)();
		//
	QUARTRENDER_API int quartRenderFunc(structPassTest)(testStruct *tstStrc);
		//drawTest, testDrawableName does nothing!, it is meant to show the true form of a function like this one, where the name of a 
		//registered drawable is passed in to specify the form of the IDrawable(i.e "earth" displays a sphere/circle with earth-like visuals, as long as "earth" is registered and described)
	QUARTRENDER_API int quartRenderFunc(drawTest)(RendererHandle renderer, ErrorLogHandle errorLog,char *testDrawableName, float posx, float posy);
	QUARTRENDER_API int quartRenderFunc(destroyAllDrawTests)();

	QUARTRENDER_API int quartRenderFunc(addTestError)(ErrorLogHandle errorLog);

	QUARTRENDER_API int quartRenderFunc(startTestRenderer)(unsigned int sizex, unsigned int sizey);
	QUARTRENDER_API int quartRenderFunc(renderImageTest)(const std::uint8_t**imgbuf, unsigned int* sizex, unsigned int* sizey);
	QUARTRENDER_API int quartRenderFunc(stopTestRenderer)();

	QUARTRENDER_API void quartRenderFunc(testStringFunc)(char* out, unsigned int *len);

	QUARTRENDER_API int quartRenderFunc(testFunc)(const char*);
	QUARTRENDER_API void quartRenderFunc(imageTest)(unsigned char** imgbuf, unsigned int* sizex, unsigned int* sizey);
	QUARTRENDER_API void quartRenderFunc(windowTest)();
}