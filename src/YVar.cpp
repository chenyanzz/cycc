#include "YVar.h"
#include "iostream"
#include "stringUtils.h"
#include "YExpression.h"
using namespace std;

YVar::~YVar() {}
const char* YVar::className() const { return "YVar"; }

void YVar::print() {
	cout << "[YVar]{name=\"" << name << "\", value=";
	val->print();
	cout << "}";
}

bool YVar::isAvailableVarName(const char* name) {
	if(name[0] >= '0' && name[0] <= '9')return false;

	for(auto it = name; *it != 0; it++) {
		const char c = *it;
		if(!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'))return false;
	}
	return true;
}

YVar::YVar(const char* name, YVal* val): name(name), val(val) {}

//bug: can't deal with int i=0,j=10;
YVar* YVar::parse(const char* str) {
	const auto str_end = str + strlen(str) - 1; //skip ';'
	auto type_begin = str;
	skipBlank(type_begin);

	auto definition_begin = find(str, str_end, '='); //include '='
	auto name_last = definition_begin - 1;
	skipBlankBack(name_last);
	auto name_begin = name_last;
	while(!isBlank(*name_begin))name_begin--;
	name_begin++;
	auto type_last = name_begin - 1;
	skipBlankBack(type_last);

	auto s_type = newString(type_begin, type_last + 1);
	YType* type = YType::parse(s_type);
	YVal* val = new YVal(type);

	auto s_name = newString(name_begin, name_last + 1);
	if(!isAvailableVarName(s_name))throw YNotAAvailableNameException(s_name);

	YVar* var = new YVar(s_name, val);

	//if var has a definition
	if(*definition_begin == '=') {
		const auto s_expr = newString(definition_begin + 1, str_end);
		var->initial_value = YExpression::parse(s_expr);
	}

	return var;
}

void YVar::init() {
	if (initial_value != nullptr) {
		//TODO what if type no the same??
		//val = initial_expression->execute();
	}
}

YVal* YVar::execute() { return val->execute(); }
