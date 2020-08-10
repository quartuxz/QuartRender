#include "IUniquelyIdentifiable.h"

unsigned long long IUniquelyIdentifiable::m_currentCounter = 0;

IUniquelyIdentifiable::IUniquelyIdentifiable():
	m_ID(m_currentCounter++)
{
}

unsigned long long IUniquelyIdentifiable::getID() const noexcept
{
	return m_ID;
}

IUniquelyIdentifiable::~IUniquelyIdentifiable()
{
}
