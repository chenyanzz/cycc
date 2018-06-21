/**
 * Created by cy on 2018/5/26.
 */

#include "YExpression.h"
#include <iostream>

using namespace std;

const std::unordered_map<YExpression::EOperatorType, YExpression::priority_t> YExpression::operator_priority = {
	{add, 4}, {sub, 4},
	{mul, 3}, {div, 3},
};

const char* YExpression::className() const { return "YExpression"; }
void YExpression::print() { cout << "(" << className() << "){" << s_expr << "}"; }

YExpression::OperationNode* YExpression::makeOperationTree(const char* str) {
	auto first = str;
	auto last = str + strlen(str);
	// //to find signs
	// /*
	// loop{
	//	
	//	parse val if meet end parse sign;
	//	if(sign priority < old 
	// }
	// */
	

	Executable* current = parseIdentifier(first);
	auto last_pri = (priority_t)1000;

	EOperatorType type;

	//forst of all needn't consider priority,just left to right
	while((first != last) && (*first != 0)) {

		type = parseSign(first);
		current = new OperationNode(type, current,parseIdentifier(first));
		// auto pri = getPriority(type);
		// //TODO: find node->father -r
		// //auto l_node = findNode(current_node, pri);
		//
		// auto l_node = current_node;
		// auto new_node = new OperationNode;
		// new_node->l_operand = l_node;
		// current_node = new OperationNode;
		// new_node->r_operand = current_node;
	}

	return (OperationNode*)current;
}

YExpression::EOperatorType YExpression::parseSign(const char*& str) {
	EOperatorType opType = UNDEFINED;
	switch(*str) {
	case 0:
		return opType;
	case '+':
		opType = add;
		str++;
		break;
	case '-':
		opType = sub;
		str++;
		break;
	case '*':
		opType = mul;
		str++;
		break;
	case '/':
		opType = div;
		str++;
		break;
	default:
		opType = UNDEFINED;
		break;
	}

	return opType;
}

Executable* YExpression::parseIdentifier(const char*& str) {
	const char* p = str;
	while(isCharInIdentifier(*p))p++;

	int len = p - str;
	char* s_identifier = new char[len + 1];

	memcpy(s_identifier, str, len);
	s_identifier[len] = 0;

	YVal* pVal;
	if(!YVal::parse(s_identifier, pVal))return nullptr;

	str = p;
	return pVal;
}

constexpr bool YExpression::isCharInIdentifier(const char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '.';
}

YExpression::priority_t YExpression::getPriority(EOperatorType type) { return operator_priority.find(type)->second; }

YExpression::OperationNode* YExpression::makeTestOperationTree() {
	//1+((2*3)-1)/4=2.25
	YVal* pv;

	auto mul_node = new OperationNode;
	mul_node->opType = mul;
	YVal::parse("2", pv);
	mul_node->l_operand = pv;
	YVal::parse("3", pv);
	mul_node->r_operand = pv;

	auto sub_node = new OperationNode;
	sub_node->opType = sub;
	sub_node->l_operand = mul_node;
	YVal::parse("1", pv);
	sub_node->r_operand = pv;

	auto div_node = new OperationNode;
	div_node->opType = div;
	div_node->l_operand = sub_node;
	YVal::parse("4", pv);
	div_node->r_operand = pv;

	auto root = new OperationNode;
	root->opType = add;
	YVal::parse("1", pv);
	root->l_operand = pv;
	root->r_operand = div_node;

	return root;
}


YExpression::OperationNode::OperationNode(EOperatorType opType, Executable* l_operand, Executable* r_operand, Executable* condition)
	: opType(opType), l_operand(l_operand), r_operand(r_operand), condition(condition) {}

YExpression::OperationNode::OperationNode() {}

const char* YExpression::OperationNode::className() const { return "YExpression::OperationNode"; }
void YExpression::OperationNode::print() {}

YVal* YExpression::OperationNode::execute() {
	YVal* l_val = l_operand->execute();
	YVal* r_val = r_operand->execute();
	YVal* ret = nullptr;

	//NOTE that || and && are short-circuited so canNOT exec(r_operand) first!

	switch(opType) {
	case add:
		ret = YVal::add(l_val, r_val);
		break;
	case sub:
		ret = YVal::sub(l_val, r_val);
		break;
	case mul:
		ret = YVal::mul(l_val, r_val);
		break;
	case div:
		ret = YVal::div(l_val, r_val);
		break;

	case UNDEFINED:
	default:
		throw YException("opType undefined");
	}

	return ret;
}

//1+2*3+4
bool YExpression::parse(const char* str, YExpression* pExp) { return true; }

YVal* YExpression::execute() {
	return nullptr;
}
