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
void YExpression::print() { cout << "(" << className() << "){" << s_expr << "}"; }

YExpression::EOperatorType YExpression::parseFrefix(const char*& str) {
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

		if (type == UNDEFINED)throw new YParseFailedException("YExpression::OperationNode", first, "unknown sign in expression");

		const auto priority = getPriority(type);

		OperationNode* father_node = getFatherNode(node_stack, priority);


		switch(type) {
		case add: case sub: case mul: case div: {
			//build a new node instead of the parent's r_operand

			//case 2-operand operators
			const auto prefix_type = parseFrefix(first);
			const auto perfix_priority = getPriority(type);

			Executable* r_operand = parseIdentifier(first);

			switch(prefix_type) {
			case neg: {
				//r_operand = -(r_operand)
				OperationNode* node = new OperationNode(prefix_type, nullptr, r_operand);
				r_operand = node;
				node_stack.push(node);
			}
			}
			OperationNode* new_node = new OperationNode(type, father_node->r_operand, r_operand);
			father_node->r_operand = new_node;
			node_stack.push(new_node);

		}
		}
	}

	if(strcmp(r_op->className(), "YVal")) {
		return r_op;
	}

	OperationNode* ret_node = (OperationNode*)r_op->r_operand;
	root_node->r_operand = nullptr;
	r_op->r_operand = nullptr;
	delete root_node, r_op;

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

YExpression::EOperatorType YExpression::parseSign(const char*& str) {
	while (*str == ' ' || *str == '\n')str++;

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

	while (*str == ' ' || *str == '\n')str++;

	const char* p = str;

	//(...)
	if(*p == '(') {
		while(*p != ')')p++;

		//*str='(', *p=')'
		char* s_expr = newString(str + 1, p);

		str = p + 1;

		return makeOperationTree(s_expr);
	}


	/*for number*/

	if(*p == '+')p++;

	//    -1, -1.23
	if(*p == '-')p++;
	//==> 1, 1.23
	while(*p >= '0' && *p <= '9') p++;

	//deal -1
	if(*p != '.') {
		char* s_int = newString(str, p);
		str = p;
		return YVal::parseInt(s_int);
	}
	//*p=='.'
	p++;

	//deal -1.23
	while(*p >= '0' && *p <= '9') p++;

	char* s_decimal = newString(str, p);
	str = p;
	return YVal::parseDecimal(s_decimal);

}

//todo: there is a bug that -1 struct.member goes wrong
constexpr bool YExpression::isCharInIdentifier(const char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '.';
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

//1+2*3+4
bool YExpression::parse(const char* str, YExpression* pExp) { return true; }

YExpression::~YExpression() {
	delete operation_tree;
}

YVal* YExpression::execute() {
	return nullptr;
}
