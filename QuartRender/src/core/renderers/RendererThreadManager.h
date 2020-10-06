#pragma once
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <optional>
#include <functional>
#include <sstream>
#include "IRenderer.h"
#include "../input/InputManager.h"


//TODO: implement dynamic resizing
class RendererThreadManager : public IUniquelyIdentifiable
{
private:
	IRenderer *m_renderer;

	RendererTypes m_rendererType;

	std::stringstream m_threadErrorLog;

	//deprecated conditional variable code
	std::condition_variable m_processCond;
	std::mutex m_lock;
	bool m_processedCond = false;
	bool m_ready = false;
	
	//signals that the RendererThreadManager destructor is called and the thread must cease execution. Always true if window is closed.
	std::atomic<bool> m_end = false;

	std::atomic<bool> m_finishedDestruction = false;

	//signals that there was an exception in thread
	std::atomic<bool> m_threadExcept = false;

	//signals that the thread finished constructing the renderer
	std::atomic<bool> m_rendererConstructed = false;
	std::function<void()> m_fn;
	//thread of renderer execution(openGL context lives here)
	std::thread m_thread;

	static void m_threadFunc(RendererThreadManager* pThis, unsigned int sizex, unsigned int sizey, RendererTypes rendererType);


	void m_signalThread();
	void m_signalEndThread();
public:

	//non-copyable or movable
	RendererThreadManager(const RendererThreadManager&) = delete;
	RendererThreadManager& operator=(const RendererThreadManager&) = delete;


	//does renderer sync
	RendererThreadManager(unsigned int sizex, unsigned int sizey, RendererTypes rendererType);
	void addDrawable(IDrawable* drawable);

	unsigned int getViewportWidth()const noexcept;
	unsigned int getViewportHeight()const noexcept;

	bool isWindowClosed()const noexcept;

	//does renderer sync
	void executeOnThread(std::function<void()> fn);

	RendererTypes getRendererType()const noexcept;

	//gets the input manager for the open window in this thread
	InputManager* getInputManager();

	std::string getThreadErrors()const;

	bool getAndAllowClose();

	//does renderer sync
	void display();


	DrawData& getDrawDataRef()noexcept;

	const DrawData& getDrawDataRef()const noexcept;

	const std::vector<std::uint8_t>* getImageBuffer()const noexcept;

	~RendererThreadManager();
};

