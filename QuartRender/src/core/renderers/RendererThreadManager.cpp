#include "RendererThreadManager.h"
#include "OnScreenRendererIMGUI.h"
#include "OnScreenRenderer.h"
#include "Renderer.h"
#include "../utilsGL.h"

//TODO: fix terrible cpu usage (probably the polling loop)
void RendererThreadManager::m_threadFunc(RendererThreadManager* pThis, unsigned int sizex, unsigned int sizey, RendererTypes rendererType)
{
	InputManager::registerInputManagerInThread();

	switch (rendererType)
	{
	case RendererTypes::onscreenRendererIMGUI:
		pThis->m_renderer = new OnScreenRendererIMGUI(sizex, sizey);
		break;
	case RendererTypes::onscreenRenderer:
		pThis->m_renderer = new OnScreenRenderer(sizex,sizey);
		break;
	case RendererTypes::offscreenRenderer:
		pThis->m_renderer = new Renderer(sizex, sizey);
		break;
	default:
		break;
	}



	pThis->m_rendererConstructed.store(true);
	while (!pThis->m_end.load()) {
		try{

			bool* localReady = &pThis->m_ready;
			std::unique_lock<std::mutex> lk(pThis->m_lock);
			pThis->m_processCond.wait(lk, [localReady]()noexcept {return *localReady; });

			//signal back routine
			auto localProcessedCond = &pThis->m_processedCond;
			auto localUniqueLock = &lk;
			auto localProcessCond = &pThis->m_processCond;
			std::function<void()> signalBack = std::function<void()>([localProcessCond, localProcessedCond, localUniqueLock]()
				{
					*localProcessedCond = true; 
					localUniqueLock->unlock();
					localProcessCond->notify_one();
				});
			//signal back routine END

			if (pThis->m_end.load()) {
				signalBack();
				break;
			}
			pThis->m_ready = false;

			pThis->m_fn();
			glfwPollEvents();

			signalBack();

			/* //deprecated non conditional variable code
			while (!pThis->m_proccessFunction.load() && !pThis->m_end.load()) {

				glfwPollEvents();
			}
			//finish execution if either the window is closed or the application is told to end
			if (pThis->m_end.load()) {
				break;
			}
			LOG_TO_CONSOLE_COND("processing started!");
			//the code after the if above only executes when only m_proccessFunction is true
			pThis->m_proccessFunction.store(false);

			//bool* localReady = &pThis->m_ready;
			//std::unique_lock<std::mutex> lk(pThis->m_lock);
			//pThis->m_processCond.wait(lk, [localReady]() {return *localReady; });
		
		
			//NOTHING BETWEEN HERE AND YONDER HAS TO BE ATOMIC
			//pThis->m_ready = false;


			//call the pending function on renderer thread(this)
			LOG_TO_CONSOLE_COND("function call start.");
			pThis->m_fn();
			LOG_TO_CONSOLE_COND("function called!");
			pThis->m_processed.store(true);
			LOG_TO_CONSOLE_COND("processing ended!");
			//lk.unlock();
			//YONDER!!!
			//pThis->m_processCond.notify_one();
			*/
		}
		catch (const std::exception&e) {
			LOG_TO_CONSOLE_COND("ERROR IN RENDERING THREAD!!!");
			LOG_TO_CONSOLE_COND(e.what());
			pThis->m_threadErrorLog << e.what() << std::endl;
			pThis->m_end.store(true);
			break;
		}

	}
	LOG_TO_CONSOLE_COND("RENDERING THREAD EXIT!");
	delete pThis->m_renderer;
	InputManager::unregisterInputManagerInThread();
	pThis->m_renderer = nullptr;
}

void RendererThreadManager::m_signalThread()
{
	/*
	LOG_TO_CONSOLE_COND("thread signalling started.");
	m_proccessFunction.store(true);
	while (!m_processed.load()) {
		if (m_end.load()) {
			m_threadErrorLog << "\n\t the rendering/window thread has already finished execution!";
			throw std::runtime_error(m_threadErrorLog.str());
		}
	}
	m_processed.store(false);
	LOG_TO_CONSOLE_COND("thread signalling ended.");
	*/

	{
		std::lock_guard<std::mutex> lk(m_lock);
		m_ready = true;
	}
	m_processCond.notify_one();

	{
		std::unique_lock<std::mutex> lk(m_lock);
		m_processCond.wait(lk, [this]()noexcept {return m_processedCond; });
		m_processedCond = false;
	}
}

void RendererThreadManager::m_signalEndThread()
{
	m_end.store(true);
	m_signalThread();
}

//needs renderer sync
RendererThreadManager::RendererThreadManager(unsigned int sizex, unsigned int sizey, RendererTypes rendererType):
	m_rendererType(rendererType),
	m_thread(m_threadFunc, this,sizex,sizey,rendererType)
{

	while (!m_rendererConstructed.load()) {

	}

	LOG_TO_CONSOLE_COND("RENDERER THREAD MANAGER CONSTRUCTION DONE!");
}

void RendererThreadManager::addDrawable(IDrawable* drawable)
{
	LOG_TO_CONSOLE_COND("adding drawable.");
	m_renderer->addDrawable(drawable);
}


unsigned int RendererThreadManager::getViewportWidth()const noexcept
{
	return m_renderer->getViewportWidth();
}

unsigned int RendererThreadManager::getViewportHeight() const noexcept
{
	return m_renderer->getViewportHeight();
}

bool RendererThreadManager::isWindowClosed() const noexcept
{
	return m_end.load();
}

//does renderer sync
void RendererThreadManager::executeOnThread(std::function<void()> fn)
{
	LOG_TO_CONSOLE_COND("execution command issue started.");
	m_fn = fn;
	m_signalThread();
	LOG_TO_CONSOLE_COND("execution command issue ended.");
}

RendererTypes RendererThreadManager::getRendererType() const noexcept
{
	return m_rendererType;
}

InputManager* RendererThreadManager::getInputManager()
{
	InputManager *retval;
	executeOnThread([&retval]() {retval = InputManager::getInputManagerForThread(); });
	return retval;
}

std::string RendererThreadManager::getThreadErrors() const
{
	return m_threadErrorLog.str();
}

bool RendererThreadManager::getAndAllowClose()
{
	if (m_end.load()) {
		return true;
	}
	if (m_renderer->windowShouldClose()) {
		m_signalEndThread();
		return true;
	}
	return false;
	
}

//needs renderer sync
//TODO: improve performance
void RendererThreadManager::display()
{
	//issue render call on rendering thread
	executeOnThread([this]() {m_renderer->display(); });
}

const std::vector<std::uint8_t>* RendererThreadManager::getImageBuffer() const noexcept
{
	return m_renderer->getImageBuffer();
}

RendererThreadManager::~RendererThreadManager()
{
	m_signalEndThread();
	m_thread.join();
}
