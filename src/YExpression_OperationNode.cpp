﻿// Created by cy on 2018/06/21
// 
// 


#include "YExpression.h"
#include <iostream>
using namespace std;

YExpression::OperationNode::OperationNode(EOperatorType opType, Executable* l_operand, Executable* r_operand, Executable* condition)
	: opType(opType), l_operand(l_operand), r_operand(r_operand), condition(condition) {}

YExpression::OperationNode::OperationNode() {}

const char* YExpression::OperationNode::className() const { return "YExpression::OperationNode"; }

void YExpression::OperationNode::print() {
	cout << "(";
	if (l_operand) l_operand->print();

	switch(opType) {
	case add:
		std::cout << "+";
		break;
	case sub:
		cout << "-";
		break;
	case mul:
		cout << "*";
		break;
	case div:
		cout << "/";
		break;
	case val:
		break;
	}
	if(r_operand) r_operand->print();
	cout << ")";
}

YVal* YExpression::OperationNode::execute() {

	//todo:  that || and && are short-circuited so canNOT exec(r_operand) first!
	//if(opType!=conditiond3,or,and) do belowings

	YVal* l_val = l_operand->execute();
	YVal* r_val = r_operand->execute();
	YVal* ret = nullptr;

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
	case val:
		ret = r_val;
		break;
	case UNDEFINED:
	default:
		throw YException("opType undefined");
	}

	return ret;
}