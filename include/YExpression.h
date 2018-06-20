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

class YExpression: public Executable{
public:

	static bool parse(const char* str, YExpression* pExp);
	YVal* execute();

	void print() override;
	const char* className() const override;

	virtual ~YExpression() = default;

	friend void test_operator();

protected:
	YExpression(const char* expr);
	const std::string s_expr;

	//if EOperatorType==val then just see OperationNode::l_operand as a var/func and execute() on it
	enum EOperatorType { add, sub, mul, div, val };

	typedef unsigned char priority_t;
	static const std::unordered_map<EOperatorType, priority_t> operator_priority;
	
	class OperationNode;
	OperationNode* operation_tree;
	static OperationNode* makeOperationTree(const char* str);
	static OperationNode* makeTestOperationTree();


};

class YExpression::OperationNode : public Executable {
public:
	/** Usages:
	* - l_operand++
	* - l_operand*r_operand
	* - condition?l_operand:r_operand
	* - l_operand[r_operand]
	*/

	Executable *l_operand, *r_operand, *condition;

	EOperatorType opType;

	OperationNode(EOperatorType opType, YExpression* l_operand,
		YExpression* r_operand = nullptr, YExpression* condition = nullptr);

	OperationNode();
	virtual ~OperationNode() = default;

	const char* className() const override;
	void print() override;
	YVal* execute() override;
};
