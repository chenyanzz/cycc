// Created by cy on 2018/06/21
// 
// 


#include "YExpression.h"
#include <iostream>
using namespace std;

YExpression::OperationNode::OperationNode(EOperatorType opType, Executable* l_operand, Executable* r_operand, Executable* condition)
	: opType(opType), l_operand(l_operand), r_operand(r_operand), condition(condition) {}

YExpression::OperationNode::OperationNode() {}

YExpression::OperationNode::~OperationNode() {
	delete l_operand, r_operand, condition;
}

const char* YExpression::OperationNode::className() const { return "YExpression::OperationNode"; }

void YExpression::OperationNode::print() {

	if(opType == val) {
		r_operand->print();
		return;
	}

	cout << "(";
	if(l_operand) l_operand->print();
	cout << " ";
	switch(opType) {
	case add:
		std::cout << "+";
		break;
	case neg:
	case sub:
		cout << "-";
		break;
	case mul:
		cout << "*";
		break;
	case div:
		cout << "/";
		break;
	default:
		cout << "???opType=" << opType;
	}
	cout << " ";
	if(r_operand) r_operand->print();
	cout << ")";
}

YVal* YExpression::OperationNode::execute() {

	//why calc in case scope:
	//cuz operators some do from l to r,while others do from r to l
	//also there are || &&

	YVal *l_val = nullptr, *r_val = nullptr;

	switch(opType) {
	case add:
		l_val = l_operand->execute();
		r_val = r_operand->execute();
		return YVal::add(l_val, r_val);
	case sub:
		l_val = l_operand->execute();
		r_val = r_operand->execute();
		return YVal::sub(l_val, r_val);
	case mul:
		l_val = l_operand->execute();
		r_val = r_operand->execute();
		return YVal::mul(l_val, r_val);
	case div:
		l_val = l_operand->execute();
		r_val = r_operand->execute();
		return YVal::div(l_val, r_val);
	case neg:
		r_val = r_operand->execute();
		return YVal::neg(r_val);
	case val:
		return r_operand->execute();

	default:
		throw YException("opType undefined");
	}
}
