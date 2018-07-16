/**
 * Created by cy on 2018/5/26.
 */

#include "YExpression.h"
#include <iostream>
#include <deque>
#include "stringUtils.h"

using namespace std;

const std::unordered_map<YExpression::EOperatorType, YExpression::priority_t> YExpression::operator_priority = {
	{add, 4}, {sub, 4},
	{mul, 3}, {div, 3},
	{neg, 2}, {front_inc, 2}, {back_inc, 2}, {front_dec, 2}, {back_inc, 2},

	{val, 100} //max
};

const char* YExpression::className() const { return "YExpression"; }
void YExpression::print() { operation_tree->print(); }
YVal* YExpression::execute() { return operation_tree->execute(); }
YExpression::~YExpression() { delete operation_tree; }

YExpression* YExpression::parse(const char* str) {
	auto exp = new YExpression;
	exp->s_expr = str;
	exp->operation_tree = makeOperationTree(str);
	return exp;
}

YExpression::OperationNode* YExpression::makeOperationTree(const char* str) {
	auto first = str;
	auto last = str + strlen(str);

	OperationNode* r_op = new OperationNode(val, nullptr, parseIdentifier(first));

	operation_stack_t node_stack;
	node_stack.push(r_op);

	//forst of all needn't consider priority,just left to right
	while(*first != 0) {
		const auto type = parseSign(first);
		const auto priority = getPriority(type);
		OperationNode* father_node = getFatherNode(node_stack, priority);

		OperationNode* new_node = new OperationNode(type, father_node->r_operand);
		father_node->r_operand = new_node;
		node_stack.push(new_node);

		new_node = parsePrefixes(first, node_stack);
		new_node->r_operand = parseIdentifier(first);
	}

	OperationNode* ret_node = (OperationNode*)r_op->r_operand;
	r_op->r_operand = nullptr;
	delete r_op;

	return ret_node;
}

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

	while(getPriority(stack.top()->opType) <= priority) {
		stack.pop();
	}

	return stack.top();
}

YExpression::OperationNode* YExpression::parsePrefixes(const char*& first, operation_stack_t& node_stack) {
	//deal with prefixes;
	OperationNode* new_node = node_stack.top();
	EOperatorType prefix_type;
	while ((prefix_type = parseFrefix(first)) != NOTHING) {
		//NOTE that prefixes are the lowest priority so that we needn't find father.
		auto node = new OperationNode(prefix_type);
		new_node->r_operand = node;
		new_node = node;
		node_stack.push(new_node);
	}
	return new_node;
}

YExpression::OperationNode* YExpression::parseParentheses(const char*& first) {
	//deal with parentheses
	const char* l_parenthesis = first;
	int deep = 0;
	do {
		if (*first == '(')deep++;
		if (*first == ')')deep--;
		first++;
	} while (deep > 0);

	char* s_expr = newString(l_parenthesis + 1, first - 1);
	return makeOperationTree(s_expr);
}

YExpression::EOperatorType YExpression::parseSign(const char*& str) {
	skipBlank(str);

	switch(*str) {
	case '+':
		str++;
		return add;
	case '-':
		str++;
		return sub;
	case '*':
		str++;
		return mul;
	case '/':
		str++;
		return div;
	default:
		throw YParseFailedException("YExpression::EOperatorType::parseSign", str, "the operator is not defined");
	}
}

YExpression::EOperatorType YExpression::parseFrefix(const char*& str) {
	skipBlank(str);

	switch(str[0]) {
	case '-': {
		const char c = str[1];
		if(c == '-') {
			str += 2;
			return front_dec;
		}

		//don't see -1 as -(1)
		//just see it as number
		if((c >= '0' && c <= '9') || c == '.') {
			return NOTHING;
		}

		str += 1;
		return neg;
	}
	case '+': {
		const char c = str[1];
		if(c == '+') {
			str += 2;
			return front_inc;
		}

		str += 1;
		return NOTHING;
	}
	}
	return NOTHING;
}

inline void skipSuffix(const char*& p) {
	while ((*p == 'L') || (*p == 'l') || (*p == 'F') || (*p == 'f') || (*p == 'u') || (*p == 'U'))p++;
}

inline void skipInt(const char*&p) {
	while (*p >= '0' && *p <= '9') p++;
}

Executable* YExpression::parseIdentifier(const char*& str) {
	skipBlank(str);
	if (*str == '(') return parseParentheses(str);
	
	const char* p = str;
	//skip signs
	if(*p == '+')p++;
	if(*p == '-')p++;

	//for a number literal
	if (*p >= '0' && *p <= '9') {
		skipInt(p);
		//deal \.\d+
		if (*p != '.') {
			skipSuffix(p);
			char* s_int = newString(str, p);
			str = p;
			return YVal::parseInt(s_int);
		}
		
		//deal \d*\.\d*
		p++;//p==decimal part
		skipInt(p);
		skipSuffix(p);
		char* s_decimal = newString(str, p);
		str = p;
		return YVal::parseDecimal(s_decimal);
	}
}

YExpression::priority_t YExpression::getPriority(EOperatorType type) { return operator_priority.find(type)->second; }

YExpression::OperationNode* YExpression::makeTestOperationTree() {
	//1+((2*3)-1)/4=2.25
	auto mul_node = new OperationNode;
	mul_node->opType = mul;
	mul_node->l_operand = YVal::parse("2");
	mul_node->r_operand = YVal::parse("3");

	auto sub_node = new OperationNode;
	sub_node->opType = sub;
	sub_node->l_operand = mul_node;
	sub_node->r_operand = YVal::parse("1");;

	auto div_node = new OperationNode;
	div_node->opType = div;
	div_node->l_operand = sub_node;
	div_node->r_operand = YVal::parse("4");;

	auto root = new OperationNode;
	root->opType = add;
	root->l_operand = YVal::parse("1");;
	root->r_operand = div_node;

	return root;
}
