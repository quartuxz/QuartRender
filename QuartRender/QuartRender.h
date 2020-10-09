#pragma once

#include <cstdint>
#include "src/core/input/inputStructs.h"
#include "src/core/drawables/drawable classes/PlanetDrawable.h"


#ifdef QUARTRENDER_EXPORTS
#define QUARTRENDER_API __declspec(dllexport)
#else
#define QUARTRENDER_API __declspec(dllimport)
#endif


#define quartRenderFuncName(name) quartRender_##name
#define defaultQuartRenderFunc(name) QUARTRENDER_API int quartRenderFuncName(name)


typedef void* ErrorLogHandle;
typedef void* RendererHandle;



struct testStruct {
	int testID;
	std::uint16_t data16;
	std::uint8_t data8;
};

extern "C" {

	//creation
		QUARTRENDER_API int quartRenderFuncName(initQuartRender)();
		QUARTRENDER_API ErrorLogHandle quartRenderFuncName(createLogger)();
		QUARTRENDER_API RendererHandle quartRenderFuncName(createRenderer)(ErrorLogHandle errorLog, unsigned int startx, unsigned int starty, unsigned int rendererType);
	//~creation

	//usage
		QUARTRENDER_API int quartRenderFuncName(renderImage)(RendererHandle renderer, ErrorLogHandle errorLog);
		QUARTRENDER_API int quartRenderFuncName(getAndAllowClose)(RendererHandle renderer, ErrorLogHandle errorLog, bool *val);
		//these dont throw
			QUARTRENDER_API uint32_t quartRenderFuncName(getViewportWidth)(RendererHandle renderer);
			QUARTRENDER_API uint32_t quartRenderFuncName(getViewportHeight)(RendererHandle renderer);
		//~these dont throw
	//~usage

	//drawing
		QUARTRENDER_API int quartRenderFuncName(createPlanet)(ErrorLogHandle errorLog,  const char *planetClassName, PlanetCharacteristics planetCharacteristics);
		QUARTRENDER_API int quartRenderFuncName(drawPlanet)(RendererHandle renderer, ErrorLogHandle errorLog, const char *planetClassName, const char *planetName, double posx, double posy);
	//~drawing

	//camara manipulation:
		QUARTRENDER_API int quartRenderFuncName(zoom)(RendererHandle renderer, ErrorLogHandle errorLog, double delta);
		QUARTRENDER_API int quartRenderFuncName(setZoom)(RendererHandle renderer, ErrorLogHandle errorLog, double value);
		//this doesnt throw
			QUARTRENDER_API double quartRenderFuncName(getZoom)(RendererHandle renderer);
		QUARTRENDER_API int quartRenderFuncName(displace)(RendererHandle renderer, ErrorLogHandle errorLog, double xdisplace, double ydisplace, double zdisplace);
	//~camara manipulation


	//deprecated
	QUARTRENDER_API void quartRenderFuncName(getRenderImage)(RendererHandle renderer, const std::uint8_t** imgbuf, unsigned int* sizex, unsigned int* sizey);

	//error management
		//TODO: implement this
		QUARTRENDER_API void quartRenderFuncName(getLogString)(ErrorLogHandle errorLog, char* str, unsigned int *len);
	//~error management
	
	//destruction
		QUARTRENDER_API int quartRenderFuncName(destroyRenderer)(RendererHandle renderer);
		QUARTRENDER_API int quartRenderFuncName(destroyLogger)(ErrorLogHandle errorLog);
		//call this when the application stops using quartRender
		QUARTRENDER_API int quartRenderFuncName(exitQuartRender)();
	//~destruction

	//util
		QUARTRENDER_API void quartRenderFuncName(getGLVersion)(char*);
	//~util


	//input
		QUARTRENDER_API int quartRenderFuncName(getAndPopLastKeyboardInput)(RendererHandle renderer, ErrorLogHandle errorLog, KeyboardInput *keyboardInput);
		QUARTRENDER_API int quartRenderFuncName(getAndPopLastMouseButtonInput)(RendererHandle renderer, ErrorLogHandle errorLog, MouseButtonInput *mouseButtonInput);
		QUARTRENDER_API int quartRenderFuncName(getAndPopLastScrollInput)(RendererHandle renderer, ErrorLogHandle errorLog, ScrollInput* scrollInput);
		QUARTRENDER_API int quartRenderFuncName(getCurrentCursorPosition)(RendererHandle renderer, ErrorLogHandle errorLog, CursorPosition *cursorPosition);
	//~input





	//tests (THEY USE std::cout/clog/cerr but only for testing) never use these functions in final product
		//run after initQuartRender but before anything else
			QUARTRENDER_API const char* quartRenderFuncName(runTests)();
		//~
		QUARTRENDER_API int quartRenderFuncName(structPassTest)(testStruct *tstStrc);
		//drawing tests
		//drawTest, testDrawableName does nothing!, it is meant to show the true form of a function like this one, where the name of a 
		//registered drawable is passed in to specify the form of the IDrawable(i.e "earth" displays a sphere/circle with earth-like visuals, as long as "earth" is registered and described)
			QUARTRENDER_API int quartRenderFuncName(drawTest)(RendererHandle renderer, ErrorLogHandle errorLog,char *testDrawableName, float posx, float posy);
			QUARTRENDER_API int quartRenderFuncName(drawCubeTest)(RendererHandle renderer, ErrorLogHandle errorLog, double posx, double posy, double posz, double anglex,double angley, double anglez);
		//~drawing tests
		QUARTRENDER_API int quartRenderFuncName(destroyAllDrawTests)();

		QUARTRENDER_API int quartRenderFuncName(addTestError)(ErrorLogHandle errorLog);

		QUARTRENDER_API int quartRenderFuncName(startTestRenderer)(unsigned int sizex, unsigned int sizey);
		QUARTRENDER_API int quartRenderFuncName(renderImageTest)(const std::uint8_t**imgbuf, unsigned int* sizex, unsigned int* sizey);
		QUARTRENDER_API int quartRenderFuncName(stopTestRenderer)();

		QUARTRENDER_API void quartRenderFuncName(testStringFunc)(char* out, unsigned int *len);

		QUARTRENDER_API int quartRenderFuncName(testFunc)(const char*);
		QUARTRENDER_API void quartRenderFuncName(imageTest)(unsigned char** imgbuf, unsigned int* sizex, unsigned int* sizey);
		
		//full tests
		QUARTRENDER_API void quartRenderFuncName(windowTest)();
		QUARTRENDER_API void quartRenderFuncName(full3DWindowTest)();
	//~tests
}