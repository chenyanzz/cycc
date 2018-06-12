/**
 * Created by cy on 2018/5/26.
 */

#include "YExpression.h"
#include <iostream>

using namespace std;

const unordered_map<string, YExpression::priority_t> YExpression::operator_priority =
{
	{"*", 3}, {"/", 3},
	{"+", 4}, {"-", 4}
};

const char* YExpression::className() const { return "YExpression"; }
void YExpression::print() { cout << "(" << className() << "){" << s_expr << "}"; }

YExpression::OperationNode* YExpression::makeOperationTree(const char* str) {
	const char* first = str;
	const char* last  = str + strlen(str);

	OperationNode* root = new OperationNode;
	//to find signs
	for(; first != last; first++) {
		char c = *first;
		//pass if c is a identifier
		if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') continue;

		switch(c) {
		case '+': {}
		}
	}

	return nullptr;
}

YExpression::OperationNode::OperationNode(EOperatorType opType,
	YExpression* l_operand, YExpression* r_operand, YExpression* condition)
	: opType(opType), l_operand(l_operand), r_operand(r_operand), condition(condition) {}

YExpression::OperationNode::OperationNode()
	: opType(val), l_operand(nullptr), r_operand(nullptr), condition(nullptr) {};

//1+2*3+4
bool YExpression::parse(const char* str, YExpression* pExp) { return true; }

YVal* YExpression::execute() {
	return nullptr;
}
