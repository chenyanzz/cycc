#pragma once

/**
 * Created by cy on 2018/5/26.
 *
 * A class for parsing expressions kind like "(i++)", "1+1" , "int_ret_func()" etc..
 */

#include <string>
#include <unordered_map>

#include "CYCC.h"
#include "Excutable.h"
#include "YVal.h"

class YExpression: public Executable {
public:

	static bool parse(const char* str, YExpression* pExp);
	YVal* execute();

	void print() override;
	const char* className() const override;

	virtual ~YExpression() = default;

protected:
	YExpression(const char* expr);
	const std::string s_expr;

	typedef unsigned char priority_t;
	static const std::unordered_map<std::string, priority_t> operator_priority;
	priority_t getPriority(std::string s);

	class OperationNode;
	OperationNode* operation_tree;
	static OperationNode* makeOperationTree(const char* str);
};

class YExpression::OperationNode {
public:
	/** Usages:
	* - l_operand++
	* - l_operand*r_operand
	* - condition?l_operand:r_operand
	*/
	Executable *l_operand, *r_operand, *condition;

	//if EOperatorType==val then just see l_operand as a var/func and execute() on it
	enum EOperatorType { add, sub, mul, div, val };

	EOperatorType opType;

	OperationNode(EOperatorType opType, YExpression* l_operand,
		YExpression* r_operand = nullptr, YExpression* condition = nullptr);

	OperationNode();
};
