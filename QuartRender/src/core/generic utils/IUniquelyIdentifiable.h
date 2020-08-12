#pragma once

using uniqueID = unsigned long long;

class IUniquelyIdentifiable {
private:
	static uniqueID m_currentCounter;
protected:
	const uniqueID m_ID;

	IUniquelyIdentifiable()noexcept;
public:
	uniqueID getID()const noexcept;
	virtual ~IUniquelyIdentifiable();
};