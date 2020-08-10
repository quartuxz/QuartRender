#pragma once

using uniqueID = unsigned long long;

class IUniquelyIdentifiable {
private:
	static uniqueID m_currentCounter;
protected:
	uniqueID m_ID;

	IUniquelyIdentifiable();
public:
	uniqueID getID()const noexcept;
	virtual ~IUniquelyIdentifiable();
};