#pragma once
#include <cstdint>

#ifdef QUARTRENDER_EXPORTS
#define QUARTRENDER_API __declspec(dllexport)
#else
#define QUARTRENDER_API __declspec(dllimport)
#endif

typedef void* ErrorLogHandle;
typedef void* RendererHandle;

extern "C" {

	//creation
	QUARTRENDER_API int initQuartRender();
	QUARTRENDER_API ErrorLogHandle createLogger();
	QUARTRENDER_API RendererHandle createRenderer(ErrorLogHandle errorLog, unsigned int startx, unsigned int starty, unsigned int rendererType);

	//usage
	QUARTRENDER_API int renderImage(RendererHandle renderer, ErrorLogHandle errorLog);
	QUARTRENDER_API int getAndAllowClose(RendererHandle renderer, ErrorLogHandle errorLog, bool *val);

		//deprecated
	QUARTRENDER_API void getRenderImage(RendererHandle renderer, const std::uint8_t** imgbuf, unsigned int* sizex, unsigned int* sizey);

	//TODO: implement this
	QUARTRENDER_API void getLogString(ErrorLogHandle errorLog, char* str, unsigned int *len);

	//destruction
	QUARTRENDER_API int destroyRenderer(RendererHandle renderer);
	QUARTRENDER_API int destroyLogger(ErrorLogHandle errorLog);
		//call this when the application stops using quartRender
	QUARTRENDER_API int exitQuartRender();


	//util
	QUARTRENDER_API void getGLVersion(char*);



	//tests (USE std::cout/clog/cerr but only for testing) never use these functions in final product
		//run after initQuartRender but before anything else
	QUARTRENDER_API const char* runTests();

		//drawTest, testDrawableName does nothing, it is meant to show the true form of a function like this one, where the name of a 
		//registered drawable is passed in to specify the form of the IDrawable(i.e "earth" displays a sphere/circle with earth-like visuals, as long as "earth" is registered and described)
	QUARTRENDER_API int drawTest(RendererHandle renderer, ErrorLogHandle errorLog,char *testDrawableName, float posx, float posy);
	QUARTRENDER_API int destroyAllDrawTests();

	QUARTRENDER_API int addTestError(ErrorLogHandle errorLog);

	QUARTRENDER_API int startTestRenderer(unsigned int sizex, unsigned int sizey);
	QUARTRENDER_API int renderImageTest(const std::uint8_t**imgbuf, unsigned int* sizex, unsigned int* sizey);
	QUARTRENDER_API int stopTestRenderer();

	QUARTRENDER_API void testStringFunc(char* out, unsigned int *len);

	QUARTRENDER_API int testFunc(const char*);
	QUARTRENDER_API void imageTest(unsigned char** imgbuf, unsigned int* sizex, unsigned int* sizey);
	QUARTRENDER_API void windowTest();
}