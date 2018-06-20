#include "YCode.h"

const char* YCode::className() const { return "YCode"; }

void YCode::print() {}

YVal* YCode::execute() {return nullptr;}

YCode* YCode::parseFromFile(const char* path) {return nullptr;}

YCode* YCode::parseFromString(const char* str) {
	YCode* ycode = new YCode;
	return ycode;
}
