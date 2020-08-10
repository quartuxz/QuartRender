#include "ErrorLog.h"

ErrorLog::ErrorLog()noexcept
{
}

ErrorLog::~ErrorLog()
{
}

std::string ErrorLog::getLog()
{
	return m_ss.str();
}

void ErrorLog::log(const std::exception&exc)
{
	m_ss << exc.what() << std::endl;
}
