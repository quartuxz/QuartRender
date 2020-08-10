#pragma once
#include <sstream>
#include <string>
#include <stdexcept>

class ErrorLog
{
private:
	std::stringstream m_ss;
public:
	ErrorLog()noexcept;
	~ErrorLog();

	std::string getLog();

	void log(const std::exception &exc);
};

