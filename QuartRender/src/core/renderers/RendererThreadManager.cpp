#include "RendererThreadManager.h"
#include "OnScreenRendererIMGUI.h"
#include "OnScreenRenderer.h"
#include "Renderer.h"
#include "../utilsGL.h"

//TODO: fix terrible cpu usage (probably the polling loop)
void RendererThreadManager::m_threadFunc(RendererThreadManager* pThis, unsigned int sizex, unsigned int sizey, RendererTypes rendererType)
{

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
	while (true) {

		bool* localReady = &pThis->m_ready;
		std::unique_lock<std::mutex> lk(pThis->m_lock);
		pThis->m_processCond.wait(lk, [localReady]()noexcept {return *localReady; });

		//signal back routine
		auto localProcessedCond = &pThis->m_processedCond;
		auto localUniqueLock = &lk;
		auto localProcessCond = &pThis->m_processCond;
		std::function<void()> signalBack = [localProcessCond, localProcessedCond, localUniqueLock]()noexcept
			{
				*localProcessedCond = true;
				localUniqueLock->unlock();
				localProcessCond->notify_one();
			};
		//signal back routine END

		try{
			if (pThis->m_end.load()) {
				signalBack();
				break;
			}
			pThis->m_ready = false;

			pThis->m_fn();



			signalBack();
		}
		catch (const std::exception&e) {
			LOG_TO_CONSOLE_COND("ERROR IN RENDERING THREAD!!!");
			LOG_TO_CONSOLE_COND(e.what());
			pThis->m_threadErrorLog << "\t" << e.what() << std::endl;
			pThis->m_end.store(true);
			pThis->m_threadExcept.store(true);
			signalBack();
			break;
		}

	}
	LOG_TO_CONSOLE_COND("RENDERING THREAD EXIT!");
	delete pThis->m_renderer;
	pThis->m_renderer = nullptr;
	pThis->m_finishedDestruction.store(true);
}

void RendererThreadManager::m_signalThread()
{

	{
		std::lock_guard<std::mutex> lk(m_lock);
		m_ready = true;
	}
	m_processCond.notify_one();

	{
		std::unique_lock<std::mutex> lk(m_lock);
		m_processCond.wait(lk, [this]()noexcept {return m_processedCond; });
		if (m_threadExcept.load()) {
			throw std::runtime_error(m_threadErrorLog.str());
		}
		m_processedCond = false;
	}
}

void RendererThreadManager::m_signalEndThread()
{
	if (m_end.load()) {
		return;
	}
	m_end.store(true);
	m_signalThread();
	//spinlock that should last some miniscule amount of time,
	while (!m_finishedDestruction.load()) {

	}
}

//needs renderer sync
RendererThreadManager::RendererThreadManager(unsigned int sizex, unsigned int sizey, RendererTypes rendererType):
	m_rendererType(rendererType),
	m_thread(m_threadFunc, this,sizex,sizey,rendererType)
{

	while (!m_rendererConstructed.load()) {

	}

	m_threadErrorLog << "ERROR IN RENDERER THREAD, RendererThreadManager ID: " << m_ID << std::endl;

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

DrawData& RendererThreadManager::getDrawDataRef() noexcept
{
	return m_renderer->getDrawDataRef();
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
