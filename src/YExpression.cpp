/**
 * Created by cy on 2018/5/26.
 */

#include "YExpression.h"
#include <iostream>
#include <deque>

using namespace std;

const std::unordered_map<YExpression::EOperatorType, YExpression::priority_t> YExpression::operator_priority = {
	{add, 4}, {sub, 4},
	{mul, 3}, {div, 3},

	{val, 100} //??
};

const char* YExpression::className() const { return "YExpression"; }
void YExpression::print() { cout << "(" << className() << "){" << s_expr << "}"; }

YExpression::OperationNode* YExpression::makeOperationTree(const char* str) {
	auto first = str;
	auto last = str + strlen(str);

	OperationNode* r_op = new OperationNode(val, nullptr, parseIdentifier(first));
	const auto const root_node = new OperationNode(val, nullptr, r_op);

	operation_stack_t node_stack;
	node_stack.push(r_op);

	//forst of all needn't consider priority,just left to right
	while((first != last) && (*first != 0)) {
		const auto type = parseSign(first);
		const auto priority = getPriority(type);

		OperationNode* father_node = getFatherNode(node_stack, priority);
		
		//build a new node instead of the parent's r_operand
		OperationNode* new_node = new OperationNode(type, father_node->r_operand, parseIdentifier(first));
		father_node->r_operand = new_node;

		node_stack.push(new_node);
	}

	return root_node;
}

//priority bigger, calc later
//there's the bug
YExpression::OperationNode* YExpression::getFatherNode(operation_stack_t& stack, priority_t priority) {
	/*
	
	1*2+3
	stack = [val,mul,add]
	new op=4

	cases:

	1*2+(3* _4)
	comes a mul=3 ==> return stack[2](add)

	(1*2+3)- _4
	comes a sub=4 ==> return stack[0](val)

	=== === ===

	all in all:
	to find a priority bigger one
	*/

	while(getPriority(stack.top()->opType)<=priority) {
		stack.pop();
	}

	return stack.top();
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

//1+2*3+4
bool YExpression::parse(const char* str, YExpression* pExp) { return true; }
YExpression::~YExpression() {
	delete operation_tree;
}

YVal* YExpression::execute() {
	return nullptr;
}
