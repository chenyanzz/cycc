#include "YVar.h"
#include "iostream"
#include "stringUtils.h"
using namespace std;

YVar::~YVar() {}
const char* YVar::className() const { return "YVar"; }

void YVar::print() {
	cout << "[YVar]{name=\"" << name << "\", value=";
	val->print();
	cout << "}";
}

bool YVar::isAvailableVarName(std::string& name) {
	if(name[0] >= '0' && name[0] <= '9')return false;

	for(auto it = name.begin(); it != name.end(); it++) {
		const char c = *it;
		if(!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'))return false;
	}
	return true;
}

YVar::YVar(std::string& name, YVal* val): name(name), val(val) {}

YVar* YVar::parse(const char* str) {
	const auto str_end = str + strlen(str);
	auto type_end = str_end;

	//for "int i"
	//first = 0, last = 3
	while(*type_end != ' ')type_end--;
	const auto s_type = newString(str, type_end);
	YType* type = YType::parse(s_type);

	string name = string(newString(type_end + 1, str_end + 1));
	if(!isAvailableVarName(name))
		throw YNotAAvailableNameException(name.c_str());

	return new YVar(name, new YVal(type));
}

YVal* YVar::execute() { return nullptr; }
