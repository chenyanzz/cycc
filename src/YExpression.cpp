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
	// auto first = str;
	// auto last  = str + strlen(str);
	//
	// auto root = new OperationNode;
	// auto current = root;
	//
	// //to find signs
	// for(; first != last; first++) {
	// 	YVal* pVal;
	// 	if(YVal::parse(first, pVal)) {
	// 		
	// 	}
	//
	// 	char c = *first;
	// 	switch(c) {
	// 	case '+':
	// 		root->opType = OperationNode::add;
	// 		break;
	// 	case '-':
	// 		root->opType = OperationNode::sub;
	// 		break;
	// 	case '*':
	// 		root->opType = OperationNode::mul;
	// 		break;
	// 	case '/':
	// 		root->opType = OperationNode::div;
	// 		break;
	// 	}
	// }
	//
	// return nullptr;

	//test: 2+3*4
	auto root    = new OperationNode;
	root->opType = OperationNode::add;
	YVal* pv;
	YVal::parse("2", pv);
	root->l_operand = pv;

	auto mul_node = new OperationNode;
	mul_node->opType = OperationNode::mul;
	YVal::parse("3", pv);
	mul_node->l_operand = pv;
	YVal::parse("4", pv);
	mul_node->r_operand = pv;

	root->r_operand = mul_node;

	auto current = root;

	return root;
}


YExpression::OperationNode::OperationNode(EOperatorType opType,
	YExpression* l_operand, YExpression* r_operand, YExpression* condition)
	: opType(opType), l_operand(l_operand), r_operand(r_operand), condition(condition) {}

YExpression::OperationNode::OperationNode()
	: opType(val), l_operand(nullptr), r_operand(nullptr), condition(nullptr) {}

const char* YExpression::OperationNode::className() const { return "YExpression::OperationNode"; }
void YExpression::OperationNode::print() {}

YVal* YExpression::OperationNode::execute() {
	YVal* l_val = l_operand->execute();
	YVal* r_val = r_operand->execute();
	switch(opType) {
	case add: {
		return YVal::mul(l_val,r_val);
	}
	}
}

//1+2*3+4
bool YExpression::parse(const char* str, YExpression* pExp) { return true; }

YVal* YExpression::execute() {
	return nullptr;
}
