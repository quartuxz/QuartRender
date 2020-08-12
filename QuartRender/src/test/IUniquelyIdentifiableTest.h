#pragma once
#include <string>
#include "../core/generic utils/IUniquelyIdentifiable.h"


class uniquelyIdentifiableTest1 : public IUniquelyIdentifiable {
public:
	uniquelyIdentifiableTest1() {

	}
};

class uniquelyIdentifiableTest2 : public IUniquelyIdentifiable {
public:
	uniquelyIdentifiableTest2() {

	}
};

inline bool runIUniquelyIdentifiableTest(std::string &testName) {
	testName = "IUniquelyIdentifiableTest";
	uniquelyIdentifiableTest1 obj1;
	if (obj1.getID() != 0) {
		return false;
	}
	uniquelyIdentifiableTest2 obj2;
	if (obj2.getID() != 1) {
		return false;
	}
	uniquelyIdentifiableTest1 obj3;
	if (obj1.getID() != 0 || obj3.getID() != 2) {
		return false;
	}
	return true;
}